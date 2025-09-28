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
QString Workload::displayName() const
{
    throw tt3::util::NotImplementedError();
}

void Workload::setDisplayName(
        const QString & /*displayName*/
    )
{
    throw tt3::util::NotImplementedError();
}

QString Workload::description() const
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
//  Implementation helpers
void Workload::_markDead()
{
    Q_ASSERT(_database->_guard.isLockedByCurrentThread());
    Q_ASSERT(_isLive);

    //  Break associations
    //  TODO Beneficiaries   _beneficiaries; //  count as "references"
    for (User * user : _assignedUsers.values())
    {
        Q_ASSERT(user->_permittedWorkloads.contains(this));
        user->_permittedWorkloads.remove(this);
        _assignedUsers.remove(user);
        this->removeReference();
        user->removeReference();
    }
    _assignedUsers.clear();
    for (Activity * activity : _contributingActivities.values())
    {
        Q_ASSERT(activity->_workload == this);
        activity->_workload = nullptr;
        _contributingActivities.remove(activity);
        this->removeReference();
        activity->removeReference();
    }
    _contributingActivities.clear();

    //  The rest is up to the base class
    Object::_markDead();
}

//////////
//  Serialization
void Workload::_serializeProperties(
        QDomElement & objectElement
    ) const
{
    Object::_serializeProperties(objectElement);

    objectElement.setAttribute("DisplayName", _displayName);
    objectElement.setAttribute("Description", _description);
}

void Workload::_serializeAggregations(
        QDomElement & objectElement
    ) const
{
    Object::_serializeAggregations(objectElement);
}

void Workload::_serializeAssociations(
        QDomElement & objectElement
    ) const
{
    Object::_serializeAssociations(objectElement);

    //  TODO    Beneficiaries   _beneficiaries; //  count as "references"
    _database->_serializeAssociation(
        objectElement,
        "AssignedUsers",
        _assignedUsers);
    _database->_serializeAssociation(
        objectElement,
        "ContributingActivities",
        _contributingActivities);
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
    _database->_deserializeAssociation(
        objectElement,
        "AssignedUsers",
        _assignedUsers);
    _database->_deserializeAssociation(
        objectElement,
        "ContributingActivities",
        _contributingActivities);
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
    for (User * user : _assignedUsers)
    {
        if (user == nullptr ||
            user->_database != this->_database || !user->_isLive)
        {   //  OOPS!
            throw tt3::db::api::DatabaseCorruptException(_database->_address);
        }
    }
    for (Activity * activity : _contributingActivities)
    {
        if (activity == nullptr ||
            activity->_database != this->_database || !activity->_isLive)
        {   //  OOPS!
            throw tt3::db::api::DatabaseCorruptException(_database->_address);
        }
    }
}

//  End of tt3-db-xml/Workload.cpp
