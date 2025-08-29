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
QString UserImpl::realName(const Credentials & credentials) const throws(WorkspaceException)
{
    tt3::util::Lock lock(_workspace->_guard);
    _ensureLive();  //  may throw

    try
    {
        Capabilities capabilities = _workspace->_validateAccessRights(credentials);
        if ((capabilities & Capabilities::Administrator) != Capabilities::None ||
            (capabilities & Capabilities::ManageUsers) != Capabilities::None)
        {   //  The caller can see all users' properties
            return _dataUser->realName();   //  may throw
        }
        else
        {   //  The caller can only see himself
            tt3::db::api::IAccount * clientAccount =
                _workspace->_database->login(credentials._login, credentials._password);    //  may throw
            if (clientAccount->user() == _dataUser)
            {
                return _dataUser->realName();   //  may throw
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
void UserImpl::setRealName(const Credentials & credentials, const QString & realName) throws(WorkspaceException);
InactivityTimeout UserImpl::inactivityTimeout(const Credentials & credentials) const throws(WorkspaceException);
void UserImpl::setInactivityTimeout(const Credentials & credentials, const InactivityTimeout & inactivityTimeout) throws(WorkspaceException);
UiLocale UserImpl::uiLocale(const Credentials & credentials) const throws(WorkspaceException);
void UserImpl::setUiLocale(const Credentials & credentials, const UiLocale & uiLocale) throws(WorkspaceException);
*/

//////////
//  Operations (associations)
Accounts UserImpl::accounts(const Credentials & credentials) const throws(WorkspaceException)
{
    tt3::util::Lock lock(_workspace->_guard);
    _ensureLive();  //  may throw

    try
    {
        Capabilities capabilities = _workspace->_validateAccessRights(credentials);
        if ((capabilities & Capabilities::Administrator) != Capabilities::None ||
            (capabilities & Capabilities::ManageUsers) != Capabilities::None)
        {   //  The caller can see all accounts of all users
            Accounts result;
            for (tt3::db::api::IAccount * dataAccount : _dataUser->accounts())  //  may throw
            {
                result.insert(_workspace->_getProxy(dataAccount));
            }
            return result;
        }
        else
        {   //  The caller can only see his own accounts
            tt3::db::api::IAccount * dataAccount =
                _workspace->_database->login(credentials._login, credentials._password);    //  may throw
            if (dataAccount->user() == _dataUser)
            {
                Accounts result;
                for (tt3::db::api::IAccount * dataAccount : _dataUser->accounts())  //  may throw
                {
                    result.insert(_workspace->_getProxy(dataAccount));
                }
                return result;
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

//  End of tt3-ws/UserImpl.cpp
