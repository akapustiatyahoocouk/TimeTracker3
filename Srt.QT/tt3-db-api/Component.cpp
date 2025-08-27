//
//  tt3-db-api/Component.cpp - tt3::db::api::Component class implementation
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
#include "tt3-db-api/API.hpp"
using namespace tt3::db::api;

//////////
//  Singleton
IMPLEMENT_SINGLETON(Component)

Component::Component()
{
    qRegisterMetaType<ChangeNotification>();
    qRegisterMetaType<DatabaseClosedNotification>();
}

Component::~Component()
{
}

//////////
//  IComponent
Component::Mnemonic Component::mnemonic() const
{
    return M(tt3-db-api);
}

QString Component::displayName() const
{
    return "TimeTracker3 database API";
}

QString Component::description() const
{
    return "Defines common API for TimeTracker3 databases";
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

Component::Subsystem * Component::subsystem() const
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
//  Component::Resources
IMPLEMENT_SINGLETON(Component::Resources)
Component::Resources::Resources()
    :   FileResourceFactory(":/tt3-db-api/Resources/tt3-db-api.txt") {}
Component::Resources::~Resources() {}

//////////
//  Component::Settings
IMPLEMENT_SINGLETON(Component::Settings)
Component::Settings::Settings() {}
Component::Settings::~Settings() {}

//  End of tt3-db-api/Component.cpp
