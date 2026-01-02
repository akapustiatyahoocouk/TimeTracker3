//
//  tt3-gui/StyleManager.cpp - tt3::gui::StyleManager class implementation
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

struct StyleManager::_Impl
{
    _Impl()
    {
        for (const auto & key : QStyleFactory::keys())
        {
            auto style = new BuiltinStyle(key);
            Q_ASSERT(!registry.contains(style->mnemonic()));
            registry[style->mnemonic()] = style;
        }
    }

    using Registry = QMap<tt3::util::Mnemonic, IStyle*>;

    tt3::util::Mutex    guard;
    Registry            registry;
};

//////////
//  Operations
Styles StyleManager::all()
{
    _Impl * impl = _impl();
    tt3::util::Lock _(impl->guard);

    auto values = impl->registry.values();
    return Styles(values.cbegin(), values.cend());
}

bool StyleManager::register(IStyle * style)
{
    Q_ASSERT(style != nullptr);

    _Impl * impl = _impl();
    tt3::util::Lock _(impl->guard);

    if (impl->registry.contains(style->mnemonic()))
    {
        return style == impl->registry[style->mnemonic()];
    }
    impl->registry[style->mnemonic()] = style;
    return true;
}

IStyle * StyleManager::find(const tt3::util::Mnemonic & mnemonic)
{
    _Impl * impl = _impl();
    tt3::util::Lock _(impl->guard);

    return impl->registry.contains(mnemonic) ? impl->registry[mnemonic] : nullptr;
}

//////////
//  Implementation helpers
StyleManager::_Impl * StyleManager::_impl()
{
    static _Impl impl;
    return &impl;
}

//  End of tt3-gui/StyleManager.cpp
