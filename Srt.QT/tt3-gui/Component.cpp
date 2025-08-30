//
//  tt3-gui/Component.cpp - tt3::gui::Component class implementation
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
#include "tt3-gui/API.hpp"
using namespace tt3::gui;

//////////
//  Singleton
IMPLEMENT_SINGLETON(Component)
Component::Component() {}
Component::~Component() {}

//////////
//  IComponent
Component::Mnemonic Component::mnemonic() const
{
    return M(tt3-gui);
}

QString Component::displayName() const
{
    return "TimeTracker3 GUI";
}

QString Component::description() const
{
    return "Defines GUI facilities for TimeTracker3";
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

tt3::util::ISubsystem * Component::subsystem() const
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

//////////
//  Component::Resources
IMPLEMENT_SINGLETON(Component::Resources)
Component::Resources::Resources()
    :   FileResourceFactory(":/tt3-gui/Resources/tt3-gui.txt") {}
Component::Resources::~Resources() {}

//////////
//  Component::Settings
IMPLEMENT_SINGLETON(Component::Settings)

Component::Settings::Settings()
    :   activeSkin(
            M(ActiveSkin),
            M(-),
            true),
        activeTheme(
            M(ActiveTheme),
            StandardThemes::System::instance()->mnemonic(),
            true),
        lastLogin(
            M(LastLogin),
            ""),
        uiLocale(
            M(UiLocale),
            QLocale(QLocale::English, QLocale::UnitedKingdom),
            true),
        loadLastWorkspaceAtStartup(
            M(LoadLastWorkspaceAtStartup),
            false),
        rememberLastLogin(
            M(RememberLastLogin),
            true),
        currentPreferences(
            M(CurrentPreferences),
            M(-))
{
    addSetting(&activeSkin);
    addSetting(&activeTheme);
    addSetting(&lastLogin);
    addSetting(&uiLocale);
    addSetting(&loadLastWorkspaceAtStartup);
    addSetting(&rememberLastLogin);
    addSetting(&currentPreferences);
}

Component::Settings::~Settings()
{
}

//  End of tt3-gui/GuiComponent.cpp
