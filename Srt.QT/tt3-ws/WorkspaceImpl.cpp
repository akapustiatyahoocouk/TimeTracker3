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
WorkspaceImpl::WorkspaceImpl(const WorkspaceAddress & address, tt3::db::api::IDatabase * database)
    :   _address(address),
        _database(database)
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
    catch (...)
    {   //  OOPS! Suppress TODO but log ?
    }
}

//////////
//  Operations (general)
WorkspaceType WorkspaceImpl::type() const
{
    return _address->workspaceType();
}

auto WorkspaceImpl::address(
    ) const -> WorkspaceAddress
{
    return _address;
}

bool WorkspaceImpl::isOpen() const
{
    tt3::util::Lock lock(_guard);

    return _database != nullptr;
}

void WorkspaceImpl::close() throws(WorkspaceException)
{
    tt3::util::Lock lock(_guard);

    if (_database != nullptr)
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

//////////
//  Operations (associations)
Users WorkspaceImpl::users(const Credentials & credentials) const throws(WorkspaceException)
{
    tt3::util::Lock lock(_guard);
    _ensureOpen();  //  may throw TODO add similar comment to all calls of throwing private methods

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
//  TODO implement ActivityTypes WorkspaceImpl::activityTypes(const Credentials & credentials) const throws(WorkspaceException);
//////////
//  Operations (access control)
bool WorkspaceImpl::canAccess(const Credentials & credentials) const throws(WorkspaceException)
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

Capabilities WorkspaceImpl::capabilities(const Credentials & credentials) const throws(WorkspaceException)
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

bool WorkspaceImpl::grantsCapability(const Credentials & credentials, Capabilities requiredCapability) const throws(WorkspaceException)
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

Account WorkspaceImpl::tryLogin(const Credentials & credentials) const throws(WorkspaceException)
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
User WorkspaceImpl::createUser(
    const Credentials & credentials,
    bool enabled, const QStringList & emailAddresses,
    const QString & realName,
    const InactivityTimeout & inactivityTimeout,
    const UiLocale & uiLocale) throws(WorkspaceException)
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

//////////
//  Implementation helpers
void WorkspaceImpl::_ensureOpen() const throws(WorkspaceException)
{
    Q_ASSERT(_guard.isLockedByCurrentThread());

    if (_database == nullptr)
    {
        throw WorkspaceClosedException();
    }
}

void WorkspaceImpl::_markClosed()
{
    Q_ASSERT(_guard.isLockedByCurrentThread());
    Q_ASSERT(_database != nullptr); //  i.e. workspace is "open"

    delete _database;
    _database = nullptr;
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

Capabilities WorkspaceImpl::_validateAccessRights(const Credentials & credentials) const throws(WorkspaceException)
{
    Q_ASSERT(_guard.isLockedByCurrentThread());
    Q_ASSERT(_database != nullptr); //  i.e. workspace is "open"

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
        tt3::db::api::IAccount * dataAccount =
            _database->tryLogin(credentials._login, credentials._password); //  may throw
        if (dataAccount != nullptr)
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
    Q_ASSERT(_database != nullptr); //  i.e. workspace is "open"
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
    Q_ASSERT(_database != nullptr); //  i.e. workspace is "open"
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
    Q_ASSERT(_database != nullptr); //  i.e. workspace is "open"
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

//////////
//  Event handlers
void WorkspaceImpl::_onDatabaseClosed(tt3::db::api::DatabaseClosedNotification notification)
{
    qDebug() << "Workspace::_onDatabaseClosed()";
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
    qDebug() << "Workspace::_onObjectCreated("
             << tt3::util::toString(notification.oid())
             << ")";
    Q_ASSERT(notification.database() == _database ||
             _database == nullptr); //  workspace is closed, but DB notifications were queued
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
    qDebug() << "Workspace::_onObjectDestroyed("
             << tt3::util::toString(notification.oid())
             << ")";
    Q_ASSERT(notification.database() == _database ||
             _database == nullptr); //  workspace is closed, but DB notifications were queued
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
    qDebug() << "Workspace::_onObjectModified("
             << tt3::util::toString(notification.oid())
             << ")";
    Q_ASSERT(notification.database() == _database ||
             _database == nullptr); //  workspace is closed, but DB notifications were queued
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
