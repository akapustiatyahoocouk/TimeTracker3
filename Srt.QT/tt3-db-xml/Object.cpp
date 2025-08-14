//
//  tt3-db-xml/Object.cpp - tt3::db::xml::Object class implementation
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
//  Construction/destruction (from DB type only)
Object::Object(Database * database, Oid oid)
    :   _database(database),
        _oid(oid)
{
    Q_ASSERT(_database != nullptr);
    Q_ASSERT(_oid != InvalidOid);
    Q_ASSERT(_database->isOpen());
}

Object::~Object()
{
}

//////////
//  tt3::db::api::IObject (general)
bool Object::isLive() const
{
    tt3::util::Lock lock(_database->_guard);
    return _isLive;
}

//////////
//  tt3::db::api::IObject (reference counting)
Object::State Object::state() const
{
    tt3::util::Lock lock(_database->_guard);
    return _state;
}

int Object::referenceCount() const
{
    tt3::util::Lock lock(_database->_guard);
    return _referenceCount;
}

void Object::addReference()
{
    tt3::util::Lock lock(_database->_guard);

    switch (_state)
    {
        case State::New:
            Q_ASSERT(_referenceCount == 0);
            Q_ASSERT(!_database->_graveyard.contains(this));
            _referenceCount++;
            _state = State::Managed;
            break;
        case State::Managed:
            Q_ASSERT(_referenceCount > 0);
            Q_ASSERT(!_database->_graveyard.contains(this));
            Q_ASSERT(_referenceCount + 1 > _referenceCount);
            if (_referenceCount + 1 > _referenceCount)
            {   //  Be defensive in release mode
                _referenceCount++;
            }
            break;
        case State::Old:
            Q_ASSERT(_referenceCount == 0);
            Q_ASSERT(_isLive ||_database->_graveyard.contains(this));
            _database->_graveyard.remove(this);
            _referenceCount++;
            _state = State::Managed;
            break;
        default:
            break;
    }
}

void Object::removeReference()
{
    tt3::util::Lock lock(_database->_guard);

    switch (_state)
    {
        case State::New:
            Q_ASSERT(_referenceCount == 0);
            Q_ASSERT(!_database->_graveyard.contains(this));
            //  Be defensive in release mode
            break;
        case State::Managed:
            Q_ASSERT(_referenceCount > 0);
            Q_ASSERT(!_database->_graveyard.contains(this));
            if (--_referenceCount == 0)
            {   //  The object becomes Old...
                _state = State::Old;
                if (!_isLive)
                {   //  ...and discardable
                    _database->_graveyard.insert(this);
                }
            }
            break;
        case State::Old:
            Q_ASSERT(_referenceCount == 0);
            Q_ASSERT(_isLive ||_database->_graveyard.contains(this));
            //  Be defensive in release mode
            break;
        default:
            break;
    }
}

//////////
//  Implementation helpers
void Object::_ensureLive() const throws (DatabaseException)
{
    Q_ASSERT(_database->_guard.isLockedByCurrentThread());

    _database->_ensureOpen();
    if (!_isLive)
    {
        throw tt3::db::api::InstanceDeadException();
    }
}

//  End of tt3-db-xml/Object.cpp
