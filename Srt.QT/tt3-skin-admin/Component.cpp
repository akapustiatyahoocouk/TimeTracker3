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
QString Component::mnemonic() const
{
    return "tt3-skin-admin";
}

QString Component::displayName() const
{
    return "TimeTracker3 Administrator skin";
}

QString Component::desciption() const
{
    return "Defines GUI Administrator skin for TimeTracker3";
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
    :   mainFrameBounds(
            "MainFrameBounds",
            QRect(32, 32, 480, 320)),
        mainFrameMaximized(
            "MainFrameMaximized",
            false)
{
    addSetting(&mainFrameBounds);
    addSetting(&mainFrameMaximized);
}

Component::Settings::~Settings()
{
}

//  End of tt3-skin-admin/AdminSkinComponent.cpp
