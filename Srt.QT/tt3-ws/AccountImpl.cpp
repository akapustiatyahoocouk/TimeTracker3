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
QString AccountImpl::login(const Credentials & credentials) const throws(WorkspaceException)
{
    tt3::util::Lock lock(_workspace->_guard);
    _ensureLive();  //  may throw

    try
    {
        //  Validate access rights
        if (!_canRead(credentials))
        {
            throw AccessDeniedException();
        }
        //  Do the work
        return _dataAccount->login();   //  may throw
    }
    catch (const tt3::util::Exception & ex)
    {
        WorkspaceException::translateAndThrow(ex);
    }
}

/*  TODO uncomment & implement
void AccountImpl::setLogin(const Credentials & credentials, const QString & login) throws(WorkspaceException);
QString AccountImpl::passwordHash(const Credentials & credentials) const throws(WorkspaceException);
void AccountImpl::setPassword(const Credentials & credentials, const QString & password) throws(WorkspaceException);
Capabilities AccountImpl::capabilities(const Credentials & credentials) const throws(WorkspaceException);
void setCapabilities(const Credentials & credentials, Capabilities capabilities) throws(WorkspaceException);
*/

//////////
//  Implementation (Access control)
bool AccountImpl::_canRead(const Credentials & credentials) const throws(WorkspaceException)
{
    try
    {
        Capabilities clientCapabilities = _workspace->_validateAccessRights(credentials); //  may throw
        if ((clientCapabilities & Capabilities::Administrator) != Capabilities::None ||
            (clientCapabilities & Capabilities::ManageUsers) != Capabilities::None)
        {   //  Can read any accounts
            return true;
        }
        //  The caller can only see his own accounts
        tt3::db::api::IAccount * clientAccount =
            _workspace->_database->tryLogin(credentials._login, credentials._password); //  may throw
        return clientAccount != nullptr && clientAccount->user() == _dataAccount->user();
    }
    catch (const tt3::util::Exception & ex)
    {
        WorkspaceException::translateAndThrow(ex);
    }
}

bool AccountImpl::_canModify(const Credentials & credentials) const throws(WorkspaceException)
{
    try
    {
        Capabilities clientCapabilities = _workspace->_validateAccessRights(credentials); //  may throw
        if ((clientCapabilities & Capabilities::Administrator) != Capabilities::None ||
            (clientCapabilities & Capabilities::ManageUsers) != Capabilities::None)
        {   //  Can modify any accounts
            return true;
        }
        //  The caller can only modify his own accounts
        tt3::db::api::IAccount * clientAccount =
            _workspace->_database->tryLogin(credentials._login, credentials._password); //  may throw
        return clientAccount != nullptr && clientAccount->user() == _dataAccount->user();
    }
    catch (const tt3::util::Exception & ex)
    {
        WorkspaceException::translateAndThrow(ex);
    }
}

bool AccountImpl::_canDestroy(const Credentials & credentials) const throws(WorkspaceException)
{
    try
    {
        Capabilities clientCapabilities = _workspace->_validateAccessRights(credentials); //  may throw
        return (clientCapabilities & Capabilities::Administrator) != Capabilities::None ||
               (clientCapabilities & Capabilities::ManageUsers) != Capabilities::None;
    }
    catch (const tt3::util::Exception & ex)
    {
        WorkspaceException::translateAndThrow(ex);
    }
}

//  End of tt3-ws/AccountImpl.cpp
