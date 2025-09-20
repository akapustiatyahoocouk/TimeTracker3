//
//  tt3-db-xml/Workload.cpp - tt3::db::xml::Workload class implementation
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
Workload::Workload(
        Database * database,
        tt3::db::api::Oid oid
    ) : Object(database, oid)
{
}

Workload::~Workload()
{
}

//////////
//  tt3::db::api::IWorkload (properties)
auto Workload::displayName(
    ) const -> QString
{
    throw tt3::util::NotImplementedError();
}

void Workload::setDisplayName(
        const QString & /*displayName*/
    )
{
    throw tt3::util::NotImplementedError();
}

auto Workload::description(
    ) const -> QString
{
    throw tt3::util::NotImplementedError();
}

void Workload::setDescription(
        const QString & /*description*/
    )
{
    throw tt3::util::NotImplementedError();
}

//////////
//  Serialization
void Workload::_serializeProperties(
        QDomElement & objectElement
    )
{
    Object::_serializeProperties(objectElement);

    objectElement.setAttribute("DisplayName", _displayName);
    objectElement.setAttribute("Description", _description);
}

void Workload::_serializeAggregations(
        QDomElement & objectElement
    )
{
    Object::_serializeAggregations(objectElement);
}

void Workload::_serializeAssociations(
        QDomElement & objectElement
    )
{
    Object::_serializeAssociations(objectElement);

    //  TODO    Beneficiaries   _beneficiaries; //  count as "references"
    //  TODO    Users           _assignedUsers; //  count as "references"
    //  TODO    Activities      _contributingActivities;    //  count as "references"
}

void Workload::_deserializeProperties(
        const QDomElement & objectElement
    )
{
    Object::_deserializeProperties(objectElement);

    _displayName = objectElement.attribute("DisplayName");
    _description = objectElement.attribute("Description");
}

void Workload::_deserializeAggregations(
        const QDomElement & objectElement
    )
{
    Object::_deserializeAggregations(objectElement);
}

void Workload::_deserializeAssociations(
        const QDomElement & objectElement
    )
{
    Object::_deserializeAssociations(objectElement);

    //  TODO    Beneficiaries   _beneficiaries; //  count as "references"
    //  TODO    Users           _assignedUsers; //  count as "references"
    //  TODO    Activities      _contributingActivities;    //  count as "references"
}

//////////
//  Validation
void Workload::_validate(
        Objects & validatedObjects
    )
{
    Object::_validate(validatedObjects);

    //  Validate properties
    if (!_database->_validator->workload()->isValidDisplayName(_displayName))
    {   //  OOPS!
        throw tt3::db::api::DatabaseCorruptException(_database->_address);
    }
    if (!_database->_validator->workload()->isValidDescription(_description))
    {   //  OOPS!
        throw tt3::db::api::DatabaseCorruptException(_database->_address);
    }

    //  Validate aggregations

    //  Validate associations
    //  TODO    Beneficiaries   _beneficiaries; //  count as "references"
    //  TODO    Users           _assignedUsers; //  count as "references"
    //  TODO    Activities      _contributingActivities;    //  count as "references"
}

//  End of tt3-db-xml/Workload.cpp
