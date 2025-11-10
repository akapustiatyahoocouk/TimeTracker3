//
//  tt3-tools-backup/BackupWriter.cpp - BackupWriter class implementation
//
//  TimeTracker3
//  Copyright (C) 2026, Andrey Kapustin
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//////////
#include "tt3-tools-backup/API.hpp"
using namespace tt3::tools::backup;

namespace
{
    QString stringize(const QString & s)
    {
        char escape[8];

        QString result;
        for (QChar c : s)
        {
            switch (c.unicode())
            {
                //  special characters
            case '\a':
                result += "\\a";
                break;
            case '\b':
                result += "\\b";
                break;
            case '\f':
                result += "\\f";
                break;
            case '\n':
                result += "\\n";
                break;
            case '\r':
                result += "\\r";
                break;
            case '\t':
                result += "\\t";
                break;
            case '\v':
                result += "\\v";
                break;
                //  literal escapes
            case '"':   case '\\':
                result += '\\';
                result += c;
                break;
                //  default case
            default:
                if (c.unicode() > 255)
                {   //  0uXXXX
                    sprintf(escape, "\\u%04X", c.unicode());
                    result += escape;
                }
                else if (c < '\x20' || c >= '\x7F')
                {   //  \xXX
                    sprintf(escape, "\\x%02X", c.unicode());
                    result += escape;
                }
                else
                {
                    result += c;
                }
                break;
            }
        }
        return '"' + result + '"';
    }
}

//////////
//  Construction/destruction
BackupWriter::BackupWriter(
        tt3::ws::Workspace workspace,
        const tt3::ws::Credentials & credentials,
        const QString & backupFileName
    ) : _workspace(workspace),
        _credentials(workspace->beginBackup(credentials)),   //  may throw
        _backupFile(backupFileName),
        _backupStream(&_backupFile),
        _objectsToWrite(_workspace->objectCount(_credentials)),
        _associationsToWrite(_workspace->objectCount(_credentials))
{
}

BackupWriter::~BackupWriter()
{
    _backupStream.flush();  //  destructors must be noexcept!
    _backupFile.close();    //  destructors must be noexcept!
    try
    {
        _workspace->releaseCredentials(_credentials);
    }
    catch (const tt3::util::Exception & ex)
    {   //  destructors must be noexcept!
        qCritical() << ex;
    }
}

//////////
//  Operations
void BackupWriter::backupWorkspace()
{
    Q_ASSERT(_objectsWritten == 0 && _associationsWritten == 0);

    //  Prepare to backup
    if (!_backupFile.open(QIODevice::WriteOnly))
    {   //  OOPS!
        throw tt3::ws::CustomWorkspaceException(_backupFile.fileName() + ": " + _backupFile.errorString());
    }

    //  Go!
    try
    {
        //  All _backup...() services may throw

        //  Objects...
        _backupObjects( //  Users + Accounts
            _workspace->users(_credentials));
        _backupObjects( //  ActivityTypes
            _workspace->activityTypes(_credentials));

        //  TODO ...and associations

        //  Cleanup & we're done.
        //  Note, that these will be called from
        //  destructor as a last resort, but calling
        //  them leads to earlier lock release and
        //  better error diagnostics.
        _backupStream.flush();
        _backupFile.close();
        _workspace->releaseCredentials(_credentials);   //  may throw
        if (_backupFile.error() != QFile::NoError)
        {   //  OOP! Disk full, etc.
            QFile::remove(_backupFile.fileName());  //  may fail, but who cares at this point...
            throw tt3::ws::CustomWorkspaceException(
                _backupFile.fileName() + ": " + _backupFile.errorString());
        }
    }
    catch (...)
    {   //  OOPS! Cleamup, then re-throw
        QFile::remove(_backupFile.fileName());  //  may fail, but who cares at this point...
        _workspace->releaseCredentials(_credentials);
        throw;
    }
}

//////////
//  Implementation helpers
void BackupWriter::_onObjectWritten()
{
    _objectsWritten++;
    Q_ASSERT(_objectsWritten <= _objectsToWrite);
}

void BackupWriter::_onAssociationWritten()
{
    _associationsWritten++;
    Q_ASSERT(_associationsWritten <= _associationsToWrite);
}

void BackupWriter::_backupObject(
        tt3::ws::User user
    )
{
    _writeObjectHeader(user);
    _writeObjectProperty("OID", user->oid());
    _writeObjectProperty("Enabled", user->enabled(_credentials));
    _writeObjectProperty("EmailAddresses", user->emailAddresses(_credentials));
    _writeObjectProperty("RealName", user->realName(_credentials));
    _writeObjectProperty("InactivityTimeout", user->inactivityTimeout(_credentials));
    _writeObjectProperty("UiLocale", user->uiLocale(_credentials));

    _backupObjects(
        user->accounts(_credentials));
}

void BackupWriter::_backupObject(
        tt3::ws::Account account
    )
{
    _writeObjectHeader(account);
    _writeObjectProperty("OID", account->oid());
    _writeObjectProperty("Enabled", account->enabled(_credentials));
    _writeObjectProperty("EmailAddresses", account->emailAddresses(_credentials));
    _writeObjectProperty("Login", account->login(_credentials));
    _writeObjectProperty("PasswordHash", account->passwordHash(_credentials));
    _writeObjectProperty("Capabilities", account->capabilities(_credentials));
}

void BackupWriter::_backupObject(
        tt3::ws::ActivityType activityType
    )
{
    _writeObjectHeader(activityType);
    _writeObjectProperty("OID", activityType->oid());
    _writeObjectProperty("DisplayName", activityType->displayName(_credentials));
    _writeObjectProperty("Description", activityType->description(_credentials));
}

void BackupWriter::_writeObjectHeader(
        const tt3::ws::Object & object
    )
{
    if (_objectsWritten != 0 || _associationsWritten != 0)
    {   //  Don't put newline before the 1st object
        _backupStream << '\n';
    }
    _backupStream << '['
                  << object->type()->mnemonic().toString()
                  << ']'
                  << '\n';
}

void BackupWriter::_writeObjectProperty(
        const QString & propertyName,
        const tt3::ws::Oid & propertyValue
    )
{
    _backupStream << propertyName
                  << '='
                  << tt3::util::toString(propertyValue)
                  << '\n';
}

void BackupWriter::_writeObjectProperty(
        const QString & propertyName,
        bool propertyValue
    )
{
    _backupStream << propertyName
                  << '='
                  << tt3::util::toString(propertyValue)
                  << '\n';
}

void BackupWriter::_writeObjectProperty(
        const QString & propertyName,
        const QString & propertyValue
    )
{
    _backupStream << propertyName
                  << '='
                  << stringize(propertyValue)
                  << '\n';
}

void BackupWriter::_writeObjectProperty(
        const QString & propertyName,
        const QStringList & propertyValue
    )
{
    QString valueString;
    for (int i = 0; i < propertyValue.size(); i++)
    {
        if (i != 0)
        {
            valueString += ',';
        }
        valueString += stringize(propertyValue[i]);
    }
    _backupStream << propertyName
                  << '='
                  << '['
                  << valueString
                  << ']'
                  << '\n';
}

void BackupWriter::_writeObjectProperty(
        const QString & propertyName,
        const tt3::ws::UiLocale & propertyValue
    )
{
    if (propertyValue.has_value())
    {
        _backupStream << propertyName
                      << '='
                      << tt3::util::toString(propertyValue.value())
                      << '\n';
    }
}

void BackupWriter::_writeObjectProperty(
        const QString & propertyName,
        const tt3::ws::InactivityTimeout & propertyValue
    )
{
    if (propertyValue.has_value())
    {
        _backupStream << propertyName
                      << '='
                      << tt3::util::toString(propertyValue.value())
                      << '\n';
    }
}

void BackupWriter::_writeObjectProperty(
        const QString & propertyName,
        const tt3::ws::Capabilities & propertyValue
    )
{
    _backupStream << propertyName
                  << '='
                  << tt3::util::toString(propertyValue)
                  << '\n';
}

//  End of tt3-tools-backup/BackupWriter.cpp
