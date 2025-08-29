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
        Capabilities capabilities = _workspace->_validateAccessRights(credentials);
        if ((capabilities & Capabilities::Administrator) != Capabilities::None ||
            (capabilities & Capabilities::ManageUsers) != Capabilities::None)
        {   //  The caller can see all accounts' properties
            return _dataAccount->login();   //  may throw
        }
        else
        {   //  The caller can only see his own accounts
            tt3::db::api::IAccount * clientAccount =
                _workspace->_database->login(credentials._login, credentials._password);    //  may throw
            if (clientAccount->user() == _dataAccount->user())
            {
                return _dataAccount->login();   //  may throw
            }
            //  OOPS! The caller is trying to see someone else
            throw AccessDeniedException();
        }
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
//  End of tt3-ws/AccountImpl.cpp
