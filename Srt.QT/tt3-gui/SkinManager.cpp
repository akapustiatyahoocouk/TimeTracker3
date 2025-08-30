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

tt3::util::Mutex SkinManager::_guard;
QMap<tt3::util::Mnemonic, ISkin*> SkinManager::_registry;

//////////
//  Operations
QSet<ISkin*> SkinManager::allSkins()
{
    tt3::util::Lock lock(_guard);

    QList<ISkin*> values = _registry.values();
    return QSet<ISkin*>(values.begin(), values.end());
}

bool SkinManager::registerSkin(ISkin * skin)
{
    tt3::util::Lock lock(_guard);

    Q_ASSERT(skin != nullptr);

    if (ISkin * registeredSkin = findSkin(skin->mnemonic()))
    {
        return skin == registeredSkin;
    }
    _registry[skin->mnemonic()] = skin;
    return true;
}

ISkin * SkinManager::findSkin(const tt3::util::Mnemonic & mnemonic)
{
    tt3::util::Lock lock(_guard);

    return _registry.contains(mnemonic) ? _registry[mnemonic] : nullptr;
}

//  End of tt3-gui/SkinManager.cpp
