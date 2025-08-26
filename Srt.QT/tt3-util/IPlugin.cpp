//
//  tt3-util/IPlugin.cpp - tt3::util::IPlugin class implementation
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
QIcon IPlugin::smallIcon() const
{
    static QIcon icon(":/tt3-util/Resources/Images/Objects/PluginSmall.png");
    return icon;
}

QIcon IPlugin::largeIcon() const
{
    static QIcon icon(":/tt3-util/Resources/Images/Objects/PluginLarge.png");
    return icon;
}

bool IPlugin::isInitialized() const
{
    return PluginManager::_initializedPlugins.contains(const_cast<IPlugin*>(this));
}

//  End of tt3-util/IPlugin.cpp
