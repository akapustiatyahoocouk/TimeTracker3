//
//  tt3-gui/CurrentSkin.cpp - tt3::gui::CurrentSkin class implementation
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

int CurrentSkin::_instanceCount = 0;
ISkin * CurrentSkin::_currentSkin = nullptr;

//////////
//  Construction/destruction
CurrentSkin::CurrentSkin()
{
    Q_ASSERT(_instanceCount == 0);
    _instanceCount++;
}

CurrentSkin::~CurrentSkin()
{
    Q_ASSERT(_instanceCount == 1);
    _instanceCount--;
}

//////////
//  Operators
void CurrentSkin::operator = (ISkin * skin)
{
    Q_ASSERT(_instanceCount == 1);
    _currentSkin = skin;
}

ISkin * CurrentSkin::operator -> () const
{
    Q_ASSERT(_instanceCount == 1);
    return _currentSkin;
}

CurrentSkin::operator ISkin * () const
{
    Q_ASSERT(_instanceCount == 1);
    return _currentSkin;
}

//////////
//  Global statics
namespace tt3::gui
{
    Q_DECL_EXPORT CurrentSkin theCurrentSkin;
}

//  End of tt3-gui/CurrentSkin.cpp
