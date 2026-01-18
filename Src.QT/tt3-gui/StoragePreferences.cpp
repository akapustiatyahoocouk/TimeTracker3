//
//  tt3-gui/StoragePreferences.cpp - tt3::gui::StoragePreferences class implementation
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
TT3_IMPLEMENT_SINGLETON(StoragePreferences)
StoragePreferences::StoragePreferences() {}
StoragePreferences::~StoragePreferences() {}

//////////
//  Preferences
StoragePreferences::Mnemonic StoragePreferences::mnemonic() const
{
    return M(/Storage);
}

QString StoragePreferences::displayName() const
{
    tt3::util::ResourceReader rr(Component::Resources::instance(), RSID(StoragePreferences));
    return rr.string(RID(DisplayName));
}

Preferences * StoragePreferences::parent() const
{
    return nullptr;
}

PreferencesEditor * StoragePreferences::createEditor()
{
    return nullptr;
}

//  End of tt3-gui/StoragePreferences.cpp
