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

void ObjectImpl::setOid(
        const Credentials & credentials,
        const Oid & oid
    )
{
    tt3::util::Lock _(_workspace->_guard);
    _ensureLive();

    try
    {
        //  Validate access rights
        if (!_workspace->_isRestoreCredentials(credentials))
        {
            throw AccessDeniedException();
        }

        //  Do the work
        auto oldOid = _dataObject->oid();
        _dataObject->setOid(oid);   //  may throw
        //  Database change has been successful - must
        //  update Workspace caches
        if (_workspace->_proxyCache.contains(oldOid))
        {
            Object proxy = _workspace->_proxyCache[oldOid];
            _workspace->_proxyCache.remove(oldOid);
            _workspace->_proxyCache[oid] = proxy;
        }
    }
    catch (const tt3::util::Exception & ex)
    {   //  OOPS! Translate & re-throw
        WorkspaceException::translateAndThrow(ex);
    }
}

bool ObjectImpl::isLive() const
{   //  No need to synchronize
    return _dataObject->isLive();
}

//////////
//  Operations (life cycle)
void ObjectImpl::destroy(
        const Credentials & credentials
    )
{
    tt3::util::Lock _(_workspace->_guard);
    _ensureLive();

    try
    {
        //  Validate access rights
        if (!_canDestroy(credentials))
        {
            throw AccessDeniedException();
        }
        if (_destroyingLosesAccess())
        {
            throw AccessWouldBeLostException();
        }

        //  Do the work
        _dataObject->destroy(); //  may throw
    }
    catch (const tt3::util::Exception & ex)
    {   //  OOPS! Translate & re-throw
        WorkspaceException::translateAndThrow(ex);
    }
}

//////////
//  Operations (access control)
bool ObjectImpl::canRead(
        const Credentials & credentials
    ) const
{
    tt3::util::Lock _(_workspace->_guard);
    _ensureLive();

    return _canRead(credentials);
}

bool ObjectImpl::canModify(
        const Credentials & credentials
    ) const
{
    tt3::util::Lock _(_workspace->_guard);
    _ensureLive();

    return _canModify(credentials);
}

bool ObjectImpl::canDestroy(
        const Credentials & credentials
    ) const
{
    tt3::util::Lock _(_workspace->_guard);
    _ensureLive();

    return _canDestroy(credentials);
}

//////////
//  Implementation helpers
void ObjectImpl::_ensureLive(
    ) const
{
    Q_ASSERT(_workspace->_guard.isLockedByCurrentThread());

    _workspace->_ensureOpen();  //  may throw
    if (!_dataObject->isLive())
    {   //  OOPS!
        throw InstanceDeadException();
    }
}

//  End of tt3-ws/ObjectImpl.cpp
