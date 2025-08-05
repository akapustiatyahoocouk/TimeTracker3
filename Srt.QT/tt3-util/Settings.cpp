//
//  tt3-util/Settings.cpp Settings class implementation
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
using namespace util;

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

AbstractSetting * Settings::findSetting(const QString & mnemonic) const
{
    if (_settings.contains(mnemonic))
    {
        return _settings[mnemonic];
    }
    return nullptr;
}

//  End of tt3-util/Settings.cpp
