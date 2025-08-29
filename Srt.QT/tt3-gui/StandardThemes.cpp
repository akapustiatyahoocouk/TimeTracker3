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
//  StandardThemes::System
IMPLEMENT_SINGLETON(StandardThemes::System)

StandardThemes::System::System()
    :   _palette(QGuiApplication::palette())
{
}

StandardThemes::System::~System()
{
}

StandardThemes::System::Mnemonic StandardThemes::System::mnemonic() const
{
    return M(System);
}

QString StandardThemes::System::displayName() const
{
    return "System";
}

QString StandardThemes::System::description() const
{
    return "The theme that uses current system colors";
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

QPalette StandardThemes::System::palette() const
{
    return _palette;
}

//////////
//  StandardThemes::Light
IMPLEMENT_SINGLETON(StandardThemes::Light)

StandardThemes::Light::Light()
{
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
    return "Light";
}

QString StandardThemes::Light::description() const
{
    return "The theme that uses light colors";
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

QPalette StandardThemes::Light::palette() const
{
    return _palette;
}

//////////
//  StandardThemes::Dark
IMPLEMENT_SINGLETON(StandardThemes::Dark)

StandardThemes::Dark::Dark()
{
    const QColor darkGray(53, 53, 53);
    const QColor gray(128, 128, 128);
    const QColor black(25, 25, 25);
    const QColor blue(42, 130, 218);

    _palette.setColor(QPalette::Window, darkGray);
    _palette.setColor(QPalette::WindowText, Qt::white);
    _palette.setColor(QPalette::Base, black);
    _palette.setColor(QPalette::AlternateBase, darkGray);
    _palette.setColor(QPalette::ToolTipBase, blue);
    _palette.setColor(QPalette::ToolTipText, Qt::white);
    _palette.setColor(QPalette::Text, Qt::white);
    _palette.setColor(QPalette::Button, darkGray);
    _palette.setColor(QPalette::ButtonText, Qt::white);
    _palette.setColor(QPalette::Link, blue);
    _palette.setColor(QPalette::Highlight, blue);
    _palette.setColor(QPalette::HighlightedText, Qt::black);

    _palette.setColor(QPalette::Active, QPalette::Button, gray.darker());
    _palette.setColor(QPalette::Disabled, QPalette::ButtonText, gray);
    _palette.setColor(QPalette::Disabled, QPalette::WindowText, gray);
    _palette.setColor(QPalette::Disabled, QPalette::Text, gray);
    _palette.setColor(QPalette::Disabled, QPalette::Light, darkGray);}

StandardThemes::Dark::~Dark()
{
}

StandardThemes::Dark::Mnemonic StandardThemes::Dark::mnemonic() const
{
    return M(Dark);
}

QString StandardThemes::Dark::displayName() const
{
    return "Dark";
}

QString StandardThemes::Dark::description() const
{
    return "The theme that uses dark colors";
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

QPalette StandardThemes::Dark::palette() const
{
    return _palette;
}

//  End of tt3-gui/StandardThemes.cpp
