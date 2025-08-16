//
//  tt3-ws/Component.cpp - tt3::ws::Component class implementation
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
//  Singleton
IMPLEMENT_SINGLETON(Component)

Component::Component()
{
    qRegisterMetaType<ChangeNotification>();
    qRegisterMetaType<WorkspaceClosedNotification>();
}

Component::~Component()
{
}

//////////
//  IComponent
QString Component::mnemonic() const
{
    return "tt3-ws";
}

QString Component::displayName() const
{
    return "TimeTracker3 workspace layer";
}

QString Component::desciption() const
{
    return "Defines workspace data access layer for TimeTracker3";
}

QString Component::copyright() const
{
    return "Copyright (C) 2026, Andrey Kapustin";
}

QVersionNumber Component::version() const
{
    return QVersionNumber(1, 0, 0);
}

QString Component::buildNumber() const
{
    return __DATE__;
}


//////////
//  Component::Settings
IMPLEMENT_SINGLETON(Component::Settings)

Component::Settings::Settings()
    :   recentWorkspaces(
            "RecentWorkspaces",
            "Recently used workspaces",
            WorkspaceAddressesList())
{
    addSetting(&recentWorkspaces);
}

Component::Settings::~Settings()
{
}

//////////
//  Operations
void Component::Settings::recordRecentWorkspace(const WorkspaceAddress & workspaceAddress)
{
    if (workspaceAddress.isValid())
    {
        WorkspaceAddressesList mru = recentWorkspaces;
        mru.removeOne(workspaceAddress);
        mru.insert(0, workspaceAddress);
        while (mru.size() > 9)  //  TODO named constans
        {
            mru.removeLast();
        }
        recentWorkspaces = mru;
    }
}

//  End of tt3-ws/Component.cpp
