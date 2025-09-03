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

void AccountImpl::setLogin(const Credentials & credentials, const QString & login) throws(WorkspaceException)
{
    tt3::util::Lock lock(_workspace->_guard);
    _ensureLive();  //  may throw

    try
    {
        //  Validate access rights.
        //  Note that a user CANNOT modify their own login!
        Capabilities clientCapabilities = _workspace->_validateAccessRights(credentials); //  may throw
        if ((clientCapabilities & Capabilities::Administrator) == Capabilities::None &&
            (clientCapabilities & Capabilities::ManageUsers) == Capabilities::None)
        {
            throw AccessDeniedException();
        }
        //  Do the work
        return _dataAccount->setLogin(login);   //  may throw
    }
    catch (const tt3::util::Exception & ex)
    {
        WorkspaceException::translateAndThrow(ex);
    }
}

QString AccountImpl::passwordHash(const Credentials & credentials) const throws(WorkspaceException)
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
        return _dataAccount->passwordHash();    //  may throw
    }
    catch (const tt3::util::Exception & ex)
    {
        WorkspaceException::translateAndThrow(ex);
    }
}

void AccountImpl::setPassword(const Credentials & credentials, const QString & password) throws(WorkspaceException)
{
    tt3::util::Lock lock(_workspace->_guard);
    _ensureLive();  //  may throw

    try
    {
        //  Validate access rights
        if (!_canModify(credentials))
        {
            throw AccessDeniedException();
        }
        //  Do the work
        _dataAccount->setPassword(password); //  may throw
    }
    catch (const tt3::util::Exception & ex)
    {
        WorkspaceException::translateAndThrow(ex);
    }
}

Capabilities AccountImpl::capabilities(const Credentials & credentials) const throws(WorkspaceException)
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
        return _dataAccount->capabilities();    //  may throw
    }
    catch (const tt3::util::Exception & ex)
    {
        WorkspaceException::translateAndThrow(ex);
    }
}

void AccountImpl::setCapabilities(const Credentials & credentials, Capabilities capabilities) throws(WorkspaceException)
{
    tt3::util::Lock lock(_workspace->_guard);
    _ensureLive();  //  may throw

    try
    {
        //  Validate access rights.
        //  Note that a user CANNOT modify their own capabilities...
        Capabilities clientCapabilities = _workspace->_validateAccessRights(credentials); //  may throw
        if ((clientCapabilities & Capabilities::Administrator) == Capabilities::None &&
            (clientCapabilities & Capabilities::ManageUsers) == Capabilities::None)
        {
            throw AccessDeniedException();
        }
        //  ...or leave workspace without an enabled admin user+account
        if ((capabilities & Capabilities::Administrator) == Capabilities::None)
        {
            bool hasEnabledAdminAccount = false;
            for (tt3::db::api::IUser * someDataUser : _dataPrincipal->database()->users())
            {
                for (tt3::db::api::IAccount * someDataAccount : someDataUser->accounts())
                {
                    if (someDataUser->enabled() &&
                        someDataAccount->enabled() &&
                        someDataAccount != this->_dataAccount &&
                        (someDataAccount->capabilities() & Capabilities::Administrator) != Capabilities::None)
                    {
                        hasEnabledAdminAccount = true;
                    }
                }
            }
            if (!hasEnabledAdminAccount)
            {
                throw AccessWouldBeLostException();
            }
        }

        //  Do the work
        return _dataAccount->setCapabilities(capabilities); //  may throw
    }
    catch (const tt3::util::Exception & ex)
    {
        WorkspaceException::translateAndThrow(ex);
    }
}

//////////
//  Operations (associations)
User AccountImpl::user(const Credentials & credentials) const throws(WorkspaceException)
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
        return _workspace->_getProxy(_dataAccount->user()); //  may throw
    }
    catch (const tt3::util::Exception & ex)
    {
        WorkspaceException::translateAndThrow(ex);
    }
}

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
