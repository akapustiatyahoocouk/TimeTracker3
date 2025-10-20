//
//  tt3-gui/GeneralStartupPreferences.cpp - tt3::gui::GeneralStartupPreferences class implementation
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
IMPLEMENT_SINGLETON(GeneralStartupPreferences)
GeneralStartupPreferences::GeneralStartupPreferences() {}
GeneralStartupPreferences::~GeneralStartupPreferences() {}

//////////
//  Preferences
GeneralStartupPreferences::Mnemonic GeneralStartupPreferences::mnemonic() const
{
    return parent()->mnemonic() + "/Startup";
}

QString GeneralStartupPreferences::displayName() const
{
    tt3::util::ResourceReader rr(Component::Resources::instance(), RSID(GeneralStartupPreferences));
    return rr.string(RID(DisplayName));
}

Preferences * GeneralStartupPreferences::parent() const
{
    return GeneralPreferences::instance();
}

PreferencesEditor * GeneralStartupPreferences::createEditor()
{
    return new GeneralStartupPreferencesEditor(nullptr);
}

//  End of tt3-gui/GeneralStartupPreferences.cpp
