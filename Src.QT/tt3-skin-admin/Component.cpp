//
//  tt3-skin-admin/Component.cpp - Component class implementation
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
#include "tt3-skin-admin/API.hpp"
using namespace tt3::skin::admin;

//////////
//  Singleton
IMPLEMENT_SINGLETON(Component)
Component::Component() {}
Component::~Component() {}

//////////
//  IComponent
Component::IPlugin * Component::plugin() const
{
    return Plugin::instance();
}

Component::Mnemonic Component::mnemonic() const
{
    return M(tt3-skin-admin);
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
    return tt3::util::StandardSubsystems::Gui::instance();
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
IMPLEMENT_SINGLETON(Component::Resources)
Component::Resources::Resources()
    :   FileResourceFactory(":/tt3-skin-admin/Resources/tt3-skin-admin.txt") {}
Component::Resources::~Resources() {}

//////////
//  Component::Settings
IMPLEMENT_SINGLETON(Component::Settings)

Component::Settings::Settings()
    :   mainFrameBounds(this, M(MainFrameBounds), QRect(32, 32, 480, 320)),
        mainFrameMaximized(this, M(MainFrameMaximized), false),
        mainFrameCurrentTab(this, M(MainFrameCurrentTab), 0)
{
}

Component::Settings::~Settings()
{
}

//  End of tt3-skin-admin/Component.cpp
