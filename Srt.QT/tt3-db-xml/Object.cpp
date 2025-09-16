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
Object::Object(Database * database, tt3::db::api::Oid oid)
    :   _database(database),
        _oid(oid)
{
    Q_ASSERT(_database != nullptr);
    Q_ASSERT(_oid != tt3::db::api::Oid::Invalid);
    Q_ASSERT(_database->isOpen());

    Q_ASSERT(_database->_guard.isLockedByCurrentThread());

    Q_ASSERT(!_database->_liveObjects.contains(_oid));
    Q_ASSERT(!_database->_graveyard.contains(_oid));
    _database->_liveObjects[_oid] = this;
}

Object::~Object()
{
    Q_ASSERT(_database->_guard.isLockedByCurrentThread());
    Q_ASSERT(!_isLive);
    Q_ASSERT(!_database->_liveObjects.contains(_oid));
    Q_ASSERT(_database->_graveyard.contains(_oid) &&
             _database->_graveyard[_oid] == this);
    Q_ASSERT(_referenceCount == 0);

    //  Unregister with parent
    _database->_graveyard.remove(_oid);
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

    Q_ASSERT((_isLive &&
              _database->_liveObjects.contains(_oid) &&
              _database->_liveObjects[_oid] == this) ||
             (!_isLive &&
             _database->_graveyard.contains(_oid) &&
             _database->_graveyard[_oid] == this));
    switch (_state)
    {
        case State::New:
            Q_ASSERT(_referenceCount == 0);
            _referenceCount++;
            _state = State::Managed;
            break;
        case State::Managed:
            Q_ASSERT(_referenceCount > 0);
            Q_ASSERT(_referenceCount + 1 > _referenceCount);
            if (_referenceCount + 1 > _referenceCount)
            {   //  Be defensive in release mode
                _referenceCount++;
            }
            break;
        case State::Old:
            Q_ASSERT(_referenceCount == 0);
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

    Q_ASSERT((_isLive &&
              _database->_liveObjects.contains(_oid) &&
              _database->_liveObjects[_oid] == this) ||
             (!_isLive &&
              _database->_graveyard.contains(_oid) &&
              _database->_graveyard[_oid] == this));
    switch (_state)
    {
        case State::New:
            Q_ASSERT(_referenceCount == 0);
            //  Be defensive in release mode
            break;
        case State::Managed:
            Q_ASSERT(_referenceCount > 0);
            if (--_referenceCount == 0)
            {   //  The object becomes Old. Note that this
                //  does not move the object to the "graveyard" -
                //  only making it "dead" will.
                _state = State::Old;
            }
            break;
        case State::Old:
            Q_ASSERT(_referenceCount == 0);
            //  Be defensive in release mode
            break;
        default:
            break;
    }
}

//////////
//  Implementation helpers
void Object::_ensureLive() const
{
    Q_ASSERT(_database->_guard.isLockedByCurrentThread());

    _database->_ensureOpen();
    if (!_isLive)
    {
        throw tt3::db::api::InstanceDeadException();
    }
}

void Object::_ensureLiveAndWritable() const
{
    Q_ASSERT(_database->_guard.isLockedByCurrentThread());

    _database->_ensureOpenAndWritable();
    if (!_isLive)
    {
        throw tt3::db::api::InstanceDeadException();
    }
}

void Object::_markDead()
{
    Q_ASSERT(_database->_guard.isLockedByCurrentThread());
    Q_ASSERT(_isLive);
    Q_ASSERT(_database->_liveObjects.contains(_oid));

    _isLive = false;
    _database->_liveObjects.remove(_oid);
    _database->_graveyard.insert(_oid, this);
    _database->_markModified();
    //  Schedule change notifications
    _database->_changeNotifier.post(
        new tt3::db::api::ObjectDestroyedNotification(
            _database, type(), _oid));
    //  Can we recycle now ?
    if (_referenceCount == 0)
    {   //  Yes!
        delete this;
    }
}

//////////
//  Serialization
void Object::_serializeProperties(
        QDomElement & objectElement
    )
{
    objectElement.setAttribute("OID", tt3::util::toString(_oid));
}

void Object::_serializeAggregations(
        QDomElement & /*parentElement*/
    )
{   //  Nothing at this level
}

void Object::_serializeAssociations(
        QDomElement & /*objectElement*/
    )
{   //  Nothing at this level
}

void Object::_deserializeProperties(
        const QDomElement & objectElement
    )
{
    tt3::db::api::Oid oid = tt3::util::fromString<tt3::db::api::Oid>(objectElement.attribute("OID", ""));
    if (oid != _oid)
    {   //  OOPS! Deserialization implemented wrong!
        throw tt3::db::api::DatabaseCorruptException(_database->_address);
    }
    //  Add entry to "deserializatio map" - we'll need
    //  it when deserializing associations
    _database->_deserializationMap.insert(this, objectElement);
}

void Object::_deserializeAggregations(
        const QDomElement & /*parentElement*/
    )
{   //  Nothing at this level
}

void Object::_deserializeAssociations(
        const QDomElement & /*objectElement*/
    )
{   //  Nothing at this level
}

//////////
//  Validation
void Object::_validate(
        Objects & validatedObjects
    )
{   //  Only validate each object ONCE
    Q_ASSERT(!validatedObjects.contains(this));
    validatedObjects.insert(this);

    //  Validate properties
    if (!_isLive || _oid == tt3::db::api::Oid::Invalid)
    {   //  OOPS!
        throw tt3::db::api::DatabaseCorruptException(_database->_address);
    }

    //  Validate associations
    if (!_database->_liveObjects.contains(_oid) ||
        _database->_liveObjects[_oid] != this ||
        _database->_graveyard.contains(_oid))
    {   //  OOPS! Primary and secondary cached do not match
        throw tt3::db::api::DatabaseCorruptException(_database->_address);
    }

    //  Validate aggregations
}

//  End of tt3-db-xml/Object.cpp
