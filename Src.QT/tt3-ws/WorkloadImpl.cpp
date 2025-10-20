//
//  tt3-ws/WorkloadImpl.cpp - tt3::ws::WorkloadImpl class implementation
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
WorkloadImpl::WorkloadImpl(
        Workspace workspace,
        tt3::db::api::IWorkload * dataWorkload
    ) : ObjectImpl(workspace, dataWorkload),
        _dataWorkload(dataWorkload)
{
    _dataWorkload->addReference();
}

WorkloadImpl::~WorkloadImpl()
{
    _dataWorkload->removeReference();
}

//////////
//  Operations (properties)
QString WorkloadImpl::displayName(
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
        return _dataWorkload->displayName();   //  may throw
    }
    catch (const tt3::util::Exception & ex)
    {   //  OOPS! Translate & re-throw
        WorkspaceException::translateAndThrow(ex);
    }
}

void WorkloadImpl::setDisplayName(
        const Credentials & credentials,
        const QString & displayName
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
        _dataWorkload->setDisplayName(displayName); //  may throw
    }
    catch (const tt3::util::Exception & ex)
    {   //  OOPS! Translate & re-throw
        WorkspaceException::translateAndThrow(ex);
    }
}

QString WorkloadImpl::description(
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
        return _dataWorkload->description();    //  may throw
    }
    catch (const tt3::util::Exception & ex)
    {   //  OOPS! Translate & re-throw
        WorkspaceException::translateAndThrow(ex);
    }
}

void WorkloadImpl::setDescription(
        const Credentials & credentials,
        const QString & description
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
        _dataWorkload->setDescription(description); //  may throw
    }
    catch (const tt3::util::Exception & ex)
    {   //  OOPS! Translate & re-throw
        WorkspaceException::translateAndThrow(ex);
    }
}

//////////
//  Operations (associations)
auto WorkloadImpl::contributingActivities(
        const Credentials & credentials
    ) const -> Activities
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
        Activities result;
        for (auto dataActivity : _dataWorkload->contributingActivities())   //  may throw
        {
            Activity activity = _workspace->_getProxy(dataActivity);
            if (activity->_canRead(credentials))    //  may throw
            {
                result.insert(activity);
            }
        }
        return result;
    }
    catch (const tt3::util::Exception & ex)
    {   //  OOPS! Translate & re-throw
        WorkspaceException::translateAndThrow(ex);
    }
}

auto WorkloadImpl::beneficiaries(
        const Credentials & credentials
    ) const -> Beneficiaries
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
        Beneficiaries result;
        for (auto dataBeneficiary : _dataWorkload->beneficiaries())   //  may throw
        {
            Beneficiary beneficiary = _workspace->_getProxy(dataBeneficiary);
            if (beneficiary->_canRead(credentials))    //  may throw
            {
                result.insert(beneficiary);
            }
        }
        return result;
    }
    catch (const tt3::util::Exception & ex)
    {   //  OOPS! Translate & re-throw
        WorkspaceException::translateAndThrow(ex);
    }
}

void WorkloadImpl::setBeneficiaries(
        const Credentials & /*credentials*/,
        const Beneficiaries & /*beneficiaries*/
    )
{
    throw tt3::util::NotImplementedError();
}

void WorkloadImpl::addBeneficiary(
        const Credentials & /*credentials*/,
        Beneficiary /*beneficiary*/
    )
{
    throw tt3::util::NotImplementedError();
}

void WorkloadImpl::removeBeneficiary(
        const Credentials & /*credentials*/,
        Beneficiary /*beneficiary*/
    )
{
    throw tt3::util::NotImplementedError();
}

auto WorkloadImpl::assignedUsers(
        const Credentials & /*credentials*/
    ) const -> Users
{
    throw tt3::util::NotImplementedError();
}

void WorkloadImpl::setAssignedUsers(
        const Credentials & /*credentials*/,
        const Users & /*users*/
    )
{
    throw tt3::util::NotImplementedError();
}

void WorkloadImpl::addAssignedUser(
        const Credentials & /*credentials*/,
        User /*user*/
    )
{
    throw tt3::util::NotImplementedError();
}

void WorkloadImpl::removeAssignedUser(
        const Credentials & /*credentials*/,
        User /*user*/
    )
{
    throw tt3::util::NotImplementedError();
}

//  End of tt3-ws/WorkloadImpl.cpp
