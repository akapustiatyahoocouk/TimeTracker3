//
//  tt3-db-xsql/Object.cpp - tt3::db::sql::Object class implementation
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
#include "tt3-db-sql/API.hpp"
using namespace tt3::db::sql;

//////////
//  Construction/destruction (from DB type only)
Object::Object(
        Database * database,
        qint64 pk
    ) : _database(database),
        _pk(pk),
        //  Cached properties
        _oid([this] { _loadCachedProperties(); })
{
    Q_ASSERT(_database != nullptr);
    Q_ASSERT(_database->isOpen());

    Q_ASSERT(_database->guard.isLockedByCurrentThread());

    Q_ASSERT(!_database->_liveObjects.contains(_pk));
    Q_ASSERT(!_database->_graveyard.contains(_pk));
    _database->_liveObjects[_pk] = this;
}

Object::~Object()
{
    Q_ASSERT(_database->guard.isLockedByCurrentThread());
    Q_ASSERT(!_isLive);
    Q_ASSERT(!_database->_liveObjects.contains(_pk));
    Q_ASSERT(_database->_graveyard.contains(_pk) &&
             _database->_graveyard[_pk] == this);
    Q_ASSERT(_referenceCount == 0);

    //  Unregister with parent
    _database->_graveyard.remove(_pk);
}

//////////
//  tt3::db::api::IObject (general)
auto Object::oid() const -> tt3::db::api::Oid
{
    tt3::util::Lock _(_database->guard);
    _ensureLive();

    return _oid;    //  cache load may throw
}

void Object::setOid(
        const tt3::db::api::Oid & oid
    )
{
    tt3::util::Lock _(_database->guard);
    _ensureLiveAndWritable();

    throw tt3::util::NotImplementedError();
}

bool Object::isLive() const
{
    tt3::util::Lock _(_database->guard);
    return _isLive;
}

//////////
//  tt3::db::api::IObject (life cycle)
void Object::destroy()
{
    tt3::util::Lock _(_database->guard);
    _ensureLiveAndWritable();   //  may throw

    //  TODO wrap in a transaction
    _makeDead();
}

//////////
//  tt3::db::api::IObject (reference counting)
Object::State Object::state() const
{
    tt3::util::Lock _(_database->guard);
    return _state;
}

int Object::referenceCount() const
{
    tt3::util::Lock _(_database->guard);
    return _referenceCount;
}

void Object::addReference()
{
    tt3::util::Lock _(_database->guard);

    Q_ASSERT((_isLive &&
              _database->_liveObjects.contains(_pk) &&
              _database->_liveObjects[_pk] == this) ||
             (!_isLive &&
              _database->_graveyard.contains(_pk) &&
              _database->_graveyard[_pk] == this));
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
    tt3::util::Lock _(_database->guard);

    Q_ASSERT((_isLive &&
              _database->_liveObjects.contains(_pk) &&
              _database->_liveObjects[_pk] == this) ||
             (!_isLive &&
              _database->_graveyard.contains(_pk) &&
              _database->_graveyard[_pk] == this));
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
void Object::_invalidateCachedProperties()
{
    Q_ASSERT(_database->guard.isLockedByCurrentThread());

    _oid.invalidate();
}

void Object::_ensureLive() const
{
    Q_ASSERT(_database->guard.isLockedByCurrentThread());

    _database->ensureOpen();
    if (!_isLive)
    {
        throw tt3::db::api::InstanceDeadException();
    }
}

void Object::_ensureLiveAndWritable() const
{
    Q_ASSERT(_database->guard.isLockedByCurrentThread());

    _database->ensureOpenAndWritable();
    if (!_isLive)
    {
        throw tt3::db::api::InstanceDeadException();
    }
}

void Object::_makeDead()
{
    Q_ASSERT(_database->guard.isLockedByCurrentThread());
    Q_ASSERT(_isLive);
    Q_ASSERT(_database->_liveObjects.contains(_pk));

    //  TODO delete [objects] row
    //  Make this object "dead"
    _isLive = false;
    _database->_liveObjects.remove(_pk);
    _database->_graveyard.insert(_pk, this);
    //  Schedule change notifications
    _database->_changeNotifier.post(
        new tt3::db::api::ObjectDestroyedNotification(
            _database, type(), _oid));
    //  Can we recycle now ?
    if (_referenceCount == 0 /* TODO uncomment &&
        _database->_activeDatabaseLocks.isEmpty()*/)
    {   //  Yes!
        delete this;
    }
}

//  End of tt3-db-sql/Object.cpp
