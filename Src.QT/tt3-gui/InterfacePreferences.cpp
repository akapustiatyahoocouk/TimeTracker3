//
//  tt3-gui/InterfacePreferences.cpp - tt3::gui::InterfacePreferences class implementation
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
TT3_IMPLEMENT_SINGLETON(InterfacePreferences)
InterfacePreferences::InterfacePreferences() {}
InterfacePreferences::~InterfacePreferences() {}

//////////
//  Preferences
InterfacePreferences::Mnemonic InterfacePreferences::mnemonic() const
{
    return M(/Interface);
}

QString InterfacePreferences::displayName() const
{
    tt3::util::ResourceReader rr(Component::Resources::instance(), RSID(InterfacePreferences));
    return rr.string(RID(DisplayName));
}

Preferences * InterfacePreferences::parent() const
{
    return nullptr;
}

PreferencesEditor * InterfacePreferences::createEditor()
{
    return nullptr;
}

//  End of tt3-gui/InterfacePreferences.cpp
