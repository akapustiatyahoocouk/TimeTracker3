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

std::atomic<int> CurrentSkin::_instanceCount = 0;

//////////
//  Construction/destruction
CurrentSkin::CurrentSkin()
    :   _currentSkin(nullptr)
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
    ISkin * before = nullptr, * after = nullptr;

    //  Change is effected in a "locked" state
    {
        tt3::util::Lock lock(_currentSkinGuard);

        Q_ASSERT(_instanceCount == 1);
        if (skin != _currentSkin)
        {
            before = _currentSkin;
            _currentSkin = skin;
            after = _currentSkin;
        }
    }
    //  Signal is sent in a "not locked" state
    if (before != after)
    {
        emit changed(before, after);
    }
}

ISkin * CurrentSkin::operator -> () const
{
    tt3::util::Lock lock(_currentSkinGuard);

    Q_ASSERT(_instanceCount == 1);
    return _currentSkin;
}

bool CurrentSkin::operator == (nullptr_t /*null*/) const
{
    tt3::util::Lock lock(_currentSkinGuard);

    Q_ASSERT(_instanceCount == 1);
    return _currentSkin == nullptr;
}

bool CurrentSkin::operator != (nullptr_t /*null*/) const
{
    tt3::util::Lock lock(_currentSkinGuard);

    Q_ASSERT(_instanceCount == 1);
    return _currentSkin != nullptr;
}

//////////
//  Global statics
namespace tt3::gui
{
    Q_DECL_EXPORT CurrentSkin theCurrentSkin;
}

//  End of tt3-gui/CurrentSkin.cpp
