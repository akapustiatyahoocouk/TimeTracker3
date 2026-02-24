//
//  tt3-db-sql/PrivateActivity.cpp - tt3::db::sql::PrivateActivity class implementation
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
PrivateActivity::PrivateActivity(
        Database * database,
        qint64 pk
    ) : Activity(database, pk),
        //  Cached propertirs
        _fkOwner([this] { _loadCachedProperties(); })

{
}

PrivateActivity::~PrivateActivity()
{
}

//////////
//  tt3::db::api::IPrivateActivity (associations)
auto PrivateActivity::owner(
    ) const -> tt3::db::api::IUser *
{
    tt3::util::Lock _(_database->guard);
    _ensureLive();  //  may throw

    return _database->_getObject<User>(_fkOwner);   //  Cache load may throw
}

//////////
//  Cached properties
void PrivateActivity::_invalidateCachedProperties()
{
    Activity::_invalidateCachedProperties();
    _fkOwner.invalidate();
}

void PrivateActivity::_loadCachedProperties()
{
    Q_ASSERT(_database->guard.isLockedByCurrentThread());

    std::unique_ptr<Statement> stat
        {   _database->createStatement(
            "SELECT [objects].[oid] AS [oid],"
            "       [objects].[type] AS [type],"
            "       [activities].[fk_parent] AS [fk_parent],"
            "       [activities].[fk_owner] AS [fk_owner],"
            "       [activities].[fk_type] AS [fk_type],"
            "       [activities].[fk_workload] AS [fk_workload],"
            "       [activities].[displayname] AS [displayname],"
            "       [activities].[description] AS [description],"
            "       [activities].[timeout] AS [timeout],"
            "       [activities].[requirecommentonstart] AS [requirecommentonstart],"
            "       [activities].[requirecommentonstop] AS [requirecommentonstop],"
            "       [activities].[fullscreenreminder] AS [fullscreenreminder],"
            "       [activities].[completed] AS [completed],"
            "       [activities].[requirecommentoncompletion] AS [requirecommentoncompletion]"
            "  FROM [objects],[activities]"
            " WHERE [objects].[pk] = ?"
            "   AND [activities].[pk] = [objects].[pk]") };
    stat->setIntParameter(0, _pk);
    std::unique_ptr<ResultSet> rs
        { stat->executeQuery() };   //  may throw
    if (!rs->next())
    {   //  OOPS! User row does not exist
        _makeDead();
        throw tt3::db::api::InstanceDeadException();
    }
    //  Activity row exists and is now "current" in "rs"
    _oid = rs->oidValue("oid");
    _displayName = rs->stringValue("displayname");
    _description = rs->stringValue("description");
    _timeout =  //  TODO use the same pattern for all optionals
        rs->isNull("timeout") ?
            tt3::db::api::InactivityTimeout() :
            rs->timeSpanValue("timeout");
    _requireCommentOnStart = rs->boolValue("requirecommentonstart");
    _requireCommentOnStop = rs->boolValue("requirecommentonstop");
    _fullScreenReminder = rs->boolValue("fullscreenreminder");
    _fkActivityType =
        rs->isNull("fk_type") ?
            std::optional<qint64>() :
            rs->intValue("fk_type");
    _fkWorkload =
        rs->isNull("fk_workload") ?
            std::optional<qint64>() :
            rs->intValue("fk_workload");
    _fkOwner = rs->intValue("fk_owner");
}

//////////
//  Implementation helpers
bool PrivateActivity::_siblingExists(const QString & displayName) const
{
    Q_ASSERT(_database->guard.isLockedByCurrentThread());
    Q_ASSERT(_isLive);
    Q_ASSERT(_database->_liveObjects.contains(_pk));

    std::unique_ptr<Statement> stat
    {   _database->createStatement(
        "SELECT [pk]"
        "  FROM [activities]"
        " WHERE [displayname] = ?"
        "   AND [pk] <> ?"
        "   AND [fk_owner] = ?"         //  Private to this User
        "   AND [completed] IS NULL") };//  Activity
    stat->setStringParameter(0, displayName);
    stat->setIntParameter(1, _pk);
    stat->setIntParameter(2, _fkOwner); //  Cache load may throw
    std::unique_ptr<ResultSet> rs
        { stat->executeQuery() };
    return rs->next();  //  row exists ?
}

//  End of tt3-db-sql/PrivateActivity.cpp
