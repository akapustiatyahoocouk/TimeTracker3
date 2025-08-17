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

int CurrentCredentials::_instanceCount = 0;
Credentials CurrentCredentials::_currentCredentials;

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
    Q_ASSERT(_instanceCount == 1);
    if (credentials != _currentCredentials)
    {
        _currentCredentials = credentials;
        emit currentCredentialsChanged();
    }
}

CurrentCredentials::operator const Credentials & () const
{
    Q_ASSERT(_instanceCount == 1);
    return _currentCredentials;
}

//////////
//  Operations
void CurrentCredentials::swap(Credentials & other)
{
    if (other != _currentCredentials)
    {
        std::swap(_currentCredentials, other);
        emit currentCredentialsChanged();
    }
}

//////////
//  Global statics
namespace tt3::ws
{
    Q_DECL_EXPORT CurrentCredentials theCurrentCredentials;
}

//  End of tt3-ws/CurrentCredentials.cpp
