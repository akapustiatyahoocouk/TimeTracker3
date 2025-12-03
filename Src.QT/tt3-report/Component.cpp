//
//  tt3-report/Component.cpp - Component class implementation
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
#include "tt3-report/API.hpp"
using namespace tt3::report;

//////////
//  Registration
TT3_IMPLEMENT_COMPONENT(Component)

//////////
//  IComponent
Component::Mnemonic Component::mnemonic() const
{
    return M(tt3-report);
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

QString Component::buildNumber() const
{
    return TT3_BUILD_DATE "-" TT3_BUILD_TIME;
}

Component::ISubsystem * Component::subsystem() const
{
    return tt3::util::StandardSubsystems::Reporting::instance();
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

void Component::iniialize()
{
}

void Component::deiniialize()
{
}

//////////
//  Component::Resources
TT3_IMPLEMENT_SINGLETON(Component::Resources)
Component::Resources::Resources()
    :   FileResourceFactory(":/tt3-report/Resources/tt3-report.txt") {}
Component::Resources::~Resources() {}

//////////
//  Component::Settings
TT3_IMPLEMENT_SINGLETON(Component::Settings)
Component::Settings::Settings() {}
Component::Settings::~Settings() {}

//  End of tt3-report/Component.cpp
