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
        _credentials(
            workspace->beginBackup(   //  may throw
                credentials,
                workspace->objectCount(credentials) * 85 + //  1,000,000 objects -> 1 day lease...
                60 * 60 * 1000  //  ...+ 1 hour
              )),
        _backupFile(backupFileName),
        _backupStream(&_backupFile),
        _objectsToWrite(_workspace->objectCount(_credentials)),
        _associationsToWrite(_workspace->objectCount(_credentials)),
        _oneObjectDelayMs(int(5000 / (_objectsToWrite + 1) + 1))
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
bool BackupWriter::backupWorkspace()
{
    Q_ASSERT(_objectsWritten == 0 && _associationsWritten == 0);

    //  Prepare to backup
    if (!_backupFile.open(QIODevice::WriteOnly))
    {   //  OOPS!
        throw tt3::ws::CustomWorkspaceException(_backupFile.fileName() + ": " + _backupFile.errorString());
    }

    //  Do we need a progress dialog ?
    if (QThread::currentThread()->eventDispatcher() != nullptr)
    {
        _progressDialog.reset(
            new BackupProgressDialog(
                gui::theCurrentSkin->dialogParent(),
                _workspace->address()->displayForm(),
                _backupFile.fileName()));
        _progressDialog->setVisible(true);
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
        _backupObjects( //  PublicActivities
            _workspace->publicActivities(_credentials));
        _backupObjects( //  PublicTasks incl. children
            _workspace->rootPublicTasks(_credentials));
        for (const auto & user :
             _sortedByOid(_workspace->users(_credentials)))
        {
            _backupObjects( //  PrivateActivities
                user->privateActivities(_credentials));
            _backupObjects( //  PrivateTasks incl. children
                user->rootPrivateTasks(_credentials));
        }
        _backupObjects( //  Projects incl. children
            _workspace->rootProjects(_credentials));
        _backupObjects( //  WorkStreams
            _workspace->workStreams(_credentials));
        _backupObjects( //  Beneficiaries
            _workspace->beneficiaries(_credentials));
        for (const auto & user :
             _sortedByOid(_workspace->users(_credentials)))
        {
            for (const auto & account : user->accounts(_credentials))
            {
                _backupObjects( //  Works
                    account->works(_credentials));
                _backupObjects( //  Events
                    account->events(_credentials));
            }
        }

        //  ...and associations
        _backupOutgoingAssociations( //  Users + Accounts
            _workspace->users(_credentials));
        _backupOutgoingAssociations( //  ActivityTypes
            _workspace->activityTypes(_credentials));
        _backupOutgoingAssociations( //  PublicActivities
            _workspace->publicActivities(_credentials));
        _backupOutgoingAssociations( //  PublicTasks incl. children
            _workspace->rootPublicTasks(_credentials));
        for (const auto & user :
             _sortedByOid(_workspace->users(_credentials)))
        {
            _backupOutgoingAssociations( //  PrivateActivities
                user->privateActivities(_credentials));
            _backupOutgoingAssociations( //  PrivateTasks incl. children
                user->rootPrivateTasks(_credentials));
        }
        _backupOutgoingAssociations( //  Projects incl. children
            _workspace->rootProjects(_credentials));
        _backupOutgoingAssociations( //  WorkStreams
            _workspace->workStreams(_credentials));
        _backupOutgoingAssociations( //  Beneficiaries
            _workspace->beneficiaries(_credentials));
        for (const auto & user :
             _sortedByOid(_workspace->users(_credentials)))
        {
            for (const auto & account : user->accounts(_credentials))
            {
                _backupOutgoingAssociations( //  Works
                    account->works(_credentials));
                _backupOutgoingAssociations( //  Events
                    account->events(_credentials));
            }
        }

        Q_ASSERT(_objectsWritten == _objectsToWrite);
        Q_ASSERT(_associationsWritten == _associationsToWrite);

        //  Cleanup & we're done.
        //  Note, that these will be called from
        //  destructor as a last resort, but calling
        //  them leads to earlier lock release and
        //  better error diagnostics.
        _progressDialog.reset(nullptr);
        _backupStream.flush();
        _backupFile.close();
        _workspace->releaseCredentials(_credentials);   //  may throw
        if (_backupFile.error() != QFile::NoError)
        {   //  OOPS! Disk full, etc.
            QFile::remove(_backupFile.fileName());  //  may fail, but who cares at this point...
            throw tt3::ws::CustomWorkspaceException(
                _backupFile.fileName() + ": " + _backupFile.errorString());
        }
        return true;
    }
    catch (const _CancelRequest &)
    {   //  Cleanup & cancel
        _progressDialog.reset(nullptr);
        _backupFile.close();
        QFile::remove(_backupFile.fileName());  //  may fail, but who cares at this point...
        _workspace->releaseCredentials(_credentials);
        return false;
    }
    catch (...)
    {   //  OOPS! Cleamup, then re-throw
        _progressDialog.reset(nullptr);
        _backupFile.close();
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
    _reportProgress();
}

void BackupWriter::_onAssociationWritten()
{
    _associationsWritten++;
    Q_ASSERT(_associationsWritten <= _associationsToWrite);
    _reportProgress();
}

void BackupWriter::_reportProgress()
{
    if (_progressDialog != nullptr)
    {
        _progressDialog->reportProgress(
            float(_objectsWritten + _associationsWritten) /
            float(_objectsToWrite + _associationsToWrite));
        if (_progressDialog->cancelRequested())
        {
            throw _CancelRequest();
        }
    }
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

    _backupObjects(user->accounts(_credentials));
}

void BackupWriter::_backupObject(
        tt3::ws::Account account
    )
{
    _writeObjectHeader(account);
    _writeObjectProperty("UserOID", account->user(_credentials)->oid());
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

void BackupWriter::_backupObject(
        tt3::ws::PublicActivity publicActivity
    )
{
    Q_ASSERT(std::dynamic_pointer_cast<tt3::ws::TaskImpl>(publicActivity) == nullptr);

    _writeObjectHeader(publicActivity);
    _writeObjectProperty("OID", publicActivity->oid());
    _writeObjectProperty("DisplayName", publicActivity->displayName(_credentials));
    _writeObjectProperty("Description", publicActivity->description(_credentials));
    _writeObjectProperty("Timeout", publicActivity->timeout(_credentials));
    _writeObjectProperty("RequireCommentOnStart", publicActivity->requireCommentOnStart(_credentials));
    _writeObjectProperty("RequireCommentOnStop", publicActivity->requireCommentOnStop(_credentials));
    _writeObjectProperty("FullScreenReminder", publicActivity->fullScreenReminder(_credentials));
}

void BackupWriter::_backupObject(
        tt3::ws::PublicTask publicTask
    )
{
    _writeObjectHeader(publicTask);
    if (publicTask->parent(_credentials) != nullptr)
    {
        _writeObjectProperty("ParentOID", publicTask->parent(_credentials)->oid());
    }
    _writeObjectProperty("OID", publicTask->oid());
    _writeObjectProperty("DisplayName", publicTask->displayName(_credentials));
    _writeObjectProperty("Description", publicTask->description(_credentials));
    _writeObjectProperty("Timeout", publicTask->timeout(_credentials));
    _writeObjectProperty("RequireCommentOnStart", publicTask->requireCommentOnStart(_credentials));
    _writeObjectProperty("RequireCommentOnStop", publicTask->requireCommentOnStop(_credentials));
    _writeObjectProperty("FullScreenReminder", publicTask->fullScreenReminder(_credentials));
    _writeObjectProperty("RequireCommentOnCompletion", publicTask->requireCommentOnCompletion(_credentials));
    _writeObjectProperty("Completed", publicTask->completed(_credentials));

    _backupObjects(publicTask->children(_credentials));
}

void BackupWriter::_backupObject(
        tt3::ws::PrivateActivity privateActivity
    )
{
    Q_ASSERT(std::dynamic_pointer_cast<tt3::ws::TaskImpl>(privateActivity) == nullptr);

    _writeObjectHeader(privateActivity);
    _writeObjectProperty("OwnerOID", privateActivity->owner(_credentials)->oid());
    _writeObjectProperty("OID", privateActivity->oid());
    _writeObjectProperty("DisplayName", privateActivity->displayName(_credentials));
    _writeObjectProperty("Description", privateActivity->description(_credentials));
    _writeObjectProperty("Timeout", privateActivity->timeout(_credentials));
    _writeObjectProperty("RequireCommentOnStart", privateActivity->requireCommentOnStart(_credentials));
    _writeObjectProperty("RequireCommentOnStop", privateActivity->requireCommentOnStop(_credentials));
    _writeObjectProperty("FullScreenReminder", privateActivity->fullScreenReminder(_credentials));
}

void BackupWriter::_backupObject(
        tt3::ws::PrivateTask privateTask
    )
{
    _writeObjectHeader(privateTask);
    _writeObjectProperty("OwnerOID", privateTask->owner(_credentials)->oid());
    if (privateTask->parent(_credentials) != nullptr)
    {
        _writeObjectProperty("ParentOID", privateTask->parent(_credentials)->oid());
    }
    _writeObjectProperty("OID", privateTask->oid());
    _writeObjectProperty("DisplayName", privateTask->displayName(_credentials));
    _writeObjectProperty("Description", privateTask->description(_credentials));
    _writeObjectProperty("Timeout", privateTask->timeout(_credentials));
    _writeObjectProperty("RequireCommentOnStart", privateTask->requireCommentOnStart(_credentials));
    _writeObjectProperty("RequireCommentOnStop", privateTask->requireCommentOnStop(_credentials));
    _writeObjectProperty("FullScreenReminder", privateTask->fullScreenReminder(_credentials));
    _writeObjectProperty("RequireCommentOnCompletion", privateTask->requireCommentOnCompletion(_credentials));
    _writeObjectProperty("Completed", privateTask->completed(_credentials));

    _backupObjects(privateTask->children(_credentials));
}

void BackupWriter::_backupObject(
        tt3::ws::Project project
    )
{
    _writeObjectHeader(project);
    if (project->parent(_credentials) != nullptr)
    {
        _writeObjectProperty("ParentOID", project->parent(_credentials)->oid());
    }
    _writeObjectProperty("OID", project->oid());
    _writeObjectProperty("DisplayName", project->displayName(_credentials));
    _writeObjectProperty("Description", project->description(_credentials));
    _writeObjectProperty("Completed", project->completed(_credentials));

    _backupObjects(project->children(_credentials));
}

void BackupWriter::_backupObject(
        tt3::ws::WorkStream workStream
    )
{
    _writeObjectHeader(workStream);
    _writeObjectProperty("OID", workStream->oid());
    _writeObjectProperty("DisplayName", workStream->displayName(_credentials));
    _writeObjectProperty("Description", workStream->description(_credentials));
}

void BackupWriter::_backupObject(
        tt3::ws::Beneficiary beneficiary
    )
{
    _writeObjectHeader(beneficiary);
    _writeObjectProperty("OID", beneficiary->oid());
    _writeObjectProperty("DisplayName", beneficiary->displayName(_credentials));
    _writeObjectProperty("Description", beneficiary->description(_credentials));
}

void BackupWriter::_backupObject(
        tt3::ws::Work work
    )
{
    _writeObjectHeader(work);
    _writeObjectProperty("OID", work->oid());
    _writeObjectProperty("ActivityOID", work->activity(_credentials)->oid());
    _writeObjectProperty("AccountOID", work->account(_credentials)->oid());
    _writeObjectProperty("StartedAt", work->startedAt(_credentials));
    _writeObjectProperty("FinishedAt", work->finishedAt(_credentials));
}

void BackupWriter::_backupObject(
        tt3::ws::Event event
    )
{
    QList<tt3::ws::Oid> activityOids =
    tt3::util::transform(
        _sortedByOid(event->activities(_credentials)),
        [](auto a)
        {
            return a->oid();
        });
    _writeObjectHeader(event);
    _writeObjectProperty("OID", event->oid());
    if (activityOids.size() == 1)
    {
        _writeObjectProperty("ActivityOID", activityOids[0]);
    }
    else if (activityOids.size() > 1)
    {
        _writeObjectProperty("ActivityOIDs", activityOids);
    }
    _writeObjectProperty("AccountOID", event->account(_credentials)->oid());
    _writeObjectProperty("OccurredAt", event->occurredAt(_credentials));
    _writeObjectProperty("Summary", event->summary(_credentials));
}

void BackupWriter::_backupOutgoingAssociations(    //  incl. Accounts
        tt3::ws::User user
    )
{
    //  User -> Workload (PermittedWorkloads)
    for (const auto & workload :
         _sortedByOid(user->permittedWorkloads(_credentials)))
    {
        _writeAssociation(
            "PermittedWorkloads",
            user,
            workload);
    }

    _backupOutgoingAssociations(user->accounts(_credentials));
}

void BackupWriter::_backupOutgoingAssociations(    //  incl. Accounts
        tt3::ws::Account account
    )
{
    //  Account -> Activity (QuickPicksList)
    for (const auto & activity :
         account->quickPicksList(_credentials))
    {
        _writeAssociation(
            "QuickPicksList",
            account,
            activity);
    }
}

void BackupWriter::_backupOutgoingAssociations(    //  incl. Accounts
        tt3::ws::ActivityType /*activityType*/
    )
{   //  No outgoing associations
}

void BackupWriter::_backupOutgoingAssociations(    //  incl. Accounts
        tt3::ws::PublicActivity publicActivity
    )
{
    if (auto activityType = publicActivity->activityType(_credentials))
    {
        _writeAssociation(
            "ActivityType",
            publicActivity,
            activityType);
    }
    if (auto workload = publicActivity->workload(_credentials))
    {
        _writeAssociation(
            "ActivityWorkload",
            publicActivity,
            workload);
    }
}

void BackupWriter::_backupOutgoingAssociations(    //  incl. Accounts
        tt3::ws::PublicTask publicTask
    )
{
    if (auto activityType = publicTask->activityType(_credentials))
    {
        _writeAssociation(
            "ActivityType",
            publicTask,
            activityType);
    }
    if (auto workload = publicTask->workload(_credentials))
    {
        _writeAssociation(
            "ActivityWorkload",
            publicTask,
            workload);
    }

    _backupOutgoingAssociations(publicTask->children(_credentials));
}

void BackupWriter::_backupOutgoingAssociations(    //  incl. Accounts
        tt3::ws::PrivateActivity privateActivity
    )
{
    if (auto activityType = privateActivity->activityType(_credentials))
    {
        _writeAssociation(
            "ActivityType",
            privateActivity,
            activityType);
    }
    if (auto workload = privateActivity->workload(_credentials))
    {
        _writeAssociation(
            "ActivityWorkload",
            privateActivity,
            workload);
    }
}

void BackupWriter::_backupOutgoingAssociations(    //  incl. Accounts
        tt3::ws::PrivateTask privateTask
    )
{
    if (auto activityType = privateTask->activityType(_credentials))
    {
        _writeAssociation(
            "ActivityType",
            privateTask,
            activityType);
    }
    if (auto workload = privateTask->workload(_credentials))
    {
        _writeAssociation(
            "ActivityWorkload",
            privateTask,
            workload);
    }

    _backupOutgoingAssociations(privateTask->children(_credentials));
}

void BackupWriter::_backupOutgoingAssociations(    //  incl. Accounts
        tt3::ws::Project project
    )
{
    //  Project -> Beneficiary (WorkloadBeneficiaries)
    for (const auto & beneficiary :
         _sortedByOid(project->beneficiaries(_credentials)))
    {
        _writeAssociation(
            "WorkloadBeneficiaries",
            project,
            beneficiary);
    }

    _backupOutgoingAssociations(project->children(_credentials));
}

void BackupWriter::_backupOutgoingAssociations(    //  incl. Accounts
        tt3::ws::WorkStream workStream
    )
{
    //  WorkStream -> Beneficiary (WorkloadBeneficiaries)
    for (const auto & beneficiary :
         _sortedByOid(workStream->beneficiaries(_credentials)))
    {
        _writeAssociation(
            "WorkloadBeneficiaries",
            workStream,
            beneficiary);
    }
}

void BackupWriter::_backupOutgoingAssociations(    //  incl. Accounts
        tt3::ws::Beneficiary /*beneficiary*/
    )
{   //  No outgoing associations
}

void BackupWriter::_backupOutgoingAssociations(    //  incl. Accounts
        tt3::ws::Work /*work*/
    )
{   //  No outgoing associations
}

void BackupWriter::_backupOutgoingAssociations(    //  incl. Accounts
        tt3::ws::Event /*event*/
    )
{   //  No outgoing associations
}

void BackupWriter::_writeObjectHeader(
        const tt3::ws::Object & object
    )
{
    if (_objectsWritten != 0 || _associationsWritten != 0)
    {   //  Don't put newline before the 1st object
        _backupStream << '\n';
    }
    _backupStream << "[Object:"
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
        const QList<tt3::ws::Oid> & propertyValue
    )
{
    if (!propertyValue.isEmpty())
    {
        _backupStream << propertyName
                      << "=[";
        for (int i = 0; i < propertyValue.size(); i++)
        {
            if (i != 0)
            {
                _backupStream << ',';
            }
            _backupStream << tt3::util::toString(propertyValue[i]);
        }
        _backupStream << "]\n";
    }
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

void BackupWriter::_writeObjectProperty(
        const QString & propertyName,
        const QDateTime & propertyValue
    )
{
    _backupStream << propertyName
                  << '='
                  << tt3::util::toString(propertyValue)
                  << '\n';
}

void BackupWriter::_writeAssociation(
        const QString & associationName,
        tt3::ws::Object from,
        tt3::ws::Object to
    )
{
    _backupStream << "\n[Association:"
                  << associationName
                  << "]\n";
    _backupStream << from->type()->mnemonic().toString()
                  << "OID="
                  << tt3::util::toString(from->oid())
                  << '\n';
    _backupStream << to->type()->mnemonic().toString()
                  << "OID="
                  << tt3::util::toString(to->oid())
                  << '\n';
}

//  End of tt3-tools-backup/BackupWriter.cpp
