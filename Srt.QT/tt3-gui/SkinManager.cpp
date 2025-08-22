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
QMap<QString, ISkin*> SkinManager::_registry;

//////////
//  Operationds
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

    ISkin * registeredSkin = findSkin(skin->mnemonic());
    if (registeredSkin != nullptr)
    {
        return skin == registeredSkin;
    }
    else
    {
        QString key = skin->mnemonic();
        _registry[key] = skin;
        return true;
    }
}

ISkin * SkinManager::findSkin(const QString & mnemonic)
{
    tt3::util::Lock lock(_guard);

    QString key = mnemonic;
    return _registry.contains(key) ? _registry[key] : nullptr;
}

//  End of tt3-gui/SkinManager.cpp
