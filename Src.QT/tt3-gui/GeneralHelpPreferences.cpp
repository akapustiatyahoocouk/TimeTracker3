//
//  tt3-gui/GeneralHelpPreferences.cpp - tt3::gui::GeneralHelpPreferences class implementation
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
TT3_IMPLEMENT_SINGLETON(GeneralHelpPreferences)
GeneralHelpPreferences::GeneralHelpPreferences() {}
GeneralHelpPreferences::~GeneralHelpPreferences() {}

//////////
//  Preferences
GeneralHelpPreferences::Mnemonic GeneralHelpPreferences::mnemonic() const
{
    return parent()->mnemonic() + "/Help";
}

QString GeneralHelpPreferences::displayName() const
{
    tt3::util::ResourceReader rr(Component::Resources::instance(), RSID(GeneralHelpPreferences));
    return rr.string(RID(DisplayName));
}

Preferences * GeneralHelpPreferences::parent() const
{
    return GeneralPreferences::instance();
}

PreferencesEditor * GeneralHelpPreferences::createEditor()
{
    return new GeneralHelpPreferencesEditor(nullptr);
}

//  End of tt3-gui/GeneralHelpPreferences.cpp
