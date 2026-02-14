//
//  tt3-db-sql/PublicTask.cpp - tt3::db::sql::PublicTask class implementation
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
PublicTask::PublicTask(
        Database * database,
        qint64 pk
    ) : Activity(database, pk),
        PublicActivity(database, pk),
        Task(database, pk)
{
}

PublicTask::~PublicTask()
{
}

//////////
//  tt3::db::api::IPublicTask (associations)
auto PublicTask::parent(
    ) const -> IPublicTask *
{
    tt3::util::Lock _(_database->guard);
    _ensureLive();  //  may throw

    return _fkParent.value().has_value() ?  //  Cache load may throw
                _database->_getObject<PublicTask>(_fkParent.value().value()) :  //  Cache load may throw
                nullptr;
}

void PublicTask::setParent(
        IPublicTask * /*parent*/
    )
{
    tt3::util::Lock _(_database->guard);
    _ensureLiveAndWritable();   //  may throw

    throw tt3::util::NotImplementedError();
}

auto PublicTask::children(
    ) const -> tt3::db::api::PublicTasks
{
    tt3::util::Lock _(_database->guard);
    _ensureLive();  //  may throw

    //  TODO cache PKs
    std::unique_ptr<Statement> stat
        {   _database->createStatement(
            "SELECT [pk]"
            "  FROM [activities]"
            " WHERE [fk_parent] = ?") };
    stat->setIntParameter(0, _pk);
    std::unique_ptr<ResultSet> rs
        { stat->executeQuery() };   //  may throw
    tt3::db::api::PublicTasks result;
    while (rs->next())
    {
        result.insert(_database->_getObject<PublicTask>(rs->intValue(0)));
    }
    return result;
}

//////////
//  tt3::db::api::IPublicTask (life cycle)
auto PublicTask::createChild(
        const QString & displayName,
        const QString & description,
        const tt3::db::api::InactivityTimeout & timeout,
        bool /*requireCommentOnStart*/,
        bool /*requireCommentOnStop*/,
        bool /*fullScreenReminder*/,
        tt3::db::api::IActivityType * activityType,
        tt3::db::api::IWorkload * /*workload*/,
        bool /*completed*/,
        bool /*requireCommentOnCompletion*/
    ) -> tt3::db::api::IPublicTask *
{
    tt3::util::Lock _(_database->guard);
    _ensureLiveAndWritable(); //  may throw

    //  Validate parameters
    if (!_database->validator()->publicTask()->isValidDisplayName(displayName))
    {
        throw tt3::db::api::InvalidPropertyValueException(
            tt3::db::api::ObjectTypes::PublicTask::instance(),
            "displayName",
            displayName);
    }
    if (!_database->validator()->publicTask()->isValidDescription(description))
    {
        throw tt3::db::api::InvalidPropertyValueException(
            tt3::db::api::ObjectTypes::PublicTask::instance(),
            "description",
            description);
    }
    if (timeout.has_value() &&
        !_database->validator()->publicTask()->isValidTimeout(timeout))
    {
        throw tt3::db::api::InvalidPropertyValueException(
            tt3::db::api::ObjectTypes::PublicTask::instance(),
            "timeout",
            timeout.value());
    }
    ActivityType * sqlActivityType = nullptr;
    if (activityType != nullptr)
    {
        sqlActivityType = dynamic_cast<ActivityType*>(activityType);
        if (sqlActivityType == nullptr ||
            sqlActivityType->_database != _database ||
            !sqlActivityType->_isLive)
        {   //  OOPS!
            throw tt3::db::api::IncompatibleInstanceException(activityType->type());
        }
    }
    /*  TODO
    Workload * xmlWorkload = nullptr;
    if (workload != nullptr)
    {
        xmlWorkload = dynamic_cast<Workload*>(workload);
        if (xmlWorkload == nullptr ||
            xmlWorkload->_database != _database ||
            !xmlWorkload->_isLive)
        {   //  OOPS!
            throw tt3::db::api::IncompatibleInstanceException(workload->type());
        }
    }
    */

    //  Display names must be unique
    if (_findChild(displayName) != nullptr)
    {   //  OOPS!
        throw tt3::db::api::AlreadyExistsException(
            tt3::db::api::ObjectTypes::PublicTask::instance(),
            "displayName",
            displayName);
    }

    throw tt3::util::NotImplementedError();
}

//////////
//  Cached properties
void PublicTask::_invalidateCachedProperties()
{
    PublicActivity::_invalidateCachedProperties();
    Task::_invalidateCachedProperties();
}

void PublicTask::_loadCachedProperties()
{   //  TODO can we lift this to Task and remove from PrivateTask's version?
    Q_ASSERT(_database->guard.isLockedByCurrentThread());

    std::unique_ptr<Statement> stat
        {   _database->createStatement(
            "SELECT [objects].[oid] AS [oid],"
            "       [objects].[type] AS [type],"
            "       [activities].[fk_parent] AS [fk_parent],"
            "       [activities].[fk_owner] AS [fk_owner],"
            "       [activities].[fk_type] AS [fk_type],"
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
    _requireCommentOnCompletion = rs->boolValue("requirecommentoncompletion");
    _completed = rs->boolValue("completed");
    _fkParent =
        rs->isNull("fk_parent") ?
            std::optional<qint64>() :
            rs->intValue("fk_parent");
}

//////////
//  Implementation helpers
void PublicTask::_deleteCascade()
{
    Q_ASSERT(_database->guard.isLockedByCurrentThread());
    Q_ASSERT(_isLive);
    Q_ASSERT(_database->_liveObjects.contains(_pk));

    //  TODO child tasks

    //  There's no _deleteCascade() at PublicActivity or Task level
    Activity::_deleteCascade();
    throw tt3::util::NotImplementedError();
}

bool PublicTask::_siblingExists(
        const QString & /*displayName*/
    ) const
{
    Q_ASSERT(_database->guard.isLockedByCurrentThread());
    Q_ASSERT(_isLive);

    throw tt3::util::NotImplementedError();
}

PublicTask * PublicTask::_findChild(
        const QString & /*displayName*/
    ) const
{
    Q_ASSERT(_database->guard.isLockedByCurrentThread());

    throw tt3::util::NotImplementedError();
}

//  End of tt3-db-sql/PublicTask.cpp
