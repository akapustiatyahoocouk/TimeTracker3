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
//  Operations (associations)
auto PublicTaskImpl::parent(
        const Credentials & credentials
    ) const -> PublicTask
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
        if (auto dataParent = _dataPublicTask->parent())  //  may throw
        {
            return _workspace->_getProxy(dataParent);
        }
        return PublicTask();
    }
    catch (const tt3::util::Exception & ex)
    {   //  OOPS! Translate & re-throw
        WorkspaceException::translateAndThrow(ex);
    }
}

void PublicTaskImpl::setParent(
        const Credentials & credentials,
        PublicTask parent
    )
{
    tt3::util::Lock lock(_workspace->_guard);
    _ensureLive();  //  may throw
    if (parent != nullptr)
    {
        if (parent->_workspace != this->_workspace)
        {   //  OOPS! Not in the same workspace!
            throw IncompatibleInstanceException(parent->type());
        }
        parent->_ensureLive();    //  may throw
    }

    try
    {
        //  Validate access rights
        if (!_canModify(credentials))
        {
            throw AccessDeniedException();
        }
        //  Do the work
        _dataPublicTask->setParent(
            (parent != nullptr) ?
                parent->_dataPublicTask :
                nullptr);   //  may throw
    }
    catch (const tt3::util::Exception & ex)
    {   //  OOPS! Translate & re-throw
        WorkspaceException::translateAndThrow(ex);
    }
}

auto PublicTaskImpl::children(
        const Credentials & credentials
    ) const -> PublicTasks
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
        PublicTasks result;
        for (auto dataChild : _dataPublicTask->children())  //  may throw
        {
            result.insert(_workspace->_getProxy(dataChild));
        }
        return result;
    }
    catch (const tt3::util::Exception & ex)
    {   //  OOPS! Translate & re-throw
        WorkspaceException::translateAndThrow(ex);
    }
}

//////////
//  Operations (life cycle)
auto PublicTaskImpl::createChild(
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
    ) -> PublicTask
{
    tt3::util::Lock lock(_workspace->_guard);
    _ensureLive();

    try
    {
        //  Check access rights
        Capabilities clientCapabilities =
            _workspace->_validateAccessRights(credentials); //  may throw
        if (!clientCapabilities.contains(Capability::Administrator) &&
            !clientCapabilities.contains(Capability::ManagePublicTasks))
        {   //  OOPS! Can't!
            throw AccessDeniedException();
        }
        //  Do the work
        tt3::db::api::IPublicTask * dataPublicTask =
            _dataPublicTask->createChild(
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
        return _workspace->_getProxy(dataPublicTask);;
    }
    catch (const tt3::util::Exception & ex)
    {   //  OOPS! Translate & re-throw
        WorkspaceException::translateAndThrow(ex);
    }
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
    {   //  OOPS! Translate & re-throw
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
        return clientCapabilities.contains(Capability::Administrator) ||
               clientCapabilities.contains(Capability::ManagePublicTasks);
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

bool PublicTaskImpl::_canDestroy(
        const Credentials & credentials
    ) const
{
    Q_ASSERT(_workspace->_guard.isLockedByCurrentThread());

    try
    {
        Capabilities clientCapabilities = _workspace->_validateAccessRights(credentials); //  may throw
        return clientCapabilities.contains(Capability::Administrator) ||
               clientCapabilities.contains(Capability::ManagePublicTasks);
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

//  End of tt3-ws/PublicTaskImpl.cpp
