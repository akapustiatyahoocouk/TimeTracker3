//
//  tt3-db-sql/User.cpp - tt3::db::sql::User class implementation
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
User::User(Database * database, qint64 pk)
    :   Principal(database, pk),
        //  Cached propertirs
        _realName([this] { _loadCachedProperties(); }),
        _inactivityTimeout([this] { _loadCachedProperties(); }),
        _uiLocale([this] { _loadCachedProperties(); })
{
}

User::~User()
{
}

//////////
//  tt3::db::api::IUser (properties)
QString User::realName() const
{
    tt3::util::Lock _(_database->guard);
    _ensureLive();  //  may throw

    return _realName;   //  Cache load may throw
}

void User::setRealName(
        const QString & realName
    )
{
    tt3::util::Lock _(_database->guard);
    _ensureLiveAndWritable();   //  may throw

    //  Validate parameters
    if (!_database->validator()->user()->isValidRealName(realName))
    {
        throw tt3::db::api::InvalidPropertyValueException(
            type(),
            "realName",
            realName);
    }

    if (realName != _realName)  //  Cache load may throw
    {   //  Make the change...
        //  TODO wrap database access into a transaction
        _saveRealName(realName);    //  may throw
        _realName = realName;
        //  ...schedule change notifications....
        _database->_changeNotifier.post(
            new tt3::db::api::ObjectModifiedNotification(
                _database, type(), _oid));
        //  TODO post change notification to the database
        //  ...and we're done
    }
}

auto User::inactivityTimeout(
    ) const -> tt3::db::api::InactivityTimeout
{
    tt3::util::Lock _(_database->guard);
    _ensureLive();  //  may throw

    return _inactivityTimeout;  //  Cache load may throw
}

void User::setInactivityTimeout(
        const tt3::db::api::InactivityTimeout & inactivityTimeout
    )
{
    tt3::util::Lock _(_database->guard);
    _ensureLiveAndWritable();   //  may throw

    //  Validate parameters
    if (inactivityTimeout.has_value() &&
        !_database->validator()->user()->isValidInactivityTimeout(inactivityTimeout.value()))
    {
        throw tt3::db::api::InvalidPropertyValueException(
            type(),
            "inactivityTimeout",
            inactivityTimeout.value());
    }

    if (inactivityTimeout != _inactivityTimeout.value())    //  Cache load may throw
    {   //  Make the change...
        //  TODO wrap database access into a transaction
        _saveInactivityTimeout(inactivityTimeout);  //  may throw
        _inactivityTimeout = inactivityTimeout;
        //  ...schedule change notifications...
        _database->_changeNotifier.post(
            new tt3::db::api::ObjectModifiedNotification(
                _database, type(), _oid));
        //  TODO post change notification to the database
        //  ...and we're done
    }
}

auto User::uiLocale(
    ) const -> tt3::db::api::UiLocale
{
    tt3::util::Lock _(_database->guard);
    _ensureLive();  //  may throw

    return _uiLocale;   //  Cache load may throw
}

void User::setUiLocale(
        const tt3::db::api::UiLocale & uiLocale
    )
{
    tt3::util::Lock _(_database->guard);
    _ensureLiveAndWritable();   //  may throw

    //  Validate parameters
    if (uiLocale.has_value() &&
        !_database->validator()->user()->isValidUiLocale(uiLocale.value()))
    {
        throw tt3::db::api::InvalidPropertyValueException(
            type(),
            "uiLocale",
            uiLocale.value());
    }

    if (uiLocale != _uiLocale.value())  //  Cache load may throw
    {   //  Make the change...
        //  TODO wrap database access into a transaction
        _saveUiLocale(uiLocale);    //  may throw
        _uiLocale = uiLocale;
        //  ...schedule change notifications...
        _database->_changeNotifier.post(
            new tt3::db::api::ObjectModifiedNotification(
                _database, type(), _oid));
        //  TODO post change notification to the database
        //  ...and we're done
    }
}

//////////
//  tt3::db::api::IUser (associations)
auto User::accounts(
    ) const -> tt3::db::api::Accounts
{
    throw tt3::util::NotImplementedError();
}

auto User::privateActivities(
    ) const -> tt3::db::api::PrivateActivities
{
    throw tt3::util::NotImplementedError();
}

auto User::privateActivitiesAndTasks(
    ) const -> tt3::db::api::PrivateActivities
{
    throw tt3::util::NotImplementedError();
}

auto User::privateTasks(
    ) const -> tt3::db::api::PrivateTasks
{
    throw tt3::util::NotImplementedError();
}

auto User::rootPrivateTasks(
    ) const -> tt3::db::api::PrivateTasks
{
    throw tt3::util::NotImplementedError();
}

auto User::permittedWorkloads(
    ) const -> tt3::db::api::Workloads
{
    throw tt3::util::NotImplementedError();
}

void User::setPermittedWorkloads(
        const tt3::db::api::Workloads & workloads
    )
{
    throw tt3::util::NotImplementedError();
}

void User::addPermittedWorkload(
        tt3::db::api::IWorkload * workload
    )
{
    throw tt3::util::NotImplementedError();
}

void User::removePermittedWorkload(
        tt3::db::api::IWorkload * workload
    )
{
    throw tt3::util::NotImplementedError();
}

//////////
//  tt3::db::api::IUser  (life cycle)
auto User::createAccount(
        bool enabled,
        const QStringList & emailAddresses,
        const QString & login,
        const QString & password,
        tt3::db::api::Capabilities capabilities
    ) -> tt3::db::api::IAccount *
{
    throw tt3::util::NotImplementedError();
}

auto User::createPrivateActivity(
        const QString & displayName,
        const QString & description,
        const tt3::db::api::InactivityTimeout & timeout,
        bool requireCommentOnStart,
        bool requireCommentOnStop,
        bool fullScreenReminder,
        tt3::db::api::IActivityType * activityType,
        tt3::db::api::IWorkload * workload
    ) -> tt3::db::api::IPrivateActivity *
{
    throw tt3::util::NotImplementedError();
}

auto User::createPrivateTask(
        const QString & displayName,
        const QString & description,
        const tt3::db::api::InactivityTimeout & timeout,
        bool requireCommentOnStart,
        bool requireCommentOnStop,
        bool fullScreenReminder,
        tt3::db::api::IActivityType * activityType,
        tt3::db::api::IWorkload * workload,
        bool completed,
        bool requireCommentOnCompletion
    ) -> tt3::db::api::IPrivateTask *
{
    throw tt3::util::NotImplementedError();
}

//////////
//  Cached properties
void User::_invalidateCachedProperties()
{
    Principal::_invalidateCachedProperties();
    _realName.invalidate();
    _inactivityTimeout.invalidate();
    _uiLocale.invalidate();
}

void User::_loadCachedProperties()
{
    throw tt3::util::NotImplementedError();
}

void User::_saveRealName(const QString & realName)
{
    throw tt3::util::NotImplementedError();
}

void User::_saveInactivityTimeout(const tt3::db::api::InactivityTimeout & inactivityTimeout)
{
    throw tt3::util::NotImplementedError();
}

void User::_saveUiLocale(const tt3::db::api::UiLocale & uiLocale)
{
    throw tt3::util::NotImplementedError();
}

//////////
//  Implementation helpers
void User::_makeDead()
{
    Q_ASSERT(_database->guard.isLockedByCurrentThread());
    Q_ASSERT(_isLive);

    throw tt3::util::NotImplementedError();

    //  The rest is up to the base class
    Principal::_makeDead();
}

//  End of tt3-db-sql/User.cpp
