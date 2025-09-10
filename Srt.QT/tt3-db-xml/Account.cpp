//
//  tt3-db-xml/Account.cpp - tt3::db::xml::Account class implementation
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
Account::Account(
    User * user,
    tt3::db::api::Oid oid)
    :   Principal(user->_database, oid)
{
    //  Register with parent
    user->_accounts.insert(this);
    this->emailAddresses();
    this->_user = user;
    user->addReference();
}

Account::~Account()
{
    Q_ASSERT(_user == nullptr);
}

//////////
//  tt3::db::api::IObject (life cycle)
void Account::destroy()
{
    tt3::util::Lock lock(_database->_guard);
    _ensureLive();  //  may throw

    //  This object is now "dead"
    _markDead();
}

//////////
//  tt3::db::api::IAccount (properties)
auto Account::login(
    ) const -> QString
{
    tt3::util::Lock lock(_database->_guard);
    _ensureLive();  //  may throw

    return _login;
}

void Account::setLogin(
        const QString & login
    )
{
    tt3::util::Lock lock(_database->_guard);
    _ensureLive();  //  may throw

    //  Validate parameters
    if (!_database->_validator->account()->isValidLogin(login))
    {
        throw tt3::db::api::InvalidPropertyValueException(
            type(),
            "login",
            login);
    }

    //  Logins must be unique per database
    Account * existingAccount = _database->_findAccount(login);
    if (existingAccount != nullptr && existingAccount != this)
    {   //  OOPS! Already there!
        throw tt3::db::api::AlreadyExistsException(
            type(),
            "login",
            login);
    }

    if (login != _login)
    {   //  Make the change...
        _login = login;
        _database->_needsSaving = true;
        //  ...and schedule change notifications
        _database->_changeNotifier.post(
            new tt3::db::api::ObjectModifiedNotification(
                _database, type(), _oid));
    }
}

auto Account::passwordHash(
    ) const -> QString
{
    tt3::util::Lock lock(_database->_guard);
    _ensureLive();  //  may throw

    return _passwordHash;
}

void Account::setPassword(
        const QString & password
    )
{
    tt3::util::Lock lock(_database->_guard);
    _ensureLive();  //  may throw

    //  Validate parameters
    if (!_database->_validator->account()->isValidPassword(password))
    {
        throw tt3::db::api::InvalidPropertyValueException(
            type(),
            "password",
            password);
    }

    //  Do the work
    std::unique_ptr<tt3::util::IMessageDigest::Builder> digestBuilder
        { tt3::util::Sha1MessageDigest::instance()->createBuilder() };
    digestBuilder->digestFragment(password);
    QString passwordHash = digestBuilder->digestAsString();

    //  Always make the change (we want XML file
    //  to STILL be saved even if new password is
    //  the same as old one)...
    _passwordHash = passwordHash;
    _database->_needsSaving = true;
    //  ...and schedule change notifications
        _database->_changeNotifier.post(
            new tt3::db::api::ObjectModifiedNotification(
                _database, type(), _oid));
}

auto Account::capabilities(
    ) const -> tt3::db::api::Capabilities
{
    tt3::util::Lock lock(_database->_guard);
    _ensureLive();  //  may throw

    return _capabilities;
}

void Account::setCapabilities(
        tt3::db::api::Capabilities capabilities
    )
{
    tt3::util::Lock lock(_database->_guard);
    _ensureLive();  //  may throw

    if (capabilities != _capabilities)
    {   //  Make the change...
        _capabilities = capabilities;
        _database->_needsSaving = true;
        //  ...and schedule change notifications
        _database->_changeNotifier.post(
            new tt3::db::api::ObjectModifiedNotification(
                _database, type(), _oid));
    }
}

//////////
//  tt3::db::api::IAccount (associations)
auto Account::user(
    ) const -> tt3::db::api::IUser *
{
    tt3::util::Lock lock(_database->_guard);
    _ensureLive();  //  may throw

    return _user;
}

auto Account::quickPickList(
    ) const -> QList<tt3::db::api::IActivity*>
{
    throw tt3::util::NotImplementedError();
}

void Account::setQuickPickList(
        const QList<tt3::db::api::IActivity*> & /*quickPickList*/
    )
{
    throw tt3::util::NotImplementedError();
}

//////////
//  Implementation helpers
void Account::_markDead()
{
    Q_ASSERT(_database->_guard.isLockedByCurrentThread());
    Q_ASSERT(_isLive);

    //  Remove from "live" caches
    Q_ASSERT(_user != nullptr && _user->_isLive);
    Q_ASSERT(_user->_accounts.contains(this));
    _user->_accounts.remove(this);
    this->removeReference();
    _user->removeReference();
    _user = nullptr;

    //  The rest is up to the base class
    Principal::_markDead();
}

//////////
//  Serialization
void Account::_serializeProperties(
        QDomElement & objectElement
    )
{
    Principal::_serializeProperties(objectElement);
    objectElement.setAttribute("Login", _login);
    objectElement.setAttribute("PasswordHash", _passwordHash);
    objectElement.setAttribute("Capabilities", tt3::util::toString(_capabilities));
}

void Account::_serializeAggregations(
        QDomElement & parentElement
    )
{
    Principal::_serializeAggregations(parentElement);
}

void Account::_deserializeProperties(
        const QDomElement & objectElement
    )
{
    Principal::_deserializeProperties(objectElement);

    _login = objectElement.attribute("Login");
    _passwordHash = objectElement.attribute("PasswordHash");
    _capabilities = tt3::util::fromString<tt3::db::api::Capabilities>(objectElement.attribute("Capabilities"));
}

void Account::_deserializeAggregations(
        const QDomElement & parentElement
    )
{
    Principal::_deserializeAggregations(parentElement);
}

//////////
//  Validation
void Account::_validate(
        Objects & validatedObjects
    )
{
    Principal::_validate(validatedObjects);

    //  Validate properties
    if (!_database->_validator->account()->isValidLogin(_login))
    {   //  OOPS!
        throw tt3::db::api::DatabaseCorruptException(_database->_address);
    }

    //  Validate associations
    if (_user == nullptr || !_user->_isLive ||
        _user->_database != _database ||
        !_user->_accounts.contains(this))
    {   //  OOPS!
        throw tt3::db::api::DatabaseCorruptException(_database->_address);
    }

    //  Validate aggregations
}

//  End of tt3-db-xml/Account.cpp
