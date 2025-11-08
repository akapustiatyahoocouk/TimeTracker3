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
//  Operators
/*  TODO kill off
bool Credentials::operator == (const Credentials & op2) const
{
    return typeid(*this) == typeid(op2) &&
           _login == op2._login && _password == op2._password;
}

bool Credentials::operator != (const Credentials & op2) const
{
    return !(*this == op2);
}

bool Credentials::operator <  (const Credentials & op2) const
{
    return (typeid(*this) == typeid(op2)) ?
            ((_login < op2._login) ||
             (_login == op2._login && _password < op2._password)) :
               typeid(*this).before(typeid(op2));
}

bool Credentials::operator <= (const Credentials & op2) const
{
    return !(op2 < *this);
}

bool Credentials::operator >  (const Credentials & op2) const
{
    return op2 < *this;
}

bool Credentials::operator >= (const Credentials & op2) const
{
    return !(*this < op2);
}
*/

//////////
//  Comparing and order
namespace tt3::ws
{
    TT3_WS_PUBLIC bool operator == (const Credentials & a, const Credentials & b)
    {
        if (typeid(a) == typeid(b))
        {   //  Some properties are common for all Credentials
            if (a._login != b._login ||
                a._password != b._password)
            {
                return false;
            }
            //  Other properties are subtype-specific
            if (auto aa = dynamic_cast<const BackupCredentials*>(&a))
            {
                auto bb = dynamic_cast<const BackupCredentials*>(&b);
                return aa->_issuedAt == bb->_issuedAt &&
                       aa->_expireAt == bb->_expireAt;
            }
            //  No more checks - a and b are equal
            return true;
        }
        //  The runtime types of the two operands differ
        return false;
    }

    TT3_WS_PUBLIC bool operator != (const Credentials & a, const Credentials & b)
    {
        return !(a == b);
    }

    TT3_WS_PUBLIC bool operator <  (const Credentials & a, const Credentials & b)
    {
        if (typeid(a) == typeid(b))
        {   //  Some properties are common for all Credentials
            if (a._login < b._login)
            {
                return true;
            }
            else if (a._login > b._login)
            {
                return false;
            }
            //  Logins equal
            if (a._password < b._password)
            {
                return true;
            }
            else if (a._password > b._password)
            {
                return false;
            }
            //  Passwords equal
            //  Other properties are subtype-specific
            if (auto aa = dynamic_cast<const BackupCredentials*>(&a))
            {
                auto bb = dynamic_cast<const BackupCredentials*>(&b);
                if (aa->_issuedAt < bb->_issuedAt)
                {
                    return true;
                }
                else if (aa->_issuedAt > bb->_issuedAt)
                {
                    return false;
                }
                if (aa->_expireAt < bb->_expireAt)
                {
                    return true;
                }
                else if (aa->_expireAt > bb->_expireAt)
                {
                    return false;
                }
                //  All properties are equal
                return false;
            }
            //  No more checks - a and b are equal
            return false;
        }
        //  The runtime types of the two operands differ
        return typeid(a).before(typeid(b));
    }

    TT3_WS_PUBLIC bool operator <= (const Credentials & a, const Credentials & b)
    {
        return !(b < a);
    }

    TT3_WS_PUBLIC bool operator >  (const Credentials & a, const Credentials & b)
    {
        return b < a;
    }

    TT3_WS_PUBLIC bool operator >= (const Credentials & a, const Credentials & b)
    {
        return !(a < b);
    }
}

//  End of tt3-ws/Credentials.cpp
