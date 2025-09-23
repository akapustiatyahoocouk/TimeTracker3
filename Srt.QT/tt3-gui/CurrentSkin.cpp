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

struct CurrentSkin::_Impl
{
    std::atomic<int>    instanceCount = 0; //  ...to disallow a second instance
    tt3::util::Mutex    guard;
    ISkin *             skin = nullptr;
};

//////////
//  Construction/destruction
CurrentSkin::CurrentSkin()
{
    _Impl * impl = _impl();
    Q_ASSERT(impl->instanceCount == 0);

    impl->instanceCount++;
}

CurrentSkin::~CurrentSkin()
{
    _Impl * impl = _impl();
    Q_ASSERT(impl->instanceCount == 1);

    impl->instanceCount--;
}

//////////
//  Operators
void CurrentSkin::operator = (ISkin * skin)
{
    ISkin * before = nullptr, * after = nullptr;

    //  Change is effected in a "locked" state
    {
        _Impl * impl = _impl();
        tt3::util::Lock lock(impl->guard);
        Q_ASSERT(impl->instanceCount == 1);

        if (skin != impl->skin)
        {
            before = impl->skin;
            impl->skin = skin;
            after = impl->skin;
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
    _Impl * impl = _impl();
    tt3::util::Lock lock(impl->guard);
    Q_ASSERT(impl->instanceCount == 1);

    return impl->skin;
}

bool CurrentSkin::operator == (nullptr_t /*null*/) const
{
    _Impl * impl = _impl();
    tt3::util::Lock lock(impl->guard);
    Q_ASSERT(impl->instanceCount == 1);

    return impl->skin == nullptr;
}

bool CurrentSkin::operator != (nullptr_t /*null*/) const
{
    _Impl * impl = _impl();
    tt3::util::Lock lock(impl->guard);
    Q_ASSERT(impl->instanceCount == 1);

    return impl->skin != nullptr;
}

//////////
//  Implementation helpers
CurrentSkin::_Impl * CurrentSkin::_impl()
{
    static _Impl impl;
    return &impl;
}

//////////
//  Global statics
namespace tt3::gui
{
    Q_DECL_EXPORT CurrentSkin theCurrentSkin;
}

//  End of tt3-gui/CurrentSkin.cpp
