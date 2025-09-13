//
//  tt3-db-xml/PublicActivity.cpp - tt3::db::xml::PublicActivity class implementation
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
PublicActivity::PublicActivity(
        Database * database,
        tt3::db::api::Oid oid)
    :   Activity(database, oid)
{
    //  Register PublicActivity with parent
    _database->_publicActivities.insert(this);
    this->addReference();
}

PublicActivity::~PublicActivity()
{
    Q_ASSERT(!_database->_publicActivities.contains(this));
}

//////////
//  tt3::db::api::IObject (life cycle)
void PublicActivity::destroy()
{
    tt3::util::Lock lock(_database->_guard);
    _ensureLiveAndWritable();   //  may throw
#ifdef Q_DEBUG
    _database->_validate(); //  may throw
#endif

    //  Associations TODO properly
    Q_ASSERT(_activityType == nullptr);
    Q_ASSERT(_workload == nullptr);
    Q_ASSERT(_works.isEmpty());
    Q_ASSERT(_events.isEmpty());

    //  This object is now "dead"
    _markDead();

//  ...and we're done
#ifdef Q_DEBUG
    _database->_validate(); //  may throw
#endif
}

//////////
//  Implementation helpers
void PublicActivity::_markDead()
{
    Q_ASSERT(_database->_guard.isLockedByCurrentThread());
    Q_ASSERT(_isLive);

    Q_ASSERT(_activityType == nullptr);
    Q_ASSERT(_workload == nullptr);
    Q_ASSERT(_works.isEmpty());
    Q_ASSERT(_events.isEmpty());

    //  Remove from "live" caches
    Q_ASSERT(_database->_publicActivities.contains(this));
    _database->_publicActivities.remove(this);
    this->removeReference();

    //  The rest is up to the base class
    Activity::_markDead();
}

//////////
//  Serialization
void PublicActivity::_serializeProperties(
        QDomElement & objectElement
    )
{
    Activity::_serializeProperties(objectElement);
}

void PublicActivity::_serializeAggregations(
        QDomElement & parentElement
    )
{
    Activity::_serializeAggregations(parentElement);
}

void PublicActivity::_deserializeProperties(
        const QDomElement & objectElement
    )
{
    Activity::_deserializeProperties(objectElement);

}

void PublicActivity::_deserializeAggregations(
        const QDomElement & parentElement
    )
{
    Activity::_deserializeAggregations(parentElement);

}

//////////
//  Validation
void PublicActivity::_validate(
        Objects & validatedObjects
    )
{
    Activity::_validate(validatedObjects);

    //  Validate properties

    //  Validate associations
    //  TODO

    //  Validate aggregations
    if (!_database->_publicActivities.contains(this))
    {   //  OOPS!
        throw tt3::db::api::DatabaseCorruptException(_database->_address);
    }
}

//  End of tt3-db-xml/User.cpp
