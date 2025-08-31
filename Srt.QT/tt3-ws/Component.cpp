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
    qRegisterMetaType<ObjectCreatedNotification>();
    qRegisterMetaType<ObjectDestroyedNotification>();
    qRegisterMetaType<ObjectModifiedNotification>();

    //  Enable objects and object pointers for QVariant
    qRegisterMetaType<ObjectImpl>();
    qRegisterMetaType<PrincipalImpl>();
    qRegisterMetaType<UserImpl>();
    qRegisterMetaType<AccountImpl>();

    qRegisterMetaType<Object>();
    qRegisterMetaType<Principal>();
    qRegisterMetaType<User>();
    qRegisterMetaType<Account>();
}

Component::~Component()
{
}

//////////
//  IComponent
Component::Mnemonic Component::mnemonic() const
{
    return M(tt3-ws);
}

QString Component::displayName() const
{
    return "TimeTracker3 workspace layer";
}

QString Component::description() const
{
    return "Defines workspace data access layer for TimeTracker3";
}

QString Component::copyright() const
{
    return tt3::util::ProductInformation::applicationCopyright();
}

QVersionNumber Component::version() const
{
    return tt3::util::ProductInformation::applicationVersion();
}

QString Component::buildNumber() const
{
    return tt3::util::ProductInformation::applicationBuildNumber();
}

tt3::util::ISubsystem * Component::subsystem() const
{
    return tt3::util::StandardSubsystems::Storage::instance();
}

Component::Resources * Component::resources() const
{
    return Resources::instance();
}

Component::Settings * Component::settings()
{
    return Settings::instance();
}

const Component::Settings * Component::settings() const
{
    return Settings::instance();
}

//////////
//  Component::Settings
IMPLEMENT_SINGLETON(Component::Settings)

Component::Settings::Settings()
    :   recentWorkspaces(
            M(RecentWorkspaces),
            WorkspaceAddressesList())
{
    addSetting(&recentWorkspaces);
}

Component::Settings::~Settings()
{
}

//////////
//  Component::Resources
IMPLEMENT_SINGLETON(Component::Resources)
Component::Resources::Resources()
    :   FileResourceFactory(":/tt3-ws/Resources/tt3-ws.txt") {}
Component::Resources::~Resources() {}

//////////
//  Operations
void Component::Settings::addRecentWorkspace(const WorkspaceAddress workspaceAddress)
{
    if (workspaceAddress != nullptr)
    {
        WorkspaceAddressesList mru = recentWorkspaces;
        mru.removeOne(workspaceAddress);
        mru.insert(0, workspaceAddress);
        while (mru.size() > MaxRecentWorkspaces)
        {
            mru.removeLast();
        }
        recentWorkspaces = mru;
    }
}

void Component::Settings::removeRecentWorkspace(const WorkspaceAddress workspaceAddress)
{
    if (workspaceAddress != nullptr)
    {
        WorkspaceAddressesList mru = recentWorkspaces;
        if (mru.removeOne(workspaceAddress))
        {
            recentWorkspaces = mru;
        }
    }
}

//  End of tt3-ws/Component.cpp
