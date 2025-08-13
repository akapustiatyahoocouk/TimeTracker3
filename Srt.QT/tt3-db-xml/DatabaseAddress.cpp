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
    tt3::util::Lock lock(databaseType->_databaseAddressesGuard);

    return _state;
}

int DatabaseAddress::referenceCount() const
{
    static DatabaseType * databaseType = DatabaseType::instance();  //  idempotent
    tt3::util::Lock lock(databaseType->_databaseAddressesGuard);

    return _referenceCount;
}

void DatabaseAddress::addReference()
{
    static DatabaseType * databaseType = DatabaseType::instance();  //  idempotent
    tt3::util::Lock lock(databaseType->_databaseAddressesGuard);

    switch (_state)
    {
        case State::New:
#ifdef QT_DEBUG
            _assertState();
#endif
            _referenceCount = 1;
            _state = State::Managed;
#ifdef QT_DEBUG
            _assertState();
#endif
            break;
        case State::Managed:
#ifdef QT_DEBUG
            _assertState();
#endif
            if (_referenceCount < INT_MAX)
            {   //  Instance remains Managed
                _referenceCount++;
            }
            else
            {   //  Can't acquire any more references
                Q_ASSERT(false);
            }
#ifdef QT_DEBUG
            _assertState();
#endif
            break;
        case State::Old:
#ifdef QT_DEBUG
            _assertState();
#endif
            _referenceCount = 1;
            _state = State::Managed;
#ifdef QT_DEBUG
            _assertState();
#endif
            break;
        default:
            Q_ASSERT(false);
    }
}

void DatabaseAddress::removeReference()
{
    static DatabaseType * databaseType = DatabaseType::instance();  //  idempotent
    tt3::util::Lock lock(databaseType->_databaseAddressesGuard);

    switch (_state)
    {
        case State::New:
#ifdef QT_DEBUG
            _assertState();
#endif
            Q_ASSERT(false);    //  Can't release a New instance
            break;
        case State::Managed:
#ifdef QT_DEBUG
            _assertState();
#endif
            if (--_referenceCount == 0)
            {   //  Instance becomes Old when losing a last reference
                _state = State::Old;
#ifdef QT_DEBUG
                _assertState();
#endif
            }
            break;
        case State::Old:
#ifdef QT_DEBUG
            _assertState();
#endif
            Q_ASSERT(false);    //  Can't release an Old instance
            break;
        default:
            Q_ASSERT(false);
    }
}

//////////
//  Implementation helpers
#ifdef QT_DEBUG
void DatabaseAddress::_assertState()
{
    static DatabaseType * databaseType = DatabaseType::instance();  //  idempotent

    Q_ASSERT(databaseType->_databaseAddressesGuard.isLockedByCurrentThread());
    switch (_state)
    {
        case State::New:
            Q_ASSERT(databaseType->_databaseAddresses.contains(_path) &&
                     databaseType->_databaseAddresses[_path] == this);
            Q_ASSERT(_referenceCount == 0);
            break;
        case State::Managed:
            Q_ASSERT(databaseType->_databaseAddresses.contains(_path) &&
                     databaseType->_databaseAddresses[_path] == this);
            Q_ASSERT(_referenceCount > 0);
            break;
        case State::Old:
            Q_ASSERT(databaseType->_databaseAddresses.contains(_path) &&
                     databaseType->_databaseAddresses[_path] == this);
            Q_ASSERT(_referenceCount == 0);
            break;
        default:
            Q_ASSERT(false);
    }
}
#endif

//  End of tt3-db-xml/DatabaseAddress.cpp
