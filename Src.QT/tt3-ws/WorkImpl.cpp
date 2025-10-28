//
//  tt3-ws/WorkImpl.cpp - tt3::ws::WorkImpl class implementation
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
WorkImpl::WorkImpl(
        Workspace workspace,
        tt3::db::api::IWork * dataWork
    ) : ObjectImpl(workspace, dataWork),
        _dataWork(dataWork)
{
    _dataWork->addReference();
}

WorkImpl::~WorkImpl()
{
    _dataWork->removeReference();
}

//////////
//  Operations (properties)
auto WorkImpl::startedAt(
        const tt3::ws::Credentials & credentials
    ) const -> QDateTime
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
        return _dataWork->startedAt();  //  may throw
    }
    catch (const tt3::util::Exception & ex)
    {   //  OOPS! Translate & re-throw
        WorkspaceException::translateAndThrow(ex);
    }
}

auto WorkImpl::finishedAt(
        const tt3::ws::Credentials & credentials
    ) const -> QDateTime
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
        return _dataWork->finishedAt(); //  may throw
    }
    catch (const tt3::util::Exception & ex)
    {   //  OOPS! Translate & re-throw
        WorkspaceException::translateAndThrow(ex);
    }
}

//////////
//  Operations (associations)
auto WorkImpl::account(
        const tt3::ws::Credentials & credentials
    ) const -> Account
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
        return _workspace->_getProxy(_dataWork->account()); //  may throw
    }
    catch (const tt3::util::Exception & ex)
    {   //  OOPS! Translate & re-throw
        WorkspaceException::translateAndThrow(ex);
    }
}

auto WorkImpl::activity(
        const tt3::ws::Credentials & credentials
    ) const -> Activity
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
        return _workspace->_getProxy(_dataWork->activity()); //  may throw
    }
    catch (const tt3::util::Exception & ex)
    {   //  OOPS! Translate & re-throw
        WorkspaceException::translateAndThrow(ex);
    }
}

//////////
//  Implementation - Access control
bool WorkImpl::_canRead(
        const Credentials & credentials
    ) const
{
    Q_ASSERT(_workspace->_guard.isLockedByCurrentThread());

    try
    {
        Capabilities clientCapabilities = _workspace->_validateAccessRights(credentials); //  may throw
        if (clientCapabilities.contains(Capability::Administrator))
        {   //  Can read any Works
            return true;
        }
        //  An ordinary user can only see their own Works
        tt3::db::api::IAccount * callerAccount =
            _workspace->_database->tryLogin(credentials._login, credentials._password); //  may throw
        return callerAccount != nullptr &&
               callerAccount->user() == _dataWork->account()->user();
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

bool WorkImpl::_canModify(
        const Credentials & credentials
    ) const
{
    Q_ASSERT(_workspace->_guard.isLockedByCurrentThread());

    try
    {
        _workspace->_validateAccessRights(credentials); //  may throw
        return false;   //  Works are immutable!
    }
    catch (const tt3::util::Exception & ex)
    {   //  OOPS! Translate & re-throw
        WorkspaceException::translateAndThrow(ex);
    }
}

bool WorkImpl::_canDestroy(
    const Credentials & credentials
    ) const
{
    Q_ASSERT(_workspace->_guard.isLockedByCurrentThread());

    try
    {
        Capabilities clientCapabilities = _workspace->_validateAccessRights(credentials); //  may throw
        if (clientCapabilities.contains(Capability::Administrator))
        {   //  Can destroy any Works
            return true;
        }
        return false;
    }
    catch (const tt3::util::Exception & ex)
    {   //  OOPS! Translate & re-throw
        WorkspaceException::translateAndThrow(ex);
    }
}

//  End of tt3-ws/WorkImpl.cpp
