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
        LockType lockType,
        QThread * lockingThread
    ) : _database(database),
        _lockType(lockType),
        _lockingThread(lockingThread)
{
    Q_ASSERT(_database != nullptr);
    Q_ASSERT(_lockingThread != nullptr);
}

DatabaseLock::~DatabaseLock()
{
    if (_database != nullptr)
    {   //  This lock is NOT stale
        tt3::util::Lock _(_database->_guard);
        _database->_activeDatabaseLocks.remove(this);
        _database->_clearOrphanedDatabaseLocks();
        //  No need to crear _database or _lockingThread
    }
}

//  End of tt3-db-xml/DatabaseLock.cpp
