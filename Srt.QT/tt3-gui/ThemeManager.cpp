//
//  tt3-gui/ThemeManager.cpp - tt3::gui::ThemeManager class implementation
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

tt3::util::Mutex ThemeManager::_guard;
QMap<QString, ITheme*> ThemeManager::_registry;

//////////
//  Operations
QSet<ITheme*> ThemeManager::allThemes()
{
    tt3::util::Lock lock(_guard);

    _registerStandardThemes();
    QList<ITheme*> values = _registry.values();
    return QSet<ITheme*>(values.begin(), values.end());
}

bool ThemeManager::registerTheme(ITheme * theme)
{
    tt3::util::Lock lock(_guard);

    Q_ASSERT(theme != nullptr);

    _registerStandardThemes();
    ITheme * registeredTheme = findTheme(theme->mnemonic());
    if (registeredTheme != nullptr)
    {
        return theme == registeredTheme;
    }
    else
    {
        QString key = theme->mnemonic();
        _registry[key] = theme;
        return true;
    }
}

ITheme * ThemeManager::findTheme(const QString & mnemonic)
{
    tt3::util::Lock lock(_guard);

    _registerStandardThemes();
    QString key = mnemonic;
    return _registry.contains(key) ? _registry[key] : nullptr;
}

//////////
//  Implementation helpers
void ThemeManager::_registerStandardThemes()
{
    Q_ASSERT(_guard.isLockedByCurrentThread());

    if (_registry.isEmpty())
    {
#define REGISTER(Subsystem)                                     \
        _registry.insert(                                       \
            StandardThemes::Subsystem::instance()->mnemonic(),  \
            StandardThemes::Subsystem::instance())
        REGISTER(System);
        REGISTER(Light);
        REGISTER(Dark);
    }
}

//  End of tt3-gui/ThemeManager.cpp
