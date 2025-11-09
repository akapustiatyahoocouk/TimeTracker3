//
//  tt3-ws/WorkspaceImpl.cpp - tt3::ws::WorkspaceImpl class implementation
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
#include "tt3-ws/API.hpp"
using namespace tt3::ws;

//////////
//  Construction/destruction - from friends only
WorkspaceImpl::WorkspaceImpl(
        WorkspaceAddress address,
        tt3::db::api::IDatabase * database)
    :   _address(address),
        _database(database),
        _isReadOnly(database->isReadOnly())
{
    Q_ASSERT(_address != nullptr);
    Q_ASSERT(_database != nullptr);

    //  Forward database change events
    connect(_database->changeNotifier(),
            &tt3::db::api::ChangeNotifier::databaseClosed,
            this,
            &WorkspaceImpl::_onDatabaseClosed);
    connect(_database->changeNotifier(),
            &tt3::db::api::ChangeNotifier::objectCreated,
            this,
            &WorkspaceImpl::_onObjectCreated);
    connect(_database->changeNotifier(),
            &tt3::db::api::ChangeNotifier::objectDestroyed,
            this,
            &WorkspaceImpl::_onObjectDestroyed);
    connect(_database->changeNotifier(),
            &tt3::db::api::ChangeNotifier::objectModified,
            this,
            &WorkspaceImpl::_onObjectModified);
}

WorkspaceImpl::~WorkspaceImpl()
{
    try
    {
        close();
    }
    catch (const tt3::util::Exception & ex)
    {   //  OOPS! Suppress, but log
        qCritical() << ex;
    }
    delete _database;
}

//////////
//  Operations (general)
WorkspaceType WorkspaceImpl::type() const
{   //  No need to synchronize
    return _address->workspaceType();
}

auto WorkspaceImpl::address(
    ) const -> WorkspaceAddress
{   //  No need to synchronize
    return _address;
}

bool WorkspaceImpl::isOpen() const
{
    tt3::util::Lock _(_guard);

    return _isOpen;
}

bool WorkspaceImpl::isReadOnly() const
{   //  No need to synchronize
    return _isReadOnly;
}

void WorkspaceImpl::close()
{
    tt3::util::Lock _(_guard);

    if (_isOpen)
    {
        try
        {
            _database->close(); //  may throw
            _markClosed();
        }
        catch (const tt3::util::Exception & ex)
        {   //  OOPS! Ensure cleanup, translate & re-throw
            qCritical() << ex;
            _markClosed();
            WorkspaceException::translateAndThrow(ex);
        }
        catch (...)
        {   //  Ensure cleanup on throw
            _markClosed();
            throw;
        }
    }
}

void WorkspaceImpl::refresh()
{
    tt3::util::Lock _(_guard);

    try
    {
        _ensureOpen();
        _database->refresh();
        //  1.  Any proxy referring to a "dead" DB Object
        //      we don't need in the proxy cache - no DB
        //      query will ever return one of those,
        //  2.  Any proxy referred to ONLY from the proxy
        //      cache can be dropped.
        for (Oid oid : _proxyCache.keys())
        {
            if (!_proxyCache[oid]->_dataObject->isLive() ||
                _proxyCache[oid].use_count() == 1)
            {   //  Removal is safe - we're iterating over keys() snapshot
                _proxyCache.remove(oid);
            }
        }
    }
    catch (const tt3::util::Exception & ex)
    {   //  OOPS! Translate & re-throw
        WorkspaceException::translateAndThrow(ex);
    }
}

//////////
//  Operations (associations)
auto WorkspaceImpl::users(
        const Credentials & credentials
    ) const -> Users
{
    tt3::util::Lock _(_guard);
    _ensureOpen();  //  may throw

    try
    {
        Capabilities clientCapabilities = _validateAccessRights(credentials); //  may throw
        Users result;
        if (clientCapabilities.contains(Capability::Administrator) ||
            clientCapabilities.contains(Capability::ManageUsers))
        {   //  The caller can see all users
            for (tt3::db::api::IUser * dataUser : _database->users())
            {
                result.insert(_getProxy(dataUser));
            }
        }
        else
        {   //  The caller can only see himself
            tt3::db::api::IAccount * dataAccount =
                _database->login(credentials._login, credentials._password);
            tt3::db::api::IUser * dataUser = dataAccount->user();
            result.insert(_getProxy(dataUser));
        }
        return result;
    }
    catch (const tt3::util::Exception & ex)
    {   //  OOPS! Translate & re-throw
        WorkspaceException::translateAndThrow(ex);
    }
}

Accounts WorkspaceImpl::accounts(
        const Credentials & credentials
    ) const
{
    tt3::util::Lock _(_guard);
    _ensureOpen();  //  may throw

    try
    {
        Capabilities clientCapabilities = _validateAccessRights(credentials); //  may throw
        Accounts result;
        if (clientCapabilities.contains(Capability::Administrator) ||
            clientCapabilities.contains(Capability::ManageUsers))
        {   //  The caller can see all accounts if all users
            for (tt3::db::api::IAccount * dataAccount : _database->accounts())
            {
                result.insert(_getProxy(dataAccount));
            }
        }
        else
        {   //  The caller can only see his own account
            tt3::db::api::IAccount * dataAccount =
                _database->login(credentials._login, credentials._password);
            tt3::db::api::IUser * dataUser = dataAccount->user();
            for (tt3::db::api::IAccount * da : dataUser->accounts())
            {
                result.insert(_getProxy(da));
            }
        }
        return result;
    }
    catch (const tt3::util::Exception & ex)
    {   //  OOPS! Translate & re-throw
        WorkspaceException::translateAndThrow(ex);
    }
}

Account WorkspaceImpl::findAccount(
        const Credentials & credentials,
        const QString & login
    ) const
{
    tt3::util::Lock _(_guard);
    _ensureOpen();  //  may throw

    try
    {
        Capabilities clientCapabilities = _validateAccessRights(credentials); //  may throw
        tt3::db::api::IAccount * dataAccount = _database->findAccount(login);
        if (dataAccount != nullptr)
        {   //  Something found - but is it visible ?
            if (clientCapabilities.contains(Capability::Administrator) ||
                clientCapabilities.contains(Capability::ManageUsers))
            {   //  The caller can see all accounts if all users
                return _getProxy(dataAccount);
            }
            else
            {   //  The caller can only see his own account
                tt3::db::api::IAccount * dataCallerAccount =
                    _database->login(credentials._login, credentials._password);
                tt3::db::api::IUser * dataUser = dataCallerAccount->user();
                if (dataUser == dataAccount->user())
                {   //  Yes!
                    return _getProxy(dataAccount);
                }
            }
        }
        //  OOPS! Not found OR belongs to a wrong user
        return nullptr;
    }
    catch (const tt3::util::Exception & ex)
    {   //  OOPS! Translate & re-throw
        WorkspaceException::translateAndThrow(ex);
    }
}

auto WorkspaceImpl::activityTypes(
        const Credentials & credentials
    ) const -> ActivityTypes
{
    tt3::util::Lock _(_guard);
    _ensureOpen();  //  may throw

    try
    {
        _validateAccessRights(credentials);
        //  The caller can see all activity types
        return tt3::util::transform(
            _database->activityTypes(), //  may throw
            [&](auto dt)
            {
                return _getProxy(dt);
            });
    }
    catch (const tt3::util::Exception & ex)
    {   //  OOPS! Translate & re-throw
        WorkspaceException::translateAndThrow(ex);
    }
}

auto WorkspaceImpl::publicActivities(
        const Credentials & credentials
    ) const -> PublicActivities
{
    tt3::util::Lock _(_guard);
    _ensureOpen();  //  may throw

    try
    {
        _validateAccessRights(credentials);
        //  The caller can see all public activities
        return tt3::util::transform(
            _database->publicActivities(),  //  may throw
            [&](auto da)
            {
                return _getProxy(da);
            });
    }
    catch (const tt3::util::Exception & ex)
    {   //  OOPS! Translate & re-throw
        WorkspaceException::translateAndThrow(ex);
    }
}

auto WorkspaceImpl::publicActivitiesAndTasks(
        const Credentials & credentials
    ) const -> PublicActivities
{
    tt3::util::Lock _(_guard);
    _ensureOpen();  //  may throw

    try
    {
        _validateAccessRights(credentials);
        //  The caller can see all public activities
        return tt3::util::transform(
            _database->publicActivitiesAndTasks(),  //  may throw
            [&](auto da)
            {
                return _getProxy(da);
            });
    }
    catch (const tt3::util::Exception & ex)
    {   //  OOPS! Translate & re-throw
        WorkspaceException::translateAndThrow(ex);
    }
}

auto WorkspaceImpl::publicTasks(
        const Credentials & credentials
    ) const -> PublicTasks
{
    tt3::util::Lock _(_guard);
    _ensureOpen();  //  may throw

    try
    {
        _validateAccessRights(credentials);
        //  The caller can see all public tasks
        return tt3::util::transform(
            _database->publicTasks(),   //  may throw
            [&](auto dt)
            {
                return _getProxy(dt);
            });
    }
    catch (const tt3::util::Exception & ex)
    {   //  OOPS! Translate & re-throw
        WorkspaceException::translateAndThrow(ex);
    }
}

auto WorkspaceImpl::rootPublicTasks(
        const Credentials & credentials
    ) const -> PublicTasks
{
    tt3::util::Lock _(_guard);
    _ensureOpen();  //  may throw

    try
    {
        _validateAccessRights(credentials);
        //  The caller can see all public tasks
        return tt3::util::transform(
            _database->rootPublicTasks(),   //  may throw
            [&](auto dt)
            {
                return _getProxy(dt);
            });
    }
    catch (const tt3::util::Exception & ex)
    {   //  OOPS! Translate & re-throw
        WorkspaceException::translateAndThrow(ex);
    }
}

auto WorkspaceImpl::projects(
        const Credentials & credentials
    ) const -> Projects
{
    tt3::util::Lock _(_guard);
    _ensureOpen();  //  may throw

    try
    {
        _validateAccessRights(credentials);
        //  The caller can see all projects
        return tt3::util::transform(
            _database->projects(),  //  may throw
            [&](auto dp)
            {
                return _getProxy(dp);
            });
    }
    catch (const tt3::util::Exception & ex)
    {   //  OOPS! Translate & re-throw
        WorkspaceException::translateAndThrow(ex);
    }
}

auto WorkspaceImpl::rootProjects(
        const Credentials & credentials
    ) const -> Projects
{
    tt3::util::Lock _(_guard);
    _ensureOpen();  //  may throw

    try
    {
        _validateAccessRights(credentials);
        //  The caller can see all projects
        return tt3::util::transform(
            _database->rootProjects(),  //  may throw
            [&](auto dp)
            {
                return _getProxy(dp);
            });
    }
    catch (const tt3::util::Exception & ex)
    {   //  OOPS! Translate & re-throw
        WorkspaceException::translateAndThrow(ex);
    }
}

auto WorkspaceImpl::workStreams(
        const Credentials & credentials
    ) const -> WorkStreams
{
    tt3::util::Lock _(_guard);
    _ensureOpen();  //  may throw

    try
    {
        _validateAccessRights(credentials);
        //  The caller can see all WorkStreams
        return tt3::util::transform(
            _database->workStreams(),   //  may throw
            [&](auto dw)
            {
                return _getProxy(dw);
            });
    }
    catch (const tt3::util::Exception & ex)
    {   //  OOPS! Translate & re-throw
        WorkspaceException::translateAndThrow(ex);
    }
}

auto WorkspaceImpl::beneficiaries(
        const Credentials & credentials
    ) const -> Beneficiaries
{
    tt3::util::Lock _(_guard);
    _ensureOpen();  //  may throw

    try
    {
        _validateAccessRights(credentials);
        //  The caller can see all Beneficiaries
        return tt3::util::transform(
            _database->beneficiaries(), //  may throw
            [&](auto db)
            {
                return _getProxy(db);
            });
    }
    catch (const tt3::util::Exception & ex)
    {   //  OOPS! Translate & re-throw
        WorkspaceException::translateAndThrow(ex);
    }
}

//////////
//  Operations (access control)
bool WorkspaceImpl::canAccess(
        const Credentials & credentials
    ) const
{
    tt3::util::Lock _(_guard);
    _ensureOpen();

    try
    {
        _validateAccessRights(credentials); //  may throw
        return true;
    }
    catch (const AccessDeniedException &)
    {
        return false;
    }
    catch (const tt3::util::Exception & ex)
    {   //  ...but let other exceptions through
        WorkspaceException::translateAndThrow(ex);
    }
}

auto WorkspaceImpl::capabilities(
        const Credentials & credentials
    ) const -> Capabilities
{
    tt3::util::Lock _(_guard);
    _ensureOpen();

    try
    {
        return _validateAccessRights(credentials);  //  may throw
    }
    catch (const tt3::util::Exception & ex)
    {   //  OOPS! Translate & re-throw
        WorkspaceException::translateAndThrow(ex);
    }
}

bool WorkspaceImpl::grantsAll(
        const Credentials & credentials,
        Capabilities requiredCapabilities
    ) const
{
    tt3::util::Lock _(_guard);
    _ensureOpen();

    try
    {
        Q_ASSERT(!requiredCapabilities.isEmpty());

        //  Do the work; be defensive in release mode
        Capabilities c = _validateAccessRights(credentials);  //  may throw
        return (c & requiredCapabilities) == requiredCapabilities;
    }
    catch (const AccessDeniedException &)
    {   //  This is a special case
        return false;
    }
    catch (const tt3::util::Exception & ex)
    {   //  OOPS! Translate & re-throw
        WorkspaceException::translateAndThrow(ex);
    }
}

bool WorkspaceImpl::grantsAny(
        const Credentials & credentials,
        Capabilities requiredCapabilities
    ) const
{
    tt3::util::Lock _(_guard);
    _ensureOpen();

    try
    {
        Q_ASSERT(!requiredCapabilities.isEmpty());

        //  Do the work; be defensive in release mode
        Capabilities c = _validateAccessRights(credentials);  //  may throw
        return !(c & requiredCapabilities).isEmpty();
    }
    catch (const AccessDeniedException &)
    {   //  This is a special case
        return false;
    }
    catch (const tt3::util::Exception & ex)
    {   //  OOPS! Translate & re-throw
        WorkspaceException::translateAndThrow(ex);
    }
}

auto WorkspaceImpl::tryLogin(
        const Credentials & credentials
    ) const -> Account
{
    tt3::util::Lock _(_guard);
    _ensureOpen();

    try
    {
        tt3::db::api::IAccount * dataAccount =
            _database->tryLogin(credentials._login, credentials._password);
        return (dataAccount != nullptr) ?
                    _getProxy(dataAccount) :
                   Account();
    }
    catch (const tt3::util::Exception & ex)
    {   //  OOPS! Translate & re-throw
        WorkspaceException::translateAndThrow(ex);
    }
}

auto WorkspaceImpl::login(
        const Credentials & credentials
    ) const -> Account
{
    tt3::util::Lock _(_guard);
    _ensureOpen();

    try
    {
        tt3::db::api::IAccount * dataAccount =
            _database->login(credentials._login, credentials._password);
        return _getProxy(dataAccount);
    }
    catch (const tt3::util::Exception & ex)
    {   //  OOPS! Translate & re-throw
        WorkspaceException::translateAndThrow(ex);
    }
}

//////////
//  Operations (life cycle)
auto WorkspaceImpl::createUser(
        const Credentials & credentials,
        bool enabled,
        const QStringList & emailAddresses,
        const QString & realName,
        const InactivityTimeout & inactivityTimeout,
        const UiLocale & uiLocale,
        const Workloads & permittedWorkloads
    ) -> User
{
    tt3::util::Lock _(_guard);
    _ensureOpen();

    try
    {
        //  Validate access rights
        Capabilities clientCapabilities = _validateAccessRights(credentials); //  may throw
        if (!clientCapabilities.contains(Capability::Administrator) &&
            !clientCapabilities.contains(Capability::ManageUsers))
        {   //  OOPS! Can't!
            throw AccessDeniedException();
        }
        //  Do the work
        tt3::db::api::Workloads dataPermittedWorkloads =
            tt3::util::transform(
                permittedWorkloads,
                [](auto w)
                {   //  Be defensive when transforming nullptrs
                    return (w != nullptr) ? w->_dataWorkload : nullptr;
                });
        tt3::db::api::IUser * dataUser =
            _database->createUser(
                enabled, emailAddresses,
                realName, inactivityTimeout, uiLocale,
                dataPermittedWorkloads);
        return _getProxy(dataUser);;
    }
    catch (const tt3::util::Exception & ex)
    {   //  OOPS! Translate & re-throw
        WorkspaceException::translateAndThrow(ex);
    }
}

auto WorkspaceImpl::createActivityType(
        const Credentials & credentials,
        const QString & displayName,
        const QString & description
    ) -> ActivityType
{
    tt3::util::Lock _(_guard);
    _ensureOpen();

    try
    {
        //  Validate access rights
        Capabilities clientCapabilities = _validateAccessRights(credentials); //  may throw
        if (!clientCapabilities.contains(Capability::Administrator) &&
            !clientCapabilities.contains(Capability::ManageActivityTypes))
        {   //  OOPS! Can't!
            throw AccessDeniedException();
        }
        //  Do the work
        tt3::db::api::IActivityType * dataActivityType =
            _database->createActivityType(displayName, description);
        return _getProxy(dataActivityType);;
    }
    catch (const tt3::util::Exception & ex)
    {   //  OOPS! Translate & re-throw
        WorkspaceException::translateAndThrow(ex);
    }
}

auto WorkspaceImpl::createPublicActivity(
        const Credentials & credentials,
        const QString & displayName,
        const QString & description,
        const InactivityTimeout & timeout,
        bool requireCommentOnStart,
        bool requireCommentOnStop,
        bool fullScreenReminder,
        ActivityType activityType,
        Workload workload
    ) -> PublicActivity
{
    tt3::util::Lock _(_guard);
    _ensureOpen();

    try
    {
        //  Validate access rights
        Capabilities clientCapabilities = _validateAccessRights(credentials); //  may throw
        if (!clientCapabilities.contains(Capability::Administrator) &&
            !clientCapabilities.contains(Capability::ManagePublicActivities))
        {   //  OOPS! Can't!
            throw AccessDeniedException();
        }
        //  Do the work
        tt3::db::api::IPublicActivity * dataPublicActivity =
            _database->createPublicActivity(
                displayName,
                description,
                timeout,
                requireCommentOnStart,
                requireCommentOnStop,
                fullScreenReminder,
                (activityType != nullptr) ? activityType->_dataActivityType : nullptr,
                (workload != nullptr) ? workload->_dataWorkload : nullptr);
        return _getProxy(dataPublicActivity);;
    }
    catch (const tt3::util::Exception & ex)
    {   //  OOPS! Translate & re-throw
        WorkspaceException::translateAndThrow(ex);
    }
}

auto WorkspaceImpl::createPublicTask(
        const Credentials & credentials,
        const QString & displayName,
        const QString & description,
        const InactivityTimeout & timeout,
        bool requireCommentOnStart,
        bool requireCommentOnStop,
        bool fullScreenReminder,
        ActivityType activityType,
        Workload workload,
        bool completed,
        bool requireCommentOnCompletion
    ) -> PublicTask
{
    tt3::util::Lock _(_guard);
    _ensureOpen();

    try
    {
        //  Validate access rights
        Capabilities clientCapabilities = _validateAccessRights(credentials); //  may throw
        if (!clientCapabilities.contains(Capability::Administrator) &&
            !clientCapabilities.contains(Capability::ManagePublicTasks))
        {   //  OOPS! Can't!
            throw AccessDeniedException();
        }
        //  Do the work
        tt3::db::api::IPublicTask * dataPublicTask =
            _database->createPublicTask(
                displayName,
                description,
                timeout,
                requireCommentOnStart,
                requireCommentOnStop,
                fullScreenReminder,
                (activityType != nullptr) ? activityType->_dataActivityType : nullptr,
                (workload != nullptr) ? workload->_dataWorkload : nullptr,
                completed,
                requireCommentOnCompletion);
        return _getProxy(dataPublicTask);;
    }
    catch (const tt3::util::Exception & ex)
    {   //  OOPS! Translate & re-throw
        WorkspaceException::translateAndThrow(ex);
    }
}

auto WorkspaceImpl::createProject(
        const Credentials & credentials,
        const QString & displayName,
        const QString & description,
        const Beneficiaries & beneficiaries,
        bool completed
    ) -> Project
{
    tt3::util::Lock _(_guard);
    _ensureOpen();

    try
    {
        //  Validate access rights
        Capabilities clientCapabilities = _validateAccessRights(credentials); //  may throw
        if (!clientCapabilities.contains(Capability::Administrator) &&
            !clientCapabilities.contains(Capability::ManageWorkloads))
        {   //  OOPS! Can't!
            throw AccessDeniedException();
        }

        //  Do the work
        tt3::db::api::Beneficiaries dataBeneficiaries =
            tt3::util::transform(
                beneficiaries,
                [](auto b)
                {   //  Be defensive when transforming nullptrs
                    return (b != nullptr) ? b->_dataBeneficiary : nullptr;
                });
        tt3::db::api::IProject * dataProject =
            _database->createProject(
                displayName,
                description,
                dataBeneficiaries,
                completed);
        return _getProxy(dataProject);;
    }
    catch (const tt3::util::Exception & ex)
    {   //  OOPS! Translate & re-throw
        WorkspaceException::translateAndThrow(ex);
    }
}

auto WorkspaceImpl::createWorkStream(
        const Credentials & credentials,
        const QString & displayName,
        const QString & description,
        const Beneficiaries & beneficiaries
    ) -> WorkStream
{
    tt3::util::Lock _(_guard);
    _ensureOpen();

    try
    {
        //  Validate access rights
        Capabilities clientCapabilities = _validateAccessRights(credentials); //  may throw
        if (!clientCapabilities.contains(Capability::Administrator) &&
            !clientCapabilities.contains(Capability::ManageWorkloads))
        {   //  OOPS! Can't!
            throw AccessDeniedException();
        }

        //  Do the work
        tt3::db::api::Beneficiaries dataBeneficiaries =
            tt3::util::transform(
                beneficiaries,
                [](auto b)
                {   //  Be defensive when transforming nullptrs
                    return (b != nullptr) ? b->_dataBeneficiary : nullptr;
                });
        tt3::db::api::IWorkStream * dataWorkStream =
            _database->createWorkStream(
                displayName,
                description,
                dataBeneficiaries);
        return _getProxy(dataWorkStream);;
    }
    catch (const tt3::util::Exception & ex)
    {   //  OOPS! Translate & re-throw
        WorkspaceException::translateAndThrow(ex);
    }
}

auto WorkspaceImpl::createBeneficiary(
        const Credentials & credentials,
        const QString & displayName,
        const QString & description,
        const Workloads & workloads
    ) -> Beneficiary
{
    tt3::util::Lock _(_guard);
    _ensureOpen();

    try
    {
        //  Validate access rights
        Capabilities clientCapabilities = _validateAccessRights(credentials); //  may throw
        if (!clientCapabilities.contains(Capability::Administrator) &&
            !clientCapabilities.contains(Capability::ManageWorkloads))
        {   //  OOPS! Can't!
            throw AccessDeniedException();
        }
        //  Do the work
        tt3::db::api::Workloads dataWorkloads =
            tt3::util::transform(
                workloads,
                [](auto w)
                {   //  Be defensive when transforming nullptrs
                    return (w != nullptr) ? w->_dataWorkload : nullptr;
                });
        tt3::db::api::IBeneficiary * dataBeneficiary =
            _database->createBeneficiary(
                displayName,
                description,
                dataWorkloads);
        return _getProxy(dataBeneficiary);;
    }
    catch (const tt3::util::Exception & ex)
    {   //  OOPS! Translate & re-throw
        WorkspaceException::translateAndThrow(ex);
    }
}

//////////
//  Operations (special access)
auto WorkspaceImpl::beginBackup(
        const Credentials & credentials
    ) -> BackupCredentials
{
    try
    {   //  We need to obtain the db::api-level
        //  lock in an UNGUARDED state!
        std::unique_ptr<tt3::db::api::IDatabaseLock> dataLock
            { _database->lock(tt3::db::api::IDatabaseLock::LockType::Read) };   //  may throw
        //  Lock obtained - the rest is guarded
        tt3::util::Lock _(_guard);
        _ensureOpen();
        //  Validate access rights
        Capabilities clientCapabilities = _validateAccessRights(credentials); //  may throw
        if (!clientCapabilities.contains(Capability::Administrator) &&
            !clientCapabilities.contains(Capability::BackupAndRestore))
        {   //  OOPS! Can't!
            throw AccessDeniedException();  //  releases the dataLock
        }
        //  Determine lease period TODO based on database size
        for (; ; )
        {   //  Loop, on the off-chance of duplicate credentials
            QString login = QUuid::createUuid().toString();     //  be random
            QString password = QUuid::createUuid().toString();  //  be random
            long leasePeriodMs = 24 * 60 * 60 * 1000;   //  TODO depending on database size!
            QDateTime now = QDateTime::currentDateTimeUtc();
            BackupCredentials backupCredentials(login, password, now, now.addMSecs(leasePeriodMs));
            if (_database->findAccount(login) == nullptr &&     //  may throw!
                !_backupCredentials.contains(backupCredentials))
            {   //  Np conflict
                _backupCredentials[backupCredentials] = dataLock.release();
                return backupCredentials;
            }   //  else keep randomizing
        }
    }
    catch (const tt3::util::Exception & ex)
    {   //  OOPS! Translate & re-throw
        WorkspaceException::translateAndThrow(ex);
    }
}

auto WorkspaceImpl::beginRestore(
    const Credentials & credentials
    ) -> RestoreCredentials
{
    try
    {   //  We need to obtain the db::api-level
        //  lock in an UNGUARDED state!
        std::unique_ptr<tt3::db::api::IDatabaseLock> dataLock
            { _database->lock(tt3::db::api::IDatabaseLock::LockType::Write) };   //  may throw
        //  Lock obtained - the rest is guarded
        tt3::util::Lock _(_guard);
        _ensureOpen();
        //  Validate access rights
        Capabilities clientCapabilities = _validateAccessRights(credentials); //  may throw
        if (!clientCapabilities.contains(Capability::Administrator) &&
            !clientCapabilities.contains(Capability::BackupAndRestore))
        {   //  OOPS! Can't!
            throw AccessDeniedException();  //  releases the dataLock
        }
        //  Determine lease period TODO based on database size
        for (; ; )
        {   //  Loop, on the off-chance of duplicate credentials
            QString login = QUuid::createUuid().toString();     //  be random
            QString password = QUuid::createUuid().toString();  //  be random
            long leasePeriodMs = 24 * 60 * 60 * 1000;   //  TODO depending on database size!
            QDateTime now = QDateTime::currentDateTimeUtc();
            RestoreCredentials restoreCredentials(login, password, now, now.addMSecs(leasePeriodMs));
            if (_database->findAccount(login) == nullptr &&     //  may throw!
                !_restoreCredentials.contains(restoreCredentials))
            {   //  Np conflict
                _restoreCredentials[restoreCredentials] = dataLock.release();
                return restoreCredentials;
            }   //  else keep randomizing
        }
    }
    catch (const tt3::util::Exception & ex)
    {   //  OOPS! Translate & re-throw
        WorkspaceException::translateAndThrow(ex);
    }
}

auto WorkspaceImpl::beginReport(
        const Credentials & credentials
    ) -> ReportCredentials
{
    try
    {   //  We need to obtain the db::api-level
        //  lock in an UNGUARDED state!
        std::unique_ptr<tt3::db::api::IDatabaseLock> dataLock
            { _database->lock(tt3::db::api::IDatabaseLock::LockType::Read) };   //  may throw
        //  Lock obtained - the rest is guarded
        tt3::util::Lock _(_guard);
        _ensureOpen();
        //  Validate access rights
        Capabilities clientCapabilities = _validateAccessRights(credentials); //  may throw
        if (!clientCapabilities.contains(Capability::Administrator) &&
            !clientCapabilities.contains(Capability::GenerateReports))
        {   //  OOPS! Can't!
            throw AccessDeniedException();  //  releases the dataLock
        }
        //  Determine lease period TODO based on database size
        for (; ; )
        {   //  Loop, on the off-chance of duplicate credentials
            QString login = QUuid::createUuid().toString();     //  be random
            QString password = QUuid::createUuid().toString();  //  be random
            long leasePeriodMs = 24 * 60 * 60 * 1000;   //  TODO depending on database size!
            QDateTime now = QDateTime::currentDateTimeUtc();
            ReportCredentials reportCredentials(login, password, now, now.addMSecs(leasePeriodMs));
            if (_database->findAccount(login) == nullptr &&     //  may throw!
                !_reportCredentials.contains(reportCredentials))
            {   //  Np conflict
                _reportCredentials[reportCredentials] = dataLock.release();
                return reportCredentials;
            }   //  else keep randomizing
        }
    }
    catch (const tt3::util::Exception & ex)
    {   //  OOPS! Translate & re-throw
        WorkspaceException::translateAndThrow(ex);
    }
}

void WorkspaceImpl::releaseCredentials(
        const BackupCredentials & backupCredentials
    )
{
    tt3::util::Lock _(_guard);
    _ensureOpen();

    try
    {
        //  Do the work
        if (_backupCredentials.contains(backupCredentials))
        {
            delete _backupCredentials[backupCredentials];
            _backupCredentials.remove(backupCredentials);
        }
    }
    catch (const tt3::util::Exception & ex)
    {   //  OOPS! Translate & re-throw
        WorkspaceException::translateAndThrow(ex);
    }
}

void WorkspaceImpl::releaseCredentials(
        const RestoreCredentials & restoreCredentials
    )
{
    tt3::util::Lock _(_guard);
    _ensureOpen();

    try
    {
        //  Do the work
        if (_restoreCredentials.contains(restoreCredentials))
        {
            delete _restoreCredentials[restoreCredentials];
            _restoreCredentials.remove(restoreCredentials);
        }
    }
    catch (const tt3::util::Exception & ex)
    {   //  OOPS! Translate & re-throw
        WorkspaceException::translateAndThrow(ex);
    }
}

void WorkspaceImpl::releaseCredentials(
        const ReportCredentials & reportCredentials
    )
{
    tt3::util::Lock _(_guard);
    _ensureOpen();

    try
    {
        //  Do the work
        if (_reportCredentials.contains(reportCredentials))
        {
            delete _reportCredentials[reportCredentials];
            _reportCredentials.remove(reportCredentials);
        }
    }
    catch (const tt3::util::Exception & ex)
    {   //  OOPS! Translate & re-throw
        WorkspaceException::translateAndThrow(ex);
    }
}

//////////
//  Implementation helpers
void WorkspaceImpl::_ensureOpen() const
{
    Q_ASSERT(_guard.isLockedByCurrentThread());

    if (!_isOpen)
    {
        throw WorkspaceClosedException();
    }
    //  If the underlying Database is read-only, a
    //  DatabaseException will be thrown when an attempt
    //  is made to modify it, and will be translated to
    //  a WorkspaceException and re-thrown.
}

void WorkspaceImpl::_markClosed()
{
    Q_ASSERT(_guard.isLockedByCurrentThread());

    _isOpen = false;
    //  Clear caches
    _goodCredentialsCache.clear();
    _badCredentialsCache.clear();
    _proxyCache.clear();
    for (auto dataDatabaseLock : _backupCredentials.values())
    {
        delete dataDatabaseLock;
    }
    for (auto dataDatabaseLock : _restoreCredentials.values())
    {
        delete dataDatabaseLock;
    }
    for (auto dataDatabaseLock : _reportCredentials.values())
    {
        delete dataDatabaseLock;
    }
    _backupCredentials.clear();
    _restoreCredentials.clear();
    _reportCredentials.clear();
    //  The "database closed" notification fro m the
    //  database will be missed, as database (along with
    //  its change notifier) no longer existsm so we
    //  need to fake a "workspace closed" signal
    emit workspaceClosed(type()->_mapWorkspace(const_cast<WorkspaceImpl*>(this)));
}

auto WorkspaceImpl::_validateAccessRights(
        const Credentials & credentials
    ) const -> Capabilities
{
    Q_ASSERT(_guard.isLockedByCurrentThread());
    Q_ASSERT(_isOpen);

    //  Is the answer already known ?
    if (_goodCredentialsCache.contains(credentials))
    {
        return _goodCredentialsCache[credentials];
    }
    if (_badCredentialsCache.contains(credentials))
    {
        throw AccessDeniedException();
    }
    //  We need to query, but keep access cache size in check
    if (_goodCredentialsCache.size() + _badCredentialsCache.size() > _AccessCacheSizeCap)
    {
        _goodCredentialsCache.clear();
        _badCredentialsCache.clear();
    }
    try
    {
        if (auto dataAccount = _database->tryLogin(credentials._login, credentials._password))  //  may throw
        {
            Capabilities capabilities = dataAccount->capabilities();    //  may throw
            _goodCredentialsCache.insert(credentials, capabilities);
            return capabilities;
        }
        else
        {
            _badCredentialsCache.insert(credentials);
            throw AccessDeniedException();
        }
    }
    catch (const tt3::util::Exception & ex)
    {   //  OOPS! Data layer error
        WorkspaceException::translateAndThrow(ex);
    }
}

User WorkspaceImpl::_getProxy(tt3::db::api::IUser * dataUser) const
{
    Q_ASSERT(_guard.isLockedByCurrentThread());
    Q_ASSERT(_isOpen);
    Q_ASSERT(dataUser != nullptr);

    Oid oid = dataUser->oid();
    if (_proxyCache.contains(oid))
    {
        User user = std::dynamic_pointer_cast<UserImpl>(_proxyCache[oid]);
        Q_ASSERT(user != nullptr);   //  Objects do not change their types OR reuse OIDs
        return user;
    }
    //  Must create a new proxy
    Workspace workspace = _address->_workspaceType->_mapWorkspace(const_cast<WorkspaceImpl*>(this));
    User user(
        new UserImpl(workspace, dataUser),
        [](UserImpl * p) { delete p; });
    _proxyCache.insert(oid, user);
    return user;
}

Account WorkspaceImpl::_getProxy(tt3::db::api::IAccount * dataAccount) const
{
    Q_ASSERT(_guard.isLockedByCurrentThread());
    Q_ASSERT(_isOpen);
    Q_ASSERT(dataAccount != nullptr);

    Oid oid = dataAccount->oid();
    if (_proxyCache.contains(oid))
    {
        Account account = std::dynamic_pointer_cast<AccountImpl>(_proxyCache[oid]);
        Q_ASSERT(account != nullptr);   //  Objects do not change their types OR reuse OIDs
        return account;
    }
    //  Must create a new proxy
    Workspace workspace = _address->_workspaceType->_mapWorkspace(const_cast<WorkspaceImpl*>(this));
    Account account(
        new AccountImpl(workspace, dataAccount),
        [](AccountImpl * p) { delete p; });
    _proxyCache.insert(oid, account);
    return account;
}

ActivityType WorkspaceImpl::_getProxy(tt3::db::api::IActivityType * dataActivityType) const
{
    Q_ASSERT(_guard.isLockedByCurrentThread());
    Q_ASSERT(_isOpen);
    Q_ASSERT(dataActivityType != nullptr);

    Oid oid = dataActivityType->oid();
    if (_proxyCache.contains(oid))
    {
        ActivityType activityType = std::dynamic_pointer_cast<ActivityTypeImpl>(_proxyCache[oid]);
        Q_ASSERT(activityType != nullptr);   //  Objects do not change their types OR reuse OIDs
        return activityType;
    }
    //  Must create a new proxy
    Workspace workspace = _address->_workspaceType->_mapWorkspace(const_cast<WorkspaceImpl*>(this));
    ActivityType activityType(
        new ActivityTypeImpl(workspace, dataActivityType),
        [](ActivityTypeImpl * p) { delete p; });
    _proxyCache.insert(oid, activityType);
    return activityType;
}

auto WorkspaceImpl::_getProxy(
        tt3::db::api::IActivity * dataActivity
    ) const -> Activity
{
    Q_ASSERT(_guard.isLockedByCurrentThread());
    Q_ASSERT(_isOpen);
    Q_ASSERT(dataActivity != nullptr);

    if (auto dataPublicTask =
        dynamic_cast<tt3::db::api::IPublicTask*>(dataActivity))
    {   //  ...then use the dedicated proxy getter
        return std::dynamic_pointer_cast<ActivityImpl>(_getProxy(dataPublicTask));
    }
    if (auto dataPrivateTask =
        dynamic_cast<tt3::db::api::IPrivateTask*>(dataActivity))
    {   //  ...then use the dedicated proxy getter
        return std::dynamic_pointer_cast<ActivityImpl>(_getProxy(dataPrivateTask));
    }
    if (auto dataPublicActivity =
        dynamic_cast<tt3::db::api::IPublicActivity*>(dataActivity))
    {   //  ...then use the dedicated proxy getter
        return std::dynamic_pointer_cast<ActivityImpl>(_getProxy(dataPublicActivity));
    }
    if (auto dataPrivateActivity =
        dynamic_cast<tt3::db::api::IPrivateActivity*>(dataActivity))
    {   //  ...then use the dedicated proxy getter
        return std::dynamic_pointer_cast<ActivityImpl>(_getProxy(dataPrivateActivity));
    }
    Q_ASSERT(false);    //  Can't happen!
    return nullptr;
}

PublicActivity WorkspaceImpl::_getProxy(
        tt3::db::api::IPublicActivity * dataPublicActivity
    ) const
{   //  What if it's a PublicTask?
    if (auto dataPublicTask =
            dynamic_cast<tt3::db::api::IPublicTask *>(dataPublicActivity))
    {   //  ...then use the dedicated proxy getter
        return std::dynamic_pointer_cast<PublicActivityImpl>(_getProxy(dataPublicTask));
    }

    //  Do the work
    Q_ASSERT(_guard.isLockedByCurrentThread());
    Q_ASSERT(_isOpen);
    Q_ASSERT(dataPublicActivity != nullptr);

    Oid oid = dataPublicActivity->oid();
    if (_proxyCache.contains(oid))
    {
        PublicActivity publicActivity = std::dynamic_pointer_cast<PublicActivityImpl>(_proxyCache[oid]);
        Q_ASSERT(publicActivity != nullptr);   //  Objects do not change their types OR reuse OIDs
        return publicActivity;
    }
    //  Must create a new proxy
    Workspace workspace = _address->_workspaceType->_mapWorkspace(const_cast<WorkspaceImpl*>(this));
    PublicActivity publicActivity(
        new PublicActivityImpl(workspace, dataPublicActivity),
        [](PublicActivityImpl * p) { delete p; });
    _proxyCache.insert(oid, publicActivity);
    return publicActivity;
}

PublicTask WorkspaceImpl::_getProxy(
        tt3::db::api::IPublicTask * dataPublicTask
    ) const
{
    Q_ASSERT(_guard.isLockedByCurrentThread());
    Q_ASSERT(_isOpen);
    Q_ASSERT(dataPublicTask != nullptr);

    Oid oid = dataPublicTask->oid();
    if (_proxyCache.contains(oid))
    {
        PublicTask publicTask = std::dynamic_pointer_cast<PublicTaskImpl>(_proxyCache[oid]);
        Q_ASSERT(publicTask != nullptr);   //  Objects do not change their types OR reuse OIDs
        return publicTask;
    }
    //  Must create a new proxy
    Workspace workspace = _address->_workspaceType->_mapWorkspace(const_cast<WorkspaceImpl*>(this));
    PublicTask publicTask(
        new PublicTaskImpl(workspace, dataPublicTask),
        [](PublicTaskImpl * p) { delete p; });
    _proxyCache.insert(oid, publicTask);
    return publicTask;
}

PrivateActivity WorkspaceImpl::_getProxy(
        tt3::db::api::IPrivateActivity * dataPrivateActivity
    ) const
{   //  What if it's a PrivateTask?
    if (auto dataPrivateTask =
        dynamic_cast<tt3::db::api::IPrivateTask *>(dataPrivateActivity))
    {   //  ...then use the dedicated proxy getter
        return std::dynamic_pointer_cast<PrivateActivityImpl>(_getProxy(dataPrivateTask));
    }

    //  Do the work
    Q_ASSERT(_guard.isLockedByCurrentThread());
    Q_ASSERT(_isOpen);
    Q_ASSERT(dataPrivateActivity != nullptr);

    Oid oid = dataPrivateActivity->oid();
    if (_proxyCache.contains(oid))
    {
        PrivateActivity privateActivity = std::dynamic_pointer_cast<PrivateActivityImpl>(_proxyCache[oid]);
        Q_ASSERT(privateActivity != nullptr);   //  Objects do not change their types OR reuse OIDs
        return privateActivity;
    }
    //  Must create a new proxy
    Workspace workspace = _address->_workspaceType->_mapWorkspace(const_cast<WorkspaceImpl*>(this));
    PrivateActivity privateActivity(
        new PrivateActivityImpl(workspace, dataPrivateActivity),
        [](PrivateActivityImpl * p) { delete p; });
    _proxyCache.insert(oid, privateActivity);
    return privateActivity;
}

auto WorkspaceImpl::_getProxy(
        tt3::db::api::IPrivateTask * dataPrivateTask
    ) const -> PrivateTask
{
    Q_ASSERT(_guard.isLockedByCurrentThread());
    Q_ASSERT(_isOpen);
    Q_ASSERT(dataPrivateTask != nullptr);

    Oid oid = dataPrivateTask->oid();
    if (_proxyCache.contains(oid))
    {
        PrivateTask privateTask = std::dynamic_pointer_cast<PrivateTaskImpl>(_proxyCache[oid]);
        Q_ASSERT(privateTask != nullptr);   //  Objects do not change their types OR reuse OIDs
        return privateTask;
    }
    //  Must create a new proxy
    Workspace workspace = _address->_workspaceType->_mapWorkspace(const_cast<WorkspaceImpl*>(this));
    PrivateTask privateTask(
        new PrivateTaskImpl(workspace, dataPrivateTask),
        [](PrivateTaskImpl * p) { delete p; });
    _proxyCache.insert(oid, privateTask);
    return privateTask;
}

Workload WorkspaceImpl::_getProxy(
        tt3::db::api::IWorkload * dataWorkload
    ) const
{
    Q_ASSERT(_guard.isLockedByCurrentThread());
    Q_ASSERT(_isOpen);
    Q_ASSERT(dataWorkload != nullptr);

    if (auto dataProject =
        dynamic_cast<tt3::db::api::IProject*>(dataWorkload))
    {   //  ...then use the dedicated proxy getter
        return std::dynamic_pointer_cast<WorkloadImpl>(_getProxy(dataProject));
    }
    if (auto dataWorkStream =
        dynamic_cast<tt3::db::api::IWorkStream*>(dataWorkload))
    {   //  ...then use the dedicated proxy getter
        return std::dynamic_pointer_cast<WorkloadImpl>(_getProxy(dataWorkStream));
    }
    Q_ASSERT(false);    //  Can't happen!
    return nullptr;
}

Project WorkspaceImpl::_getProxy(
        tt3::db::api::IProject * dataProject
    ) const
{
    Q_ASSERT(_guard.isLockedByCurrentThread());
    Q_ASSERT(_isOpen);
    Q_ASSERT(dataProject != nullptr);

    Oid oid = dataProject->oid();
    if (_proxyCache.contains(oid))
    {
        Project project = std::dynamic_pointer_cast<ProjectImpl>(_proxyCache[oid]);
        Q_ASSERT(project != nullptr);   //  Objects do not change their types OR reuse OIDs
        return project;
    }
    //  Must create a new proxy
    Workspace workspace = _address->_workspaceType->_mapWorkspace(const_cast<WorkspaceImpl*>(this));
    Project project(
        new ProjectImpl(workspace, dataProject),
        [](ProjectImpl * p) { delete p; });
    _proxyCache.insert(oid, project);
    return project;
}

WorkStream WorkspaceImpl::_getProxy(
        tt3::db::api::IWorkStream * dataWorkStream
    ) const
{
    Q_ASSERT(_guard.isLockedByCurrentThread());
    Q_ASSERT(_isOpen);
    Q_ASSERT(dataWorkStream != nullptr);

    Oid oid = dataWorkStream->oid();
    if (_proxyCache.contains(oid))
    {
        WorkStream workStream = std::dynamic_pointer_cast<WorkStreamImpl>(_proxyCache[oid]);
        Q_ASSERT(workStream != nullptr);   //  Objects do not change their types OR reuse OIDs
        return workStream;
    }
    //  Must create a new proxy
    Workspace workspace = _address->_workspaceType->_mapWorkspace(const_cast<WorkspaceImpl*>(this));
    WorkStream workStream(
        new WorkStreamImpl(workspace, dataWorkStream),
        [](WorkStreamImpl * p) { delete p; });
    _proxyCache.insert(oid, workStream);
    return workStream;
}

Beneficiary WorkspaceImpl::_getProxy(
        tt3::db::api::IBeneficiary * dataBeneficiary
    ) const
{
    Q_ASSERT(_guard.isLockedByCurrentThread());
    Q_ASSERT(_isOpen);
    Q_ASSERT(dataBeneficiary != nullptr);

    Oid oid = dataBeneficiary->oid();
    if (_proxyCache.contains(oid))
    {
        Beneficiary beneficiary = std::dynamic_pointer_cast<BeneficiaryImpl>(_proxyCache[oid]);
        Q_ASSERT(beneficiary != nullptr);   //  Objects do not change their types OR reuse OIDs
        return beneficiary;
    }
    //  Must create a new proxy
    Workspace workspace = _address->_workspaceType->_mapWorkspace(const_cast<WorkspaceImpl*>(this));
    Beneficiary beneficiary(
        new BeneficiaryImpl(workspace, dataBeneficiary),
        [](BeneficiaryImpl * p) { delete p; });
    _proxyCache.insert(oid, beneficiary);
    return beneficiary;
}

Work WorkspaceImpl::_getProxy(
        tt3::db::api::IWork * dataWork
    ) const
{
    Q_ASSERT(_guard.isLockedByCurrentThread());
    Q_ASSERT(_isOpen);
    Q_ASSERT(dataWork != nullptr);

    Oid oid = dataWork->oid();
    if (_proxyCache.contains(oid))
    {
        Work work = std::dynamic_pointer_cast<WorkImpl>(_proxyCache[oid]);
        Q_ASSERT(work != nullptr);   //  Objects do not change their types OR reuse OIDs
        return work;
    }
    //  Must create a new proxy
    Workspace workspace = _address->_workspaceType->_mapWorkspace(const_cast<WorkspaceImpl*>(this));
    Work work(
        new WorkImpl(workspace, dataWork),
        [](WorkImpl * p) { delete p; });
    _proxyCache.insert(oid, work);
    return work;
}

Event WorkspaceImpl::_getProxy(
        tt3::db::api::IEvent * dataEvent
    ) const
{
    Q_ASSERT(_guard.isLockedByCurrentThread());
    Q_ASSERT(_isOpen);
    Q_ASSERT(dataEvent != nullptr);

    Oid oid = dataEvent->oid();
    if (_proxyCache.contains(oid))
    {
        Event event = std::dynamic_pointer_cast<EventImpl>(_proxyCache[oid]);
        Q_ASSERT(event != nullptr);   //  Objects do not change their types OR reuse OIDs
        return event;
    }
    //  Must create a new proxy
    Workspace workspace = _address->_workspaceType->_mapWorkspace(const_cast<WorkspaceImpl*>(this));
    Event event(
        new EventImpl(workspace, dataEvent),
        [](EventImpl * p) { delete p; });
    _proxyCache.insert(oid, event);
    return event;
}

void WorkspaceImpl::_clearExpiredBackupCredentials()
{
    QDateTime now = QDateTime::currentDateTimeUtc();
    for (auto key : _backupCredentials.keys())
    {
        if (now >= key._expireAt)
        {
            delete _backupCredentials[key]; //  release the lock
            _backupCredentials.remove(key);
        }
    }
}

void WorkspaceImpl::_clearExpiredRestoreCredentials()
{
    QDateTime now = QDateTime::currentDateTimeUtc();
    for (auto key : _restoreCredentials.keys())
    {
        if (now >= key._expireAt)
        {
            delete _restoreCredentials[key]; //  release the lock
            _restoreCredentials.remove(key);
        }
    }
}

void WorkspaceImpl::_clearExpiredReportCredentials()
{
    QDateTime now = QDateTime::currentDateTimeUtc();
    for (auto key : _reportCredentials.keys())
    {
        if (now >= key._expireAt)
        {
            delete _reportCredentials[key]; //  release the lock
            _reportCredentials.remove(key);
        }
    }
}

//////////
//  Signal handlers
void WorkspaceImpl::_onDatabaseClosed(tt3::db::api::DatabaseClosedNotification notification)
{
    Q_ASSERT(notification.database() == _database);
    //  Stop listening to database notifications
    disconnect(_database->changeNotifier(),
               &tt3::db::api::ChangeNotifier::databaseClosed,
               this,
               &WorkspaceImpl::_onDatabaseClosed);
    disconnect(_database->changeNotifier(),
               &tt3::db::api::ChangeNotifier::objectCreated,
               this,
               &WorkspaceImpl::_onObjectCreated);
    disconnect(_database->changeNotifier(),
               &tt3::db::api::ChangeNotifier::objectDestroyed,
               this,
               &WorkspaceImpl::_onObjectDestroyed);
    disconnect(_database->changeNotifier(),
               &tt3::db::api::ChangeNotifier::objectModified,
               this,
               &WorkspaceImpl::_onObjectModified);
    //  Translate & re-issue
    emit workspaceClosed(
        WorkspaceClosedNotification(
            _address->_workspaceType->_mapWorkspace(this)));
}

void WorkspaceImpl::_onObjectCreated(tt3::db::api::ObjectCreatedNotification notification)
{
    Q_ASSERT(notification.database() == _database);
    //  Any change to users/accounts must drop the access rights cache
    if (notification.objectType() == ObjectTypes::User::instance() ||
        notification.objectType() == ObjectTypes::Account::instance())
    {
        tt3::util::Lock _(_guard);
        _goodCredentialsCache.clear();
        _badCredentialsCache.clear();
    }
    //  Translate & re-issue
    emit objectCreated(
        ObjectCreatedNotification(
            _address->_workspaceType->_mapWorkspace(this),
            notification.objectType(),
            notification.oid()));
}

void WorkspaceImpl::_onObjectDestroyed(tt3::db::api::ObjectDestroyedNotification notification)
{
    Q_ASSERT(notification.database() == _database);
    //  Any change to users/accounts must drop the access rights cache
    if (notification.objectType() == ObjectTypes::User::instance() ||
        notification.objectType() == ObjectTypes::Account::instance())
    {
        tt3::util::Lock _(_guard);
        _goodCredentialsCache.clear();
        _badCredentialsCache.clear();
        //  There's no need to cache destroyed objects
        _proxyCache.remove(notification.oid());
    }
    else
    {   //  There's no need to cache destroyed objects
        tt3::util::Lock _(_guard);
        _proxyCache.remove(notification.oid());
    }
    //  Translate & re-issue
    emit objectDestroyed(
        ObjectDestroyedNotification(
            _address->_workspaceType->_mapWorkspace(this),
            notification.objectType(),
            notification.oid()));
}

void WorkspaceImpl::_onObjectModified(tt3::db::api::ObjectModifiedNotification notification)
{
    Q_ASSERT(notification.database() == _database);
    //  Any change to users/accounts must drop the access rights cache
    if (notification.objectType() == ObjectTypes::User::instance() ||
        notification.objectType() == ObjectTypes::Account::instance())
    {
        tt3::util::Lock _(_guard);
        _goodCredentialsCache.clear();
        _badCredentialsCache.clear();
    }
    //  Translate & re-issue
    emit objectModified(
        ObjectModifiedNotification(
            _address->_workspaceType->_mapWorkspace(this),
            notification.objectType(),
            notification.oid()));
}

//  End of tt3-ws/WorkspaceImpl.cpp
