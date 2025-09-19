//
//  tt3-db-xml/Activity.cpp - tt3::db::xml::Activity class implementation
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
Activity::Activity(
        Database * database,
        tt3::db::api::Oid oid)
    :   Object(database, oid)
{
}

Activity::~Activity()
{
}

//////////
//  tt3::db::api::IActivity (general)
auto Activity::displayName(
    ) const -> QString
{
    tt3::util::Lock lock(_database->_guard);
    _ensureLive();  //  may throw
    //  We assume database is consistent since last change

    return _displayName;
}

void Activity::setDisplayName(
        const QString & /*displayName*/
    )
{
    throw tt3::util::NotImplementedError();
}

auto Activity::description(
    ) const -> QString
{
    tt3::util::Lock lock(_database->_guard);
    _ensureLive();  //  may throw
    //  We assume database is consistent since last change

    return _description;
}

void Activity::setDescription(
        const QString & /*description*/
    )
{
    throw tt3::util::NotImplementedError();
}

auto Activity::timeout(
    ) const -> tt3::db::api::InactivityTimeout
{
    tt3::util::Lock lock(_database->_guard);
    _ensureLive();  //  may throw
    //  We assume database is consistent since last change

    return _timeout;
}

void Activity::setTimeout(
        const tt3::db::api::InactivityTimeout & /*timeout*/
    )
{
    throw tt3::util::NotImplementedError();
}

bool Activity::requireCommentOnStart(
    ) const
{
    tt3::util::Lock lock(_database->_guard);
    _ensureLive();  //  may throw
    //  We assume database is consistent since last change

    return _requireCommentOnStart;
}

void Activity::setRequireCommentOnStart(
        bool /*requireCommentOnStart*/
    )
{
    throw tt3::util::NotImplementedError();
}

bool Activity::requireCommentOnFinish(
    ) const
{
    tt3::util::Lock lock(_database->_guard);
    _ensureLive();  //  may throw
    //  We assume database is consistent since last change

    return _requireCommentOnFinish;
}

void Activity::setRequireCommentOnFinish(
        bool /*requireCommentOnFinish*/
    )
{
    throw tt3::util::NotImplementedError();
}

bool Activity::fullScreenReminder(
    ) const
{
    tt3::util::Lock lock(_database->_guard);
    _ensureLive();  //  may throw
    //  We assume database is consistent since last change

    return _fullScreenReminder;
}

void Activity::setFullScreenReminder(
        bool /*fullScreenReminder*/
    )
{
    throw tt3::util::NotImplementedError();
}

//////////
//  tt3::db::api::IActivity (associations)
auto Activity::activityType(
    ) const -> tt3::db::api::IActivityType *
{
    tt3::util::Lock lock(_database->_guard);
    _ensureLive();  //  may throw
    //  We assume database is consistent since last change

    return _activityType;
}

void Activity::setActivityType(
        tt3::db::api::IActivityType * /*activityType*/
    )
{
    throw tt3::util::NotImplementedError();
}

auto Activity::workload(
    ) const -> tt3::db::api::IWorkload *
{
    tt3::util::Lock lock(_database->_guard);
    _ensureLive();  //  may throw
    //  We assume database is consistent since last change

    return _workload;
}

void Activity::setWorkload(
        tt3::db::api::IWorkload * /*workload*/
    )
{
    throw tt3::util::NotImplementedError();
}

auto Activity::works(
    ) const -> tt3::db::api::Works
{
    throw tt3::util::NotImplementedError();
}

auto Activity::events(
    ) const -> tt3::db::api::Events
{
    throw tt3::util::NotImplementedError();
}

//////////
//  Serialization
void Activity::_serializeProperties(
        QDomElement & objectElement
    )
{
    Object::_serializeProperties(objectElement);

    objectElement.setAttribute("DisplayName", _displayName);
    objectElement.setAttribute("Description", _description);
    if (_timeout.has_value())
    {
        objectElement.setAttribute("Timeout", tt3::util::toString(_timeout.value()));
    }
    objectElement.setAttribute("RequireCommentOnStart", tt3::util::toString(_requireCommentOnStart));
    objectElement.setAttribute("RequireCommentOnFinish", tt3::util::toString(_requireCommentOnFinish));
    objectElement.setAttribute("FullScreenReminder", tt3::util::toString(_fullScreenReminder));
}

void Activity::_serializeAggregations(
        QDomElement & objectElement
    )
{
    Object::_serializeAggregations(objectElement);
}

void Activity::_serializeAssociations(
        QDomElement & objectElement
    )
{
    Object::_serializeAssociations(objectElement);

    if (_activityType != nullptr)
    {
        objectElement.setAttribute("ActivityType", tt3::util::toString(_activityType->_oid));
    }
    //  TODO    Workload *      _workload = nullptr;    //  counts as "references" uness nullptr
    //  TODO    Works           _works;     //  count as "references"
    //  TODO    Events          _events;    //  count as "references"
}

void Activity::_deserializeProperties(
        const QDomElement & objectElement
    )
{
    Object::_deserializeProperties(objectElement);

    _displayName = objectElement.attribute("DisplayName");
    _description = objectElement.attribute("Description");
    if (objectElement.hasAttribute("Timeout"))
    {
        _timeout =
            tt3::util::fromString<tt3::util::TimeSpan>(
                objectElement.attribute("Timeout"));
    }
    _requireCommentOnStart = tt3::util::fromString<bool>(objectElement.attribute("RequireCommentOnStart"));
    _requireCommentOnFinish = tt3::util::fromString<bool>(objectElement.attribute("RequireCommentOnFinish"));
    _fullScreenReminder = tt3::util::fromString<bool>(objectElement.attribute("FullScreenReminder"));
}

void Activity::_deserializeAggregations(
        const QDomElement & objectElement
    )
{
    Object::_deserializeAggregations(objectElement);
}

void Activity::_deserializeAssociations(
        const QDomElement & objectElement
    )
{
    Object::_deserializeAssociations(objectElement);

    if (objectElement.hasAttribute("ActivityType"))
    {
        _activityType =
            _database->_getObject<ActivityType*>(
                tt3::util::fromString<tt3::db::api::Oid>(
                    objectElement.attribute("ActivityType")));
    }
    //  TODO    Workload *      _workload = nullptr;    //  counts as "references" uness nullptr
    //  TODO    Works           _works;     //  count as "references"
    //  TODO    Events          _events;    //  count as "references"
}

//////////
//  Validation
void Activity::_validate(
        Objects & validatedObjects
    )
{
    Object::_validate(validatedObjects);

    //  Validate properties
    if (!_database->_validator->activity()->isValidDisplayName(_displayName))
    {   //  OOPS!
        throw tt3::db::api::DatabaseCorruptException(_database->_address);
    }
    if (!_database->_validator->activity()->isValidDescription(_description))
    {   //  OOPS!
        throw tt3::db::api::DatabaseCorruptException(_database->_address);
    }
    if (_timeout.has_value() &&
        !_database->_validator->activity()->isValidTimeout(_timeout.value()))
    {   //  OOPS!
        throw tt3::db::api::DatabaseCorruptException(_database->_address);
    }

    //  Validate aggregations

    //  Validate associations
    if (_activityType != nullptr)
    {
        if (_activityType->_database != this->_database ||
            !_activityType->_isLive ||
            !_activityType->_activities.contains(this))
        {   //  OOPS!
            throw tt3::db::api::DatabaseCorruptException(_database->_address);
        }
    }
    if (_workload != nullptr)
    {
        if (_workload->_database != this->_database ||
            !_workload->_isLive ||
            !_workload->_contributingActivities.contains(this))
        {   //  OOPS!
            throw tt3::db::api::DatabaseCorruptException(_database->_address);
        }
    }
    //  TODO works, events
}

//  End of tt3-db-xml/Activity.cpp
