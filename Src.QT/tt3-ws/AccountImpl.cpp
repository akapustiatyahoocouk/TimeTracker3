//
//  tt3-ws/AccountImpl.cpp - tt3::ws::AccountImpl class implementation
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
AccountImpl::AccountImpl(Workspace workspace, tt3::db::api::IAccount * dataAccount)
    :   PrincipalImpl(workspace, dataAccount),
        _dataAccount(dataAccount)
{
    _dataAccount->addReference();
}

AccountImpl::~AccountImpl()
{
    _dataAccount->removeReference();
}

//////////
//  Operations (properties)
QString AccountImpl::login(
        const Credentials & credentials
    ) const
{
    tt3::util::Lock lock(_workspace->_guard);
    _ensureLive();  //  may throw

    try
    {
        //  Validate access rights
        if (!_canRead(credentials)) //  may throw
        {
            throw AccessDeniedException();
        }
        //  Do the work
        return _dataAccount->login();   //  may throw
    }
    catch (const tt3::util::Exception & ex)
    {   //  OOPS! Translate & re-throw
        WorkspaceException::translateAndThrow(ex);
    }
}

void AccountImpl::setLogin(
        const Credentials & credentials,
        const QString & login
    )
{
    tt3::util::Lock lock(_workspace->_guard);
    _ensureLive();  //  may throw

    try
    {
        //  Validate access rights.
        //  Note that a user CANNOT modify their own login!
        Capabilities clientCapabilities = _workspace->_validateAccessRights(credentials); //  may throw
        if (!clientCapabilities.contains(Capability::Administrator) &&
            !clientCapabilities.contains(Capability::ManageUsers))
        {
            throw AccessDeniedException();
        }
        //  Do the work
        return _dataAccount->setLogin(login);   //  may throw
    }
    catch (const tt3::util::Exception & ex)
    {   //  OOPS! Translate & re-throw
        WorkspaceException::translateAndThrow(ex);
    }
}

QString AccountImpl::passwordHash(
        const Credentials & credentials
    ) const
{
    tt3::util::Lock lock(_workspace->_guard);
    _ensureLive();  //  may throw

    try
    {
        //  Validate access rights
        if (!_canRead(credentials)) //  may throw
        {
            throw AccessDeniedException();
        }
        //  Do the work
        return _dataAccount->passwordHash();    //  may throw
    }
    catch (const tt3::util::Exception & ex)
    {   //  OOPS! Translate & re-throw
        WorkspaceException::translateAndThrow(ex);
    }
}

void AccountImpl::setPassword(
        const Credentials & credentials,
        const QString & password
    )
{
    tt3::util::Lock lock(_workspace->_guard);
    _ensureLive();  //  may throw

    try
    {
        //  Validate access rights
        if (!_canModify(credentials))   //  may throw
        {
            throw AccessDeniedException();
        }
        //  Do the work
        _dataAccount->setPassword(password); //  may throw
    }
    catch (const tt3::util::Exception & ex)
    {   //  OOPS! Translate & re-throw
        WorkspaceException::translateAndThrow(ex);
    }
}

auto AccountImpl::capabilities(
        const Credentials & credentials
    ) const -> Capabilities
{
    tt3::util::Lock lock(_workspace->_guard);
    _ensureLive();  //  may throw

    try
    {
        //  Validate access rights
        if (!_canRead(credentials)) //  may throw
        {
            throw AccessDeniedException();
        }
        //  Do the work
        return _dataAccount->capabilities();    //  may throw
    }
    catch (const tt3::util::Exception & ex)
    {   //  OOPS! Translate & re-throw
        WorkspaceException::translateAndThrow(ex);
    }
}

void AccountImpl::setCapabilities(
        const Credentials & credentials,
        Capabilities capabilities
    )
{
    tt3::util::Lock lock(_workspace->_guard);
    _ensureLive();  //  may throw

    try
    {
        //  Validate access rights.
        //  Note that a user CANNOT modify their own capabilities...
        Capabilities clientCapabilities = _workspace->_validateAccessRights(credentials); //  may throw
        if (!clientCapabilities.contains(Capability::Administrator) &&
            !clientCapabilities.contains(Capability::ManageUsers))
        {
            throw AccessDeniedException();
        }
        //  ...or leave workspace without an enabled admin user+account
        if (_dataAccount->capabilities().contains(Capability::Administrator) &&
            !capabilities.contains(Capability::Administrator))
        {   //  This account is losing its Administrator capability
            if (_destroyingLosesAccess())
            {
                throw AccessWouldBeLostException();
            }
        }

        //  Do the work
        return _dataAccount->setCapabilities(capabilities); //  may throw
    }
    catch (const tt3::util::Exception & ex)
    {   //  OOPS! Translate & re-throw
        WorkspaceException::translateAndThrow(ex);
    }
}

//////////
//  Operations (associations)
auto AccountImpl::user(
        const Credentials & credentials
    ) const -> User
{
    tt3::util::Lock lock(_workspace->_guard);
    _ensureLive();  //  may throw

    try
    {
        //  Validate access rights
        if (!_canRead(credentials)) //  may throw
        {
            throw AccessDeniedException();
        }
        //  Do the work
        return _workspace->_getProxy(_dataAccount->user()); //  may throw
    }
    catch (const tt3::util::Exception & ex)
    {   //  OOPS! Translate & re-throw
        WorkspaceException::translateAndThrow(ex);
    }
}

auto AccountImpl::quickPicksList(
        const Credentials & credentials
    ) const -> QList<Activity>
{
    tt3::util::Lock lock(_workspace->_guard);
    _ensureLive();  //  may throw

    try
    {
        //  Validate access rights
        if (!_canRead(credentials)) //  may throw
        {
            throw AccessDeniedException();
        }
        //  Do the work
        QList<Activity> result;
        for (tt3::db::api::IActivity * dataActivity : _dataAccount->quickPicksList())
        {
            result.append(_workspace->_getProxy(dataActivity));
        }
        return result;
    }
    catch (const tt3::util::Exception & ex)
    {   //  OOPS! Translate & re-throw
        WorkspaceException::translateAndThrow(ex);
    }
}

void AccountImpl::setQuickPicksList(
        const Credentials & credentials,
        const QList<Activity> & quickPicksList
    )
{
    tt3::util::Lock lock(_workspace->_guard);
    _ensureLive();  //  may throw

    try
    {
        //  Validate access rights.
        if (!_canModify(credentials)) //  may throw
        {
            throw AccessDeniedException();
        }
        //  Do the work
        QList<tt3::db::api::IActivity*> dataQuickPicksList;
        for (Activity activity : quickPicksList)
        {
            dataQuickPicksList.append(
                (activity != nullptr) ?
                    activity->_dataActivity :
                    nullptr);
        }
        _dataAccount->setQuickPicksList(dataQuickPicksList);//  may throw
    }
    catch (const tt3::util::Exception & ex)
    {   //  OOPS! Translate & re-throw
        WorkspaceException::translateAndThrow(ex);
    }
}

auto AccountImpl::works(
        const Credentials & credentials
    ) const -> Works
{
    tt3::util::Lock lock(_workspace->_guard);
    _ensureLive();  //  may throw

    try
    {
        //  Validate access rights
        if (!_canRead(credentials)) //  may throw
        {
            throw AccessDeniedException();
        }
        //  Do the work
        Works result;
        for (tt3::db::api::IWork * dataWork : _dataAccount->works())
        {
            result.insert(_workspace->_getProxy(dataWork));
        }
        return result;
    }
    catch (const tt3::util::Exception & ex)
    {   //  OOPS! Translate & re-throw
        WorkspaceException::translateAndThrow(ex);
    }
}

auto AccountImpl::works(
        const Credentials & credentials,
        const QDateTime & from,
        const QDateTime & to
    ) const -> Works
{
    tt3::util::Lock lock(_workspace->_guard);
    _ensureLive();  //  may throw

    try
    {
        //  Validate access rights
        if (!_canRead(credentials)) //  may throw
        {
            throw AccessDeniedException();
        }
        //  Do the work
        Works result;
        for (tt3::db::api::IWork * dataWork : _dataAccount->works(from, to))
        {
            result.insert(_workspace->_getProxy(dataWork));
        }
        return result;
    }
    catch (const tt3::util::Exception & ex)
    {   //  OOPS! Translate & re-throw
        WorkspaceException::translateAndThrow(ex);
    }
}

auto AccountImpl::events(
        const Credentials & credentials
    ) const -> Events
{
    tt3::util::Lock lock(_workspace->_guard);
    _ensureLive();  //  may throw

    try
    {
        //  Validate access rights
        if (!_canRead(credentials)) //  may throw
        {
            throw AccessDeniedException();
        }
        //  Do the work
        Events result;
        for (tt3::db::api::IEvent * dataEvent : _dataAccount->events())
        {
            result.insert(_workspace->_getProxy(dataEvent));
        }
        return result;
    }
    catch (const tt3::util::Exception & ex)
    {   //  OOPS! Translate & re-throw
        WorkspaceException::translateAndThrow(ex);
    }
}

auto AccountImpl::events(
        const Credentials & credentials,
        const QDateTime & from,
        const QDateTime & to
    ) const -> Events
{
    tt3::util::Lock lock(_workspace->_guard);
    _ensureLive();  //  may throw

    try
    {
        //  Validate access rights
        if (!_canRead(credentials)) //  may throw
        {
            throw AccessDeniedException();
        }
        //  Do the work
        Events result;
        for (tt3::db::api::IEvent * dataEvent : _dataAccount->events(from, to))
        {
            result.insert(_workspace->_getProxy(dataEvent));
        }
        return result;
    }
    catch (const tt3::util::Exception & ex)
    {   //  OOPS! Translate & re-throw
        WorkspaceException::translateAndThrow(ex);
    }
}

/////////
//  Operations (life cycle)
auto AccountImpl::createWork(
        const Credentials & credentials,
        const QDateTime & startedAt,
        const QDateTime & finishedAt,
        Activity activity
    ) -> Work
{
    tt3::util::Lock lock(_workspace->_guard);
    _ensureLive();  //  may throw

    try
    {
        //  Check access rights
        Capabilities clientCapabilities = _workspace->_validateAccessRights(credentials); //  may throw
        if (clientCapabilities.contains(Capability::Administrator))
        {   //  can log Work items for any account/activity
        }
        else if (clientCapabilities.contains(Capability::LogWork))
        {   //  Can log Work items aganst public Activities/Tasks and
            //  caller's own private Activities/Tasks
            tt3::db::api::IAccount * callerAccount =
                _workspace->_database->tryLogin(credentials._login, credentials._password); //  may throw
            if (callerAccount == nullptr ||
                callerAccount->user() != this->_dataAccount->user())
            {   //  OOPS! Can't!
                throw AccessDeniedException();
            }
            if (auto dataPrivateActivity =
                dynamic_cast<tt3::db::api::IPrivateActivity*>(activity->_dataActivity))
            {   //  The private Activity/Tak must belong to the caller!
                if (dataPrivateActivity->owner() != callerAccount->user())
                {   //  OOPS! Can't!
                    throw AccessDeniedException();
                }
            }
        }
        else
        {   //  OOPS! Can't!
            throw AccessDeniedException();
        }
        //  Do the work
        tt3::db::api::IWork * dataWork =
            _dataAccount->createWork(   //  may throw
                startedAt,
                finishedAt,
                activity->_dataActivity);
        return _workspace->_getProxy(dataWork);
    }
    catch (const tt3::util::Exception & ex)
    {   //  OOPS! Translate & re-throw
        WorkspaceException::translateAndThrow(ex);
    }
}

auto AccountImpl::createEvent(
        const Credentials & credentials,
        const QDateTime & occurredAt,
        const QString & summary,
        const Activities & activities
    ) -> Event
{
    tt3::util::Lock lock(_workspace->_guard);
    _ensureLive();  //  may throw

    try
    {
        //  Check access rights
        Capabilities clientCapabilities = _workspace->_validateAccessRights(credentials); //  may throw
        if (clientCapabilities.contains(Capability::Administrator))
        {   //  can log Events for any account/activity
        }
        else if (clientCapabilities.contains(Capability::LogEvents))
        {   //  Can log Events aganst public Activities/Tasks and
            //  caller's own private Activities/Tasks
            tt3::db::api::IAccount * callerAccount =
                _workspace->_database->tryLogin(credentials._login, credentials._password); //  may throw
            if (callerAccount == nullptr ||
                callerAccount->user() != this->_dataAccount->user())
            {   //  OOPS! Can't!
                throw AccessDeniedException();
            }
            for (Activity activity : activities)
            {
                if (activity != nullptr)
                {   //  OOPS! Can't!
                    if (auto dataPrivateActivity =
                        dynamic_cast<tt3::db::api::IPrivateActivity*>(activity->_dataActivity))
                    {   //  The private Activity/Tak must belong to the caller!
                        if (dataPrivateActivity->owner() != callerAccount->user())
                        {   //  OOPS! Can't!
                            throw AccessDeniedException();
                        }
                    }
                }
            }
        }
        else
        {   //  OOPS! Can't!
            throw AccessDeniedException();
        }
        //  Do the work
        tt3::db::api::Activities dataActivities =
            tt3::util::transform<tt3::db::api::IActivity *, Activity>(
                activities,
                [](auto a)
                {   //  Be defensive when transforming nullptrs
                    return (a != nullptr) ? a->_dataActivity : nullptr;
                });
        tt3::db::api::IEvent * dataEvent =
            _dataAccount->createEvent(   //  may throw
                occurredAt,
                summary,
                dataActivities);
        return _workspace->_getProxy(dataEvent);
    }
    catch (const tt3::util::Exception & ex)
    {   //  OOPS! Translate & re-throw
        WorkspaceException::translateAndThrow(ex);
    }
}

//////////
//  Implementation (Access control)
bool AccountImpl::_canRead(
        const Credentials & credentials
    ) const
{
    Q_ASSERT(_workspace->_guard.isLockedByCurrentThread());

    try
    {
        Capabilities clientCapabilities = _workspace->_validateAccessRights(credentials); //  may throw
        if (clientCapabilities.contains(Capability::Administrator) ||
            clientCapabilities.contains(Capability::ManageUsers))
        {   //  Can read any accounts
            return true;
        }
        //  The caller can only see his own accounts
        tt3::db::api::IAccount * callerAccount =
            _workspace->_database->tryLogin(credentials._login, credentials._password); //  may throw
        return callerAccount != nullptr &&
               callerAccount->user() == _dataAccount->user();
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

bool AccountImpl::_canModify(
        const Credentials & credentials
    ) const
{
    Q_ASSERT(_workspace->_guard.isLockedByCurrentThread());

    try
    {
        Capabilities clientCapabilities = _workspace->_validateAccessRights(credentials); //  may throw
        if (clientCapabilities.contains(Capability::Administrator) ||
            clientCapabilities.contains(Capability::ManageUsers))
        {   //  Can modify any accounts
            return true;
        }
        //  The caller can only modify his own accounts
        tt3::db::api::IAccount * callerAccount =
            _workspace->_database->tryLogin(credentials._login, credentials._password); //  may throw
        return callerAccount != nullptr &&
               callerAccount->user() == _dataAccount->user();
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

bool AccountImpl::_canDestroy(
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

bool AccountImpl::_destroyingLosesAccess(
    ) const
{
    Q_ASSERT(_workspace->_guard.isLockedByCurrentThread());

    try
    {
        //  If there is an enabled user...
        for (tt3::db::api::IUser * aDataUser : _workspace->_database->users())
        {
            if (aDataUser->enabled())
            {   //  ...with an enabled admin account other than this one...
                for (tt3::db::api::IAccount * aDataAccount : aDataUser->accounts())
                {
                    if (aDataAccount != _dataAccount &&
                        aDataAccount->enabled() &&
                        aDataAccount->capabilities().contains(Capability::Administrator))
                    {   //  ...then we CAN destroy this Account
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

//  End of tt3-ws/AccountImpl.cpp
