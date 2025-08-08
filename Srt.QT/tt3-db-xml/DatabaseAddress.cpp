//
//  tt3-db-xml/DatabaseAddress.cpp - tt3::db::xml::DatabaseAddress class implementation
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
//  Construction/destruction(from DB type only)
DatabaseAddress::DatabaseAddress(const QString & path)
    :   _path(QFileInfo(path).absoluteFilePath())
{
}

DatabaseAddress:: ~DatabaseAddress()
{
}

//////////
//  tt3::db::api::IDatabaseAddress (general)
tt3::db::api::IDatabaseType * DatabaseAddress::databaseType() const
{
    return DatabaseType::instance();
}

QString DatabaseAddress::displayForm() const
{
    return _path;
}

QString DatabaseAddress::externalForm() const
{
    return _path;
}

//////////
//  tt3::db::api::IDatabaseAddress (reference counting)
DatabaseAddress::State DatabaseAddress::state() const
{
    static DatabaseType * databaseType = DatabaseType::instance();  //  idempotent
    QMutexLocker<QMutex> locker(&databaseType->_databaseAddressesGuard);

    return _state;
}

int DatabaseAddress::referenceCount() const
{
    static DatabaseType * databaseType = DatabaseType::instance();  //  idempotent
    QMutexLocker<QMutex> locker(&databaseType->_databaseAddressesGuard);

    return _referenceCount;
}

void DatabaseAddress::acquireReference()
{
    static DatabaseType * databaseType = DatabaseType::instance();  //  idempotent
    QMutexLocker<QMutex> locker(&databaseType->_databaseAddressesGuard);

    switch (_state)
    {
        case State::New:
            _checkState();
            databaseType->_newDatabaseAddresses.remove(_path);
            databaseType->_managedDatabaseAddresses[_path] = this;
            _referenceCount = 1;
            _state = State::Managed;
            _checkState();
            break;
        case State::Managed:
            _checkState();
            if (_referenceCount < INT_MAX)
            {   //  Instance remains Managed
                _referenceCount++;
            }
            else
            {   //  Can't acquire any more references
                Q_ASSERT(false);
            }
            _checkState();
            break;
        case State::Old:
            _checkState();
            databaseType->_oldDatabaseAddresses.remove(_path);
            databaseType->_managedDatabaseAddresses[_path] = this;
            _referenceCount = 1;
            _state = State::Managed;
            _checkState();
            break;
        default:
            Q_ASSERT(false);
    }
}

void DatabaseAddress::releaseReference()
{
    static DatabaseType * databaseType = DatabaseType::instance();  //  idempotent
    QMutexLocker<QMutex> locker(&databaseType->_databaseAddressesGuard);

    switch (_state)
    {
        case State::New:
            _checkState();
            Q_ASSERT(false);    //  Can't release a New instance
            break;
        case State::Managed:
            _checkState();
            if (--_referenceCount == 0)
            {   //  Instance becomes Old whe n losing a last reference
                databaseType->_managedDatabaseAddresses.remove(_path);
                databaseType->_oldDatabaseAddresses[_path] = this;
                _state = State::Old;
                _checkState();
            }
            break;
        case State::Old:
            _checkState();
            Q_ASSERT(false);    //  Can't release an Old instance
            break;
        default:
            Q_ASSERT(false);
    }
}

//////////
//  Implementation helpers
void DatabaseAddress::_checkState()
{
    static DatabaseType * databaseType = DatabaseType::instance();  //  idempotent

    switch (_state)
    {
        case State::New:
            Q_ASSERT(databaseType->_newDatabaseAddresses.contains(_path) &&
                     databaseType->_newDatabaseAddresses[_path] == this);
            Q_ASSERT(!databaseType->_managedDatabaseAddresses.contains(_path));
            Q_ASSERT(!databaseType->_oldDatabaseAddresses.contains(_path));
            Q_ASSERT(_referenceCount == 0);
            break;
        case State::Managed:
            Q_ASSERT(databaseType->_managedDatabaseAddresses.contains(_path) &&
                     databaseType->_managedDatabaseAddresses[_path] == this);
            Q_ASSERT(!databaseType->_newDatabaseAddresses.contains(_path));
            Q_ASSERT(!databaseType->_oldDatabaseAddresses.contains(_path));
            Q_ASSERT(_referenceCount > 0);
            break;
        case State::Old:
            Q_ASSERT(databaseType->_oldDatabaseAddresses.contains(_path) &&
                     databaseType->_oldDatabaseAddresses[_path] == this);
            Q_ASSERT(!databaseType->_newDatabaseAddresses.contains(_path));
            Q_ASSERT(!databaseType->_managedDatabaseAddresses.contains(_path));
            Q_ASSERT(_referenceCount == 0);
            break;
        default:
            Q_ASSERT(false);
    }
}

//  End of tt3-db-xml/DatabaseAddress.cpp
