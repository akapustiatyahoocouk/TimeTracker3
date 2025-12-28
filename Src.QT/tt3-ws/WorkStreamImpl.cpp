//
//  tt3-ws/WorkStreamImpl.cpp - tt3::ws::WorkStreamImpl class implementation
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
WorkStreamImpl::WorkStreamImpl(
        Workspace workspace,
        tt3::db::api::IWorkStream * dataWorkStream
    ) : WorkloadImpl(workspace, dataWorkStream),
        _dataWorkStream(dataWorkStream)
{
    _dataWorkStream->addReference();
}

WorkStreamImpl::~WorkStreamImpl()
{
    _dataWorkStream->removeReference();
}

//////////
//  Implementation (Access control)
bool WorkStreamImpl::_canRead(
        const Credentials & credentials
    ) const
{
    Q_ASSERT(_workspace->_guard.isLockedByCurrentThread());

    try
    {
        if (_workspace->_isBackupCredentials(credentials) ||
            _workspace->_isRestoreCredentials(credentials) ||
            _workspace->_isReportCredentials(credentials))
        {   //  Special access - can read anything
            return true;
        }
        //  Anyone authorized to access a Workspace
        //  can see all PublicActivities there
        _workspace->_validateAccessRights(credentials); //  may throw
        return true;
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

bool WorkStreamImpl::_canModify(
        const Credentials & credentials
    ) const
{
    Q_ASSERT(_workspace->_guard.isLockedByCurrentThread());

    try
    {
        if (_workspace->_isRestoreCredentials(credentials))
        {   //  Special access - can modify anything
            return true;
        }
        if (_workspace->_isBackupCredentials(credentials) ||
            _workspace->_isReportCredentials(credentials))
        {   //  Special access - cannot modify anything
            return false;
        }
        Capabilities clientCapabilities = _workspace->_validateAccessRights(credentials); //  may throw
        return clientCapabilities.contains(Capability::Administrator) ||
               clientCapabilities.contains(Capability::ManageWorkloads);
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

bool WorkStreamImpl::_canDestroy(
        const Credentials & credentials
    ) const
{
    Q_ASSERT(_workspace->_guard.isLockedByCurrentThread());

    try
    {
        if (_workspace->_isRestoreCredentials(credentials))
        {   //  Special access - can destroy anything
            return true;
        }
        if (_workspace->_isBackupCredentials(credentials) ||
            _workspace->_isReportCredentials(credentials))
        {   //  Special access - cannot destroy anything
            return false;
        }
        Capabilities clientCapabilities = _workspace->_validateAccessRights(credentials); //  may throw
        return clientCapabilities.contains(Capability::Administrator) ||
               clientCapabilities.contains(Capability::ManageWorkloads);
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

//  End of tt3-ws/WorkStreamImpl.cpp
