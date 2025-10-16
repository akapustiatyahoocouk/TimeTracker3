//
//  tt3/Component.cpp - tt3::Component class implementation
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
#include "tt3/API.hpp"
using namespace tt3;

//////////
//  Singleton
IMPLEMENT_SINGLETON(Component)
Component::Component() {}
Component::~Component() {}

//////////
//  IComponent
auto Component::mnemonic(
    ) const -> tt3::util::Mnemonic
{
    return M(tt3);
}

QString Component::displayName() const
{
    static Resources * resources = Resources::instance();   //  idempotent
    return resources->string(RSID(Component), RID(DisplayName));
}

QString Component::description() const
{
    static Resources * resources = Resources::instance();   //  idempotent
    return resources->string(RSID(Component), RID(Description));
}

QString Component::copyright() const
{
    return tt3::util::ProductInformation::applicationCopyright();
}

auto Component::version(
    ) const -> QVersionNumber
{
    return tt3::util::ProductInformation::applicationVersion();
}

QString Component::buildNumber() const
{
    return tt3::util::ProductInformation::applicationBuildNumber();
}

auto Component::subsystem(
    ) const -> tt3::util::ISubsystem *
{
    return tt3::util::StandardSubsystems::Applications::instance();
}

auto Component::resources(
    ) const -> Component::Resources *
{
    return Resources::instance();
}

auto Component::settings(
    ) -> Component::Settings *
{
    return Settings::instance();
}

auto Component::settings(
    ) const -> const Component::Settings *
{
    return Settings::instance();
}

//////////
//  Component::Resources
IMPLEMENT_SINGLETON(Component::Resources)
Component::Resources::Resources()
    :   FileResourceFactory(":/tt3/Resources/tt3.txt") {}
Component::Resources::~Resources() {}

//////////
//  Component::Settings
IMPLEMENT_SINGLETON(Component::Settings)
Component::Settings::Settings() {}
Component::Settings::~Settings() {}

//  End of tt3/Component.cpp
