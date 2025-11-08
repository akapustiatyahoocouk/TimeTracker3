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
//  Operators
bool BackupCredentials::operator == (const BackupCredentials & op2) const
{
    return Credentials::operator == (op2) ? //  types are equal on true!
            (this->_issuedAt == op2._issuedAt &&
             this->_expireAt == op2._expireAt) :
            false;
}

bool BackupCredentials::operator != (const BackupCredentials & op2) const
{
    return !(*this == op2);
}

bool BackupCredentials::operator < (const BackupCredentials & op2) const
{
    if (typeid(*this) == typeid(op2))
    {   //  Types are equal)
        if (Credentials::operator < (op2))
        {
            return true;
        }
        else if (Credentials::operator ==(op2))
        {
            return (_issuedAt < op2._issuedAt) ||
                   (_issuedAt == op2._issuedAt && _expireAt < op2._expireAt);
        }
        else
        {
            return false;
        }
    }
    //  Else types are different
    return typeid(*this).before(typeid(op2));
}

bool BackupCredentials::operator <= (const BackupCredentials & op2) const
{
    return !(op2 < *this);
}

bool BackupCredentials::operator > (const BackupCredentials & op2) const
{
    return op2 < *this;
}

bool BackupCredentials::operator >= (const BackupCredentials & op2) const
{
    return !(*this < op2);
}

//////////
//  Credentials
bool BackupCredentials::isValid() const
{
    return Credentials::isValid() &&
           _issuedAt.isValid() && _expireAt.isValid();
}

//  End of tt3-ws/Credentials.cpp
