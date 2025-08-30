//
//  tt3-gui/CurrentTheme.cpp - tt3::gui::CurrentTheme class implementation
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

std::atomic<int> CurrentTheme::_instanceCount = 0;

//////////
//  Construction/destruction
CurrentTheme::CurrentTheme()
    :   _currentTheme(nullptr)
{
    Q_ASSERT(_instanceCount == 0);
    _instanceCount++;
}

CurrentTheme::~CurrentTheme()
{
    Q_ASSERT(_instanceCount == 1);
    _instanceCount--;
}

//////////
//  Operators
void CurrentTheme::operator = (ITheme * theme)
{
    ITheme * before = nullptr, * after = nullptr;

    //  Change is effected in a "locked" state
    {
        tt3::util::Lock lock(_currentThemeGuard);

        Q_ASSERT(_instanceCount == 1);
        if (theme != _currentTheme)
        {
            before = _currentTheme;
            _currentTheme = theme;
            after = _currentTheme;
        }
    }
    //  Signal is sent in a "not locked" state
    if (before != after)
    {
        ((QApplication*)QApplication::instance())->setStyleSheet(after->css());
        emit changed(before, after);
    }
}

ITheme * CurrentTheme::operator -> () const
{
    tt3::util::Lock lock(_currentThemeGuard);

    Q_ASSERT(_instanceCount == 1);
    return _currentTheme;
}

bool CurrentTheme::operator == (nullptr_t /*null*/) const
{
    tt3::util::Lock lock(_currentThemeGuard);

    Q_ASSERT(_instanceCount == 1);
    return _currentTheme == nullptr;
}

bool CurrentTheme::operator != (nullptr_t /*null*/) const
{
    tt3::util::Lock lock(_currentThemeGuard);

    Q_ASSERT(_instanceCount == 1);
    return _currentTheme != nullptr;
}

//////////
//  Global statics
namespace tt3::gui
{
    Q_DECL_EXPORT CurrentTheme theCurrentTheme;
}

//  End of tt3-gui/CurrentTheme.cpp
