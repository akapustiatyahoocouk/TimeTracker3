//
//  tt3-gui/StandardThemes.cpp - tt3::gui::StandardThemes class implementation
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
//  Operations
Themes StandardThemes::all()
{
    static const Themes result
    {
        System::instance(),
        Light::instance(),
        Dark::instance()
    };
    return result;
}

//////////
//  StandardThemes::System
TT3_IMPLEMENT_SINGLETON(StandardThemes::System)
StandardThemes::System::System() {}
StandardThemes::System::~System() {}

StandardThemes::System::Mnemonic StandardThemes::System::mnemonic() const
{
    return M(System);
}

QString StandardThemes::System::displayName() const
{
    return Component::Resources::instance()->string(
        RSID(StandardThemes), RID(System.DisplayName));
}

QString StandardThemes::System::description() const
{
    return Component::Resources::instance()->string(
        RSID(StandardThemes), RID(System.Description));
}

QIcon StandardThemes::System::smallIcon() const
{
    static const QIcon icon(":/tt3-gui/Resources/Images/Misc/SystemThemeLarge.png");
    return icon;
}

QIcon StandardThemes::System::largeIcon() const
{
    static const QIcon icon(":/tt3-gui/Resources/Images/Misc/SystemThemeSmall.png");
    return icon;
}

QString StandardThemes::System::css() const
{
    return _css;
}

//////////
//  StandardThemes::Light
TT3_IMPLEMENT_SINGLETON(StandardThemes::Light)

StandardThemes::Light::Light()
{
    QFile file(":/tt3-gui/Resources/Light.qss");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        _css = QTextStream(&file).readAll();
    }
    else
    {   //  OOPS!
        Q_ASSERT(false);
    }
}

StandardThemes::Light::~Light()
{
}

StandardThemes::Light::Mnemonic StandardThemes::Light::mnemonic() const
{
    return M(Light);
}

QString StandardThemes::Light::displayName() const
{
    return Component::Resources::instance()->string(
        RSID(StandardThemes), RID(Light.DisplayName));
}

QString StandardThemes::Light::description() const
{
    return Component::Resources::instance()->string(
        RSID(StandardThemes), RID(Light.Description));
}

QIcon StandardThemes::Light::smallIcon() const
{
    static const QIcon icon(":/tt3-gui/Resources/Images/Misc/LightThemeLarge.png");
    return icon;
}

QIcon StandardThemes::Light::largeIcon() const
{
    static const QIcon icon(":/tt3-gui/Resources/Images/Misc/LightThemeSmall.png");
    return icon;
}

QString StandardThemes::Light::css() const
{
    return _css;
}

//////////
//  StandardThemes::Dark
TT3_IMPLEMENT_SINGLETON(StandardThemes::Dark)

StandardThemes::Dark::Dark()
{
    QFile file(":/tt3-gui/Resources/Dark.qss");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        _css = QTextStream(&file).readAll();
    }
    else
    {   //  OOPS!
        Q_ASSERT(false);
    }
}

StandardThemes::Dark::~Dark()
{
}

StandardThemes::Dark::Mnemonic StandardThemes::Dark::mnemonic() const
{
    return M(Dark);
}

QString StandardThemes::Dark::displayName() const
{
    return Component::Resources::instance()->string(
        RSID(StandardThemes), RID(Dark.DisplayName));
}

QString StandardThemes::Dark::description() const
{
    return Component::Resources::instance()->string(
        RSID(StandardThemes), RID(Dark.Description));
}

QIcon StandardThemes::Dark::smallIcon() const
{
    static const QIcon icon(":/tt3-gui/Resources/Images/Misc/DarkThemeLarge.png");
    return icon;
}

QIcon StandardThemes::Dark::largeIcon() const
{
    static const QIcon icon(":/tt3-gui/Resources/Images/Misc/DarkThemeSmall.png");
    return icon;
}

QString StandardThemes::Dark::css() const
{
    return _css;
}

//  End of tt3-gui/StandardThemes.cpp
