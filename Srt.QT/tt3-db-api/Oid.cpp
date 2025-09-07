//
//  tt3-db-api/Oid.cpp - tt3::db::api::Oid class implementation
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
#include "tt3-db-api/API.hpp"
using namespace tt3::db::api;

namespace
{
    bool isHexDigit(QChar c)
    {
        return (c >= '0' && c <= '9') ||
               (c >= 'a' && c <= 'f') ||
               (c >= 'A' && c <= 'F');
    }
}

//////////
//  Constants
const Oid Oid::Invalid;

//////////
//  Operations
Oid Oid::createRandom()
{
    for (; ; )
    {
        QUuid uuid = QUuid::createUuid();
        if (!uuid.isNull())
        {
            return Oid(uuid);
        }
    }
}

//////////
//  Formatting and parsing
template <> TT3_DB_API_PUBLIC QString tt3::util::toString<tt3::db::api::Oid>(const tt3::db::api::Oid & value)
{
    return value._impl.toString().toUpper();
}

template <> TT3_DB_API_PUBLIC tt3::db::api::Oid tt3::util::fromString<tt3::db::api::Oid>(const QString & s, qsizetype & scan) throws(tt3::util::ParseException)
{
    if (scan < 0 || scan + 38 > s.length())
    {
        throw tt3::util::ParseException(s, scan);
    }
    if (s[scan] == '{' &&
        isHexDigit(s[scan + 1]) &&
        isHexDigit(s[scan + 2]) &&
        isHexDigit(s[scan + 3]) &&
        isHexDigit(s[scan + 4]) &&
        isHexDigit(s[scan + 5]) &&
        isHexDigit(s[scan + 6]) &&
        isHexDigit(s[scan + 7]) &&
        isHexDigit(s[scan + 8]) &&
        s[scan + 9] == '-' &&
        isHexDigit(s[scan + 10]) &&
        isHexDigit(s[scan + 11]) &&
        isHexDigit(s[scan + 12]) &&
        isHexDigit(s[scan + 13]) &&
        s[scan + 14] == '-' &&
        isHexDigit(s[scan + 15]) &&
        isHexDigit(s[scan + 16]) &&
        isHexDigit(s[scan + 17]) &&
        isHexDigit(s[scan + 18]) &&
        s[scan + 19] == '-' &&
        isHexDigit(s[scan + 20]) &&
        isHexDigit(s[scan + 21]) &&
        isHexDigit(s[scan + 22]) &&
        isHexDigit(s[scan + 23]) &&
        s[scan + 24] == '-' &&
        isHexDigit(s[scan + 25]) &&
        isHexDigit(s[scan + 26]) &&
        isHexDigit(s[scan + 27]) &&
        isHexDigit(s[scan + 28]) &&
        isHexDigit(s[scan + 29]) &&
        isHexDigit(s[scan + 20]) &&
        isHexDigit(s[scan + 31]) &&
        isHexDigit(s[scan + 32]) &&
        isHexDigit(s[scan + 33]) &&
        isHexDigit(s[scan + 34]) &&
        isHexDigit(s[scan + 35]) &&
        isHexDigit(s[scan + 36]) &&
        s[scan + 37] == '}')
    {
        QUuid uuid(s.mid(scan, 38));
        scan += 38;
        return Oid(uuid);
    }
    throw tt3::util::ParseException(s, scan);
}

//////////
//  Alforithms
TT3_DB_API_PUBLIC size_t tt3::db::api::qHash(const Oid & key, size_t seed)
{
    return qHash(key._impl, seed);
}

//  End of tt3-db-api/Oid.cpp
