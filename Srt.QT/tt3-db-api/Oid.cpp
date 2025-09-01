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
    return value.toString();
}

template <> TT3_DB_API_PUBLIC tt3::db::api::Oid tt3::util::fromString<tt3::db::api::Oid>(const QString & s, int & scan) throws(tt3::util::ParseException)
{
    if (scan < 0 || scan + 38 > s.length())
    {
        throw tt3::util::ParseException(s, scan);
    }
    //  For now we disallow parsing invalid OIDs
    QUuid uuid(s.mid(scan, 38));
    if (uuid.isNull())
    {
        throw tt3::util::ParseException(s, scan);
    }
    scan += 38;
    return Oid(uuid);
}

//  End of tt3-db-api/Oid.cpp
