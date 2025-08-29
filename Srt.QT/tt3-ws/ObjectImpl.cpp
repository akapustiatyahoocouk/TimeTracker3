//
//  tt3-ws/ObjectImpl.cpp - tt3::ws::ObjectImpl class implementation
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
ObjectImpl::ObjectImpl(Workspace workspace, tt3::db::api::IObject * dataObject)
    :   _workspace(workspace),
        _dataObject(dataObject)
{
    Q_ASSERT(_workspace != nullptr);
    Q_ASSERT(_dataObject != nullptr);
    Q_ASSERT(_workspace->_guard.isLockedByCurrentThread());

    _dataObject->addReference();
}

ObjectImpl::~ObjectImpl()
{
    _dataObject->removeReference();
}

//////////
//  Operations (general)
ObjectType * ObjectImpl::type() const
{   //  No need to synchronize
    return _dataObject->type();
}

Workspace ObjectImpl::workspace() const
{   //  No need to synchronize
    return _workspace;
}

Oid ObjectImpl::oid() const
{   //  No need to synchronize
    return _dataObject->oid();
}

bool ObjectImpl::isLive() const
{   //  No need to synchronize
    return _dataObject->isLive();
}

//////////
//  Operations (life cycle)
//  TODO implement void ObjectImpl::destroy(const Credentials & credentials) throws(WorkspaceException);

//////////
//  Implementation helpers
void ObjectImpl::_ensureLive() const throws(WorkspaceException)
{
    Q_ASSERT(_workspace->_guard.isLockedByCurrentThread());

    _workspace->_ensureOpen();  //  may throw
    if (!_dataObject->isLive())
    {   //  OOPS!
        throw InstanceDeadException();
    }
}

//  End of tt3-ws/ObjectImpl.cpp
