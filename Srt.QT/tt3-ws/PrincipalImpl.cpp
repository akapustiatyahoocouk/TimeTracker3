//
//  tt3-ws/PrincipalImpl.cpp - tt3::ws::PrincipalImpl class implementation
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
PrincipalImpl::PrincipalImpl(Workspace workspace, tt3::db::api::IPrincipal * dataPrincipal)
    :   ObjectImpl(workspace, dataPrincipal),
        _dataPrincipal(dataPrincipal)
{
    _dataPrincipal->addReference();
}

PrincipalImpl::~PrincipalImpl()
{
    _dataPrincipal->removeReference();
}

//////////
//  Operations (properties)
bool PrincipalImpl::enabled(const Credentials & credentials) const throws(WorkspaceException)
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
        return _dataPrincipal->enabled();   //  may throw
    }
    catch (const tt3::util::Exception & ex)
    {
        WorkspaceException::translateAndThrow(ex);
    }
}

/*  TODO implement
void PrincipalImpl::setEnabled(const Credentials & credentials, bool enabled) throws(WorkspaceException);
QStringList PrincipalImpl::emailAddresses(const Credentials & credentials) const throws(WorkspaceException);
void PrincipalImpl::setEmailAddresses(const Credentials & credentials, const QStringList & emailAddresses) throws(WorkspaceException);
*/

//  End of tt3-ws/PrincipalImpl.cpp
