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

struct ThemeManager::_Impl
{
    _Impl()
    {
#define REGISTER(Subsystem)                     \
        registry.insert(                        \
            Subsystem::instance()->mnemonic(),  \
            Subsystem::instance())

        REGISTER(StandardThemes::System);
        REGISTER(StandardThemes::Light);
        REGISTER(StandardThemes::Dark);
    }

    tt3::util::Mutex                    guard;
    QMap<tt3::util::Mnemonic, ITheme*>  registry;
};

//////////
//  Operations
QSet<ITheme*> ThemeManager::allThemes()
{
    _Impl * impl = _impl();
    tt3::util::Lock lock(impl->guard);

    QList<ITheme*> values = impl->registry.values();
    return QSet<ITheme*>(values.cbegin(), values.cend());
}

bool ThemeManager::registerTheme(ITheme * theme)
{
    Q_ASSERT(theme != nullptr);

    _Impl * impl = _impl();
    tt3::util::Lock lock(impl->guard);

    if (impl->registry.contains(theme->mnemonic()))
    {
        return theme == impl->registry[theme->mnemonic()];
    }
    impl->registry[theme->mnemonic()] = theme;
    return true;
}

ITheme * ThemeManager::findTheme(const tt3::util::Mnemonic & mnemonic)
{
    _Impl * impl = _impl();
    tt3::util::Lock lock(impl->guard);

    return impl->registry.contains(mnemonic) ? impl->registry[mnemonic] : nullptr;
}

//////////
//  Implementation helpers
ThemeManager::_Impl * ThemeManager::_impl()
{
    static _Impl impl;
    return &impl;
}

//  End of tt3-gui/ThemeManager.cpp
