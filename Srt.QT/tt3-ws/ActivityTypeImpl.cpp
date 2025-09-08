//
//  tt3-ws/ActivityTypeImpl.cpp - tt3::ws::ActivityTypeImpl class implementation
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
ActivityTypeImpl::ActivityTypeImpl(
        Workspace workspace,
        tt3::db::api::IActivityType * dataActivityType)
    :   ObjectImpl(workspace, dataActivityType),
        _dataActivityType(dataActivityType)
{
    _dataActivityType->addReference();
}

ActivityTypeImpl::~ActivityTypeImpl()
{
    _dataActivityType->removeReference();
}

//////////
//  Operations (properties)
auto ActivityTypeImpl::displayName(
        const Credentials & /*credentials*/
    ) const -> QString
{
    throw CustomWorkspaceException("Not yet implemented");
}

void ActivityTypeImpl::setDisplayName(
        const Credentials & /*credentials*/,
        const QString & /*displayName*/
    )
{
    throw CustomWorkspaceException("Not yet implemented");
}

auto ActivityTypeImpl::description(
        const Credentials & /*credentials*/
    ) const -> QString
{
    throw CustomWorkspaceException("Not yet implemented");
}

void ActivityTypeImpl::setDescription(
        const Credentials & /*credentials*/,
        const QString & /*description*/
    )
{
    throw CustomWorkspaceException("Not yet implemented");
}

//////////
//  Operations (associations)
auto ActivityTypeImpl::activities(
        const Credentials & /*credentials*/
    ) const -> Activities
{
    throw CustomWorkspaceException("Not yet implemented");
}

//////////
//  Implementation - Access control
bool ActivityTypeImpl::_canRead(
        const Credentials & /*credentials*/
    ) const
{
    throw CustomWorkspaceException("Not yet implemented");
}

bool ActivityTypeImpl::_canModify(
        const Credentials & /*credentials*/
    ) const
{
    throw CustomWorkspaceException("Not yet implemented");
}

bool ActivityTypeImpl::_canDestroy(
        const Credentials & /*credentials*/
    ) const
{
    throw CustomWorkspaceException("Not yet implemented");
}

//  End of tt3-ws/ActivityTypeImpl.cpp
