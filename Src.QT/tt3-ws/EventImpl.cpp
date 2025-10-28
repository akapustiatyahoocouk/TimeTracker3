//
//  tt3-ws/EventImpl.cpp - tt3::ws::EventImpl class implementation
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
EventImpl::EventImpl(
        Workspace workspace,
        tt3::db::api::IEvent * dataEvent
    ) : ObjectImpl(workspace, dataEvent),
        _dataEvent(dataEvent)
{
    _dataEvent->addReference();
}

EventImpl::~EventImpl()
{
    _dataEvent->removeReference();
}

//////////
//  Operations (properties)
auto EventImpl::occurredAt(
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
        return _dataEvent->occurredAt();    //  may throw
    }
    catch (const tt3::util::Exception & ex)
    {   //  OOPS! Translate & re-throw
        WorkspaceException::translateAndThrow(ex);
    }
}

QString EventImpl::summary(
        const tt3::ws::Credentials & credentials
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
        return _dataEvent->summary();    //  may throw
    }
    catch (const tt3::util::Exception & ex)
    {   //  OOPS! Translate & re-throw
        WorkspaceException::translateAndThrow(ex);
    }
}

//////////
//  Operations (associations)
auto EventImpl::account(
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
        return _workspace->_getProxy(_dataEvent->account()); //  may throw
    }
    catch (const tt3::util::Exception & ex)
    {   //  OOPS! Translate & re-throw
        WorkspaceException::translateAndThrow(ex);
    }
}

auto EventImpl::activities(
        const tt3::ws::Credentials & credentials
    ) const -> Activities
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
            _dataEvent->activities(),   //  may throw
            [&](auto da)
            {
                return _workspace->_getProxy(da);
            });
    }
    catch (const tt3::util::Exception & ex)
    {   //  OOPS! Translate & re-throw
        WorkspaceException::translateAndThrow(ex);
    }
}

//////////
//  Implementation - Access control
bool EventImpl::_canRead(
        const Credentials & credentials
    ) const
{
    Q_ASSERT(_workspace->_guard.isLockedByCurrentThread());

    try
    {
        Capabilities clientCapabilities = _workspace->_validateAccessRights(credentials); //  may throw
        if (clientCapabilities.contains(Capability::Administrator))
        {   //  Can read any Events
            return true;
        }
        //  An ordinary user can only see their own Events
        tt3::db::api::IAccount * callerAccount =
            _workspace->_database->tryLogin(credentials._login, credentials._password); //  may throw
        return callerAccount != nullptr &&
               callerAccount->user() == _dataEvent->account()->user();
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

bool EventImpl::_canModify(
        const Credentials & credentials
    ) const
{
    Q_ASSERT(_workspace->_guard.isLockedByCurrentThread());

    try
    {
        _workspace->_validateAccessRights(credentials); //  may throw
        return false;   //  Events are immutable!
    }
    catch (const tt3::util::Exception & ex)
    {   //  OOPS! Translate & re-throw
        WorkspaceException::translateAndThrow(ex);
    }
}

bool EventImpl::_canDestroy(
        const Credentials & credentials
    ) const
{
    Q_ASSERT(_workspace->_guard.isLockedByCurrentThread());

    try
    {
        Capabilities clientCapabilities = _workspace->_validateAccessRights(credentials); //  may throw
        if (clientCapabilities.contains(Capability::Administrator))
        {   //  Can destroy any Events
            return true;
        }
        return false;
    }
    catch (const tt3::util::Exception & ex)
    {   //  OOPS! Translate & re-throw
        WorkspaceException::translateAndThrow(ex);
    }
}

//  End of tt3-ws/EventImpl.cpp
