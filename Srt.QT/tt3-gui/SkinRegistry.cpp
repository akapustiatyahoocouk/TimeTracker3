//
//  tt3-gui/SkinRegistry.cpp - tt3::gui::SkinRegistry class implementation
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

QMap<QString, ISkin*> SkinRegistry::_registry;

//////////
//  Operationds
QSet<ISkin*> SkinRegistry::allSkins()
{
    QList<ISkin*> values = _registry.values();
    return QSet<ISkin*>(values.begin(), values.end());
}

bool SkinRegistry::registerSkin(ISkin * skin)
{   //  TODO synchronize ?
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

ISkin * SkinRegistry::findSkin(const QString & mnemonic)
{   //  TODO synchronize ?
    QString key = mnemonic;
    return _registry.contains(key) ? _registry[key] : nullptr;
}

//  End of tt3-gui/SkinRegistry.cpp
