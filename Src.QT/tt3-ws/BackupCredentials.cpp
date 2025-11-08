//
//  tt3-ws/BackupCredentials.cpp - tt3::ws::BackupCredentials class implementation
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
//  Construction/destruction/assignment
BackupCredentials::BackupCredentials(
        const QString & login,
        const QString & password,
        const QDateTime & issuedAt,
        const QDateTime & expireAt
    ) : Credentials(login, password),
        _issuedAt(issuedAt),
        _expireAt(expireAt)
{
}

//////////
//  Credentials
bool BackupCredentials::isValid() const
{
    return Credentials::isValid() &&
           _issuedAt.isValid() && _expireAt.isValid();
}

//////////
//  Comparison and order
int BackupCredentials::compare(const Credentials & op2) const
{
    return -op2.compare2(*this);
}

int BackupCredentials::compare2(const Credentials & op2) const
{   //  BackupCredentials <=> Credentials
    Q_ASSERT(typeid(*this) == typeid(BackupCredentials));
    Q_ASSERT(typeid(op2) == typeid(Credentials));

    return typeid(*this).before(typeid(op2)) ? -1 : 1;  //  can't be equal!
}

int BackupCredentials::compare2(const BackupCredentials & op2) const
{   //  BackupCredentials <=> BackupCredentials
    Q_ASSERT(typeid(*this) == typeid(BackupCredentials));
    Q_ASSERT(typeid(op2) == typeid(BackupCredentials));

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

    if (_issuedAt < op2._issuedAt)
    {
        return -1;
    }
    if (_issuedAt > op2._issuedAt)
    {
        return 1;
    }

    if (_expireAt < op2._expireAt)
    {
        return -1;
    }
    if (_expireAt > op2._expireAt)
    {
        return 1;
    }

    return 0;
}

//  End of tt3-ws/Credentials.cpp
