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
User::User(Database * database, tt3::db::api::Oid oid)
    :   Principal(database, oid)
{
    //  Register User with parent
    _database->_users.insert(this);
    this->addReference();
}

User::~User()
{
    Q_ASSERT(!_database->_users.contains(this));
    Q_ASSERT(_accounts.isEmpty());
}

//////////
//  tt3::db::api::IObject (life cycle)
void User::destroy() throws(tt3::db::api::DatabaseException)
{
    tt3::util::Lock lock(_database->_guard);
    _ensureLive();  //  may throw

    //  Aggregated objects
    for (Account * account : _accounts.values())
    {
        account->destroy();
    }

    //  This object is now "dead"
    _markDead();
}

//////////
//  tt3::db::api::IUser (properties)
QString User::realName() const throws(tt3::db::api::DatabaseException)
{
    tt3::util::Lock lock(_database->_guard);
    _ensureLive();  //  may throw

    return _realName;
}

void User::setRealName(const QString & realName) throws(tt3::db::api::DatabaseException)
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
        _database->_changeNotifier.post(
            new tt3::db::api::ObjectModifiedNotification(
                _database, type(), _oid));
    }
}

tt3::db::api::InactivityTimeout User::inactivityTimeout() const throws(tt3::db::api::DatabaseException)
{
    tt3::util::Lock lock(_database->_guard);
    _ensureLive();  //  may throw

    return _inactivityTimeout;
}

void User::setInactivityTimeout(const tt3::db::api::InactivityTimeout & inactivityTimeout) throws(tt3::db::api::DatabaseException)
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
        _database->_changeNotifier.post(
            new tt3::db::api::ObjectModifiedNotification(
                _database, type(), _oid));
    }
}

tt3::db::api::UiLocale User::uiLocale() const throws(tt3::db::api::DatabaseException)
{
    tt3::util::Lock lock(_database->_guard);
    _ensureLive();  //  may throw

    return _uiLocale;
}

void User::setUiLocale(const tt3::db::api::UiLocale & uiLocale) throws(tt3::db::api::DatabaseException)
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
        _database->_changeNotifier.post(
            new tt3::db::api::ObjectModifiedNotification(
                _database, type(), _oid));
    }
}

//////////
//  tt3::db::api::IUser  (life cycle)
tt3::db::api::IAccount *
User::createAccount(
    bool enabled, const QStringList & emailAddresses,
    const QString & login, const QString & password,
    tt3::db::api::Capabilities capabilities) throws(tt3::db::api::DatabaseException)
{
    tt3::util::Lock lock(_database->_guard);
    _ensureLive();  //  may throw

    //  Validate parameters
    if (!_database->_validator->principal()->isValidEmailAddresses(emailAddresses))
    {
        throw tt3::db::api::InvalidPropertyValueException(
            tt3::db::api::ObjectTypes::Account::instance(),
            "emailAddresses",
            emailAddresses.join(','));
    }
    if (!_database->_validator->account()->isValidLogin(login))
    {
        throw tt3::db::api::InvalidPropertyValueException(
            tt3::db::api::ObjectTypes::Account::instance(),
            "login",
            login);
    }
    if (!_database->_validator->account()->isValidPassword(password))
    {
        throw tt3::db::api::InvalidPropertyValueException(
            tt3::db::api::ObjectTypes::Account::instance(),
            "password",
            password);
    }

    //  Logins must be unique per database
    if (_database->_findAccount(login) != nullptr)
    {   //  OOPS! Already there!
        throw tt3::db::api::AlreadyExistsException(
            tt3::db::api::ObjectTypes::Account::instance(),
            "login",
            login);
    }

    //  Do the work - create & initialize the Account...
    std::unique_ptr<tt3::util::IMessageDigest::Builder> digestBuilder
        { tt3::util::Sha1MessageDigest::instance()->createBuilder() };
    digestBuilder->digestFragment(password);
    QString passwordHash = digestBuilder->digestAsString();

    Account * account = new Account(this, _database->_generateOid()); //  registers with User
    account->_enabled = enabled;
    account->_emailAddresses = emailAddresses;
    account->_login = login;
    account->_passwordHash = passwordHash;
    account->_capabilities = capabilities & tt3::db::api::Capabilities::All;
    //  ...register it...
    Q_ASSERT(_accounts.contains(account));
    Q_ASSERT(account->_user == this);
    _database->_needsSaving = true;
    //  ...schedule change notifications...
    _database->_changeNotifier.post(
        new tt3::db::api::ObjectModifiedNotification(
            _database, type(), _oid));
    _database->_changeNotifier.post(
        new tt3::db::api::ObjectCreatedNotification(
            _database, account->type(), account->_oid));
    //  ...and we're done
    return account;
}

//////////
//  tt3::db::api::IUser (associations)
tt3::db::api::Accounts User::accounts() const throws(tt3::db::api::DatabaseException)
{
    tt3::util::Lock lock(_database->_guard);
    _ensureLive();  //  may throw

    return tt3::db::api::Accounts(_accounts.begin(), _accounts.end());
}

//////////
//  Implementation helpers
void User::_markDead()
{
    Q_ASSERT(_database->_guard.isLockedByCurrentThread());
    Q_ASSERT(_isLive);

    Q_ASSERT(_accounts.isEmpty());

    //  Remove from "live" caches
    Q_ASSERT(_database->_users.contains(this));
    _database->_users.remove(this);
    this->removeReference();

    //  The rest is up to the base class
    Principal::_markDead();
}

//////////
//  Serialization
void User::_serializeProperties(QDomElement & objectElement)
{
    Principal::_serializeProperties(objectElement);

    objectElement.setAttribute("RealName", _realName);
    if (_inactivityTimeout.has_value())
    {
        objectElement.setAttribute("InactivityTimeout", tt3::util::toString(_inactivityTimeout.value()));
    }
    if (_uiLocale.has_value())
    {
        objectElement.setAttribute("UiLocale", tt3::util::toString(_uiLocale.value()));
    }
}

void User::_serializeAggregations(QDomElement & parentElement)
{
    Principal::_serializeAggregations(parentElement);

    //  Do the accounts
    QDomElement accountsElement = parentElement.ownerDocument().createElement("Accounts");
    parentElement.appendChild(accountsElement);
    for (Account * account : _accounts)
    {   //  TODO try to sort by OID to reduce changes
        QDomElement accountElement = parentElement.ownerDocument().createElement("Account");
        accountsElement.appendChild(accountElement);
        //  Serialize user features
        account->_serializeProperties(accountElement);
        account->_serializeAggregations(accountElement);
    }
}

void User::_deserializeProperties(const QDomElement & objectElement) throws(tt3::util::ParseException)
{
    Principal::_deserializeProperties(objectElement);

    _realName = objectElement.attribute("RealName");
    if (objectElement.hasAttribute("InactivityTimeout"))
    {
        _inactivityTimeout =
            tt3::util::fromString<tt3::util::TimeSpan>(
                objectElement.attribute("InactivityTimeout"));
    }
    if (objectElement.hasAttribute("UiLocale"))
    {
        _uiLocale =
            tt3::util::fromString<QLocale>(
                objectElement.attribute("UiLocale"));
    }
}

void User::_deserializeAggregations(const QDomElement & parentElement) throws(tt3::util::ParseException)
{
    Principal::_deserializeAggregations(parentElement);

    //  Do the accounts
    QDomElement accountsElement = _database->_childElement(parentElement, "Accounts");  //  may throw
    for (QDomElement accountElement : _database->_childElements(accountsElement, "Account"))
    {
        tt3::db::api::Oid oid = tt3::util::fromString<tt3::db::api::Oid>(accountElement.attribute("OID", ""));
        if (_database->_liveObjects.contains(oid))
        {   //  OOPS!
            throw tt3::db::api::DatabaseCorruptException(_database->_address);
        }
        Account * account = new Account(this, oid);
        account->_deserializeProperties(accountElement);
        account->_deserializeAggregations(accountElement);
    }
}

//////////
//  Validation
void User::_validate(QSet<Object*> & validatedObjects) throws(tt3::db::api::DatabaseException)
{
    Principal::_validate(validatedObjects);

    //  Validate properties
    if (!_database->_validator->user()->isValidRealName(_realName))
    {   //  OOPS!
        throw tt3::db::api::DatabaseCorruptException(_database->_address);
    }
    if (_inactivityTimeout.has_value() &&
        !_database->_validator->user()->isValidInactivityTimeout(_inactivityTimeout.value()))
    {   //  OOPS!
        throw tt3::db::api::DatabaseCorruptException(_database->_address);
    }
    if (_uiLocale.has_value() &&
        !_database->_validator->user()->isValidUiLocale(_uiLocale.value()))
    {   //  OOPS!
        throw tt3::db::api::DatabaseCorruptException(_database->_address);
    }

    //  TODO Validate associations

    //  Validate aggregations
    for (Account * account : _accounts)
    {
        if (account == nullptr || !account->_isLive ||
            account->_database != _database ||
            account->_user != this)
        {   //  OOPS!
            throw tt3::db::api::DatabaseCorruptException(_database->_address);
        }
        account->_validate(validatedObjects);
    }
}

//  End of tt3-db-xml/User.cpp
