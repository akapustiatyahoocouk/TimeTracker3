//
//  tt3-ws/ProjectImpl.cpp - tt3::ws::ProjectImpl class implementation
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
ProjectImpl::ProjectImpl(
        Workspace workspace,
        tt3::db::api::IProject * dataProject
    ) : WorkloadImpl(workspace, dataProject),
        _dataProject(dataProject)
{
    _dataProject->addReference();
}

ProjectImpl::~ProjectImpl()
{
    _dataProject->removeReference();
}

//////////
//  Operations (properties)
bool ProjectImpl::completed(
        const Credentials & credentials
    ) const
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
        return _dataProject->completed();   //  may throw
    }
    catch (const tt3::util::Exception & ex)
    {   //  OOPS! Translate & re-throw
        WorkspaceException::translateAndThrow(ex);
    }
}

void ProjectImpl::setCompleted(
        const Credentials & credentials,
        bool completed
    )
{
    tt3::util::Lock lock(_workspace->_guard);
    _ensureLive();  //  may throw

    try
    {
        //  Validate access rights
        if (!_canModify(credentials))
        {
            throw AccessDeniedException();
        }
        //  Do the work
        _dataProject->setCompleted(completed); //  may throw
    }
    catch (const tt3::util::Exception & ex)
    {   //  OOPS! Translate & re-throw
        WorkspaceException::translateAndThrow(ex);
    }
}

//////////
//  Operations (associations)
auto ProjectImpl::parent(
        const Credentials & credentials
    ) const -> Project
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
        if (auto dataParent = _dataProject->parent())  //  may throw
        {
            return _workspace->_getProxy(dataParent);
        }
        return Project();
    }
    catch (const tt3::util::Exception & ex)
    {   //  OOPS! Translate & re-throw
        WorkspaceException::translateAndThrow(ex);
    }
}

void ProjectImpl::setParent(
        const Credentials & credentials,
        Project parent
    )
{
    tt3::util::Lock lock(_workspace->_guard);
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
        //  Do the work
        _dataProject->setParent(
            (parent != nullptr) ?
                parent->_dataProject :
                nullptr);   //  may throw
    }
    catch (const tt3::util::Exception & ex)
    {   //  OOPS! Translate & re-throw
        WorkspaceException::translateAndThrow(ex);
    }
}

auto ProjectImpl::children(
        const Credentials & credentials
    ) const -> Projects
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
        return tt3::util::transform(
            _dataProject->children(),   //  may throw
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
auto ProjectImpl::createChild(
        const Credentials & credentials,
        const QString & displayName,
        const QString & description,
        const Beneficiaries & beneficiaries,
        bool completed
    ) -> Project
{
    tt3::util::Lock lock(_workspace->_guard);
    _ensureLive();

    try
    {
        //  Check access rights
        Capabilities clientCapabilities =
            _workspace->_validateAccessRights(credentials); //  may throw
        if (!clientCapabilities.contains(Capability::Administrator) &&
            !clientCapabilities.contains(Capability::ManageWorkloads))
        {   //  OOPS! Can't!
            throw AccessDeniedException();
        }
        //  Do the work
        tt3::db::api::Beneficiaries dataBeneficiaries;
        for (Beneficiary beneficiary : beneficiaries)
        {
            dataBeneficiaries.insert(
                (beneficiary != nullptr) ?
                    beneficiary->_dataBeneficiary :
                    nullptr);
        }
        tt3::db::api::IProject * dataProject =
            _dataProject->createChild(
                displayName,
                description,
                dataBeneficiaries,
                completed);
        return _workspace->_getProxy(dataProject);;
    }
    catch (const tt3::util::Exception & ex)
    {   //  OOPS! Translate & re-throw
        WorkspaceException::translateAndThrow(ex);
    }
}

//////////
//  Implementation (Access control)
bool ProjectImpl::_canRead(
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

bool ProjectImpl::_canModify(
        const Credentials & credentials
    ) const
{
    Q_ASSERT(_workspace->_guard.isLockedByCurrentThread());

    try
    {
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

bool ProjectImpl::_canDestroy(
        const Credentials & credentials
    ) const
{
    Q_ASSERT(_workspace->_guard.isLockedByCurrentThread());

    try
    {
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

//  End of tt3-ws/ProjectImpl.cpp
