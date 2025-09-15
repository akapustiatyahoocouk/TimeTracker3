//
//  tt3-ws/PublicActivityImpl.cpp - tt3::ws::PublicActivityImpl class implementation
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
PublicActivityImpl::PublicActivityImpl(
        Workspace workspace,
        tt3::db::api::IPublicActivity * dataPublicActivity
    ) : ActivityImpl(workspace, dataPublicActivity),
        _dataPublicActivity(dataPublicActivity)
{
    _dataPublicActivity->addReference();
}

PublicActivityImpl::~PublicActivityImpl()
{
    _dataPublicActivity->removeReference();
}

//////////
//  Implementation (Access control)
bool PublicActivityImpl::_canRead(
        const Credentials & credentials
    ) const
{
    Q_ASSERT(_workspace->_guard.isLockedByCurrentThread());

    try
    {   //  Anyone authorized to access a Workspace
        //  can see all PublicActivities there
        _workspace->_validateAccessRights(credentials); //  may throw
        return true;
    }
    catch (const AccessDeniedException &)
    {   //  This is a special case!
        return false;
    }
    catch (const tt3::util::Exception & ex)
    {
        WorkspaceException::translateAndThrow(ex);
    }
}

bool PublicActivityImpl::_canModify(
        const Credentials & credentials
    ) const
{
    Q_ASSERT(_workspace->_guard.isLockedByCurrentThread());

    try
    {
        Capabilities clientCapabilities = _workspace->_validateAccessRights(credentials); //  may throw
        return (clientCapabilities & Capabilities::Administrator) != Capabilities::None ||
               (clientCapabilities & Capabilities::ManagePublicActivities) != Capabilities::None;
    }
    catch (const AccessDeniedException &)
    {   //  This is a special case!
        return false;
    }
    catch (const tt3::util::Exception & ex)
    {
        WorkspaceException::translateAndThrow(ex);
    }
}

bool PublicActivityImpl::_canDestroy(
        const Credentials & credentials
    ) const
{
    Q_ASSERT(_workspace->_guard.isLockedByCurrentThread());

    try
    {
        Capabilities clientCapabilities = _workspace->_validateAccessRights(credentials); //  may throw
        return (clientCapabilities & Capabilities::Administrator) != Capabilities::None ||
               (clientCapabilities & Capabilities::ManagePublicActivities) != Capabilities::None;
    }
    catch (const AccessDeniedException &)
    {   //  This is a special case!
        return false;
    }
    catch (const tt3::util::Exception & ex)
    {
        WorkspaceException::translateAndThrow(ex);
    }
}


//  End of tt3-ws/PublicActivityImpl.cpp
