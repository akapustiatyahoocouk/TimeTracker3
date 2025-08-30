//
//  tt3-util/CurrentLocale.cpp - tt3::gui::CurrentLocale class implementation
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

std::atomic<int> CurrentLocale::_instanceCount = 0;

//////////
//  Construction/destruction
CurrentLocale::CurrentLocale()
{
    Q_ASSERT(_instanceCount == 0);
    _instanceCount++;
}

CurrentLocale::~CurrentLocale()
{
    Q_ASSERT(_instanceCount == 1);
    _instanceCount--;
}

//////////
//  Operators
void CurrentLocale::operator = (const QLocale & locale)
{
    QLocale before, after;

    //  Change is effected in a "locked" state
    {
        Lock lock(_currentLocaleGuard);

        Q_ASSERT(_instanceCount == 1);
        QLocale defaultLocale;
        if (locale != defaultLocale)
        {
            before = defaultLocale;
            QLocale::setDefault(locale);
            after = locale;
        }
    }
    //  Signal is sent in a "not locked" state
    if (before != after)
    {
        emit changed(before, after);
    }
}

CurrentLocale::operator QLocale () const
{
    Lock lock(_currentLocaleGuard);
    return QLocale();
}

//////////
//  Global statics
namespace tt3::util
{
    Q_DECL_EXPORT CurrentLocale theCurrentLocale;
}

//  End of tt3-gui/CurrentTheme.cpp
