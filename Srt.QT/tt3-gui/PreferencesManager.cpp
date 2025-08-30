//
//  tt3-gui/PreferencesManager.cpp - tt3::gui::PreferencesManager class implementation
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

tt3::util::Mutex PreferencesManager::_guard;
QMap<tt3::util::Mnemonic, Preferences*> PreferencesManager::_registry;

//////////
//  Operations
QSet<Preferences*> PreferencesManager::allPreferences()
{
    tt3::util::Lock lock(_guard);

    QList<Preferences*> values = _registry.values();
    return QSet<Preferences*>(values.begin(), values.end());
}

QSet<Preferences*> PreferencesManager::rootPreferences()
{
    tt3::util::Lock lock(_guard);

    QList<Preferences*> values = _registry.values();
    QSet<Preferences*> result;
    for (Preferences * preferences : values)
    {
        if (preferences->parent() == nullptr)
        {
            result.insert(preferences);
        }
    }
    return result;
}

bool PreferencesManager::registerPreferences(Preferences * preferences)
{
    Q_ASSERT(preferences != nullptr);

    tt3::util::Lock lock(_guard);

    if (preferences->parent() != nullptr &&
        !registerPreferences(preferences->parent()))
    {   //  OOPS! No parent - no children
        return false;
    }

    if (Preferences * registeredPreferences = findPreferences(preferences->mnemonic()))
    {
        return preferences == registeredPreferences;
    }
    _registry[preferences->mnemonic()] = preferences;
    return true;
}

Preferences * PreferencesManager::findPreferences(const tt3::util::Mnemonic & mnemonic)
{
    tt3::util::Lock lock(_guard);

    return _registry.contains(mnemonic) ? _registry[mnemonic] : nullptr;
}

//  End of tt3-gui/PreferencesManager.cpp
