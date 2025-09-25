//
//  tt3-ws/PublicTaskImpl.cpp - tt3::ws::PublicTaskImpl class implementation
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
PublicTaskImpl::PublicTaskImpl(
        Workspace workspace,
        tt3::db::api::IPublicTask * dataPublicTask
    ) : ActivityImpl(workspace, dataPublicTask),
        PublicActivityImpl(workspace, dataPublicTask),
        TaskImpl(workspace, dataPublicTask),
        _dataPublicTask(dataPublicTask)
{
    _dataPublicTask->addReference();
}

PublicTaskImpl::~PublicTaskImpl()
{
    _dataPublicTask->removeReference();
}

//////////
//  Implementation (Access control)
bool PublicTaskImpl::_canRead(
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

bool PublicTaskImpl::_canModify(
        const Credentials & credentials
    ) const
{
    Q_ASSERT(_workspace->_guard.isLockedByCurrentThread());

    try
    {
        Capabilities clientCapabilities = _workspace->_validateAccessRights(credentials); //  may throw
        return (clientCapabilities & Capabilities::Administrator) != Capabilities::None ||
               (clientCapabilities & Capabilities::ManagePublicTasks) != Capabilities::None;
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

bool PublicTaskImpl::_canDestroy(
        const Credentials & credentials
    ) const
{
    Q_ASSERT(_workspace->_guard.isLockedByCurrentThread());

    try
    {
        Capabilities clientCapabilities = _workspace->_validateAccessRights(credentials); //  may throw
        return (clientCapabilities & Capabilities::Administrator) != Capabilities::None ||
               (clientCapabilities & Capabilities::ManagePublicTasks) != Capabilities::None;
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

//  End of tt3-ws/PublicTaskImpl.cpp
