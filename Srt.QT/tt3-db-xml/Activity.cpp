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
    throw tt3::util::NotImplementedError();
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
    throw tt3::util::NotImplementedError();
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
    throw tt3::util::NotImplementedError();
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
    throw tt3::util::NotImplementedError();
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
    throw tt3::util::NotImplementedError();
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
    throw tt3::util::NotImplementedError();
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
    throw tt3::util::NotImplementedError();
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
    throw tt3::util::NotImplementedError();
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
    objectElement.setAttribute("FullScreenRemonder", tt3::util::toString(_fullScreenRemonder));
}

void Activity::_serializeAggregations(
        QDomElement & parentElement
    )
{
    Object::_serializeAggregations(parentElement);
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
    _fullScreenRemonder = tt3::util::fromString<bool>(objectElement.attribute("FullScreenRemonder"));


    throw tt3::util::NotImplementedError();
}

void Activity::_deserializeAggregations(
        const QDomElement & parentElement
    )
{
    Object::_deserializeAggregations(parentElement);
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
    //  TODO

    //  Validate aggregations
}

//  End of tt3-db-xml/Activity.cpp
