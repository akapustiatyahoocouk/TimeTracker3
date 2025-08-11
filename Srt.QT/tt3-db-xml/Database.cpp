//
//  tt3-db-xml/Database.cpp - tt3::db::xml::Database class implementation
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
Database::Database(DatabaseAddress * address, _Mode mode)
    :   _address(address),
        _validator(tt3::db::api::DefaultValidator::instance())
{
    Q_ASSERT(_address != nullptr);

    //  We need to create a lock before we do anything about the database
    std::unique_ptr<_LockRefresher> lockRefresher { new _LockRefresher(this) };

    //  Start the lock refresher
    _lockRefresher = lockRefresher.release();
    _lockRefresher->start();
}

Database::~Database()
{
    try
    {
        close();
    }
    catch (...)
    {   //  OOPS! TODO log?
    }
}

//////////
//  tt3::db::api::IDatabase (general)
bool Database::isOpen()
{
    tt3::util::Lock lock(_guard);

    return _lockRefresher != nullptr;
}

void Database::close() throws(DatabaseException)
{
    tt3::util::Lock lock(_guard);

    //  TODO implement
}

//////////
//  Database::_LockRefresher
Database::_LockRefresher::_LockRefresher(Database * database)
    :   _database(database),
        _lockFile(database->_address->_path + ".lock")
{
    //  Open/create the lock file
    if (_lockFile.exists())
    {   //  Can we take over the stale lock ?
        QFileInfo fileInfo(_lockFile);
        if (fileInfo.isFile())
        {
            QDateTime lastModifiedAt = fileInfo.lastModified(QTimeZone::UTC);
            QDateTime utcNow = QDateTime::currentDateTimeUtc();
            if (lastModifiedAt.secsTo(utcNow) < StaleTimeoutMin * 60)
            {   //  OOPS! Lock too young
                throw tt3::db::api::DatabaseInUseException(_database->_address);
            }
            //  Lock is stale - take it over
            _lockFile.setFileTime(utcNow, QFileDevice::FileTime::FileModificationTime);
        }
        else
        {   //  OOPS! Can't take over a non-file
            throw tt3::db::api::DatabaseCorruptException(_database->_address);
        }
    }
    else
    {
        if (!_lockFile.open(QIODevice::NewOnly))
        {   //  OOPS! Can't!
            throw tt3::db::api::DatabaseException(_lockFile.errorString());
        }
    }
}

Database::_LockRefresher::~_LockRefresher()
{
    _lockFile.close();
    _lockFile.remove();
}

void Database::_LockRefresher::run()
{
    const int WaitChunkMs = 1000;

    QDateTime waitChunkStartedAt = QDateTime::currentDateTimeUtc();
    while (!_stopRequested)
    {
        msleep(WaitChunkMs);
        QDateTime utcNow = QDateTime::currentDateTimeUtc();
        if (waitChunkStartedAt.secsTo(utcNow) < RefreshIntervalMin)
        {   //  Keep waiting
            continue;
        }
        //  It's time to update the last modification time of the lock file
        _lockFile.setFileTime(utcNow, QFileDevice::FileTime::FileModificationTime);
    }
}

//  End of tt3-db-xml/Database.cpp
