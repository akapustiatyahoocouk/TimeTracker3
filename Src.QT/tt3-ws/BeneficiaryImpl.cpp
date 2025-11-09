//
//  tt3-ws/BeneficiaryImpl.cpp - tt3::ws::BeneficiaryImpl class implementation
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
BeneficiaryImpl::BeneficiaryImpl(
        Workspace workspace,
        tt3::db::api::IBeneficiary * dataBeneficiary
    ) : ObjectImpl(workspace, dataBeneficiary),
        _dataBeneficiary(dataBeneficiary)
{
    _dataBeneficiary->addReference();
}

BeneficiaryImpl::~BeneficiaryImpl()
{
    _dataBeneficiary->removeReference();
}

//////////
//  Operations (properties)
QString BeneficiaryImpl::displayName(
        const Credentials & credentials
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
        return _dataBeneficiary->displayName();   //  may throw
    }
    catch (const tt3::util::Exception & ex)
    {   //  OOPS! Translate & re-throw
        WorkspaceException::translateAndThrow(ex);
    }
}

void BeneficiaryImpl::setDisplayName(
        const Credentials & credentials,
        const QString & displayName
    )
{
    tt3::util::Lock _(_workspace->_guard);
    _ensureLive();  //  may throw

    try
    {
        //  Validate access rights
        if (!_canModify(credentials))
        {
            throw AccessDeniedException();
        }

        //  Do the work
        _dataBeneficiary->setDisplayName(displayName); //  may throw
    }
    catch (const tt3::util::Exception & ex)
    {   //  OOPS! Translate & re-throw
        WorkspaceException::translateAndThrow(ex);
    }
}

QString BeneficiaryImpl::description(
        const Credentials & credentials
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
        return _dataBeneficiary->description();   //  may throw
    }
    catch (const tt3::util::Exception & ex)
    {   //  OOPS! Translate & re-throw
        WorkspaceException::translateAndThrow(ex);
    }
}

void BeneficiaryImpl::setDescription(
        const Credentials & credentials,
        const QString & description
    )
{
    tt3::util::Lock _(_workspace->_guard);
    _ensureLive();  //  may throw

    try
    {
        //  Validate access rights
        if (!_canModify(credentials))
        {
            throw AccessDeniedException();
        }

        //  Do the work
        _dataBeneficiary->setDescription(description); //  may throw
    }
    catch (const tt3::util::Exception & ex)
    {   //  OOPS! Translate & re-throw
        WorkspaceException::translateAndThrow(ex);
    }
}

//////////
//  Operations (associations)
auto BeneficiaryImpl:: workloads(
        const Credentials & credentials
    ) const -> Workloads
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
            _dataBeneficiary->workloads(),
            [&](auto dw)
            {
                return _workspace->_getProxy(dw);
            });
    }
    catch (const tt3::util::Exception & ex)
    {   //  OOPS! Translate & re-throw
        WorkspaceException::translateAndThrow(ex);
    }
}

void BeneficiaryImpl::setWorkloads(
        const Credentials & credentials,
        const Workloads & workloads
    )
{
    tt3::util::Lock _(_workspace->_guard);
    _ensureLive();  //  may throw

    try
    {
        //  Validate access rights
        if (!_canModify(credentials)) //  may throw
        {
            throw AccessDeniedException();
        }
        for (Workload workload : workloads)
        {
            if (workload != nullptr &&
                !workload->_canModify(credentials))
            {   //  OOPS!
                throw AccessDeniedException();
            }
        }

        //  Do the work
        _dataBeneficiary->setWorkloads( //  may throw
            tt3::util::transform(
                workloads,
                [](auto w)
                {
                    return (w != nullptr) ? w->_dataWorkload : nullptr;
                }));
    }
    catch (const tt3::util::Exception & ex)
    {   //  OOPS! Translate & re-throw
        WorkspaceException::translateAndThrow(ex);
    }
}

void BeneficiaryImpl::addWorkload(
        const Credentials & credentials,
        Workload workload
    )
{
    tt3::util::Lock _(_workspace->_guard);
    _ensureLive();  //  may throw

    try
    {
        //  Validate access rights.
        if (!_canModify(credentials)) //  may throw
        {
            throw AccessDeniedException();
        }
        if (workload != nullptr &&
            !workload->_canModify(credentials))
        {   //  OOPS!
            throw AccessDeniedException();
        }

        //  Do the work
        _dataBeneficiary->addWorkload(  //  may throw
            (workload != nullptr) ? workload->_dataWorkload : nullptr);
    }
    catch (const tt3::util::Exception & ex)
    {   //  OOPS! Translate & re-throw
        WorkspaceException::translateAndThrow(ex);
    }
}

void BeneficiaryImpl::removeWorkload(
        const Credentials & credentials,
        Workload workload
    )
{
    tt3::util::Lock _(_workspace->_guard);
    _ensureLive();  //  may throw

    try
    {
        //  Validate access rights.
        if (!_canModify(credentials)) //  may throw
        {
            throw AccessDeniedException();
        }
        if (workload != nullptr &&
            !workload->_canModify(credentials))
        {   //  OOPS!
            throw AccessDeniedException();
        }

        //  Do the work
        _dataBeneficiary->removeWorkload(  //  may throw
            (workload != nullptr) ? workload->_dataWorkload : nullptr);
    }
    catch (const tt3::util::Exception & ex)
    {   //  OOPS! Translate & re-throw
        WorkspaceException::translateAndThrow(ex);
    }
}

//////////
//  Implementation - Access control
bool BeneficiaryImpl::_canRead(
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
        _workspace->_validateAccessRights(credentials); //  may throw
        //  Anyone authorized to access workspace can see all activity types
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

bool BeneficiaryImpl::_canModify(
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
        if (clientCapabilities.contains(Capability::Administrator) ||
            clientCapabilities.contains(Capability::ManageBeneficiaries))
        {   //  Can modify any activoty type
            return true;
        }
        return false;
    }
    catch (const tt3::util::Exception & ex)
    {   //  OOPS! Translate & re-throw
        WorkspaceException::translateAndThrow(ex);
    }
}

bool BeneficiaryImpl::_canDestroy(
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
        if (clientCapabilities.contains(Capability::Administrator) ||
            clientCapabilities.contains(Capability::ManageBeneficiaries))
        {   //  Can destroy any activoty type
            return true;
        }
        return false;
    }
    catch (const tt3::util::Exception & ex)
    {   //  OOPS! Translate & re-throw
        WorkspaceException::translateAndThrow(ex);
    }
}

//  End of tt3-ws/BeneficiaryImpl.cpp
