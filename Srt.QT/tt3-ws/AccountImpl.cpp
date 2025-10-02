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
