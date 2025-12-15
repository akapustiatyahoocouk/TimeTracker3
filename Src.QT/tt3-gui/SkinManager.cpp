//
//  tt3-gui/SkinManager.cpp - tt3::gui::SkinManager class implementation
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

struct SkinManager::_Impl
{
    using Registry = QMap<tt3::util::Mnemonic, ISkin*>;

    tt3::util::Mutex    guard;
    Registry            registry;
};

//////////
//  Operations
QSet<ISkin*> SkinManager::allSkins()
{
    _Impl * impl = _impl();
    tt3::util::Lock _(impl->guard);

    QList<ISkin*> values = impl->registry.values();
    return QSet<ISkin*>(values.cbegin(), values.cend());
}

bool SkinManager::registerSkin(ISkin * skin)
{
    Q_ASSERT(skin != nullptr);

    _Impl * impl = _impl();
    tt3::util::Lock _(impl->guard);

    if (impl->registry.contains(skin->mnemonic()))
    {
        return skin == impl->registry[skin->mnemonic()];
    }
    impl->registry[skin->mnemonic()] = skin;
    return true;
}

bool SkinManager::unregisterSkin(ISkin * skin)
{
    Q_ASSERT(skin != nullptr);

    _Impl * impl = _impl();
    tt3::util::Lock _(impl->guard);

    auto key = skin->mnemonic();
    if (impl->registry.contains(key) &&
        impl->registry[key] == skin)
    {   //  Guard against an impersonator
        impl->registry.remove(key);
        return true;
    }
    return false;
}

ISkin * SkinManager::findSkin(const tt3::util::Mnemonic & mnemonic)
{
    _Impl * impl = _impl();
    tt3::util::Lock _(impl->guard);

    return impl->registry.contains(mnemonic) ? impl->registry[mnemonic] : nullptr;
}

//////////
//  Implementation helpers
SkinManager::_Impl * SkinManager::_impl()
{
    static _Impl impl;
    return &impl;
}

//  End of tt3-gui/SkinManager.cpp
