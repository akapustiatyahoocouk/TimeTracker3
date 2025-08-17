//
//  tt3-ws/Credentials.cpp - tt3::ws::Credentials class implementation
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

//////////
//  Constats
const Credentials Credentials::Invalid;

//////////
//  Construction/destruction/assignment
Credentials::Credentials(const QString & login, const QString & password)
    :   _login(login.trimmed()),
        _password(password)
{
    if (_login.isEmpty())
    {   //  Normalize invalid credentials
        _password = "";
    }
}

//  End of tt3-ws/Credentials.cpp
