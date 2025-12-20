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

struct CurrentLocale::_Impl
{
    std::atomic<int>    instanceCount = 0;  //  ...to disallow a second instance
    tt3::util::Mutex    guard;
};

//////////
//  Construction/destruction
CurrentLocale::CurrentLocale()
{
    _Impl * impl = _impl();
    Q_ASSERT(impl->instanceCount == 0);

    impl->instanceCount++;
}

CurrentLocale::~CurrentLocale()
{
    _Impl * impl = _impl();
    Q_ASSERT(impl->instanceCount == 1);

    impl->instanceCount--;
}

//////////
//  Operators
CurrentLocale & CurrentLocale::operator = (const QLocale & locale)
{
    QLocale before, after;

    //  Change is effected in a "locked" state
    {
        _Impl * impl = _impl();
        Lock _(impl->guard);

        Q_ASSERT(impl->instanceCount == 1);
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
    //  Done
}

CurrentLocale::operator QLocale () const
{
    _Impl * impl = _impl();
    Lock _(impl->guard);

    return QLocale();
}

//////////
//  Implementation helpers
CurrentLocale::_Impl * CurrentLocale::_impl()
{
    static _Impl impl;
    return &impl;
}

//////////
//  Global statics
namespace tt3::util
{
    Q_DECL_EXPORT CurrentLocale theCurrentLocale;
}

//  End of tt3-gui/CurrentTheme.cpp
