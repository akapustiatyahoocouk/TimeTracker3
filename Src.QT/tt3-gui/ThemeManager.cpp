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
        for (auto theme : StandardThemes::all())
        {
            Q_ASSERT(!registry.contains(theme->mnemonic()));
            registry[theme->mnemonic()] = theme;
        }

#define REGISTER(Theme)                     \
        registry.insert(                    \
            Theme::instance()->mnemonic(),  \
            Theme::instance())

        REGISTER(StandardThemes::System);
        REGISTER(StandardThemes::Light);
        REGISTER(StandardThemes::Dark);
    }

    using Registry = QMap<tt3::util::Mnemonic, ITheme*>;

    tt3::util::Mutex    guard;
    Registry            registry;
};

//////////
//  Operations
Themes ThemeManager::all()
{
    _Impl * impl = _impl();
    tt3::util::Lock _(impl->guard);

    auto values = impl->registry.values();
    return Themes(values.cbegin(), values.cend());
}

bool ThemeManager::register(ITheme * theme)
{
    Q_ASSERT(theme != nullptr);

    _Impl * impl = _impl();
    tt3::util::Lock _(impl->guard);

    if (impl->registry.contains(theme->mnemonic()))
    {
        return theme == impl->registry[theme->mnemonic()];
    }
    impl->registry[theme->mnemonic()] = theme;
    return true;
}

ITheme * ThemeManager::find(const tt3::util::Mnemonic & mnemonic)
{
    _Impl * impl = _impl();
    tt3::util::Lock _(impl->guard);

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
