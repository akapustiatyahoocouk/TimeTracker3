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
    if (!_assignedUsers.isEmpty())
    {
        objectElement.setAttribute(
            "AssignedUsers",
            Database::_map<QString,User*>(
                Database::_sortedByOid(_assignedUsers),
                [](auto u) { return tt3::util::toString(u->_oid); })
                .join(","));
    }
    if (!_contributingActivities.isEmpty())
    {
        objectElement.setAttribute(
            "ContributingActivities",
            Database::_map<QString,Activity*>(
                Database::_sortedByOid(_contributingActivities),
                [](auto a) { return tt3::util::toString(a->_oid); })
                .join(","));
    }
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
    if (objectElement.hasAttribute("AssignedUsers"))
    {
        _assignedUsers =
            _database->_asSet(
                Database::_map<User*,QString>(
                    objectElement.attribute("AssignedUsers").split(','),
                    [&](auto s)
                    {
                        return _database->_getObject<User*>(
                            tt3::util::fromString(s, tt3::db::api::Oid::Invalid));
                    }));
        for (User * user : _assignedUsers)
        {
            user->addReference();
        }
    }
    if (objectElement.hasAttribute("ContributingActivities"))
    {
        _contributingActivities =
            _database->_asSet(
                Database::_map<Activity*,QString>(
                    objectElement.attribute("ContributingActivities").split(','),
                    [&](auto s)
                    {
                        return _database->_getObject<Activity*>(
                            tt3::util::fromString(s, tt3::db::api::Oid::Invalid));
                    }));
        for (Activity * activity : _contributingActivities)
        {
            activity->addReference();
        }
    }
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
