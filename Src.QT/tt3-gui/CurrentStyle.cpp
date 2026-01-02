//
//  tt3-gui/CurrentStyle.cpp - tt3::gui::CurrentStyle class implementation
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

namespace tt3::gui
{
    extern CurrentTheme theCurrentTheme;
}

struct CurrentStyle::_Impl
{
    std::atomic<int>    instanceCount = 0;  //  ...to disallow a second instance
    tt3::util::Mutex    guard;
    IStyle *            style = nullptr;
};

//////////
//  Construction/destruction
CurrentStyle::CurrentStyle()
{
    _Impl * impl = _impl();
    Q_ASSERT(impl->instanceCount == 0);

    impl->instanceCount++;
}

CurrentStyle::~CurrentStyle()
{
    _Impl * impl = _impl();
    Q_ASSERT(impl->instanceCount == 1);

    impl->instanceCount--;
}

//////////
//  Operators
void CurrentStyle::operator = (IStyle * style)
{
    IStyle * before = nullptr, * after = nullptr;

    //  Change is effected in a "locked" state
    {
        _Impl * impl = _impl();
        tt3::util::Lock _(impl->guard);
        Q_ASSERT(impl->instanceCount == 1);

        if (style != impl->style)
        {
            before = impl->style;
            impl->style = style;
            after = impl->style;
        }
    }
    //  Signal is sent in a "not locked" state
    if (before != after)
    {
        if (after != nullptr)
        {
            after->apply();
            //  Must re-apply the current theme - setting
            //  the current style reverts to default palette
            if (theCurrentTheme != nullptr)
            {
                qApp->setStyleSheet(theCurrentTheme->css());
            }
        }
        emit changed(before, after);
    }
}

IStyle * CurrentStyle::operator -> () const
{
    _Impl * impl = _impl();
    tt3::util::Lock _(impl->guard);

    Q_ASSERT(impl->instanceCount == 1);
    return impl->style;
}

bool CurrentStyle::operator == (nullptr_t /*null*/) const
{
    _Impl * impl = _impl();
    tt3::util::Lock _(impl->guard);
    Q_ASSERT(impl->instanceCount == 1);

    return impl->style == nullptr;
}

bool CurrentStyle::operator != (nullptr_t /*null*/) const
{
    _Impl * impl = _impl();
    tt3::util::Lock _(impl->guard);
    Q_ASSERT(impl->instanceCount == 1);

    return impl->style != nullptr;
}

//////////
//  Implementation helpers
CurrentStyle::_Impl * CurrentStyle::_impl()
{
    static _Impl impl;
    return &impl;
}

//////////
//  Global statics
namespace tt3::gui
{
    Q_DECL_EXPORT CurrentStyle theCurrentStyle;
}

//  End of tt3-gui/CurrentStyle.cpp
