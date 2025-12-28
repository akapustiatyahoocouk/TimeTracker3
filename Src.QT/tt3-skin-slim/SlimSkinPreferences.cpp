//
//  tt3-skin-slim/SlimSkinPreferences.cpp - tt3::skin::slim::SlimSkinPreferences class implementation
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
TT3_IMPLEMENT_SINGLETON(SlimSkinPreferences)
SlimSkinPreferences::SlimSkinPreferences() {}
SlimSkinPreferences::~SlimSkinPreferences() {}

//////////
//  Preferences
SlimSkinPreferences::Mnemonic SlimSkinPreferences::mnemonic() const
{
    return parent()->mnemonic() + "/SlimSkin";
}

QString SlimSkinPreferences::displayName() const
{
    tt3::util::ResourceReader rr(Component::Resources::instance(), RSID(SlimSkinPreferences));
    return rr.string(RID(DisplayName));
}

auto SlimSkinPreferences::parent(
    ) const -> tt3::gui::Preferences *
{
    return tt3::gui::InterfacePreferences::instance();
}

auto SlimSkinPreferences::createEditor(
    ) -> tt3::gui::PreferencesEditor *
{
    return new SlimSkinPreferencesEditor(nullptr);
}

//  End of tt3-skin-slim/SlimSkinPreferences.cpp
