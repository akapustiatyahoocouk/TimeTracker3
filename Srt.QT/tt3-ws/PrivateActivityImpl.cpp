//
//  tt3-ws/PrivateActivityImpl.cpp - tt3::ws::PrivateActivityImpl class implementation
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
PrivateActivityImpl::PrivateActivityImpl(
        Workspace workspace,
        tt3::db::api::IPrivateActivity * dataPrivateActivity
    ) : ActivityImpl(workspace, dataPrivateActivity),
        _dataPrivateActivity(dataPrivateActivity)
{
    _dataPrivateActivity->addReference();
}

PrivateActivityImpl::~PrivateActivityImpl()
{
    _dataPrivateActivity->removeReference();
}

//////////
//  Operations (associations)
auto PrivateActivityImpl::owner(
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
        return _workspace->_getProxy(_dataPrivateActivity->owner()); //  may throw
    }
    catch (const tt3::util::Exception & ex)
    {   //  OOPS! Translate & re-throw
        WorkspaceException::translateAndThrow(ex);
    }
}

//////////
//  Implementation (Access control)
bool PrivateActivityImpl::_canRead(
        const Credentials & credentials
    ) const
{
    Q_ASSERT(_workspace->_guard.isLockedByCurrentThread());

    try
    {
        Q_ASSERT(_workspace->_guard.isLockedByCurrentThread());

        try
        {
            Capabilities clientCapabilities = _workspace->_validateAccessRights(credentials); //  may throw
            if (clientCapabilities.contains(Capability::Administrator))
            {   //  Can read private activities of all users
                return true;
            }
            //  The caller can only see his own private activities
            tt3::db::api::IAccount * callerAccount =
                _workspace->_database->tryLogin(credentials._login, credentials._password); //  may throw
            return callerAccount != nullptr &&
                   callerAccount->user() == _dataPrivateActivity->owner();   //  may throw
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
    catch (const AccessDeniedException &)
    {   //  This is a special case!
        return false;
    }
    catch (const tt3::util::Exception & ex)
    {   //  OOPS! Translate & re-throw
        WorkspaceException::translateAndThrow(ex);
    }
}

bool PrivateActivityImpl::_canModify(
        const Credentials & credentials
    ) const
{
    Q_ASSERT(_workspace->_guard.isLockedByCurrentThread());

    try
    {
        Q_ASSERT(_workspace->_guard.isLockedByCurrentThread());

        try
        {
            Capabilities clientCapabilities = _workspace->_validateAccessRights(credentials); //  may throw
            if (clientCapabilities.contains(Capability::Administrator))
            {   //  Can modify private activities of all users
                return true;
            }
            //  The caller can only modify his own private activities
            //  IF they ALSO have the corresponding capability
            tt3::db::api::IAccount * callerAccount =
                _workspace->_database->tryLogin(credentials._login, credentials._password); //  may throw
            return clientCapabilities.contains(Capability::ManagePrivateActivities) &&
                   callerAccount != nullptr &&
                   callerAccount->user() == _dataPrivateActivity->owner();   //  may throw
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
    catch (const AccessDeniedException &)
    {   //  This is a special case!
        return false;
    }
    catch (const tt3::util::Exception & ex)
    {   //  OOPS! Translate & re-throw
        WorkspaceException::translateAndThrow(ex);
    }
}

bool PrivateActivityImpl::_canDestroy(
        const Credentials & credentials
    ) const
{
    Q_ASSERT(_workspace->_guard.isLockedByCurrentThread());

    try
    {
        Q_ASSERT(_workspace->_guard.isLockedByCurrentThread());

        try
        {
            Capabilities clientCapabilities = _workspace->_validateAccessRights(credentials); //  may throw
            if (clientCapabilities.contains(Capability::Administrator))
            {   //  Can destroy private activities of all users
                return true;
            }
            //  The caller can only destroy his own private activities
            //  IF they ALSO have the corresponding capability
            tt3::db::api::IAccount * callerAccount =
                _workspace->_database->tryLogin(credentials._login, credentials._password); //  may throw
            return clientCapabilities.contains(Capability::ManagePrivateActivities) &&
                   callerAccount != nullptr &&
                   callerAccount->user() == _dataPrivateActivity->owner();   //  may throw
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
    catch (const AccessDeniedException &)
    {   //  This is a special case!
        return false;
    }
    catch (const tt3::util::Exception & ex)
    {   //  OOPS! Translate & re-throw
        WorkspaceException::translateAndThrow(ex);
    }
}

//  End of tt3-ws/PrivateActivityImpl.cpp
