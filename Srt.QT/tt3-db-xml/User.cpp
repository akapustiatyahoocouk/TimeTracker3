//
//  tt3-db-xml/User.cpp - tt3::db::xml::User class implementation
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
User::User(Database * database, Oid oid)
    :   Principal(database, oid)
{
}

User::~User()
{
}

//////////
//  tt3::db::api::IDatabaseObject (life cycle)
void User::destroy() throws(DatabaseException)
{
    throw tt3::db::api::DatabaseException("Not yet implemented");
}

//////////
//  tt3::db::api::IUser (properties)
QString User::realName() const throws(DatabaseException)
{
    tt3::util::Lock lock(_database->_guard);
    _ensureLive();  //  may throw

    return _realName;
}

void User::setRealName(const QString & realName) throws(DatabaseException)
{
    tt3::util::Lock lock(_database->_guard);
    _ensureLive();  //  may throw

    //  Validate parameters
    if (!_database->_validator->user()->isValidRealName(realName))
    {
        throw tt3::db::api::InvalidPropertyValueException(
            type(),
            "realName",
            realName);
    }
    if (realName != _realName)
    {   //  Make the change...
        _realName = realName;
        _database->_needsSaving = true;
        //  ...and schedule change notifications
        //  TODO
    }
}

std::optional<tt3::util::TimeSpan>
User::inactivityTimeout() const throws(DatabaseException)
{
    tt3::util::Lock lock(_database->_guard);
    _ensureLive();  //  may throw

    return _inactivityTimeout;
}

void User::setInactivityTimeout(const std::optional<tt3::util::TimeSpan> & inactivityTimeout) throws(DatabaseException)
{
    tt3::util::Lock lock(_database->_guard);
    _ensureLive();  //  may throw

    //  Validate parameters
    if (inactivityTimeout.has_value() &&
        !_database->_validator->user()->isValidInactivityTimeout(inactivityTimeout.value()))
    {
        throw tt3::db::api::InvalidPropertyValueException(
            type(),
            "inactivityTimeout",
            inactivityTimeout.value());
    }
    if (inactivityTimeout != _inactivityTimeout)
    {   //  Make the change...
        _inactivityTimeout = inactivityTimeout;
        _database->_needsSaving = true;
        //  ...and schedule change notifications
        //  TODO
    }
}

std::optional<QLocale>
User::uiLocale() const throws(DatabaseException)
{
    tt3::util::Lock lock(_database->_guard);
    _ensureLive();  //  may throw

    return _uiLocale;
}

void User::setUiLocale(const std::optional<QLocale> & uiLocale) throws(DatabaseException)
{
    tt3::util::Lock lock(_database->_guard);
    _ensureLive();  //  may throw

    //  Validate parameters
    if (uiLocale.has_value() &&
        !_database->_validator->user()->isValidUiLocale(uiLocale.value()))
    {
        throw tt3::db::api::InvalidPropertyValueException(
            type(),
            "uiLocale",
            uiLocale.value());
    }
    if (uiLocale != _uiLocale)
    {   //  Make the change...
        _uiLocale = uiLocale;
        _database->_needsSaving = true;
        //  ...and schedule change notifications
        //  TODO
    }
}

//  End of tt3-db-xml/User.cpp
