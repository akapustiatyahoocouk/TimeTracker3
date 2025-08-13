//
//  tt3-db-xml/Database.cpp - tt3::db::xml::Database class implementation
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
#include "tt3-db-xml/API.hpp"
using namespace tt3::db::xml;

//////////
//  Construction/destruction
Database::Database(DatabaseAddress * address, _Mode mode)
    :   _address(address),
        _validator(tt3::db::api::DefaultValidator::instance())
{
    Q_ASSERT(_address != nullptr);

    //  We need to create a lock before we do anything about the database
    std::unique_ptr<_LockRefresher> lockRefresher { new _LockRefresher(this) };

    //  Now for the content
    switch (mode)
    {
        case _Mode::_Create:
            //  Need to save e,pty DB content
            _save();
            break;
        case _Mode::_Open:
            //  Need to load existing DB content
            //  TODO
            break;
        default:
            Q_ASSERT(false);
    }
    Q_ASSERT(!_needsSaving);

    //  Finally, take over and start the lock refresher
    _lockRefresher = lockRefresher.release();
    _lockRefresher->start();
}

Database::~Database()
{
    try
    {
        close();
    }
    catch (...)
    {   //  OOPS! TODO log?
    }
}

//////////
//  tt3::db::api::IDatabase (general)
bool Database::isOpen() const
{
    tt3::util::Lock lock(_guard);

    return _lockRefresher != nullptr;
}

void Database::close() throws(DatabaseException)
{
    tt3::util::Lock lock(_guard);

    if (_needsSaving)
    {
        try
        {
            _save();    //  may throw!
            _markClosed();
        }
        catch (...)
        {   //  Cleanup & re-throw
            _markClosed();
            throw;
        }
    }
}

//////////
//  tt3::db::api::IDatabase (associations)
tt3::db::api::Users Database::users() const throws(DatabaseException)
{
    tt3::util::Lock lock(_guard);

    return tt3::db::api::Users(_users.cbegin(), _users.cend());
}

tt3::db::api::Accounts Database::accounts() const throws(DatabaseException)
{
    tt3::util::Lock lock(_guard);

    tt3::db::api::Accounts result;
    for (User * user : _users)
    {
        result += tt3::db::api::Accounts(user->_accounts.begin(), user->_accounts.end());
    }
    return result;
}

//////////
//  tt3::db::api::IDatabase (access control)
tt3::db::api::IAccount * Database::tryLogin(const QString & login, const QString & password) const throws(DatabaseException)
{
    static tt3::util::MessageDigest * sha1 = tt3::util::Sha1MessageDigest::instance();  //  idempotent

    tt3::util::Lock lock(_guard);

    std::unique_ptr<tt3::util::MessageDigest::Builder> sha1Builder { sha1->createBuilder() };
    sha1Builder->digest(password);
    QString passwordHash = sha1Builder->digestAsString();

    for (User * user : _users)
    {
        if (user->_enabled)
        {
            for (Account * account : user->_accounts)
            {
                if (account->_enabled && account->_login == login && account->_passwordHash == passwordHash)
                {
                    return account;
                }
            }
        }
    }
    return nullptr;
}

//////////
//  Implementation helpers
void Database::_ensureOpen()
{
    Q_ASSERT(_guard.isLockedByCurrentThread());

    if (_lockRefresher == nullptr)
    {   //  OOPS!
        throw tt3::db::api::DatabaseClosedException();
    }
}

void Database::_markClosed()
{
    Q_ASSERT(_guard.isLockedByCurrentThread());

    if (_lockRefresher != nullptr)
    {
        _lockRefresher->requestStop();
        if (!_lockRefresher->wait(_LockRefresher::StaleTimeoutMin * 60 * 1000))
        {   /// OOPS! Force termination
            _lockRefresher->terminate();
            _lockRefresher->wait(ULONG_MAX);
        }
        delete _lockRefresher;
        _lockRefresher = nullptr;
    }
}

//////////
//  Serialization
void Database::_save() throws(DatabaseException)
{
    //  Create DOM document with a root node
    QDomDocument document;
    QDomProcessingInstruction xmlDeclaration = document.createProcessingInstruction("xml", "version='1.0' encoding='UTF-8' standalone='yes'");
    document.appendChild(xmlDeclaration);

    QDomElement rootElement = document.createElement("TT3");
    rootElement.setAttribute("FormatVersion", "1");
    document.appendChild(rootElement);

    //  Save DOM
    QFile file(_address->_path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {   //  OOPS!
        throw tt3::db::api::DatabaseException(file.errorString());
    }
    QTextStream stream(&file);
    document.save(stream, 4);
    file.close();
    _needsSaving = false;
}

//////////
//  Database::_LockRefresher
Database::_LockRefresher::_LockRefresher(Database * database)
    :   _database(database),
        _lockFile(database->_address->_path + ".lock")
{
    //  Open/create the lock file
    if (_lockFile.exists())
    {   //  Can we take over the stale lock ?
        QFileInfo fileInfo(_lockFile);
        if (fileInfo.isFile())
        {
            if (!_lockFile.open(QIODevice::ReadWrite))
            {   //  OOPS! Can't!
                throw tt3::db::api::DatabaseException(_lockFile.errorString());
            }
            QDateTime lastModifiedAt = fileInfo.lastModified(QTimeZone::UTC);
            QDateTime utcNow = QDateTime::currentDateTimeUtc();
            qDebug() << lastModifiedAt
                     << " -> "
                     << utcNow
                     << " = "
                     << lastModifiedAt.secsTo(utcNow);
            if (lastModifiedAt.secsTo(utcNow) < StaleTimeoutMin * 60)
            {   //  OOPS! Lock too young
                throw tt3::db::api::DatabaseInUseException(_database->_address);
            }
            //  Lock is stale - take it over
            _lockFile.setFileTime(utcNow, QFileDevice::FileTime::FileModificationTime);
        }
        else
        {   //  OOPS! Can't take over a non-file
            throw tt3::db::api::DatabaseCorruptException(_database->_address);
        }
    }
    else
    {
        if (!_lockFile.open(QIODevice::NewOnly))
        {   //  OOPS! Can't!
            throw tt3::db::api::DatabaseException(_lockFile.errorString());
        }
    }
}

Database::_LockRefresher::~_LockRefresher()
{
    _lockFile.close();
    _lockFile.remove();
}

void Database::_LockRefresher::run()
{
    const int WaitChunkMs = 1000;

    QDateTime waitChunkStartedAt = QDateTime::currentDateTimeUtc();
    while (!_stopRequested)
    {
        msleep(WaitChunkMs);
        QDateTime utcNow = QDateTime::currentDateTimeUtc();
        /*  TODO kill off
        qDebug() << waitChunkStartedAt
                 << " -> "
                 << utcNow
                 << " = "
                 << waitChunkStartedAt.secsTo(utcNow);
        */
        if (waitChunkStartedAt.secsTo(utcNow) < RefreshIntervalMin)
        {   //  Keep waiting
            continue;
        }
        //  It's time to update the last modification time of the lock file
        _lockFile.setFileTime(utcNow, QFileDevice::FileTime::FileModificationTime);
    }
}

//  End of tt3-db-xml/Database.cpp
