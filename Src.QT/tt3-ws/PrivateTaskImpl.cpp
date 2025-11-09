//
//  tt3-ws/PrivateTaskImpl.cpp - tt3::ws::PrivateTaskImpl class implementation
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
PrivateTaskImpl::PrivateTaskImpl(
        Workspace workspace,
        tt3::db::api::IPrivateTask * dataPrivateTask
    ) : ActivityImpl(workspace, dataPrivateTask),
        PrivateActivityImpl(workspace, dataPrivateTask),
        TaskImpl(workspace, dataPrivateTask),
    _dataPrivateTask(dataPrivateTask)
{
    _dataPrivateTask->addReference();
}

PrivateTaskImpl::~PrivateTaskImpl()
{
    _dataPrivateTask->removeReference();
}

//////////
//  Operations (associations)
auto PrivateTaskImpl::parent(
        const Credentials & credentials
    ) const -> PrivateTask
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
        if (auto dataParent = _dataPrivateTask->parent())  //  may throw
        {
            return _workspace->_getProxy(dataParent);
        }
        return PrivateTask();
    }
    catch (const tt3::util::Exception & ex)
    {   //  OOPS! Translate & re-throw
        WorkspaceException::translateAndThrow(ex);
    }
}

void PrivateTaskImpl::setParent(
        const Credentials & credentials,
        PrivateTask parent
    )
{
    tt3::util::Lock _(_workspace->_guard);
    _ensureLive();  //  may throw
    if (parent != nullptr)
    {   //  If in different Workspace, then in different
        //  Database, so don't need to check at this level
        parent->_ensureLive();    //  may throw
    }

    try
    {
        //  Validate access rights
        if (!_canModify(credentials))
        {
            throw AccessDeniedException();
        }
        //  If caller can modify this private task,
        //  the caller can also modify its old
        //  and new parents

        //  Do the work
        _dataPrivateTask->setParent(
            (parent != nullptr) ?
                parent->_dataPrivateTask :
                nullptr);   //  may throw
    }
    catch (const tt3::util::Exception & ex)
    {   //  OOPS! Translate & re-throw
        WorkspaceException::translateAndThrow(ex);
    }
}

auto PrivateTaskImpl::children(
        const Credentials & credentials
    ) const -> PrivateTasks
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
        //  If the caller can read this private task,
        //  the caller can also read all its children

        //  Do the work
        return tt3::util::transform(
            _dataPrivateTask->children(),   //  may thr
            [&](auto dc)
            {
                return _workspace->_getProxy(dc);
            });
    }
    catch (const tt3::util::Exception & ex)
    {   //  OOPS! Translate & re-throw
        WorkspaceException::translateAndThrow(ex);
    }
}

//////////
//  Operations (life cycle)
auto PrivateTaskImpl::createChild(
        const Credentials & credentials,
        const QString & displayName,
        const QString & description,
        const InactivityTimeout & timeout,
        bool requireCommentOnStart,
        bool requireCommentOnStop,
        bool fullScreenReminder,
        ActivityType activityType,
        Workload workload,
        bool completed,
        bool requireCommentOnCompletion
    ) -> PrivateTask
{
    tt3::util::Lock _(_workspace->_guard);
    _ensureLive();

    try
    {
        //  Validate access rights
        if (_workspace->_isBackupCredentials(credentials) ||
            _workspace->_isReportCredentials(credentials))
        {   //  Special access - cannot modify anything
            throw AccessDeniedException();
        }
        if (!_workspace->_isRestoreCredentials(credentials))
        {   //  No special access - use standard access rules
            Capabilities clientCapabilities =
                _workspace->_validateAccessRights(credentials); //  may throw
            if (!clientCapabilities.contains(Capability::Administrator) &&
                !clientCapabilities.contains(Capability::ManagePrivateTasks))
            {   //  OOPS! Can't!
                throw AccessDeniedException();
            }   //  Else special access - can modify anything
        }
        //  Do the work
        tt3::db::api::IPrivateTask * dataPrivateTask =
            _dataPrivateTask->createChild(
                displayName,
                description,
                timeout,
                requireCommentOnStart,
                requireCommentOnStop,
                fullScreenReminder,
                (activityType != nullptr) ? activityType->_dataActivityType : nullptr,
                (workload != nullptr) ? workload->_dataWorkload : nullptr,
                completed,
                requireCommentOnCompletion);
        return _workspace->_getProxy(dataPrivateTask);;
    }
    catch (const tt3::util::Exception & ex)
    {   //  OOPS! Translate & re-throw
        WorkspaceException::translateAndThrow(ex);
    }
}

//////////
//  Implementation (Access control)
bool PrivateTaskImpl::_canRead(
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
        Capabilities clientCapabilities = _workspace->_validateAccessRights(credentials); //  may throw
        if (clientCapabilities.contains(Capability::Administrator))
        {   //  Can read private activities of all users
            return true;
        }
        //  The caller can only see his own private tasks
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

bool PrivateTaskImpl::_canModify(
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
        if (clientCapabilities.contains(Capability::Administrator))
        {   //  Can modify private tasks of all users
            return true;
        }
        //  The caller can only modify his own private tasks
        //  IF they ALSO have the corresponding capability
        tt3::db::api::IAccount * callerAccount =
            _workspace->_database->tryLogin(credentials._login, credentials._password); //  may throw
        return clientCapabilities.contains(Capability::ManagePrivateTasks) &&
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

bool PrivateTaskImpl::_canDestroy(
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
        if (clientCapabilities.contains(Capability::Administrator))
        {   //  Can destroy private tasks of all users
            return true;
        }
        //  The caller can only destroy his own private tasks
        //  IF they ALSO have the corresponding capability
        tt3::db::api::IAccount * callerAccount =
            _workspace->_database->tryLogin(credentials._login, credentials._password); //  may throw
        return clientCapabilities.contains(Capability::ManagePrivateTasks) &&
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

//  End of tt3-ws/PrivateTaskImpl.cpp
