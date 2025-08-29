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

std::atomic<int> CurrentCredentials::_instanceCount = 0;

//////////
//  Construction/destruction
CurrentCredentials::CurrentCredentials()
{
    Q_ASSERT(_instanceCount == 0);
    _instanceCount++;
}

CurrentCredentials::~CurrentCredentials()
{
    Q_ASSERT(_instanceCount == 1);
    _instanceCount--;
}

//////////
//  Operators
void CurrentCredentials::operator = (const Credentials & credentials)
{
    Credentials before, after;

    //  Change is effected i n a "locked" state
    {
        tt3::util::Lock lock(_currentCredentialsGuard);

        Q_ASSERT(_instanceCount == 1);
        if (credentials != _currentCredentials)
        {
            before = _currentCredentials;
            after = credentials;
            _currentCredentials = credentials;
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
    tt3::util::Lock lock(_currentCredentialsGuard);

    Q_ASSERT(_instanceCount == 1);
    return _currentCredentials;
}

//////////
//  Operations
bool CurrentCredentials::isValid() const
{
    tt3::util::Lock lock(_currentCredentialsGuard);

    Q_ASSERT(_instanceCount == 1);
    return _currentCredentials.isValid();
}

QString CurrentCredentials::login() const
{
    tt3::util::Lock lock(_currentCredentialsGuard);

    Q_ASSERT(_instanceCount == 1);
    return _currentCredentials.login();
}

//////////
//  Global statics
namespace tt3::ws
{
    Q_DECL_EXPORT CurrentCredentials theCurrentCredentials;
}

//  End of tt3-ws/CurrentCredentials.cpp
