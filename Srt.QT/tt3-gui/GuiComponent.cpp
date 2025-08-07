//
//  tt3-gui/GuiComponent.cpp - tt3::gui::GuiComponent class implementation
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
IMPLEMENT_SINGLETON(GuiComponent)
GuiComponent::GuiComponent() {}
GuiComponent::~GuiComponent() {}

//////////
//  IComponent
QString GuiComponent::mnemonic() const
{
    return "tt3-gui";
}

QString GuiComponent::displayName() const
{
    return "TimeTracker3 GUI";
}

QString GuiComponent::desciption() const
{
    return "Defines GUI facilities for TimeTracker3";
}

QString GuiComponent::copyright() const
{
    return "Copyright (C) 2026, Andrey Kapustin";
}

QVersionNumber GuiComponent::version() const
{
    return QVersionNumber(1, 0, 0);
}

QString GuiComponent::buildNumber() const
{
    return __TIMESTAMP__;
}

//  End of tt3-gui/GuiComponent.cpp
