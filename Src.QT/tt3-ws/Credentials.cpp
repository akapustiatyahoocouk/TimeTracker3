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

//////////
//  Comparison and order
int Credentials::compare(const Credentials & op2) const
{
    return -op2.compare2(*this);
}

int Credentials::compare2(const Credentials & op2) const
{   //  Credentials <=> Credentials
    Q_ASSERT(typeid(*this) == typeid(Credentials));
    Q_ASSERT(typeid(op2) == typeid(Credentials));

    if (_login < op2._login)
    {
        return -1;
    }
    if (_login > op2._login)
    {
        return 1;
    }
    if (_password < op2._password)
    {
        return -1;
    }
    if (_password > op2._password)
    {
        return 1;
    }
    return 0;
}

int Credentials::compare2(const BackupCredentials & op2) const
{   //  Credentialc <=> BackupCredentials
    Q_ASSERT(typeid(*this) == typeid(Credentials));
    Q_ASSERT(typeid(op2) == typeid(BackupCredentials));

    return typeid(*this).before(typeid(op2)) ? -1 : 1;  //  can't be equal!
}

int Credentials::compare2(const RestoreCredentials & op2) const
{   //  Credentialc <=> RestoreCredentials
    Q_ASSERT(typeid(*this) == typeid(Credentials));
    Q_ASSERT(typeid(op2) == typeid(RestoreCredentials));

    return typeid(*this).before(typeid(op2)) ? -1 : 1;  //  can't be equal!
}

int Credentials::compare2(const ReportCredentials & op2) const
{   //  Credentialc <=> ReportCredentials
    Q_ASSERT(typeid(*this) == typeid(Credentials));
    Q_ASSERT(typeid(op2) == typeid(ReportCredentials));

    return typeid(*this).before(typeid(op2)) ? -1 : 1;  //  can't be equal!
}

//  End of tt3-ws/Credentials.cpp
