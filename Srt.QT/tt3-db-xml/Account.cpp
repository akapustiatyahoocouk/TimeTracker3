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
        tt3::db::api::Oid oid
    ) : Principal(user->_database, oid)
{
    //  Register with parent
    user->_accounts.insert(this);
    this->addReference();
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
//  tt3::db::api::IAccount (properties)
QString Account::login() const
{
    tt3::util::Lock lock(_database->_guard);
    _ensureLive();  //  may throw
    //  We assume database is consistent since last change

    return _login;
}

void Account::setLogin(
        const QString & login
    )
{
    tt3::util::Lock lock(_database->_guard);
    _ensureLiveAndWritable();   //  may throw
#ifdef Q_DEBUG
    _database->_validate(); //  may throw
#endif

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
        _database->_markModified();
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

QString Account::passwordHash() const
{
    tt3::util::Lock lock(_database->_guard);
    _ensureLive();  //  may throw
    //  We assume database is consistent since last change

    return _passwordHash;
}

void Account::setPassword(
        const QString & password
    )
{
    tt3::util::Lock lock(_database->_guard);
    _ensureLiveAndWritable();   //  may throw
#ifdef Q_DEBUG
    _database->_validate(); //  may throw
#endif

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
    _database->_markModified();
    //  ...schedule change notifications...
    _database->_changeNotifier.post(
        new tt3::db::api::ObjectModifiedNotification(
            _database, type(), _oid));
    //  ...and we're done
#ifdef Q_DEBUG
    _database->_validate(); //  may throw
#endif
}

auto Account::capabilities(
    ) const -> tt3::db::api::Capabilities
{
    tt3::util::Lock lock(_database->_guard);
    _ensureLive();  //  may throw
    //  We assume database is consistent since last change

    return _capabilities;
}

void Account::setCapabilities(
        tt3::db::api::Capabilities capabilities
    )
{
    tt3::util::Lock lock(_database->_guard);
    _ensureLiveAndWritable();   //  may throw
#ifdef Q_DEBUG
    _database->_validate(); //  may throw
#endif

    if (capabilities != _capabilities)
    {   //  Make the change...
        _capabilities = capabilities;
        _database->_markModified();
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
//  tt3::db::api::IAccount (associations)
auto Account::user(
    ) const -> tt3::db::api::IUser *
{
    tt3::util::Lock lock(_database->_guard);
    _ensureLive();  //  may throw
    //  We assume database is consistent since last change

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

auto Account::works(
    ) const -> tt3::db::api::Works
{
    throw tt3::util::NotImplementedError();
}

auto Account::events(
    ) const -> tt3::db::api::Events
{
    throw tt3::util::NotImplementedError();
}

//////////
//  tt3::db::api::IAccount (life cycle)
auto Account::createWork(
    const QDateTime & /*startedAt*/,
    const QDateTime & /*finishedAt*/,
    tt3::db::api::IActivity * /*activity*/
    ) -> tt3::db::api::IWork *
{
    throw tt3::util::NotImplementedError();
}

auto Account::createEvent(
    const QDateTime & /*occurredAt*/,
    const QString & /*summary*/,
    tt3::db::api::IActivity * /*activity*/
    ) -> tt3::db::api::IEvent *
{
    throw tt3::util::NotImplementedError();
}

//////////
//  Implementation helpers
void Account::_markDead()
{
    Q_ASSERT(_database->_guard.isLockedByCurrentThread());
    Q_ASSERT(_isLive);

    //  Break associations
    _quickPickList.clear(); //  The _quickPickList is a one-way association

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
    ) const
{
    Principal::_serializeProperties(objectElement);

    objectElement.setAttribute("Login", _login);
    objectElement.setAttribute("PasswordHash", _passwordHash);
    objectElement.setAttribute("Capabilities", tt3::util::toString(_capabilities));
}

void Account::_serializeAggregations(
        QDomElement & objectElement
    ) const
{
    Principal::_serializeAggregations(objectElement);

    //  TODO Works           _works;         //  count as "reference"
    //  TODO Events          _events;        //  count as "reference"
}

void Account::_serializeAssociations(
        QDomElement & objectElement
    ) const
{
    Principal::_serializeAssociations(objectElement);

    _database->_serializeAssociation(
        objectElement,
        "QuickPickList",
        _quickPickList);
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
        const QDomElement & objectElement
    )
{
    Principal::_deserializeAggregations(objectElement);

    //  TODO Works           _works;         //  count as "reference"
    //  TODO Events          _events;        //  count as "reference"
}

void Account::_deserializeAssociations(
        const QDomElement & objectElement
    )
{
    Principal::_deserializeAssociations(objectElement);

    _database->_deserializeAssociation(
        objectElement,
        "QuickPickList",
        _quickPickList);
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

    //  Validate aggregations
    //  TODO Works           _works;         //  count as "reference"
    //  TODO Events          _events;        //  count as "reference"

    //  Validate associations
    if (_user == nullptr || !_user->_isLive ||
        _user->_database != _database ||
        !_user->_accounts.contains(this))
    {   //  OOPS!
        throw tt3::db::api::DatabaseCorruptException(_database->_address);
    }
    for (Activity * activity : _quickPickList)
    {
        if (activity == nullptr ||
            activity->_database != this->_database || !activity->_isLive)
        {   //  OOPS! We don't check for a back link - the
            //  "quick pick list" is a uni-directional association
            throw tt3::db::api::DatabaseCorruptException(_database->_address);
        }
    }
}

//  End of tt3-db-xml/Account.cpp
