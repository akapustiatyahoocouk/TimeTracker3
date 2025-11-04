//
//  tt3-db-xml/Event.cpp - tt3::db::xml::Event class implementation
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
Event::Event(
        Account * account,
        tt3::db::api::Oid oid
    ) : Object(account->_database, oid),
        _account(account)
{
    //  Register with parent
    _account->_events.insert(this);
    this->addReference();
    _account->addReference();
}

Event::~Event()
{
    Q_ASSERT(_account == nullptr);
    Q_ASSERT(_activities.isEmpty());
}

//////////
//  tt3::db::api::IEvent (properties)
auto Event::occurredAt(
    ) const -> QDateTime
{
    tt3::util::Lock _(_database->_guard);
    _ensureLive();  //  may throw
    //  We assume database is consistent since last change

    return _occurredAt;
}

auto Event::summary(
    ) const -> QString
{
    tt3::util::Lock _(_database->_guard);
    _ensureLive();  //  may throw
    //  We assume database is consistent since last change

    return _summary;
}

//////////
//  tt3::db::api::IEvent (associations)
auto Event::account(
    ) const -> tt3::db::api::IAccount *
{
    tt3::util::Lock _(_database->_guard);
    _ensureLive();  //  may throw
    //  We assume database is consistent since last change

    return _account;
}

auto Event::activities(
    ) const -> tt3::db::api::Activities
{
    tt3::util::Lock _(_database->_guard);
    _ensureLive();  //  may throw
    //  We assume database is consistent since last change

    return tt3::db::api::Activities(_activities.cbegin(), _activities.cend());
}

//////////
//  Implementation helpers
void Event::_makeDead()
{
    Q_ASSERT(_database->_guard.isLockedByCurrentThread());
    Q_ASSERT(_isLive);

    //  Break associations
    Q_ASSERT(_account != nullptr && _account->_isLive);
    Q_ASSERT(_account->_events.contains(this));
    _account->_events.remove(this);
    this->removeReference();
    _account->removeReference();
    _account = nullptr;

    for (Activity * activity : _activities.values())
    {
        Q_ASSERT(activity != nullptr && activity->_isLive);
        Q_ASSERT(activity->_events.contains(this));
        activity->_events.remove(this);
        this->removeReference();
        activity->removeReference();
    }
    _activities.clear();

    //  The rest is up to the base class
    Object::_makeDead();
}

//////////
//  Serialization
void Event::_serializeProperties(
        QDomElement & objectElement
    ) const
{
    Object::_serializeProperties(objectElement);

    objectElement.setAttribute("OccurredAt", tt3::util::toString(_occurredAt));
    objectElement.setAttribute("Summary", _summary);
}

void Event::_serializeAggregations(
        QDomElement & objectElement
    ) const
{
    Object::_serializeAggregations(objectElement);
}

void Event::_serializeAssociations(
        QDomElement & objectElement
    ) const
{
    Object::_serializeAssociations(objectElement);

    _database->_serializeAssociation(
        objectElement,
        "Activities",
        _activities);
}

void Event::_deserializeProperties(
        const QDomElement & objectElement
    )
{
    Object::_deserializeProperties(objectElement);

    _occurredAt = tt3::util::fromString(objectElement.attribute("OccurredAt"), _occurredAt);
    _summary = objectElement.attribute("Summary");
}

void Event::_deserializeAggregations(
        const QDomElement & objectElement
    )
{
    Object::_deserializeAggregations(objectElement);
}

void Event::_deserializeAssociations(
        const QDomElement & objectElement
    )
{
    Object::_deserializeAssociations(objectElement);

    _database->_deserializeAssociation(
        objectElement,
        "Activities",
        _activities);
}

//////////
//  Validation
void Event::_validate(
        Objects & validatedObjects
    )
{
    Object::_validate(validatedObjects);

    //  Validate properties
    if (!_database->_validator->event()->isValidOccurredAt(_occurredAt))
    {   //  OOPS!
        throw tt3::db::api::DatabaseCorruptException(_database->_address);
    }
    if (!_database->_validator->event()->isValidSummary(_summary))
    {   //  OOPS!
        throw tt3::db::api::DatabaseCorruptException(_database->_address);
    }

    //  Validate associations
    if (_account == nullptr || !_account->_isLive ||
        _account->_database != _database ||
        !_account->_events.contains(this))
    {   //  OOPS!
        throw tt3::db::api::DatabaseCorruptException(_database->_address);
    }
    for (Activity * activity : qAsConst(_activities))
    {
        if (activity == nullptr || !activity->_isLive ||
            activity->_database != _database ||
            !activity->_events.contains(this))
        {   //  OOPS!
            throw tt3::db::api::DatabaseCorruptException(_database->_address);
        }
    }
}

//  End of tt3-db-xml/Account.cpp
