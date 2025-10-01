//
//  tt3-db-xml/PrivateActivity.cpp - tt3::db::xml::PrivateActivity class implementation
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
PrivateActivity::PrivateActivity(
        User * owner,
        tt3::db::api::Oid oid
    ) : Activity(owner->_database, oid)
{
    //  Register PrivateActivity with parent
    _owner->_privateActivities.insert(this);
    this->addReference();
    this->_owner = owner;
    owner->addReference();
}

PrivateActivity::~PrivateActivity()
{
    Q_ASSERT(!_owner->_privateActivities.contains(this));
}

//////////
//  tt3::db::api::IObject (life cycle)
void PrivateActivity::destroy()
{
    tt3::util::Lock lock(_database->_guard);
    _ensureLiveAndWritable();   //  may throw
#ifdef Q_DEBUG
    _database->_validate(); //  may throw
#endif

    _markDead();

#ifdef Q_DEBUG
    _database->_validate(); //  may throw
#endif
}

//////////
//  tt3::db::api::IPrivateActivity (associations)
auto PrivateActivity::owner(
    ) const -> tt3::db::api::IUser *
{
    tt3::util::Lock lock(_database->_guard);
    _ensureLive();  //  may throw
    //  We assume database is consistent since last change

    return _owner;
}

//////////
//  Implementation helpers
bool PrivateActivity::_siblingExists(const QString & displayName) const
{
    Q_ASSERT(_database->_guard.isLockedByCurrentThread());
    Q_ASSERT(_isLive);

    PrivateActivity * found =
        _owner->_findPrivateActivity(displayName);
    return found != nullptr && found != this;
}

void PrivateActivity::_markDead()
{
    Q_ASSERT(_database->_guard.isLockedByCurrentThread());
    Q_ASSERT(_isLive);

    //  Break associations
    Q_ASSERT(_owner != nullptr && _owner->_isLive);
    Q_ASSERT(_owner->_privateActivities.contains(this));
    _owner->_privateActivities.remove(this);
    this->removeReference();
    _owner->removeReference();
    _owner = nullptr;

    //  The rest is up to the base class
    Activity::_markDead();
}

//////////
//  Serialization
void PrivateActivity::_serializeProperties(
        QDomElement & objectElement
    ) const
{
    Activity::_serializeProperties(objectElement);
}

void PrivateActivity::_serializeAggregations(
        QDomElement & objectElement
    ) const
{
    Activity::_serializeAggregations(objectElement);
}

void PrivateActivity::_serializeAssociations(
        QDomElement & objectElement
    ) const
{
    Activity::_serializeAssociations(objectElement);
}

void PrivateActivity::_deserializeProperties(
        const QDomElement & objectElement
    )
{
    Activity::_deserializeProperties(objectElement);
}

void PrivateActivity::_deserializeAggregations(
        const QDomElement & objectElement
    )
{
    Activity::_deserializeAggregations(objectElement);
}

void PrivateActivity::_deserializeAssociations(
        const QDomElement & objectElement
    )
{
    Activity::_deserializeAssociations(objectElement);
}

//////////
//  Validation
void PrivateActivity::_validate(
        Objects & validatedObjects
    )
{
    Activity::_validate(validatedObjects);

    //  Validate properties

    //  Validate aggregations

    //  Validate associations
    if (_owner == nullptr || !_owner->_isLive ||
        _owner->_database != _database ||
        !_owner->_privateActivities.contains(this))
    {   //  OOPS!
        throw tt3::db::api::DatabaseCorruptException(_database->_address);
    }
}

//  End of tt3-db-xml/PrivateActivity.cpp
