//
//  tt3-db-sql/Account.cpp - tt3::db::sql::Account class implementation
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
#include "tt3-db-sql/API.hpp"
using namespace tt3::db::sql;

//////////
//  Construction/destruction (from DB type only)
Account::Account(
        Database * database,
        qint64 pk
    ) : Principal(database, pk),
        //  Cached properties
        _login([this] { _loadCachedProperties(); }),
        _passwordHash([this] { _loadCachedProperties(); }),
        _capabilities([this] { _loadCachedProperties(); })
{
}

Account::~Account()
{
}

//////////
//  tt3::db::api::IAccount (properties)
QString Account::login() const
{
    tt3::util::Lock _(_database->guard);
    _ensureLive();  //  may throw

    return _login;  //  Cache load may throw
}

void Account::setLogin(
        const QString & login
    )
{
    tt3::util::Lock _(_database->guard);
    _ensureLiveAndWritable();   //  may throw

    //  Validate parameters
    if (!_database->validator()->account()->isValidLogin(login))
    {
        throw tt3::db::api::InvalidPropertyValueException(
            type(),
            "login",
            login);
    }

    //  Logins must be unique per database
    auto existingAccount = _database->findAccount(login);
    if (existingAccount != nullptr && existingAccount != this)
    {   //  OOPS! Already there!
        throw tt3::db::api::AlreadyExistsException(
            type(),
            "login",
            login);
    }

    if (login != _login)    //  Cache load may throw
    {   //  Make the change...
        //  TODO wrap database access into a transaction
        _saveLogin(login);  //  may throw
        _login = login;
        //  ...schedule change notifications...
        _database->_changeNotifier.post(
            new tt3::db::api::ObjectModifiedNotification(
                _database, type(), _oid));
        //  TODO post change notification to the database
        //  ...and we're done
    }
}

QString Account::passwordHash() const
{
    tt3::util::Lock _(_database->guard);
    _ensureLive();  //  may throw

    return _passwordHash;   //  Cache load may throw
}

void Account::setPassword(
        const QString & password
    )
{
    tt3::util::Lock _(_database->guard);
    _ensureLiveAndWritable();   //  may throw

    //  Validate parameters
    if (!_database->validator()->account()->isValidPassword(password))
    {
        throw tt3::db::api::InvalidPropertyValueException(
            type(),
            "password",
            password);
    }

    //  Do the work
    std::unique_ptr<tt3::util::IMessageDigest::Builder> digestBuilder
        { tt3::util::StandardMessageDigests::Sha1::instance()->createBuilder() };
    digestBuilder->digestFragment(password);
    QString passwordHash = digestBuilder->digestAsString();

    if (passwordHash != _passwordHash)  //  Cache load may throw
    {   //  Make the change...
        //  TODO wrap database access into a transaction
        _savePasswordHash(passwordHash);    //  may throw
        _passwordHash = passwordHash;
        //  ...schedule change notifications...
        _database->_changeNotifier.post(
            new tt3::db::api::ObjectModifiedNotification(
                _database, type(), _oid));
        //  TODO post change notification to the database
        //  ...and we're done
    }
}

auto Account::capabilities(
    ) const -> tt3::db::api::Capabilities
{
    tt3::util::Lock _(_database->guard);
    _ensureLive();  //  may throw

    return _capabilities;   //  Cache load may throw
}

void Account::setCapabilities(
        tt3::db::api::Capabilities capabilities
    )
{
    tt3::util::Lock _(_database->guard);
    _ensureLiveAndWritable();   //  may throw

    if (capabilities != _capabilities)
    {   //  Make the change...
        //  TODO wrap database access into a transaction
        _saveCapabilities(capabilities);    //  may throw
        _capabilities = capabilities;
        //  ...schedule change notifications...
        _database->_changeNotifier.post(
            new tt3::db::api::ObjectModifiedNotification(
                _database, type(), _oid));
        //  TODO post change notification to the database
        //  ...and we're done
    }
}

//////////
//  tt3::db::api::IAccount (associations)
auto Account::user(
    ) const -> tt3::db::api::IUser *
{
    tt3::util::Lock _(_database->guard);
    _ensureLive();  //  may throw

    throw tt3::util::NotImplementedError();
}

auto Account::quickPicksList(
    ) const -> QList<tt3::db::api::IActivity*>
{
    tt3::util::Lock _(_database->guard);
    _ensureLive();  //  may throw

    throw tt3::util::NotImplementedError();
}

void Account::setQuickPicksList(
        const QList<tt3::db::api::IActivity*> & quickPicksList
    )
{
    tt3::util::Lock _(_database->guard);
    _ensureLiveAndWritable();   //  may throw

    throw tt3::util::NotImplementedError();
}

auto Account::works(
    ) const -> tt3::db::api::Works
{
    tt3::util::Lock _(_database->guard);
    _ensureLive();  //  may throw

    throw tt3::util::NotImplementedError();
}

auto Account::works(
        const QDateTime & from,
        const QDateTime & to
    ) const -> tt3::db::api::Works
{
    tt3::util::Lock _(_database->guard);
    _ensureLive();  //  may throw

    throw tt3::util::NotImplementedError();
}

auto Account::events(
    ) const -> tt3::db::api::Events
{
    tt3::util::Lock _(_database->guard);
    _ensureLive();  //  may throw

    throw tt3::util::NotImplementedError();
}

auto Account::events(
        const QDateTime & from,
        const QDateTime & to
    ) const -> tt3::db::api::Events
{
    tt3::util::Lock _(_database->guard);
    _ensureLive();  //  may throw

    throw tt3::util::NotImplementedError();
}

//////////
//  tt3::db::api::IAccount (life cycle)
auto Account::createWork(
        const QDateTime & startedAt,
        const QDateTime & finishedAt,
        tt3::db::api::IActivity * activity
    ) -> tt3::db::api::IWork *
{
    tt3::util::Lock _(_database->guard);
    _ensureLiveAndWritable();   //  may throw

    throw tt3::util::NotImplementedError();
}

auto Account::createEvent(
        const QDateTime & occurredAt,
        const QString & summary,
        const tt3::db::api::Activities & activities
    ) -> tt3::db::api::IEvent *
{
    tt3::util::Lock _(_database->guard);
    _ensureLiveAndWritable();   //  may throw

    throw tt3::util::NotImplementedError();
}

//////////
//  Cached properties
void Account::_invalidateCachedProperties()
{
    Principal::_invalidateCachedProperties();
    _login.invalidate();
    _passwordHash.invalidate();
    _capabilities.invalidate();
}

void Account::_loadCachedProperties()
{
    throw tt3::util::NotImplementedError();
}

void Account::_saveLogin(const QString & login)
{
    throw tt3::util::NotImplementedError();
}

void Account::_savePasswordHash(const QString & passwordHash)
{
    throw tt3::util::NotImplementedError();
}

void Account::_saveCapabilities(tt3::db::api::Capabilities  capabilities)
{
    throw tt3::util::NotImplementedError();
}

//////////
//  Implementation helpers
void Account::_makeDead()
{
    Q_ASSERT(_database->guard.isLockedByCurrentThread());
    Q_ASSERT(_isLive);

    throw tt3::util::NotImplementedError();

    //  The rest is up to the base class
    Principal::_makeDead();
}

//  End of tt3-db-sql/Account.cpp
