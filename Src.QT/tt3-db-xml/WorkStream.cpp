//
//  tt3-db-xml/WorkStream.cpp - tt3::db::xml::WorkStream class implementation
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
WorkStream::WorkStream(
        Database * database,
        tt3::db::api::Oid oid
    ) : Workload(database, oid)
{
    //  Register WorkStream with parent
    _database->_workStreams.insert(this);
    this->addReference();
}

WorkStream::~WorkStream()
{
    Q_ASSERT(!_database->_workStreams.contains(this));
}

//////////
//  Implementation helpers
bool WorkStream::_siblingExists(
        const QString & displayName
    ) const
{
    Q_ASSERT(_database->_guard.isLockedByCurrentThread());
    Q_ASSERT(_isLive);

    WorkStream * sibling = _database->_findWorkStream(displayName);
    return sibling != nullptr && sibling != this;
}

void WorkStream::_makeDead()
{
    Q_ASSERT(_database->_guard.isLockedByCurrentThread());
    Q_ASSERT(_isLive);

    //  Remove from "live" caches
    Q_ASSERT(_database->_workStreams.contains(this));
    _database->_workStreams.remove(this);
    this->removeReference();

    //  The rest is up to the base class
    Workload::_makeDead();
}

//////////
//  Serialization
void WorkStream::_serializeProperties(
        QDomElement & objectElement
    ) const
{
    Workload::_serializeProperties(objectElement);
}

void WorkStream::_serializeAggregations(
        QDomElement & objectElement
    ) const
{
    Workload::_serializeAggregations(objectElement);
}

void WorkStream::_serializeAssociations(
        QDomElement & objectElement
    ) const
{
    Workload::_serializeAssociations(objectElement);
}

void WorkStream::_deserializeProperties(
        const QDomElement & objectElement
    )
{
    Workload::_deserializeProperties(objectElement);
}

void WorkStream::_deserializeAggregations(
        const QDomElement & objectElement
    )
{
    Workload::_deserializeAggregations(objectElement);
}

void WorkStream::_deserializeAssociations(
        const QDomElement & objectElement
    )
{
    Workload::_deserializeAssociations(objectElement);
}

//////////
//  Validation
void WorkStream::_validate(
        Objects & validatedObjects
    )
{
    Workload::_validate(validatedObjects);

    //  Validate properties

    //  Validate aggregations
    if (!_database->_workStreams.contains(this))
    {   //  OOPS!
        throw tt3::db::api::DatabaseCorruptException(_database->_address);
    }

    //  Validate associations
}

//  End of tt3-db-xml/Project.cpp
