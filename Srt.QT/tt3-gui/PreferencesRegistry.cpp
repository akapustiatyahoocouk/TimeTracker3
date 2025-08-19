//
//  tt3-gui/PreferencesRegistry.cpp - tt3::gui::PreferencesRegistry class implementation
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

QMap<QString, Preferences*> PreferencesRegistry::_registry;

//////////
//  Operationds
QSet<Preferences*> PreferencesRegistry::allPreferences()
{   //  TODO synchronize ?
    QList<Preferences*> values = _registry.values();
    return QSet<Preferences*>(values.begin(), values.end());
}

QSet<Preferences*> PreferencesRegistry::rootPreferences()
{   //  TODO synchronize ?
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

bool PreferencesRegistry::registerPreferences(Preferences * preferences)
{   //  TODO synchronize ?
    Q_ASSERT(preferences != nullptr);

    if (preferences->parent() != nullptr &&
        !registerPreferences(preferences->parent()))
    {   //  OOPS! No parent - no children
        return false;
    }

    Preferences * registeredPreferences = findPreferences(preferences->mnemonic());
    if (registeredPreferences != nullptr)
    {
        return preferences == registeredPreferences;
    }
    else
    {
        QString key = preferences->mnemonic();
        _registry[key] = preferences;
        return true;
    }
}

Preferences * PreferencesRegistry::findPreferences(const QString & mnemonic)
{   //  TODO synchronize ?
    QString key = mnemonic;
    return _registry.contains(key) ? _registry[key] : nullptr;
}

//  End of tt3-gui/PreferencesRegistry.cpp
