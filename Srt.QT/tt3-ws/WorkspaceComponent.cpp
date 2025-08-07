//
//  tt3-ws/WorkspaceComponent.cpp - tt3::ws::WorkspaceComponent class implementation
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

IMPLEMENT_SINGLETON(WorkspaceComponent)
WorkspaceComponent::WorkspaceComponent() {}
WorkspaceComponent::~WorkspaceComponent() {}

//////////
//  IComponent
QString WorkspaceComponent::mnemonic() const
{
    return "tt3-ws";
}

QString WorkspaceComponent::displayName() const
{
    return "TimeTracker3 workspace layer";
}

QString WorkspaceComponent::desciption() const
{
    return "Defines workspace data access layer for TimeTracker3";
}

QString WorkspaceComponent::copyright() const
{
    return "Copyright (C) 2026, Andrey Kapustin";
}

QVersionNumber WorkspaceComponent::version() const
{
    return QVersionNumber(1, 0, 0);
}

QString WorkspaceComponent::buildNumber() const
{
    return __TIMESTAMP__;
}

//  End of tt3-ws/WorkspaceComponent.cpp
