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
        qCritical() << ex.errorMessage();
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
    tt3::util::Lock lock(_guard);

    return _isOpen;
}

bool WorkspaceImpl::isReadOnly() const
{   //  No need to synchronize
    return _isReadOnly;
}

void WorkspaceImpl::close()
{
    tt3::util::Lock lock(_guard);

    if (_isOpen)
    {
        try
        {
            _database->close(); //  may throw
            _markClosed();
        }
        catch (const tt3::util::Exception & ex)
        {   //  OOPS! Ensure cleanup, translate & re-throw
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
    tt3::util::Lock lock(_guard);

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
    {
        WorkspaceException::translateAndThrow(ex);
    }
}

//////////
//  Operations (associations)
auto WorkspaceImpl::users(
        const Credentials & credentials
    ) const -> Users
{
    tt3::util::Lock lock(_guard);
    _ensureOpen();  //  may throw

    try
    {
        Capabilities clientCapabilities = _validateAccessRights(credentials); //  may throw
        Users result;
        if ((clientCapabilities & Capabilities::Administrator) != Capabilities::None ||
            (clientCapabilities & Capabilities::ManageUsers) != Capabilities::None)
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
    {
        WorkspaceException::translateAndThrow(ex);
    }
}

//  TODO implement Accounts WorkspaceImpl::accounts(const Credentials & credentials) const throws(WorkspaceException);
//  TODO implement Account WorkspaceImpl::findAccount(const Credentials & credentials, const QString & login) const throws(WorkspaceException);

auto WorkspaceImpl::activityTypes(
        const Credentials & credentials
    ) const -> ActivityTypes
{
    tt3::util::Lock lock(_guard);
    _ensureOpen();  //  may throw

    try
    {
        _validateAccessRights(credentials);
        //  The caller can see all activity types
        ActivityTypes result;
        for (tt3::db::api::IActivityType * dataActivityType : _database->activityTypes())
        {
            result.insert(_getProxy(dataActivityType));
        }
        return result;
    }
    catch (const tt3::util::Exception & ex)
    {
        WorkspaceException::translateAndThrow(ex);
    }
}

auto WorkspaceImpl::publicActivities(
        const Credentials & credentials
    ) const -> PublicActivities
{
    tt3::util::Lock lock(_guard);
    _ensureOpen();  //  may throw

    try
    {
        _validateAccessRights(credentials);
        //  The caller can see all public activities
        PublicActivities result;
        for (tt3::db::api::IPublicActivity * dataPublicActivity : _database->publicActivities())
        {
            result.insert(_getProxy(dataPublicActivity));
        }
        return result;
    }
    catch (const tt3::util::Exception & ex)
    {
        WorkspaceException::translateAndThrow(ex);
    }
}

auto WorkspaceImpl::publicActivitiesAndTasks(
        const Credentials & credentials
    ) const -> PublicActivities
{
    tt3::util::Lock lock(_guard);
    _ensureOpen();  //  may throw

    try
    {
        _validateAccessRights(credentials);
        //  The caller can see all public activities
        PublicActivities result;
        for (tt3::db::api::IPublicActivity * dataPublicActivity : _database->publicActivitiesAndTasks())
        {
            result.insert(_getProxy(dataPublicActivity));
        }
        return result;
    }
    catch (const tt3::util::Exception & ex)
    {
        WorkspaceException::translateAndThrow(ex);
    }
}

auto WorkspaceImpl::publicTasks(
        const Credentials & credentials
    ) const -> PublicTasks
{
    tt3::util::Lock lock(_guard);
    _ensureOpen();  //  may throw

    try
    {
        _validateAccessRights(credentials);
        //  The caller can see all public activities
        PublicTasks result;
        for (tt3::db::api::IPublicTask * dataPublicTask : _database->publicTasks())
        {
            result.insert(_getProxy(dataPublicTask));
        }
        return result;
    }
    catch (const tt3::util::Exception & ex)
    {
        WorkspaceException::translateAndThrow(ex);
    }
}

auto WorkspaceImpl::rootPublicTasks(
        const Credentials & credentials
    ) const -> PublicTasks
{
    tt3::util::Lock lock(_guard);
    _ensureOpen();  //  may throw

    try
    {
        _validateAccessRights(credentials);
        //  The caller can see all public activities
        PublicTasks result;
        for (tt3::db::api::IPublicTask * dataPublicTask : _database->rootPublicTasks())
        {
            result.insert(_getProxy(dataPublicTask));
        }
        return result;
    }
    catch (const tt3::util::Exception & ex)
    {
        WorkspaceException::translateAndThrow(ex);
    }
}

//////////
//  Operations (access control)
bool WorkspaceImpl::canAccess(
        const Credentials & credentials
    ) const
{
    tt3::util::Lock lock(_guard);
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
    tt3::util::Lock lock(_guard);
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

bool WorkspaceImpl::grantsCapability(
        const Credentials & credentials,
        Capabilities requiredCapability
    ) const
{
    tt3::util::Lock lock(_guard);
    _ensureOpen();

    try
    {
        Q_ASSERT(requiredCapability == Capabilities::Administrator ||
                 requiredCapability == Capabilities::ManageUsers ||
                 requiredCapability == Capabilities::ManageActivityTypes ||
                 requiredCapability == Capabilities::ManageBeheficiaries ||
                 requiredCapability == Capabilities::ManageWorkloads ||
                 requiredCapability == Capabilities::ManagePublicActivities ||
                 requiredCapability == Capabilities::ManagePublicTasks ||
                 requiredCapability == Capabilities::ManagePrivateActivities ||
                 requiredCapability == Capabilities::ManagePrivateTasks ||
                 requiredCapability == Capabilities::LogWork ||
                 requiredCapability == Capabilities::LogEvents ||
                 requiredCapability == Capabilities::GenerateReports ||
                 requiredCapability == Capabilities::BackupAndRestore);

        //  Do the work; be defensive in release mode
        Capabilities c = _validateAccessRights(credentials);  //  may throw
        return (c & requiredCapability) == requiredCapability;
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
    tt3::util::Lock lock(_guard);
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
    {   //  Translate & re-throw
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
        const UiLocale & uiLocale
    ) -> User
{
    tt3::util::Lock lock(_guard);
    _ensureOpen();

    try
    {
        //  Check access rights
        Capabilities clientCapabilities = _validateAccessRights(credentials); //  may throw
        if ((clientCapabilities & Capabilities::Administrator) == Capabilities::None &&
            (clientCapabilities & Capabilities::ManageUsers) == Capabilities::None)
        {   //  OOPS! Can't!
            throw AccessDeniedException();
        }
        //  Do the work
        tt3::db::api::IUser * dataUser =
            _database->createUser(
                enabled, emailAddresses,
                realName, inactivityTimeout, uiLocale);
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
    tt3::util::Lock lock(_guard);
    _ensureOpen();

    try
    {
        //  Check access rights
        Capabilities clientCapabilities = _validateAccessRights(credentials); //  may throw
        if ((clientCapabilities & Capabilities::Administrator) == Capabilities::None &&
            (clientCapabilities & Capabilities::ManageActivityTypes) == Capabilities::None)
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
    tt3::util::Lock lock(_guard);
    _ensureOpen();

    try
    {
        //  Check access rights
        Capabilities clientCapabilities = _validateAccessRights(credentials); //  may throw
        if ((clientCapabilities & Capabilities::Administrator) == Capabilities::None &&
            (clientCapabilities & Capabilities::ManagePublicActivities) == Capabilities::None)
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
    tt3::util::Lock lock(_guard);
    _ensureOpen();

    try
    {
        //  Check access rights
        Capabilities clientCapabilities = _validateAccessRights(credentials); //  may throw
        if ((clientCapabilities & Capabilities::Administrator) == Capabilities::None &&
            (clientCapabilities & Capabilities::ManagePublicTasks) == Capabilities::None)
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
    //  The "database closed" notification fro m the
    //  database will be missed, as database (along with
    //  its change notifier) no longer existsm so we
    //  need to fake a "workspace closed" signal
    //  TODO uncomment & fix the bug emit workspaceClosed(WorkspacePtr(this));
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

PublicActivity WorkspaceImpl::_getProxy(
        tt3::db::api::IPublicActivity * dataPublicActivity
    ) const
{   //  What if it's a PublicTask?
    if (auto dataPublicTask =
            dynamic_cast<tt3::db::api::IPublicTask *>(dataPublicActivity))
    {   //  ...then use the dedicated proxy getter
        return std::dynamic_pointer_cast<PublicTaskImpl>(_getProxy(dataPublicTask));
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

Workload WorkspaceImpl::_getProxy(tt3::db::api::IWorkload * dataWorkload) const
{
    Q_ASSERT(_guard.isLockedByCurrentThread());
    Q_ASSERT(_isOpen);
    Q_ASSERT(dataWorkload != nullptr);

    Q_ASSERT(false);    //  TODO implement properly;
    return nullptr;
}

//////////
//  Event handlers
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
        tt3::util::Lock lock(_guard);
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
        tt3::util::Lock lock(_guard);
        _goodCredentialsCache.clear();
        _badCredentialsCache.clear();
        //  There's no need to cache destroyed objects
        _proxyCache.remove(notification.oid());
    }
    else
    {   //  There's no need to cache destroyed objects
        tt3::util::Lock lock(_guard);
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
        tt3::util::Lock lock(_guard);
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
