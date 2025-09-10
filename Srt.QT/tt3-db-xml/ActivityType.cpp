//
//  tt3-db-xml/ActivityType.cpp - tt3::db::xml::ActivityType class implementation
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
ActivityType::ActivityType(
        Database * database,
        tt3::db::api::Oid oid)
    :   Object(database, oid)
{
    //  Register User with parent
    _database->_activityTypes.insert(this);
    this->addReference();
}

ActivityType::~ActivityType()
{
    Q_ASSERT(!_database->_activityTypes.contains(this));
    Q_ASSERT(_activities.isEmpty());
}

//////////
//  tt3::db::api::IObject (life cycle)
void ActivityType::destroy()
{
    tt3::util::Lock lock(_database->_guard);
    _ensureLive();  //  may throw
#ifdef Q_DEBUG
    _database->_validate(); //  may throw
#endif

    //  Dis-associate from other objects
    //  TODO properly
    Q_ASSERT(_activities.isEmpty());

    //  This object is now "dead"
    _markDead();

    //  ...and we're done
#ifdef Q_DEBUG
    _database->_validate(); //  may throw
#endif
}

//////////
//  tt3::db::api::IActivityType (properties)
auto ActivityType::displayName(
    ) const -> QString
{
    tt3::util::Lock lock(_database->_guard);
    _ensureLive();  //  may throw
#ifdef Q_DEBUG
    _database->_validate(); //  may throw
#endif

    return _displayName;
}

void ActivityType::setDisplayName(
        const QString & displayName
    )
{
    tt3::util::Lock lock(_database->_guard);
    _ensureLive();  //  may throw
#ifdef Q_DEBUG
    _database->_validate(); //  may throw
#endif

    //  Validate parameters
    if (!_database->_validator->activityType()->isValidDisplayName(displayName))
    {
        throw tt3::db::api::InvalidPropertyValueException(
            type(),
            "displayName",
            displayName);
    }
    if (displayName != _displayName)
    {   //  Make the change (but no duplication)...
        if (_database->_findActivityType(displayName) != nullptr)
        {
            throw tt3::db::api::AlreadyExistsException(
                type(),
                "displayName",
                displayName);
        }
        _displayName = displayName;
        _database->_needsSaving = true;
        //  ...schedule change notifications...
        _database->_changeNotifier.post(
            new tt3::db::api::ObjectModifiedNotification(
                _database, type(), _oid));
        //  ...and we're done
#ifdef Q_DEBUG
        _database->_validate(); //  may throw
#endif
    }
}

auto ActivityType::description(
    ) const -> QString
{
    tt3::util::Lock lock(_database->_guard);
    _ensureLive();  //  may throw
#ifdef Q_DEBUG
    _database->_validate(); //  may throw
#endif

    return _description;
}

void ActivityType::setDescription(
        const QString & description
    )
{
    tt3::util::Lock lock(_database->_guard);
    _ensureLive();  //  may throw
#ifdef Q_DEBUG
    _database->_validate(); //  may throw
#endif

    //  Validate parameters
    if (!_database->_validator->activityType()->isValidDescription(description))
    {
        throw tt3::db::api::InvalidPropertyValueException(
            type(),
            "description",
            description);
    }
    if (description != _description)
    {   //  Make the change...
        _description = description;
        _database->_needsSaving = true;
        //  ...schedule change notifications...
        _database->_changeNotifier.post(
            new tt3::db::api::ObjectModifiedNotification(
                _database, type(), _oid));
        //  ...and we're done
#ifdef Q_DEBUG
        _database->_validate(); //  may throw
#endif
    }
}

//////////
//  tt3::db::api::IActivityType (associations)
auto ActivityType::activities(
    ) const -> tt3::db::api::Activities
{
    throw tt3::util::NotImplementedError();
}

//////////
//  Implementation helpers
void ActivityType::_markDead()
{
    Q_ASSERT(_database->_guard.isLockedByCurrentThread());
    Q_ASSERT(_isLive);

    Q_ASSERT(_activities.isEmpty());

    //  Remove from "live" caches
    Q_ASSERT(_database->_activityTypes.contains(this));
    _database->_activityTypes.remove(this);
    this->removeReference();

    //  The rest is up to the base class
    Object::_markDead();
}

//////////
//  Serialization
void ActivityType::_serializeProperties(
        QDomElement & objectElement
    )
{
    Object::_serializeProperties(objectElement);

    objectElement.setAttribute("DisplayName", _displayName);
    objectElement.setAttribute("Description", _description);
}

void ActivityType::_serializeAggregations(
        QDomElement & parentElement
    )
{
    Object::_serializeAggregations(parentElement);
}

void ActivityType::_deserializeProperties(
        const QDomElement & objectElement
    )
{
    Object::_deserializeProperties(objectElement);

    _displayName = objectElement.attribute("DisplayName");
    _description = objectElement.attribute("Description");
}

void ActivityType::_deserializeAggregations(
        const QDomElement & parentElement
    )
{
    Object::_deserializeAggregations(parentElement);
}

//////////
//  Validation
void ActivityType::_validate(
        Objects & validatedObjects
    )
{
    Object::_validate(validatedObjects);

    //  Validate properties
    if (!_database->_validator->activityType()->isValidDisplayName(_displayName))
    {   //  OOPS!
        throw tt3::db::api::DatabaseCorruptException(_database->_address);
    }
    if (!_database->_validator->activityType()->isValidDescription(_description))
    {   //  OOPS!
        throw tt3::db::api::DatabaseCorruptException(_database->_address);
    }

    //  Validate aggregations
}

//  End of tt3-db-xml/ActivityType.cpp
