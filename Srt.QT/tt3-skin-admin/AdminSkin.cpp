//
//  tt3-skin-admin/AdminSkin.cpp - AdminSkin class implementation
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
IMPLEMENT_SINGLETON(AdminSkin)

AdminSkin::AdminSkin()
    :   _smallIcon(":/tt3-skin-admin/Resources/Images/Objects/AdminSkinSmall.png"),
        _largeIcon(":/tt3-skin-admin/Resources/Images/Objects/AdminSkinLarge.png")
{
}

AdminSkin::~AdminSkin()
{
}

//////////
//  gui::ISkin
QString AdminSkin::mnemonic() const
{
    return "Admin";
}

QString AdminSkin::displayName() const
{
    return "Administrator";
}

QString AdminSkin::desciption() const
{
    return "Provides full management facilities for TimeTracker3 workspaces";
}

QIcon AdminSkin::smallIcon() const
{
    return _smallIcon;
}

QIcon AdminSkin::largeIcon() const
{
    return _largeIcon;
}

bool AdminSkin::isDefault() const
{
    return true;
}

//////////
//  gui::ISkin (state)
bool AdminSkin::isActive() const
{
    return _mainFrame != nullptr;
}

void AdminSkin::activate()
{
    if (_mainFrame == nullptr)
    {
        _mainFrame = new MainFrame();
        _mainFrame->show();
    }
    _mainFrame->activateWindow();
    _mainFrame->raise();
}

void AdminSkin::deactivate()
{
    if (_mainFrame != nullptr)
    {
        _mainFrame->hide();
        delete _mainFrame;
        _mainFrame = nullptr;
    }
}

//  End of tt3-gui/AdminSkin.cpp
