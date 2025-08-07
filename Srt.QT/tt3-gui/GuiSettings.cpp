//
//  tt3-gui/GuiSettings.cpp - tt3::gui::GuiSettings class implementation
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

//////////
//  Singleton
IMPLEMENT_SINGLETON(GuiSettings)

GuiSettings::GuiSettings()
    :   activeSkin(
          "ActiveSkin",
          "The identifier of the currently active skin",
          "")
{
    addSetting(&activeSkin);
}
GuiSettings::~GuiSettings()
{
}

//  End of tt3-gui/GuiSettings.cpp
