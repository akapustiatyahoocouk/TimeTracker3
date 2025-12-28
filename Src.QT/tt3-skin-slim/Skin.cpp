//
//  tt3-skin-slim/Skin.cpp - tt3::skin::slim::Skin class implementation
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
#include "tt3-skin-slim/API.hpp"
using namespace tt3::skin::slim;

//////////
//  Singleton
TT3_IMPLEMENT_SINGLETON(Skin)
Skin::Skin() {}
Skin::~Skin() {}

//////////
//  gui::ISkin
Skin::Mnemonic Skin::mnemonic() const
{
    return M(Slim);
}

QString Skin::displayName() const
{
    static Component::Resources *const resources = Component::Resources::instance();   //  idempotent
    return resources->string(RSID(Skin), RID(DisplayName));
}

QString Skin::description() const
{
    static Component::Resources *const resources = Component::Resources::instance();   //  idempotent
    return resources->string(RSID(Skin), RID(Description));
}

QIcon Skin::smallIcon() const
{
    static const QIcon icon(":/tt3-skin-slim/Resources/Images/Objects/SlimSkinSmall.png");
    return icon;
}

QIcon Skin::largeIcon() const
{
    static const QIcon icon(":/tt3-skin-slim/Resources/Images/Objects/SlimSkinLarge.png");
    return icon;
}

bool Skin::isDefault() const
{
    return false;
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
    if (!_mainFrame->isVisible())
    {   //  If hidden, restore from system tray
        _mainFrame->_onActionRestore();
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
    return nullptr; //  It's a "side window", really
}

//  End of tt3-skin-slim/Skin.cpp
