//
//  tt3-db-xml/Work.cpp - tt3::db::xml::Work class implementation
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
Work::Work(
        Account * account,
        tt3::db::api::Oid oid
    ) : Object(account->_database, oid)
{
    //  Register with parent
    account->_works.insert(this);
    this->addReference();
    this->_account = account;
    account->addReference();
}

Work::~Work()
{
    Q_ASSERT(_account == nullptr);
}

//////////
//  tt3::db::api::IObject (life cycle)
void Work::destroy()
{
    tt3::util::Lock lock(_database->_guard);
    _ensureLiveAndWritable();   //  may throw
#ifdef Q_DEBUG
    _database->_validate(); //  may throw
#endif

    //  This object is now "dead"
    _markDead();

//  ...and we're done
#ifdef Q_DEBUG
    _database->_validate(); //  may throw
#endif
}

//////////
//  tt3::db::api::IWork (properties)
auto Work::startedAt(
    ) const -> QDateTime
{
    tt3::util::Lock lock(_database->_guard);
    _ensureLive();  //  may throw
    //  We assume database is consistent since last change

    return _startedAt;
}

auto Work::finishedAt(
    ) const -> QDateTime
{
    tt3::util::Lock lock(_database->_guard);
    _ensureLive();  //  may throw
    //  We assume database is consistent since last change

    return _finishedAt;
}

//////////
//  tt3::db::api::IWork (associations)
auto Work::account(
    ) const -> tt3::db::api::IAccount *
{
    tt3::util::Lock lock(_database->_guard);
    _ensureLive();  //  may throw
    //  We assume database is consistent since last change

    return _account;
}

auto Work::activity(
    ) const -> tt3::db::api::IActivity *
{
    tt3::util::Lock lock(_database->_guard);
    _ensureLive();  //  may throw
    //  We assume database is consistent since last change

    return _activity;
}


//////////
//  Implementation helpers
void Work::_markDead()
{
    Q_ASSERT(_database->_guard.isLockedByCurrentThread());
    Q_ASSERT(_isLive);

    //  Break associations
    Q_ASSERT(_account != nullptr && _account->_isLive);
    Q_ASSERT(_account->_works.contains(this));
    _account->_works.remove(this);
    this->removeReference();
    _account->removeReference();
    _account = nullptr;

    //  The rest is up to the base class
    Object::_markDead();
}

//////////
//  Serialization
void Work::_serializeProperties(
        QDomElement & objectElement
    ) const
{
    Object::_serializeProperties(objectElement);

    objectElement.setAttribute("StartedAt", tt3::util::toString(_startedAt));
    objectElement.setAttribute("FinishedAt", tt3::util::toString(_finishedAt));
}

void Work::_serializeAggregations(
        QDomElement & objectElement
    ) const
{
    Object::_serializeAggregations(objectElement);
}

void Work::_serializeAssociations(
        QDomElement & objectElement
    ) const
{
    Object::_serializeAssociations(objectElement);

    _database->_serializeAssociation(
        objectElement,
        "Activity",
        _activity);
}

void Work::_deserializeProperties(
        const QDomElement & objectElement
    )
{
    Object::_deserializeProperties(objectElement);

    _startedAt = tt3::util::fromString(objectElement.attribute("StartedAt"), _startedAt);
    _finishedAt = tt3::util::fromString(objectElement.attribute("FinishedAt"), _finishedAt);
}

void Work::_deserializeAggregations(
        const QDomElement & objectElement
    )
{
    Object::_deserializeAggregations(objectElement);
}

void Work::_deserializeAssociations(
        const QDomElement & objectElement
    )
{
    Object::_deserializeAssociations(objectElement);

    _database->_deserializeAssociation(
        objectElement,
        "Activity",
        _activity);
}

//////////
//  Validation
void Work::_validate(
        Objects & validatedObjects
    )
{
    Object::_validate(validatedObjects);

    //  Validate properties
    if (!_database->_validator->work()->isValidStartedFinishedAt(_startedAt, _finishedAt))
    {   //  OOPS!
        throw tt3::db::api::DatabaseCorruptException(_database->_address);
    }

    //  Validate associations
    if (_account == nullptr || !_account->_isLive ||
        _account->_database != _database ||
        !_account->_works.contains(this))
    {   //  OOPS!
        throw tt3::db::api::DatabaseCorruptException(_database->_address);
    }
    if (_activity == nullptr || !_activity->_isLive ||
        _activity->_database != _database ||
        !_activity->_works.contains(this))
    {   //  OOPS!
        throw tt3::db::api::DatabaseCorruptException(_database->_address);
    }
}

//  End of tt3-db-xml/Account.cpp
