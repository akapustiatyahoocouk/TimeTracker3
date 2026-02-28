//
//  tt3-db-sql/Event.cpp - tt3::db::sql::Event class implementation
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
Event::Event(
        Database * database,
        qint64 pk
    ) : Object(database, pk),
        //  Cached properties
        _occurredAt([this] { _loadCachedProperties(); }),
        _summary([this] { _loadCachedProperties(); }),
        _fkAccount([this] { _loadCachedProperties(); }),
        _fkActivities([this] { _loadCachedProperties(); })
{
}

Event::~Event()
{
}

//////////
//  tt3::db::api::IEvent (properties)
auto Event::occurredAt(
    ) const -> QDateTime
{
    tt3::util::Lock _(_database->guard);
    _ensureLive();  //  may throw

    return _occurredAt; //  Cache load may throw
}

auto Event::summary(
    ) const -> QString
{
    tt3::util::Lock _(_database->guard);
    _ensureLive();  //  may throw

    return _summary;    //  Cache load may throw
}

//////////
//  tt3::db::api::IEvent (associations)
auto Event::account(
    ) const -> tt3::db::api::IAccount *
{
    tt3::util::Lock _(_database->guard);
    _ensureLive();  //  may throw

    return _database->_getObject<Account>(_fkAccount);  //  Cache load may throw
}

auto Event::activities(
    ) const -> tt3::db::api::Activities
{
    tt3::util::Lock _(_database->guard);
    _ensureLive();  //  may throw

    tt3::db::api::Activities result;
    for (qint64 fkActivity : _fkActivities.value()) //  Cache load may throw
    {
        result.insert(_database->_getActivity(fkActivity));
    }
    return result;
}

//////////
//  Cached properties
void Event::_invalidateCachedProperties()
{
    Object::_invalidateCachedProperties();
    _occurredAt.invalidate();
    _summary.invalidate();
    _fkAccount.invalidate();
    _fkActivities.invalidate();
}

void Event::_loadCachedProperties()
{
    Q_ASSERT(_database->guard.isLockedByCurrentThread());

    std::unique_ptr<Statement> stat
    {   _database->createStatement(
            "SELECT [objects].[oid] AS [oid],"
            "       [events].[fk_account] AS [fk_account],"
            "       [events].[fk_activity1] AS [fk_activity1],"
            "       [events].[fk_activity2] AS [fk_activity2],"
            "       [events].[occurredat] AS [occurredat],"
            "       [events].[summary] AS [summary]"
            "  FROM [objects],[events]"
            " WHERE [objects].[pk] = ?"
            "   AND [events].[pk] = [objects].[pk]") };
    stat->setIntParameter(0, _pk);
    std::unique_ptr<ResultSet> rs
        { stat->executeQuery() };   //  may throw
    if (!rs->next())
    {   //  OOPS! User row does not exist
        _makeDead();
        throw tt3::db::api::InstanceDeadException();
    }
    //  Work row exists and is now "current" in "rs"
    _oid = rs->oidValue("oid");
    _occurredAt = tt3::util::fromString<QDateTime>(rs->stringValue("occurredat"));
    _summary = rs->stringValue("summary");
    _fkAccount = rs->intValue("fk_account");
    QSet<qint64> fkActivities;
    if (!rs->isNull("fk_activity1"))
    {
        fkActivities.insert(rs->intValue("fk_activity1"));
    }
    if (!rs->isNull("fk_activity2"))
    {
        fkActivities.insert(rs->intValue("fk_activity2"));
    }
    _fkActivities = fkActivities;
}

//////////
//  Implementation helpers
void Event::_deleteCascade()  //  may thro
{
    Q_ASSERT(_database->guard.isLockedByCurrentThread());
    Q_ASSERT(_isLive);
    Q_ASSERT(_database->_liveObjects.contains(_pk));

    //  Nothing here - can we make Object's
    //  _deleteCascade() non-abstract and kill this method ?
}

void Event::_removeFromDatabase()
{
    Q_ASSERT(_database->guard.isLockedByCurrentThread());
    Q_ASSERT(_isLive);
    Q_ASSERT(_database->_liveObjects.contains(_pk));

    //  Schedule notifications for associated items
    try
    {
        auto account = _database->_getObject<Account>(_fkAccount);  //  Cache load may thro
        _database->_changeNotifier.post(
            new tt3::db::api::ObjectModifiedNotification(
                _database, account->type(), account->_oid));  //  may throw
        //  TODO write notifications to the database
    }
    catch (...)
    {   //  Better a lost notification than a failed deletion
    }
    try
    {
        for (qint64 fkActivity : _fkActivities.value()) //  Cache load may throw
        {
            auto activity = _database->_getActivity(fkActivity);  //  Cache load may thro
            _database->_changeNotifier.post(
                new tt3::db::api::ObjectModifiedNotification(
                    _database, activity->type(), activity->_oid));  //  may throw
            //  TODO write notifications to the database
        }
    }
    catch (...)
    {   //  Better a lost notification than a failed deletion
    }

    //  Delete database row
    std::unique_ptr<Statement> stat
    {   _database->createStatement(
            "DELETE FROM [events]"
            " WHERE [pk] = ?") };
    stat->setIntParameter(0, _pk);
    stat->execute();    //  may throw

    //  The rest is up to the base class
    Object::_removeFromDatabase();
}

//  End of tt3-db-sql/Event.cpp
