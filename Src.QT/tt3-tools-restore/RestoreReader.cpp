//
//  tt3-tools-restore/RestoreReader.cpp - RestoreReader class implementation
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
#include "tt3-tools-restore/API.hpp"
using namespace tt3::tools::restore;

//////////
//  Construction/destruction
RestoreReader::RestoreReader(
        tt3::ws::Workspace workspace,
        const tt3::ws::Credentials & credentials,
        const QString & backupFileName
    ) : _workspace(workspace),
        _adminCredentials(credentials),
        _bytesToRead(QFileInfo(backupFileName).size()),
        _recordCount(_bytesToRead / 217 + 1),   //  real backup files were measured
        _oneRecordDelayMs(int(10000 / _recordCount)),
        _restoreCredentials(
            workspace->beginRestore(    //  may throw
                credentials,
                _recordCount * 85 +     //  1,000,000 objects -> 1 day lease...
                    60 * 60 * 1000)),   //  ...+ 1 hour
        _restoreFile(backupFileName),
        _restoreStream(&_restoreFile)
{
    //  Prepare record handler dispatch table
    _recordHandlers["Object:User"] = &RestoreReader::_processUserRecord;
    _recordHandlers["Object:Account"] = &RestoreReader::_processAccountRecord;
    _recordHandlers["Object:ActivityType"] = &RestoreReader::_processActivityTypeRecord;
    _recordHandlers["Object:PublicActivity"] = &RestoreReader::_processPublicActivityRecord;
    _recordHandlers["Object:PublicTask"] = &RestoreReader::_processPublicTaskRecord;
    _recordHandlers["Object:PrivateActivity"] = &RestoreReader::_processPrivateActivityRecord;
    _recordHandlers["Object:PrivateTask"] = &RestoreReader::_processPrivateTaskRecord;
    _recordHandlers["Object:Project"] = &RestoreReader::_processProjectRecord;
    _recordHandlers["Object:WorkStream"] = &RestoreReader::_processWorkStreamRecord;
    _recordHandlers["Object:Beneficiary"] = &RestoreReader::_processBeneficiaryRecord;
    _recordHandlers["Object:Work"] = &RestoreReader::_processWorkRecord;
    _recordHandlers["Object:Event"] = &RestoreReader::_processEventRecord;
    _recordHandlers["Association:QuickPicksList"] = &RestoreReader::_processQuickPicksListAssociationRecord;
    _recordHandlers["Association:PermittedWorkloads"] = &RestoreReader::_processPermittedWorkloadsAssociationRecord;
    _recordHandlers["Association:ActivityType"] = &RestoreReader::_processActivityTypeAssociationRecord;
    _recordHandlers["Association:ActivityWorkload"] = &RestoreReader::_processActivityWorkloadAssociationRecord;
    _recordHandlers["Association:WorkloadBeneficiaries"] = &RestoreReader::_processWorkloadBeneficiariesAssociationRecord;
}

RestoreReader::~RestoreReader()
{
    _restoreFile.close();   //  destructors must be noexcept!
    try
    {
        _workspace->releaseCredentials(_restoreCredentials);
    }
    catch (const tt3::util::Exception & ex)
    {   //  destructors must be noexcept!
        qCritical() << ex;
    }
}

//////////
//  Operations
bool RestoreReader::restoreWorkspace()
{
    Q_ASSERT(_bytesRead == 0);

    //  Prepare to restore
    if (!_restoreFile.open(QIODevice::ReadOnly))
    {   //  OOPS!
        throw tt3::ws::CustomWorkspaceException(_restoreFile.fileName() + ": " + _restoreFile.errorString());
    }

    //  Do we need a progress dialog ?
    if (QThread::currentThread()->eventDispatcher() != nullptr)
    {
        _progressDialog.reset(
            new RestoreProgressDialog(
                gui::theCurrentSkin->dialogParent(),
                _workspace->address()->displayForm(),
                _restoreFile.fileName()));
        _progressDialog->setVisible(true);
    }

    //  Go!
    _record.reset();
    try
    {
        //  All _restore...() services may throw
        while (!_restoreStream.atEnd())
        {
            QString line = _restoreStream.readLine().trimmed();
            if (line.startsWith("[") && line.endsWith("]"))
            {   //  New recpord starts here
                if (_record.isValid())
                {
                    _processRecord();
                }
                _record.reset(line.mid(1, line.length() - 2));
                continue;
            }
            qsizetype eqIndex = line.indexOf('=');
            if (eqIndex != -1)
            {   //  name=value
                _record.fields[line.left(eqIndex)] = line.mid(eqIndex + 1);
            }
        }
        if (_record.isValid())
        {
            _processRecord();
        }

        //  Cleanup & we're done.
        //  Note, that these will be called from
        //  destructor as a last resort, but calling
        //  them leads to earlier lock release and
        //  better error diagnostics.
        _progressDialog.reset(nullptr);
        _restoreFile.close();
        _workspace->releaseCredentials(_restoreCredentials);   //  may throw
        return true;
    }
    catch (const _CancelRequest &)
    {   //  Cleanup & cancel
        _progressDialog.reset(nullptr);
        _restoreFile.close();
        _workspace->releaseCredentials(_restoreCredentials);
        return false;
    }
    catch (...)
    {   //  OOPS! Cleamup, then re-throw
        _progressDialog.reset(nullptr);
        _restoreFile.close();
        _workspace->releaseCredentials(_restoreCredentials);
        throw;
    }
}

//////////
//  Implementation helpers
void RestoreReader::_reportProgress()
{
    if (_progressDialog != nullptr)
    {
        float progress =
            (_bytesToRead == 0) ?
                0.0f :
                float(_restoreFile.pos()) / float(_bytesToRead);
        _progressDialog->reportProgress(progress);
        if (_progressDialog->cancelRequested())
        {
            throw _CancelRequest();
        }
    }
}

void RestoreReader::_processRecord()
{
    Q_ASSERT(_recordHandlers.contains(_record.type));
    (this->*_recordHandlers[_record.type])();
    _reportProgress();
}

void RestoreReader::_processUserRecord()
{
    auto oid =
        _record.fetchField<tt3::ws::Oid>("OID");
    auto enabled =
        _record.fetchField<bool>("Enabled");
    auto emailAddresses =
        _record.fetchField<QStringList>("EmailAddresses");
    auto realName =
        _record.fetchField<QString>("RealName");
    auto inactivityTimeout =
        _record.fetchOptionalField<tt3::ws::InactivityTimeout>("InactivityTimeout");
    auto uiLocale =
        _record.fetchOptionalField<tt3::ws::UiLocale>("UiLocale");

    auto user =
        _workspace->createUser( //  may throw
            _restoreCredentials,
            enabled,
            emailAddresses,
            realName,
            inactivityTimeout,
            uiLocale,
            tt3::ws::Workloads());
    user->setOid(_restoreCredentials, oid);
}

void RestoreReader::_processAccountRecord()
{
    auto userOid =
        _record.fetchField<tt3::ws::Oid>("UserOID");
    auto oid =
        _record.fetchField<tt3::ws::Oid>("OID");
    auto enabled =
        _record.fetchField<bool>("Enabled");
    auto emailAddresses =
        _record.fetchField<QStringList>("EmailAddresses");
    auto login =
        _record.fetchField<QString>("Login");
    auto passwordHash =
        _record.fetchField<QString>("PasswordHash");
    auto capabilities =
        _record.fetchField<tt3::ws::Capabilities>("Capabilities");

    auto user =
        _workspace->getObjectByOid<tt3::ws::User>(_restoreCredentials, userOid);
    auto account =
        user->createAccount(    //  may throw
            _restoreCredentials,
            enabled,
            emailAddresses,
            login,
            "",
            capabilities);
    account->setOid(_restoreCredentials, oid);
    account->_setPasswordHash(passwordHash);    //  may throw
}

void RestoreReader::_processActivityTypeRecord()
{
    auto oid =
        _record.fetchField<tt3::ws::Oid>("OID");
    auto displayName =
        _record.fetchField<QString>("DisplayName");
    auto description =
        _record.fetchField<QString>("Description");

    auto activityType =
        _workspace->createActivityType( //  may throw
            _restoreCredentials,
            displayName,
            description);
    activityType->setOid(_restoreCredentials, oid); //  may throw
}

void RestoreReader::_processPublicActivityRecord()
{
    auto oid =
        _record.fetchField<tt3::ws::Oid>("OID");
    auto displayName =
        _record.fetchField<QString>("DisplayName");
    auto description =
        _record.fetchField<QString>("Description");
    auto timeout =
        _record.fetchOptionalField<tt3::ws::InactivityTimeout>("Timeout");
    auto requireCommentOnStart =
        _record.fetchField<bool>("RequireCommentOnStart");
    auto requireCommentOnStop =
        _record.fetchField<bool>("RequireCommentOnStop");
    auto fullScreenReminder =
        _record.fetchField<bool>("FullScreenReminder");

    auto publicActivity =
        _workspace->createPublicActivity(   //  may throw
            _restoreCredentials,
            displayName,
            description,
            timeout,
            requireCommentOnStart,
            requireCommentOnStop,
            fullScreenReminder,
            nullptr,
            nullptr);
    publicActivity->setOid(_restoreCredentials, oid);   //  may throw
}

void RestoreReader::_processPublicTaskRecord()
{
    auto oid =
        _record.fetchField<tt3::ws::Oid>("OID");
    auto displayName =
        _record.fetchField<QString>("DisplayName");
    auto description =
        _record.fetchField<QString>("Description");
    auto timeout =
        _record.fetchOptionalField<tt3::ws::InactivityTimeout>("Timeout");
    auto requireCommentOnStart =
        _record.fetchField<bool>("RequireCommentOnStart");
    auto requireCommentOnStop =
        _record.fetchField<bool>("RequireCommentOnStop");
    auto fullScreenReminder =
        _record.fetchField<bool>("FullScreenReminder");
    auto requireCommentOnCompletion =
        _record.fetchField<bool>("RequireCommentOnCompletion");
    auto completed =
        _record.fetchField<bool>("Completed");

    tt3::ws::PublicTask publicTask;
    if (_record.fields.contains("ParentOID"))
    {
        auto parentOid =
            _record.fetchField<tt3::ws::Oid>("ParentOID");
        auto parent =
            _workspace->getObjectByOid<tt3::ws::PublicTask>(_restoreCredentials, parentOid);
        publicTask =
            parent->createChild(
                _restoreCredentials,
                displayName,
                description,
                timeout,
                requireCommentOnStart,
                requireCommentOnStop,
                fullScreenReminder,
                nullptr,
                nullptr,
                completed,
                requireCommentOnCompletion);
    }
    else
    {
        publicTask =
            _workspace->createPublicTask(
                _restoreCredentials,
                displayName,
                description,
                timeout,
                requireCommentOnStart,
                requireCommentOnStop,
                fullScreenReminder,
                nullptr,
                nullptr,
                completed,
                requireCommentOnCompletion);
    }
    publicTask->setOid(_restoreCredentials, oid);   //  may throw
}

void RestoreReader::_processPrivateActivityRecord()
{
    auto ownerOid =
        _record.fetchField<tt3::ws::Oid>("OwnerOID");
    auto oid =
        _record.fetchField<tt3::ws::Oid>("OID");
    auto displayName =
        _record.fetchField<QString>("DisplayName");
    auto description =
        _record.fetchField<QString>("Description");
    auto timeout =
        _record.fetchOptionalField<tt3::ws::InactivityTimeout>("Timeout");
    auto requireCommentOnStart =
        _record.fetchField<bool>("RequireCommentOnStart");
    auto requireCommentOnStop =
        _record.fetchField<bool>("RequireCommentOnStop");
    auto fullScreenReminder =
        _record.fetchField<bool>("FullScreenReminder");

    auto owner =
        _workspace->getObjectByOid<tt3::ws::User>(_restoreCredentials, ownerOid);
    auto privateActivity =
        owner->createPrivateActivity(   //  may throw
            _restoreCredentials,
            displayName,
            description,
            timeout,
            requireCommentOnStart,
            requireCommentOnStop,
            fullScreenReminder,
            nullptr,
            nullptr);
    privateActivity->setOid(_restoreCredentials, oid);   //  may throw
}

void RestoreReader::_processPrivateTaskRecord()
{
    auto ownerOid =
        _record.fetchField<tt3::ws::Oid>("OwnerOID");
    auto oid =
        _record.fetchField<tt3::ws::Oid>("OID");
    auto displayName =
        _record.fetchField<QString>("DisplayName");
    auto description =
        _record.fetchField<QString>("Description");
    auto timeout =
        _record.fetchOptionalField<tt3::ws::InactivityTimeout>("Timeout");
    auto requireCommentOnStart =
        _record.fetchField<bool>("RequireCommentOnStart");
    auto requireCommentOnStop =
        _record.fetchField<bool>("RequireCommentOnStop");
    auto fullScreenReminder =
        _record.fetchField<bool>("FullScreenReminder");
    auto requireCommentOnCompletion =
        _record.fetchField<bool>("RequireCommentOnCompletion");
    auto completed =
        _record.fetchField<bool>("Completed");

    tt3::ws::PrivateTask privateTask;
    if (_record.fields.contains("ParentOID"))
    {
        auto parentOid =
            _record.fetchField<tt3::ws::Oid>("ParentOID");
        auto parent =
            _workspace->getObjectByOid<tt3::ws::PrivateTask>(_restoreCredentials, parentOid);
        privateTask =
            parent->createChild(
                _restoreCredentials,
                displayName,
                description,
                timeout,
                requireCommentOnStart,
                requireCommentOnStop,
                fullScreenReminder,
                nullptr,
                nullptr,
                completed,
                requireCommentOnCompletion);
    }
    else
    {
        auto owner =
            _workspace->getObjectByOid<tt3::ws::User>(_restoreCredentials, ownerOid);
        privateTask =
            owner->createPrivateTask(
                _restoreCredentials,
                displayName,
                description,
                timeout,
                requireCommentOnStart,
                requireCommentOnStop,
                fullScreenReminder,
                nullptr,
                nullptr,
                completed,
                requireCommentOnCompletion);
    }
    privateTask->setOid(_restoreCredentials, oid);   //  may throw
}

void RestoreReader::_processProjectRecord()
{
    auto oid =
        _record.fetchField<tt3::ws::Oid>("OID");
    auto displayName =
        _record.fetchField<QString>("DisplayName");
    auto description =
        _record.fetchField<QString>("Description");
    auto completed =
        _record.fetchField<bool>("Completed");

    tt3::ws::Project project;
    if (_record.fields.contains("ParentOID"))
    {
        auto parentOid =
            _record.fetchField<tt3::ws::Oid>("ParentOID");
        auto parent =
            _workspace->getObjectByOid<tt3::ws::Project>(_restoreCredentials, parentOid);
        project =
            parent->createChild(
                _restoreCredentials,
                displayName,
                description,
                tt3::ws::Beneficiaries(),
                completed);
    }
    else
    {
        project =
            _workspace->createProject(
                _restoreCredentials,
                displayName,
                description,
                tt3::ws::Beneficiaries(),
                completed);
    }
    project->setOid(_restoreCredentials, oid);   //  may throw
}

void RestoreReader::_processWorkStreamRecord()
{
    auto oid =
        _record.fetchField<tt3::ws::Oid>("OID");
    auto displayName =
        _record.fetchField<QString>("DisplayName");
    auto description =
        _record.fetchField<QString>("Description");

    tt3::ws::WorkStream workStream =
        _workspace->createWorkStream(
            _restoreCredentials,
            displayName,
            description,
            tt3::ws::Beneficiaries());
    workStream->setOid(_restoreCredentials, oid);   //  may throw
}

void RestoreReader::_processBeneficiaryRecord()
{
    auto oid =
        _record.fetchField<tt3::ws::Oid>("OID");
    auto displayName =
        _record.fetchField<QString>("DisplayName");
    auto description =
        _record.fetchField<QString>("Description");

    tt3::ws::Beneficiary beneficiary =
        _workspace->createBeneficiary(
            _restoreCredentials,
            displayName,
            description,
            tt3::ws::Workloads());
    beneficiary->setOid(_restoreCredentials, oid);   //  may throw
}

void RestoreReader::_processWorkRecord()
{
    auto oid =
        _record.fetchField<tt3::ws::Oid>("OID");
    auto activityOid =
        _record.fetchField<tt3::ws::Oid>("ActivityOID");
    auto accountOid =
        _record.fetchField<tt3::ws::Oid>("AccountOID");
    auto startedAt =
        _record.fetchField<QDateTime>("StartedAt");
    auto finishedAt =
        _record.fetchField<QDateTime>("FinishedAt");

    auto activity =
        _workspace->getObjectByOid<tt3::ws::Activity>(_restoreCredentials, activityOid);
    auto account =
        _workspace->getObjectByOid<tt3::ws::Account>(_restoreCredentials, accountOid);
    auto work =
        account->createWork(
            _restoreCredentials,
            startedAt,
            finishedAt,
            activity);
    work->setOid(_restoreCredentials, oid);   //  may throw
}

void RestoreReader::_processEventRecord()
{
    auto oid =
        _record.fetchField<tt3::ws::Oid>("OID");
    QList<tt3::ws::Oid> activityOids;
    if (_record.fields.contains("ActivityOID"))
    {
        activityOids.append(
            _record.fetchField<tt3::ws::Oid>("ActivityOID"));
    }
    else if (_record.fields.contains("ActivityOIDs"))
    {
        activityOids =
            _record.fetchField<QList<tt3::ws::Oid>>("ActivityOIDs");
    }
    auto accountOid =
        _record.fetchField<tt3::ws::Oid>("AccountOID");
    auto occurredAt =
        _record.fetchField<QDateTime>("OccurredAt");
    auto summary =
        _record.fetchField<QString>("Summary");

    auto account =
        _workspace->getObjectByOid<tt3::ws::Account>(_restoreCredentials, accountOid);
    tt3::ws::Activities activities =
        tt3::util::transform(
            QSet<tt3::ws::Oid>(activityOids.cbegin(), activityOids.cend()),
            [&](const auto & o)
            {
                return _workspace->getObjectByOid<tt3::ws::Activity>(_restoreCredentials, o);
            });

    auto event =
        account->createEvent(
            _restoreCredentials,
            occurredAt,
            summary,
            activities);
    event->setOid(_restoreCredentials, oid);   //  may throw
}

void RestoreReader::_processQuickPicksListAssociationRecord()
{
    auto accountOid =
        _record.fetchField<tt3::ws::Oid>("AccountOID");
    auto account =
        _workspace->getObjectByOid<tt3::ws::Account>(_restoreCredentials, accountOid);

    tt3::ws::Activity activity = _resolveActivity();

    auto quickPicksList =
        account->quickPicksList(_restoreCredentials);
    if (!quickPicksList.contains(activity))
    {
        quickPicksList.append(activity);
    }
    account->setQuickPicksList(_restoreCredentials, quickPicksList);
}

void RestoreReader::_processPermittedWorkloadsAssociationRecord()
{
    auto userOid =
        _record.fetchField<tt3::ws::Oid>("UserOID");
    auto user =
        _workspace->getObjectByOid<tt3::ws::User>(_restoreCredentials, userOid);

    tt3::ws::Workload workload = _resolveWorkload();

    user->addPermittedWorkload(_restoreCredentials, workload);
}

void RestoreReader::_processActivityTypeAssociationRecord()
{
    tt3::ws::Activity activity = _resolveActivity();

    auto activityTypeOid =
        _record.fetchField<tt3::ws::Oid>("ActivityTypeOID");
    auto activityType =
        _workspace->getObjectByOid<tt3::ws::ActivityType>(_restoreCredentials, activityTypeOid);

    activity->setActivityType(_restoreCredentials, activityType);
}

void RestoreReader::_processActivityWorkloadAssociationRecord()
{
    tt3::ws::Activity activity = _resolveActivity();
    tt3::ws::Workload workload = _resolveWorkload();

    activity->setWorkload(_restoreCredentials, workload);
}

void RestoreReader::_processWorkloadBeneficiariesAssociationRecord()
{
    tt3::ws::Workload workload = _resolveWorkload();

    auto beneficiaryOid =
        _record.fetchField<tt3::ws::Oid>("BeneficiaryOID");
    auto beneficiary =
        _workspace->getObjectByOid<tt3::ws::Beneficiary>(_restoreCredentials, beneficiaryOid);

    workload->addBeneficiary(_restoreCredentials, beneficiary);
}

auto RestoreReader::_resolveActivity(
    ) -> tt3::ws::Activity
{
    if (_record.fields.contains("PublicActivityOID"))
    {
        auto publicActivityOid =
            _record.fetchField<tt3::ws::Oid>("PublicActivityOID");
        return _workspace->getObjectByOid<tt3::ws::PublicActivity>(_restoreCredentials, publicActivityOid);
    }
    else if (_record.fields.contains("PublicTaskOID"))
    {
        auto publicTaskOid =
            _record.fetchField<tt3::ws::Oid>("PublicTaskOID");
        return _workspace->getObjectByOid<tt3::ws::PublicTask>(_restoreCredentials, publicTaskOid);
    }
    else if (_record.fields.contains("PrivateActivityOID"))
    {
        auto privateActivityOid =
            _record.fetchField<tt3::ws::Oid>("PrivateActivityOID");
        return _workspace->getObjectByOid<tt3::ws::PrivateActivity>(_restoreCredentials, privateActivityOid);
    }
    else if (_record.fields.contains("PrivateTaskOID"))
    {
        auto privateTaskOid =
            _record.fetchField<tt3::ws::Oid>("PrivateTaskOID");
        return _workspace->getObjectByOid<tt3::ws::PrivateTask>(_restoreCredentials, privateTaskOid);
    }
    else
    {
        throw BackupFileCorruptException(_restoreFile.fileName());
    }
}

auto RestoreReader::_resolveWorkload(
    ) -> tt3::ws::Workload
{
    if (_record.fields.contains("ProjectOID"))
    {
        auto projectOID =
            _record.fetchField<tt3::ws::Oid>("ProjectOID");
        return _workspace->getObjectByOid<tt3::ws::Project>(_restoreCredentials, projectOID);
    }
    else if (_record.fields.contains("WorkStreamOID"))
    {
        auto workStreamOID =
            _record.fetchField<tt3::ws::Oid>("WorkStreamOID");
        return _workspace->getObjectByOid<tt3::ws::WorkStream>(_restoreCredentials, workStreamOID);
    }
    else
    {
        throw BackupFileCorruptException(_restoreFile.fileName());
    }
}

//////////
//  Parsing
namespace
{
    int xdigit(QChar c)
    {
        if (c >= '0' && c <= '9')
        {
            return c.unicode() - '0';
        }
        else if (c >= 'a' && c <= 'f')
        {
            return c.unicode() - 'a' + 10;
        }
        else if (c >= 'A' && c <= 'F')
        {
            return c.unicode() - 'A' + 10;
        }
        else
        {
            return -1;
        }
    }
}

template <> TT3_TOOLS_RESTORE_PUBLIC
QString tt3::tools::restore::parse<QString>(const QString & s, qsizetype & scan)
{
    if (scan < 0 || scan > s.length())
    {   //  OOPS! Can't
        throw tt3::util::ParseException(s, scan);
    }
    //  Skip opening quote
    if (scan < s.length() && s[scan] == '"')
    {
        scan++;
    }
    else
    {
        throw tt3::util::ParseException(s, scan);
    }
    //  Parse innards
    QString result;
    while (scan < s.length())
    {
        if (s[scan] == '"')
        {   //  Closing quote is here!
            break;
        }
        if (s[scan] != '\\')
        {   //  A literal character
            result += s[scan];
            scan++;
            continue;
        }
        //  We have an escape sequence. Skip '\'
        scan++;
        if (scan >= s.length())
        {   //  OOPS! Missing!
            throw tt3::util::ParseException(s, scan);
        }
        //  Special character ?
        if (s[scan] == 'a')
        {   //  \a
            result += '\a';
            scan++;
        }
        else if (s[scan] == 'b')
        {   //  \b
            result += '\b';
            scan++;
        }
        else if (s[scan] == 'f')
        {   //  \f
            result += '\f';
            scan++;
        }
        else if (s[scan] == 'n')
        {   //  \n
            result += '\n';
            scan++;
        }
        else if (s[scan] == 'r')
        {   //  \r
            result += '\r';
            scan++;
        }
        else if (s[scan] == 't')
        {   //  \t
            result += '\t';
            scan++;
        }
        else if (s[scan] == 'v')
        {   //  \a
            result += '\v';
            scan++;
        }
        //  Numeric escape ?
        else if (s[scan] == 'x')
        {   //  \xXX
            scan++;
            if (scan + 1 < s.length() &&
                xdigit(s[scan]) != -1 && xdigit(s[scan + 1]) != -1)
            {
                result += QChar(xdigit(s[scan]) * 16 +
                                xdigit(s[scan + 1]));
                scan += 2;
            }
            else
            {   //  OOPS! Invalid hex escape!
                throw tt3::util::ParseException(s, scan);
            }
        }
        else if (s[scan] == 'u')
        {   //  \uXXXX
            scan++;
            if (scan + 3 < s.length() &&
                xdigit(s[scan]) != -1 && xdigit(s[scan + 1]) != -1 &&
                xdigit(s[scan + 2]) != -1 && xdigit(s[scan + 3]) != -1)
            {
                result += QChar(xdigit(s[scan]) * 4096 +
                                xdigit(s[scan + 1]) * 256 +
                                xdigit(s[scan + 2]) * 16 +
                                xdigit(s[scan + 3]));
                scan += 4;
            }
            else
            {   //  OOPS! Invalid hex escape!
                throw tt3::util::ParseException(s, scan);
            }
        }
        //  Literal escape
        else
        {
            result += s[scan++];
        }
    }
    //  Skip closing quote
    if (scan < s.length() && s[scan] == '"')
    {
        scan++;
    }
    else
    {
        throw tt3::util::ParseException(s, scan);
    }
    //  Done
    return result;
}

template <> TT3_TOOLS_RESTORE_PUBLIC
QStringList tt3::tools::restore::parse<QStringList>(const QString & s, qsizetype & scan)
{
    if (scan < 0 || scan > s.length())
    {   //  OOPS! Can't
        throw tt3::util::ParseException(s, scan);
    }
    //  Skip opening bracket
    if (scan < s.length() && s[scan] == '[')
    {
        scan++;
    }
    else
    {
        throw tt3::util::ParseException(s, scan);
    }
    //  Parse innards
    QStringList result;
    if (scan < s.length() && s[scan] != ']')
    {   //  The list is not empty
        for (; ; )
        {
            QString element = parse<QString>(s, scan);   //  may throw
            result.append(element);
            //  More ?
            if (scan < s.length() && s[scan] == ',')
            {   //  Yes
                scan++;
            }
            else
            {   //  No
                break;
            }
        }
    }
    //  Skip closing bracket
    if (scan < s.length() && s[scan] == ']')
    {
        scan++;
    }
    else
    {
        throw tt3::util::ParseException(s, scan);
    }
    //  Done
    return result;
}

template <> TT3_TOOLS_RESTORE_PUBLIC
tt3::ws::InactivityTimeout tt3::tools::restore::parse<tt3::ws::InactivityTimeout>(const QString & s, qsizetype & scan)
{
    return tt3::util::fromString<tt3::util::TimeSpan>(s, scan);
}

template <> TT3_TOOLS_RESTORE_PUBLIC
tt3::ws::UiLocale tt3::tools::restore::parse<tt3::ws::UiLocale>(const QString & s, qsizetype & scan)
{
    return tt3::util::fromString<QLocale>(s, scan);
}

template <> TT3_TOOLS_RESTORE_PUBLIC
QList<tt3::ws::Oid> tt3::tools::restore::parse<QList<tt3::ws::Oid>>(const QString & s, qsizetype & scan)
{
    if (scan < 0 || scan > s.length())
    {   //  OOPS! Can't
        throw tt3::util::ParseException(s, scan);
    }
    //  Skip opening bracket
    if (scan < s.length() && s[scan] == '[')
    {
        scan++;
    }
    else
    {
        throw tt3::util::ParseException(s, scan);
    }
    //  Parse innards
    QList<tt3::ws::Oid> result;
    if (scan < s.length() && s[scan] != ']')
    {   //  The list is not empty
        for (; ; )
        {
            tt3::ws::Oid element = parse<tt3::ws::Oid>(s, scan);   //  may throw
            result.append(element);
            //  More ?
            if (scan < s.length() && s[scan] == ',')
            {   //  Yes
                scan++;
            }
            else
            {   //  No
                break;
            }
        }
    }
    //  Skip closing bracket
    if (scan < s.length() && s[scan] == ']')
    {
        scan++;
    }
    else
    {
        throw tt3::util::ParseException(s, scan);
    }
    //  Done
    return result;
}

//  End of tt3-tools-restore/RestoreReader.cpp
