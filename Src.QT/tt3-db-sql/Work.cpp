//
//  tt3-db-sql/Work.cpp - tt3::db::sql::Work class implementation
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
Work::Work(
        Database * database,
        qint64 pk
    ) : Object(database, pk),
        //  Cached roperties
        _startedAt([this] { _loadCachedProperties(); }),
        _finishedAt([this] { _loadCachedProperties(); }),
        _fkAccount([this] { _loadCachedProperties(); }),
        _fkActivity([this] { _loadCachedProperties(); })
{
}

Work::~Work()
{
}

//////////
//  tt3::db::api::IWork (properties)
auto Work::startedAt(
    ) const -> QDateTime
{
    tt3::util::Lock _(_database->guard);
    _ensureLive();  //  may throw

    return _startedAt;  //  Cache load may throw
}

auto Work::finishedAt(
    ) const -> QDateTime
{
    tt3::util::Lock _(_database->guard);
    _ensureLive();  //  may throw

    return _finishedAt; //  Cache load may throw
}

//////////
//  tt3::db::api::IWork (associations)
auto Work::account(
    ) const -> tt3::db::api::IAccount *
{
    tt3::util::Lock _(_database->guard);
    _ensureLive();  //  may throw

    return _database->_getObject<Account>(_fkAccount);  //  Cache load may throw
}

auto Work::activity(
    ) const -> tt3::db::api::IActivity *
{
    tt3::util::Lock _(_database->guard);
    _ensureLive();  //  may throw

    return _database->_getActivity(_fkActivity);    //  Cache load may throw
}

//////////
//  Cached roperties
void Work::_invalidateCachedProperties()
{
    Object::_invalidateCachedProperties();
    _startedAt.invalidate();
    _finishedAt.invalidate();
    _fkAccount.invalidate();
    _fkActivity.invalidate();
}

void Work::_loadCachedProperties()
{
    Q_ASSERT(_database->guard.isLockedByCurrentThread());

    std::unique_ptr<Statement> stat
    {   _database->createStatement(
        "SELECT [objects].[oid] AS [oid],"
        "       [works].[fk_account] AS [fk_account],"
        "       [works].[fk_activity] AS [fk_activity],"
        "       [works].[startedat] AS [startedat],"
        "       [works].[finishedat] AS [finishedat]"
        "  FROM [objects],[works]"
        " WHERE [objects].[pk] = ?"
        "   AND [works].[pk] = [objects].[pk]") };
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
    _startedAt = tt3::util::fromString<QDateTime>(rs->stringValue("startedat"));
    _finishedAt = tt3::util::fromString<QDateTime>(rs->stringValue("finishedat"));
    _fkAccount = rs->intValue("fk_account");
    _fkActivity = rs->intValue("fk_activity");
}

//////////
//  Implementation helpers
void Work::_deleteCascade()  //  may thro
{
    Q_ASSERT(_database->guard.isLockedByCurrentThread());
    Q_ASSERT(_isLive);
    Q_ASSERT(_database->_liveObjects.contains(_pk));

    //  Nothing here - can we make Object's
    //  _deleteCascade() non-abstract and kill this method ?
}

void Work::_removeFromDatabase()
{
    Q_ASSERT(_database->guard.isLockedByCurrentThread());
    Q_ASSERT(_isLive);
    Q_ASSERT(_database->_liveObjects.contains(_pk));

    //  TODO and notifications issued

    std::unique_ptr<Statement> stat
    {   _database->createStatement(
        "DELETE FROM [works]"
        " WHERE [pk] = ?") };
    stat->setIntParameter(0, _pk);
    stat->execute();    //  may throw
    Object::_removeFromDatabase();
}

//  End of tt3-db-sql/Work.cpp
