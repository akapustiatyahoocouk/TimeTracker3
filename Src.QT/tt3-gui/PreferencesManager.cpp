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

struct PreferencesManager::_Impl
{
    _Impl()
    {
#define REGISTER(Preferences)                   \
        registry.insert(                        \
            Preferences::instance()->mnemonic(),\
            Preferences::instance())

        REGISTER(GeneralPreferences);
        REGISTER(GeneralAppearancePreferences);
        REGISTER(GeneralStartupPreferences);
        REGISTER(GeneralDialogsPreferences);
    }

    tt3::util::Mutex                        guard;
    QMap<tt3::util::Mnemonic, Preferences*> registry;
};

//////////
//  Operations
QSet<Preferences*> PreferencesManager::allPreferences()
{
    _Impl * impl = _impl();
    tt3::util::Lock _(impl->guard);

    QList<Preferences*> values = impl->registry.values();
    return QSet<Preferences*>(values.cbegin(), values.cend());
}

QSet<Preferences*> PreferencesManager::rootPreferences()
{
    _Impl * impl = _impl();
    tt3::util::Lock _(impl->guard);

    QList<Preferences*> values = impl->registry.values();
    QSet<Preferences*> result;
    for (Preferences * preferences : qAsConst(values))
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

    _Impl * impl = _impl();
    tt3::util::Lock _(impl->guard);

    if (preferences->parent() != nullptr &&
        !registerPreferences(preferences->parent()))
    {   //  OOPS! No parent - no children
        return false;
    }

    if (impl->registry.contains(preferences->mnemonic()))
    {
        return preferences == impl->registry[preferences->mnemonic()];
    }
    impl->registry[preferences->mnemonic()] = preferences;
    return true;
}

Preferences * PreferencesManager::findPreferences(const tt3::util::Mnemonic & mnemonic)
{
    _Impl * impl = _impl();
    tt3::util::Lock _(impl->guard);

    return impl->registry.contains(mnemonic) ? impl->registry[mnemonic] : nullptr;
}

//////////
//  Implementation helpers
PreferencesManager::_Impl * PreferencesManager::_impl()
{
    static _Impl impl;
    return &impl;
}

//  End of tt3-gui/PreferencesManager.cpp
