//
//  tt3-db-xml/DatabaseLock.cpp - tt3::db::xml::DatabaseLock class implementation
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
#include "tt3-db-xml/API.hpp"
using namespace tt3::db::xml;

//////////
//  Construction/destruction
DatabaseLock::DatabaseLock(
        Database * database,
        LockType lockType
    ) : _database(database),
        _lockType(lockType)
{
    Q_ASSERT(_database != nullptr);
}

DatabaseLock::~DatabaseLock()
{
    if (Database * database = _database)
    {   //  This lock is NOT stale. The atomicity of access
        //  is ensured by _database being std::atomic
        tt3::util::Lock _(database->_guard);
        database->_activeDatabaseLocks.remove(this);
        database->_clearOrphanedDatabaseLocks();
    }
}

//  End of tt3-db-xml/DatabaseLock.cpp
