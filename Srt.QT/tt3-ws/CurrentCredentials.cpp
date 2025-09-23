//
//  tt3-ws/CurrentCredentials.cpp - tt3::gui::CurrentCredentials class implementation
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
#include "tt3-ws/API.hpp"
using namespace tt3::ws;

struct CurrentCredentials::_Impl
{
    std::atomic<int>    instanceCount = 0;  //  ...to disallow a second instance
    tt3::util::Mutex    guard;
    Credentials         credentials;
};

//////////
//  Construction/destruction
CurrentCredentials::CurrentCredentials()
{
    _Impl * impl = _impl();
    Q_ASSERT(impl->instanceCount == 0);

    impl->instanceCount++;
}

CurrentCredentials::~CurrentCredentials()
{
    _Impl * impl = _impl();
    Q_ASSERT(impl->instanceCount == 1);

    impl->instanceCount--;
}

//////////
//  Operators
void CurrentCredentials::operator = (const Credentials & credentials)
{
    Credentials before, after;

    //  Change is effected i n a "locked" state
    {
        _Impl * impl = _impl();
        tt3::util::Lock lock(impl->guard);

        Q_ASSERT(impl->instanceCount == 1);
        if (credentials != impl->credentials)
        {
            before = impl->credentials;
            impl->credentials = credentials;
            after = impl->credentials;
        }
    }
    //  Notification is issued in a "not locked" state
    if (before != after)
    {
        emit changed(before, after);
    }
}

CurrentCredentials::operator const Credentials & () const
{
    _Impl * impl = _impl();
    tt3::util::Lock lock(impl->guard);
    Q_ASSERT(impl->instanceCount == 1);

    return impl->credentials;
}

//////////
//  Operations
bool CurrentCredentials::isValid() const
{
    _Impl * impl = _impl();
    tt3::util::Lock lock(impl->guard);
    Q_ASSERT(impl->instanceCount == 1);

    return impl->credentials.isValid();
}

QString CurrentCredentials::login() const
{
    _Impl * impl = _impl();
    tt3::util::Lock lock(impl->guard);
    Q_ASSERT(impl->instanceCount == 1);

    return impl->credentials.login();
}

//////////
//  Implementation helpers
CurrentCredentials::_Impl * CurrentCredentials::_impl()
{
    static _Impl impl;
    return &impl;
}

//////////
//  Global statics
namespace tt3::ws
{
    Q_DECL_EXPORT CurrentCredentials theCurrentCredentials;
}

//  End of tt3-ws/CurrentCredentials.cpp
