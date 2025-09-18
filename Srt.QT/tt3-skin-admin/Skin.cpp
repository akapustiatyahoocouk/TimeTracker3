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
IMPLEMENT_SINGLETON(Skin)
Skin::Skin() {}
Skin::~Skin() {}

//////////
//  gui::ISkin
Skin::Mnemonic Skin::mnemonic() const
{
    return M(Admin);
}

QString Skin::displayName() const
{
    return "Administrator";
}

QString Skin::description() const
{
    return "Provides full management facilities for TimeTracker3 workspaces";
}

QIcon Skin::smallIcon() const
{
    static const QIcon icon(":/tt3-skin-admin/Resources/Images/Objects/AdminSkinSmall.png");
    return icon;
}

QIcon Skin::largeIcon() const
{
    static const QIcon icon(":/tt3-skin-admin/Resources/Images/Objects/AdminSkinLarge.png");
    return icon;
}

bool Skin::isDefault() const
{
    return true;
}

//////////
//  gui::ISkin (state)
bool Skin::isActive() const
{
    return _mainFrame != nullptr;
}

void Skin::activate()
{
    if (_mainFrame == nullptr)
    {
        _mainFrame = new MainFrame();
        _mainFrame->show();
    }
    _mainFrame->activateWindow();
    _mainFrame->raise();
}

void Skin::deactivate()
{
    if (_mainFrame != nullptr)
    {
        _mainFrame->hide();
        delete _mainFrame;
        _mainFrame = nullptr;
    }
}

QMainWindow * Skin::mainWindow()
{
    return _mainFrame;
}

//  End of tt3-gui/Skin.cpp
