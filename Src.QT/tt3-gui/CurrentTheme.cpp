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

struct CurrentTheme::_Impl
{
    std::atomic<int>    instanceCount = 0;  //  ...to disallow a second instance
    tt3::util::Mutex    guard;
    ITheme *            theme = nullptr;
};

//////////
//  Construction/destruction
CurrentTheme::CurrentTheme()
{
    _Impl * impl = _impl();
    Q_ASSERT(impl->instanceCount == 0);

    impl->instanceCount++;
}

CurrentTheme::~CurrentTheme()
{
    _Impl * impl = _impl();
    Q_ASSERT(impl->instanceCount == 1);

    impl->instanceCount--;
}

//////////
//  Operators
void CurrentTheme::operator = (ITheme * theme)
{
    ITheme * before = nullptr, * after = nullptr;

    //  Change is effected in a "locked" state
    {
        _Impl * impl = _impl();
        tt3::util::Lock _(impl->guard);
        Q_ASSERT(impl->instanceCount == 1);

        if (theme != impl->theme)
        {
            before = impl->theme;
            impl->theme = theme;
            after = impl->theme;
        }
    }
    //  Signal is sent in a "not locked" state
    if (before != after)
    {
        static_cast<QApplication*>(QApplication::instance())->setStyleSheet(after->css());
        emit changed(before, after);
    }
}

ITheme * CurrentTheme::operator -> () const
{
    _Impl * impl = _impl();
    tt3::util::Lock _(impl->guard);

    Q_ASSERT(impl->instanceCount == 1);
    return impl->theme;
}

bool CurrentTheme::operator == (nullptr_t /*null*/) const
{
    _Impl * impl = _impl();
    tt3::util::Lock _(impl->guard);
    Q_ASSERT(impl->instanceCount == 1);

    return impl->theme == nullptr;
}

bool CurrentTheme::operator != (nullptr_t /*null*/) const
{
    _Impl * impl = _impl();
    tt3::util::Lock _(impl->guard);
    Q_ASSERT(impl->instanceCount == 1);

    return impl->theme != nullptr;
}

//////////
//  Implementation helpers
CurrentTheme::_Impl * CurrentTheme::_impl()
{
    static _Impl impl;
    return &impl;
}

//////////
//  Global statics
namespace tt3::gui
{
    Q_DECL_EXPORT CurrentTheme theCurrentTheme;
}

//  End of tt3-gui/CurrentTheme.cpp
