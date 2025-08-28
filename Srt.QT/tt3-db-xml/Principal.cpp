//
//  tt3-db-xml/Principal.cpp - tt3::db::xml::Principal class implementation
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
Principal::Principal(Database * database, tt3::db::api::Oid oid)
    :   Object(database, oid)
{
}

Principal::~Principal()
{
}

//////////
//  tt3::db::api::IPrincipal (general)
bool Principal::enabled() const throws(tt3::db::api::DatabaseException)
{
    tt3::util::Lock lock(_database->_guard);
    _ensureLive();  //  may throw

    return _enabled;
}

void Principal::setEnabled(bool enabled) throws(tt3::db::api::DatabaseException)
{
    tt3::util::Lock lock(_database->_guard);
    _ensureLive();  //  may throw

    if (enabled != _enabled)
    {   //  Make the change...
        _enabled = enabled;
        _database->_needsSaving = true;
        //  ...and schedule change notifications
        _database->_changeNotifier.post(
            new tt3::db::api::ObjectModifiedNotification(
                _database, type(), _oid));
    }
}

QStringList Principal::emailAddresses() const throws(tt3::db::api::DatabaseException)
{
    tt3::util::Lock lock(_database->_guard);
    _ensureLive();  //  may throw

    return _emailAddresses;
}

void Principal::setEmailAddresses(const QStringList & emailAddresses) throws(tt3::db::api::DatabaseException)
{
    tt3::util::Lock lock(_database->_guard);
    _ensureLive();  //  may throw

    //  Validate parameters
    if (!_database->_validator->principal()->isValidEmailAddresses(emailAddresses))
    {
        throw tt3::db::api::InvalidPropertyValueException(
            type(),
            "emailAddresses",
            emailAddresses.join(','));
    }
    if (emailAddresses != _emailAddresses)
    {   //  Make the change...
        _emailAddresses = emailAddresses;
        _database->_needsSaving = true;
        //  ...and schedule change notifications
        _database->_changeNotifier.post(
            new tt3::db::api::ObjectModifiedNotification(
                _database, type(), _oid));
    }
}

//////////
//  Implementation helpers
void Principal::_markDead()
{
    Q_ASSERT(_database->_guard.isLockedByCurrentThread());
    Q_ASSERT(_isLive);

    Object::_markDead();
}

//////////
//  Serialization
void Principal::_serializeProperties(QDomElement & objectElement)
{
    Object::_serializeProperties(objectElement);

    objectElement.setAttribute("Enabled", tt3::util::toString(_enabled));
    if (!_emailAddresses.isEmpty())
    {   //  A valid e-mail address has no ',' in it
        objectElement.setAttribute("EmailAddresses", _emailAddresses.join(','));
    }
}

void Principal::_serializeAggregations(QDomElement & parentElement)
{
    Object::_serializeAggregations(parentElement);
}

void Principal::_deserializeProperties(const QDomElement & objectElement) throws(tt3::util::ParseException)
{
    Object::_deserializeProperties(objectElement);

    _enabled = tt3::util::fromString<bool>(objectElement.attribute("Enabled"));
    if (objectElement.hasAttribute("EmailAddresses"))
    {   //  A valid e-mail address has no ',' in it
        _emailAddresses = objectElement.attribute("EmailAddresses").split(',');
    }
}

void Principal::_deserializeAggregations(const QDomElement & parentElement) throws(tt3::util::ParseException)
{
    Object::_deserializeAggregations(parentElement);
}

//////////
//  Validation
void Principal::_validate(QSet<Object*> & validatedObjects) throws(tt3::db::api::DatabaseException)
{
    Object::_validate(validatedObjects);

    //  Validate properties
    if (!_database->_validator->principal()->isValidEmailAddresses(_emailAddresses))
    {   //  OOPS!
        throw tt3::db::api::DatabaseCorruptException(_database->_address);
    }

    //  Validate aggregations
}

//  End of tt3-db-xml/Principal.cpp
