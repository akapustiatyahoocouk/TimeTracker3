//
//  tt3-db-xml/Preferences.cpp - tt3::db::xml::Preferences class implementation
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
#include "tt3-db-xml/API.hpp"
using namespace tt3::db::xml;

//////////
//  Singleton
TT3_IMPLEMENT_SINGLETON(Preferences)
Preferences::Preferences() {}
Preferences::~Preferences() {}

//////////
//  Preferences
Preferences::Mnemonic Preferences::mnemonic() const
{
    return parent()->mnemonic() + "/XmlFile";
}

QString Preferences::displayName() const
{
    tt3::util::ResourceReader rr(Component::Resources::instance(), RSID(Preferences));
    return rr.string(RID(DisplayName));
}

auto Preferences::parent(
    ) const -> tt3::gui::Preferences *
{
    return tt3::gui::StoragePreferences::instance();
}

auto Preferences::createEditor(
    ) -> tt3::gui::PreferencesEditor *
{
    return nullptr; //  TODO
}

//  End of tt3-db-xml/Preferences.cpp
