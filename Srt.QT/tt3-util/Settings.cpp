//
//  tt3-util/Settings.cpp - tt3::util::Settings class implementation
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
#include "tt3-util/API.hpp"
using namespace tt3::util;

//////////
//  Operations
bool Settings::addSetting(AbstractSetting * setting)
{
    Q_ASSERT(setting != nullptr);

    if (!_settings.contains(setting->mnemonic()))
    {
        _settings[setting->mnemonic()] = setting;
        return true;
    }
    return false;
}

QSet<AbstractSetting*> Settings::settings() const
{
    QList<AbstractSetting*> values = _settings.values();
    return QSet<AbstractSetting*>(values.begin(), values.end());
}

AbstractSetting * Settings::findSetting(const Mnemonic & mnemonic) const
{
    return _settings.contains(mnemonic) ? _settings[mnemonic] : nullptr;
}

//  End of tt3-util/Settings.cpp
