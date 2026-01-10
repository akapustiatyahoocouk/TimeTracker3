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
//  Registration
TT3_IMPLEMENT_COMPONENT(Component)

//////////
//  IComponent
Component::Mnemonic Component::mnemonic() const
{
    return M(tt3-ws);
}

QString Component::displayName() const
{
    static Resources *const resources = Resources::instance();   //  idempotent
    return resources->string(RSID(Component), RID(DisplayName));
}

QString Component::description() const
{
    static Resources *const resources = Resources::instance();   //  idempotent
    return resources->string(RSID(Component), RID(Description));
}

QString Component::copyright() const
{
    static Resources *const resources = Resources::instance();   //  idempotent
    return resources->string(RSID(Component), RID(Copyright), QString(TT3_BUILD_DATE).left(4));
}

QVersionNumber Component::version() const
{
    return tt3::util::fromString<QVersionNumber>(TT3_VERSION);
}

QString Component::stage() const
{
    return TT3_STAGE;
}

QString Component::buildNumber() const
{
    return TT3_BUILD_DATE "-" TT3_BUILD_TIME;
}

auto Component::license() const -> tt3::util::ILicense *
{
    return tt3::util::StandardLicenses::Gpl3::instance();
}

Component::ISubsystem * Component::subsystem() const
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

void Component::initialize()
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
    qRegisterMetaType<ActivityType>();
    qRegisterMetaType<Activity>();
    qRegisterMetaType<PublicActivity>();
    qRegisterMetaType<PrivateActivity>();
    qRegisterMetaType<Task>();
    qRegisterMetaType<PublicTask>();
    qRegisterMetaType<PrivateTask>();
    qRegisterMetaType<Work>();
    qRegisterMetaType<Event>();
    qRegisterMetaType<Workload>();
    qRegisterMetaType<Project>();
    qRegisterMetaType<WorkStream>();
    qRegisterMetaType<Beneficiary>();
}

void Component::deinitialize()
{
}

//////////
//  Component::Settings
TT3_IMPLEMENT_SINGLETON(Component::Settings)

Component::Settings::Settings()
    :   recentWorkspaces(this, M(RecentWorkspaces), WorkspaceAddressesList())
{
}

Component::Settings::~Settings()
{
}

//////////
//  Component::Resources
TT3_IMPLEMENT_SINGLETON(Component::Resources)
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
