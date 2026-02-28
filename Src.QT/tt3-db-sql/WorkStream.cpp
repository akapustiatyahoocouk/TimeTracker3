//
//  tt3-db-sql/WorkStream.cpp - tt3::db::sql::WorkStream class implementation
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
WorkStream::WorkStream(
        Database * database,
        qint64 pk
    ) : Workload(database, pk)
{
}

WorkStream::~WorkStream()
{
}

//////////
//  Cached properties
void WorkStream::_loadCachedProperties()
{
    Q_ASSERT(_database->guard.isLockedByCurrentThread());

    std::unique_ptr<Statement> stat
    {   _database->createStatement(
            "SELECT [objects].[oid] AS [oid],"
            "       [workloads].[displayname] AS [displayname],"
            "       [workloads].[description] AS [description]"
            "  FROM [objects],[workloads]"
            " WHERE [objects].[pk] = ?"
            "   AND [workloads].[pk] = [objects].[pk]") };
    stat->setIntParameter(0, _pk);
    std::unique_ptr<ResultSet> rs
        { stat->executeQuery() };   //  may throw
    if (!rs->next())
    {   //  OOPS! User row does not exist
        _makeDead();
        throw tt3::db::api::InstanceDeadException();
    }
    //  Workloads row exists and is now "current" in "rs"
    _oid = rs->oidValue("oid");
    _displayName = rs->stringValue("displayname");
    _description = rs->stringValue("description");
}

//////////
//  Implementation helpers
bool WorkStream::_siblingExists(
        const QString & displayName
    ) const
{
    Q_ASSERT(_database->guard.isLockedByCurrentThread());
    Q_ASSERT(_isLive);

    std::unique_ptr<Statement> stat
    {   _database->createStatement(
            "SELECT [pk]"
            "  FROM [workloads]"
            " WHERE [displayname] = ?"
            "   AND [pk] <> ?"
            "   AND [completed] IS NULL") };//  WorkStream
    stat->setStringParameter(0, displayName);
    stat->setIntParameter(1, _pk);
    std::unique_ptr<ResultSet> rs
        { stat->executeQuery() };
    return rs->next();  //  row exists ?
}

//  End of tt3-db-sql/WorkStream.cpp
