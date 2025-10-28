//
//  tt3-ws/UserImpl.cpp - tt3::ws::UserImpl class implementation
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
//  Construction/destruction
UserImpl::UserImpl(Workspace workspace, tt3::db::api::IUser * dataUser)
    :   PrincipalImpl(workspace, dataUser),
        _dataUser(dataUser)
{
    _dataUser->addReference();
}

UserImpl::~UserImpl()
{
    _dataUser->removeReference();
}

//////////
//  Operations (properties)
QString UserImpl::realName(
        const Credentials & credentials
    ) const
{
    tt3::util::Lock _(_workspace->_guard);
    _ensureLive();  //  may throw

    try
    {
        //  Validate access rights
        if (!_canRead(credentials))
        {
            throw AccessDeniedException();
        }
        //  Do the work
        return _dataUser->realName();   //  may throw
    }
    catch (const tt3::util::Exception & ex)
    {   //  OOPS! Translate & re-throw
        WorkspaceException::translateAndThrow(ex);
    }
}

void UserImpl::setRealName(
        const Credentials & credentials,
        const QString & realName
    )
{
    tt3::util::Lock _(_workspace->_guard);
    _ensureLive();  //  may throw

    try
    {
        //  Validate access rights
        if (!_canModify(credentials))
        {
            throw AccessDeniedException();
        }
        //  Do the work
        return _dataUser->setRealName(realName);    //  may throw
    }
    catch (const tt3::util::Exception & ex)
    {   //  OOPS! Translate & re-throw
        WorkspaceException::translateAndThrow(ex);
    }
}

auto UserImpl::inactivityTimeout(
        const Credentials & credentials
    ) const -> InactivityTimeout
{
    tt3::util::Lock _(_workspace->_guard);
    _ensureLive();  //  may throw

    try
    {
        //  Validate access rights
        if (!_canRead(credentials))
        {
            throw AccessDeniedException();
        }
        //  Do the work
        return _dataUser->inactivityTimeout();  //  may throw
    }
    catch (const tt3::util::Exception & ex)
    {   //  OOPS! Translate & re-throw
        WorkspaceException::translateAndThrow(ex);
    }
}

void UserImpl::setInactivityTimeout(
        const Credentials & credentials,
        const InactivityTimeout & inactivityTimeout
    )
{
    tt3::util::Lock _(_workspace->_guard);
    _ensureLive();  //  may throw

    try
    {
        //  Validate access rights
        if (!_canModify(credentials))
        {
            throw AccessDeniedException();
        }
        //  Do the work
        return _dataUser->setInactivityTimeout(inactivityTimeout);  //  may throw
    }
    catch (const tt3::util::Exception & ex)
    {   //  OOPS! Translate & re-throw
        WorkspaceException::translateAndThrow(ex);
    }
}

auto UserImpl::uiLocale(
        const Credentials & credentials
    ) const -> UiLocale
{
    tt3::util::Lock _(_workspace->_guard);
    _ensureLive();  //  may throw

    try
    {
        //  Validate access rights
        if (!_canRead(credentials))
        {
            throw AccessDeniedException();
        }
        //  Do the work
        return _dataUser->uiLocale();   //  may throw
    }
    catch (const tt3::util::Exception & ex)
    {   //  OOPS! Translate & re-throw
        WorkspaceException::translateAndThrow(ex);
    }
}

void UserImpl::setUiLocale(
        const Credentials & credentials,
        const UiLocale & uiLocale
    )
{
    tt3::util::Lock _(_workspace->_guard);
    _ensureLive();  //  may throw

    try
    {
        //  Validate access rights
        if (!_canModify(credentials))
        {
            throw AccessDeniedException();
        }
        //  Do the work
        return _dataUser->setUiLocale(uiLocale);    //  may throw
    }
    catch (const tt3::util::Exception & ex)
    {   //  OOPS! Translate & re-throw
        WorkspaceException::translateAndThrow(ex);
    }
}

//////////
//  Operations (associations)
auto UserImpl::accounts(
        const Credentials & credentials
    ) const -> Accounts
{
    tt3::util::Lock _(_workspace->_guard);
    _ensureLive();  //  may throw

    try
    {
        Capabilities capabilities = _workspace->_validateAccessRights(credentials);
        if (capabilities.contains(Capability::Administrator) ||
            capabilities.contains(Capability::ManageUsers))
        {   //  The caller can see all accounts of all users
            return tt3::util::transform(
                _dataUser->accounts(),  //  may throw
                [&](auto da)
                {
                    return _workspace->_getProxy(da);
                });
        }
        else
        {   //  The caller can only see his own accounts
            tt3::db::api::IAccount * dataAccount =
                _workspace->_database->login(credentials._login, credentials._password);    //  may throw
            if (dataAccount->user() == _dataUser)
            {
                return tt3::util::transform(
                    _dataUser->accounts(),  //  may throw
                    [&](auto da)
                    {
                        return _workspace->_getProxy(da);
                    });
            }
            //  OOPS! The caller is trying to see someone else
            throw AccessDeniedException();
        }
    }
    catch (const tt3::util::Exception & ex)
    {   //  OOPS! Translate & re-throw
        WorkspaceException::translateAndThrow(ex);
    }
}

auto UserImpl::privateActivities(
        const Credentials & credentials
    ) const -> PrivateActivities
{
    tt3::util::Lock _(_workspace->_guard);
    _ensureLive();  //  may throw

    try
    {
        Capabilities capabilities = _workspace->_validateAccessRights(credentials);
        if (capabilities.contains(Capability::Administrator))
        {   //  The caller can see all private activities of all users
            return tt3::util::transform(
                _dataUser->privateActivities(), //  may throw
                [&](auto da)
                {
                    return _workspace->_getProxy(da);
                });
        }
        else
        {   //  The caller can only see his own private activities
            tt3::db::api::IAccount * dataAccount =
                _workspace->_database->login(credentials._login, credentials._password);    //  may throw
            if (dataAccount->user() == _dataUser)
            {
                return tt3::util::transform(
                    _dataUser->privateActivities(), //  may throw
                    [&](auto da)
                    {
                        return _workspace->_getProxy(da);
                    });
            }
            //  OOPS! The caller is trying to see someone else's private activities
            throw AccessDeniedException();
        }
    }
    catch (const tt3::util::Exception & ex)
    {   //  OOPS! Translate & re-throw
        WorkspaceException::translateAndThrow(ex);
    }
}

auto UserImpl::privateActivitiesAndTasks(
        const Credentials & credentials
    ) const -> PrivateActivities
{
    tt3::util::Lock _(_workspace->_guard);
    _ensureLive();  //  may throw

    try
    {
        Capabilities capabilities = _workspace->_validateAccessRights(credentials);
        if (capabilities.contains(Capability::Administrator))
        {   //  The caller can see all private activities and tasks of all users
            return tt3::util::transform(
                _dataUser->privateActivitiesAndTasks(), //  may throw
                [&](auto da)
                {
                    return _workspace->_getProxy(da);
                });
        }
        else
        {   //  The caller can only see his own private activities and tasks
            tt3::db::api::IAccount * dataAccount =
                _workspace->_database->login(credentials._login, credentials._password);    //  may throw
            if (dataAccount->user() == _dataUser)
            {
                return tt3::util::transform(
                    _dataUser->privateActivitiesAndTasks(), //  may throw
                    [&](auto da)
                    {
                        return _workspace->_getProxy(da);
                    });
            }
            //  OOPS! The caller is trying to see someone else's private activities and tasks
            throw AccessDeniedException();
        }
    }
    catch (const tt3::util::Exception & ex)
    {   //  OOPS! Translate & re-throw
        WorkspaceException::translateAndThrow(ex);
    }
}

auto UserImpl::privateTasks(
        const Credentials & credentials
    ) const -> PrivateTasks
{
    tt3::util::Lock _(_workspace->_guard);
    _ensureLive();  //  may throw

    try
    {
        Capabilities capabilities = _workspace->_validateAccessRights(credentials);
        if (capabilities.contains(Capability::Administrator))
        {   //  The caller can see all private tasks of all users
            return tt3::util::transform(
                _dataUser->privateTasks(), //  may throw
                [&](auto dt)
                {
                    return _workspace->_getProxy(dt);
                });
        }
        else
        {   //  The caller can only see his own private tasks
            tt3::db::api::IAccount * dataAccount =
                _workspace->_database->login(credentials._login, credentials._password);    //  may throw
            if (dataAccount->user() == _dataUser)
            {
                return tt3::util::transform(
                    _dataUser->privateTasks(), //  may throw
                    [&](auto dt)
                    {
                        return _workspace->_getProxy(dt);
                    });
            }
            //  OOPS! The caller is trying to see someone else's private tasks
            throw AccessDeniedException();
        }
    }
    catch (const tt3::util::Exception & ex)
    {   //  OOPS! Translate & re-throw
        WorkspaceException::translateAndThrow(ex);
    }
}

auto UserImpl::rootPrivateTasks(
        const Credentials & credentials
    ) const -> PrivateTasks
{
    tt3::util::Lock _(_workspace->_guard);
    _ensureLive();  //  may throw

    try
    {
        Capabilities capabilities = _workspace->_validateAccessRights(credentials);
        if (capabilities.contains(Capability::Administrator))
        {   //  The caller can see all private tasks of all users
            return tt3::util::transform(
                _dataUser->rootPrivateTasks(),  //  may throw
                [&](auto dt)
                {
                    return _workspace->_getProxy(dt);
                });
        }
        else
        {   //  The caller can only see his own private tasks
            tt3::db::api::IAccount * dataAccount =
                _workspace->_database->login(credentials._login, credentials._password);    //  may throw
            if (dataAccount->user() == _dataUser)
            {
                return tt3::util::transform(
                    _dataUser->rootPrivateTasks(),  //  may throw
                    [&](auto dt)
                    {
                        return _workspace->_getProxy(dt);
                    });
            }
            //  OOPS! The caller is trying to see someone else's private tasks
            throw AccessDeniedException();
        }
    }
    catch (const tt3::util::Exception & ex)
    {   //  OOPS! Translate & re-throw
        WorkspaceException::translateAndThrow(ex);
    }
}

auto UserImpl::permittedWorkloads(
        const Credentials & credentials
    ) const -> Workloads
{
    tt3::util::Lock _(_workspace->_guard);
    _ensureLive();  //  may throw

    try
    {
        //  Validate access rights
        if (!_canRead(credentials))
        {
            throw AccessDeniedException();
        }
        //  Do the work
        return tt3::util::transform(
            _dataUser->permittedWorkloads(),
            [&](auto dw)
            {
                return _workspace->_getProxy(dw);
            });
    }
    catch (const tt3::util::Exception & ex)
    {   //  OOPS! Translate & re-throw
        WorkspaceException::translateAndThrow(ex);
    }
}

void UserImpl::setPermittedWorkloads(
        const Credentials & credentials,
        const Workloads & workloads
    )
{
    tt3::util::Lock _(_workspace->_guard);
    _ensureLive();  //  may throw

    try
    {
        //  Validate access rights.
        if (!_canModify(credentials)) //  may throw
        {
            throw AccessDeniedException();
        }
        //  Do the work
        _dataUser->setPermittedWorkloads(   //  may throw
            tt3::util::transform(
                workloads,
                [](auto w)
                {   //  Be defensive when transforming nullptrs
                    return (w != nullptr) ? w->_dataWorkload : nullptr;
                }));
    }
    catch (const tt3::util::Exception & ex)
    {   //  OOPS! Translate & re-throw
        WorkspaceException::translateAndThrow(ex);
    }
}

void UserImpl::addPermittedWorkload(
        const Credentials & credentials,
        Workload workload
    )
{
    tt3::util::Lock _(_workspace->_guard);
    _ensureLive();  //  may throw

    try
    {
        //  Validate access rights.
        if (!_canModify(credentials)) //  may throw
        {
            throw AccessDeniedException();
        }
        //  Do the work
        _dataUser->addPermittedWorkload(    //  may throw
            (workload != nullptr) ? workload->_dataWorkload : nullptr);
    }
    catch (const tt3::util::Exception & ex)
    {   //  OOPS! Translate & re-throw
        WorkspaceException::translateAndThrow(ex);
    }
}

void UserImpl::removePermittedWorkload(
        const Credentials & credentials,
        Workload workload
    )
{
    tt3::util::Lock _(_workspace->_guard);
    _ensureLive();  //  may throw

    try
    {
        //  Validate access rights.
        if (!_canModify(credentials)) //  may throw
        {
            throw AccessDeniedException();
        }
        //  Do the work
        _dataUser->removePermittedWorkload(    //  may throw
            (workload != nullptr) ? workload->_dataWorkload : nullptr);
    }
    catch (const tt3::util::Exception & ex)
    {   //  OOPS! Translate & re-throw
        WorkspaceException::translateAndThrow(ex);
    }
}

//////////
//  Operations (life cycle)
auto UserImpl::createAccount(
        const Credentials & credentials,
        bool enabled,
        const QStringList & emailAddresses,
        const QString & login,
        const QString & password,
        Capabilities capabilities
    ) -> Account
{
    tt3::util::Lock _(_workspace->_guard);
    _ensureLive();

    try
    {
        //  Check access rights
        Capabilities clientCapabilities = _workspace->_validateAccessRights(credentials); //  may throw
        if (!clientCapabilities.contains(Capability::Administrator) &&
            !clientCapabilities.contains(Capability::ManageUsers))
        {   //  OOPS! Can't!
            throw AccessDeniedException();
        }
        //  Do the work
        tt3::db::api::IAccount * dataAccount =
            _dataUser->createAccount(
                enabled,
                emailAddresses,
                login,
                password,
                capabilities);
        return _workspace->_getProxy(dataAccount);;
    }
    catch (const tt3::util::Exception & ex)
    {   //  OOPS! Translate & re-throw
        WorkspaceException::translateAndThrow(ex);
    }
}

auto UserImpl::createPrivateActivity(
        const Credentials & credentials,
        const QString & displayName,
        const QString & description,
        const InactivityTimeout & timeout,
        bool requireCommentOnStart,
        bool requireCommentOnStop,
        bool fullScreenReminder,
        ActivityType activityType,
        Workload workload
    ) -> PrivateActivity
{
    tt3::util::Lock _(_workspace->_guard);
    _ensureLive();

    try
    {
        //  Check access rights
        Capabilities clientCapabilities = _workspace->_validateAccessRights(credentials); //  may throw
        if (clientCapabilities.contains(Capability::Administrator))
        {   //  Can create PrivateActivities for any User
        }
        else if (clientCapabilities.contains(Capability::ManagePrivateActivities))
        {   //  Can ONLY create their own PrivateActivities
            if (_workspace->_database->login(   //  may throw
                    credentials._login, credentials._password)->user() != _dataUser)
            {
                throw AccessDeniedException();
            }
        }
        else
        {   //  OOPS! Can't!
            throw AccessDeniedException();
        }
        //  Do the work
        tt3::db::api::IPrivateActivity * dataPrivateActivity =
            _dataUser->createPrivateActivity(
                displayName,
                description,
                timeout,
                requireCommentOnStart,
                requireCommentOnStop,
                fullScreenReminder,
                (activityType != nullptr) ? activityType->_dataActivityType : nullptr,
                (workload != nullptr) ? workload->_dataWorkload : nullptr);
        return _workspace->_getProxy(dataPrivateActivity);;
    }
    catch (const tt3::util::Exception & ex)
    {   //  OOPS! Translate & re-throw
        WorkspaceException::translateAndThrow(ex);
    }
}

auto UserImpl::createPrivateTask(
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
    ) -> PrivateTask
{
    tt3::util::Lock _(_workspace->_guard);
    _ensureLive();

    try
    {
        //  Check access rights
        Capabilities clientCapabilities = _workspace->_validateAccessRights(credentials); //  may throw
        if (clientCapabilities.contains(Capability::Administrator))
        {   //  Can create PrivateTasks for any User
        }
        else if (clientCapabilities.contains(Capability::ManagePrivateTasks))
        {   //  Can ONLY create their own PrivateTasks
            if (_workspace->_database->login(   //  may throw
                    credentials._login, credentials._password)->user() != _dataUser)
            {
                throw AccessDeniedException();
            }
        }
        else
        {   //  OOPS! Can't!
            throw AccessDeniedException();
        }
        //  Do the work
        tt3::db::api::IPrivateTask * dataPrivateTask =
            _dataUser->createPrivateTask(
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
        return _workspace->_getProxy(dataPrivateTask);;
    }
    catch (const tt3::util::Exception & ex)
    {   //  OOPS! Translate & re-throw
        WorkspaceException::translateAndThrow(ex);
    }
}

//////////
//  Implementation (Access control)
bool UserImpl::_canRead(
        const Credentials & credentials
    ) const
{
    Q_ASSERT(_workspace->_guard.isLockedByCurrentThread());

    try
    {
        Capabilities clientCapabilities = _workspace->_validateAccessRights(credentials); //  may throw
        if (clientCapabilities.contains(Capability::Administrator) ||
            clientCapabilities.contains(Capability::ManageUsers))
        {   //  Can read any user
            return true;
        }
        //  Otherwise user can only read himself
        tt3::db::api::IAccount * callerAccount =
            _workspace->_database->tryLogin(credentials._login, credentials._password);    //  may throw
        return callerAccount != nullptr &&
               callerAccount->user() == _dataUser;
    }
    catch (const AccessDeniedException &)
    {   //  This is a special case!
        return false;
    }
    catch (const tt3::util::Exception & ex)
    {   //  OOPS! Translate & re-throw
        WorkspaceException::translateAndThrow(ex);
    }
}

bool UserImpl::_canModify(
        const Credentials & credentials
    ) const
{
    Q_ASSERT(_workspace->_guard.isLockedByCurrentThread());

    try
    {
        Capabilities clientCapabilities = _workspace->_validateAccessRights(credentials); //  may throw
        if (clientCapabilities.contains(Capability::Administrator) ||
            clientCapabilities.contains(Capability::ManageUsers))
        {   //  Can modify any user
            return true;
        }
        //  Otherwise user can only modify himself
        tt3::db::api::IAccount * callerAccount =
            _workspace->_database->tryLogin(credentials._login, credentials._password);    //  may throw
        return callerAccount != nullptr &&
               callerAccount->user() == _dataUser;
    }
    catch (const AccessDeniedException &)
    {   //  This is a special case!
        return false;
    }
    catch (const tt3::util::Exception & ex)
    {   //  OOPS! Translate & re-throw
        WorkspaceException::translateAndThrow(ex);
    }
}

bool UserImpl::_canDestroy(
        const Credentials & credentials
    ) const
{
    Q_ASSERT(_workspace->_guard.isLockedByCurrentThread());

    try
    {
        Capabilities clientCapabilities = _workspace->_validateAccessRights(credentials); //  may throw
        return clientCapabilities.contains(Capability::Administrator) ||
               clientCapabilities.contains(Capability::ManageUsers);
    }
    catch (const AccessDeniedException &)
    {   //  This is a special case!
        return false;
    }
    catch (const tt3::util::Exception & ex)
    {   //  OOPS! Translate & re-throw
        WorkspaceException::translateAndThrow(ex);
    }
}

bool UserImpl::_destroyingLosesAccess(
    ) const
{
    Q_ASSERT(_workspace->_guard.isLockedByCurrentThread());

    try
    {
        //  If there is another enabled user...
        for (tt3::db::api::IUser * aDataUser : _workspace->_database->users())
        {
            if (aDataUser->enabled() && aDataUser != _dataUser)
            {   //  ...with an enabled admin account...
                for (tt3::db::api::IAccount * aDataAccount : aDataUser->accounts())
                {
                    if (aDataAccount->enabled() &&
                        aDataAccount->capabilities().contains(Capability::Administrator))
                    {   //  ...then we CAN destroy this User
                        return false;
                    }
                }
            }
        }
        //  Otherwise we're stuck with this User
        return true;
    }
    catch (const tt3::util::Exception & ex)
    {   //  OOPS! Translate & re-throw
        WorkspaceException::translateAndThrow(ex);
    }
}

//  End of tt3-ws/UserImpl.cpp
