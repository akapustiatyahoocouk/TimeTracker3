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
        IPublicTask * parent
    )
{
    tt3::util::Lock _(_database->guard);
    _ensureLiveAndWritable();   //  may throw

    PublicTask * sqlParent = nullptr;
    if (parent != nullptr)
    {
        sqlParent = dynamic_cast<PublicTask*>(parent);
        if (sqlParent == nullptr ||
            !sqlParent->_isLive ||
            sqlParent->_database != this->_database)
        {   //  OOPS!
            throw tt3::db::api::IncompatibleInstanceException(parent->type());
        }
    }
    std::optional<qint64> fkParent =
        (sqlParent != nullptr) ?
            sqlParent->_pk :
            std::optional<qint64>();
    if (fkParent != _fkParent.value())  //  Cache load may throw
    {   //  Make the change
        //  Begin transaction for the changes
        Transaction transaction(_database); //  may throw
        //  ...ensuring we're not creating a parent/child loop...
        if (sqlParent != nullptr)
        {
            PublicTasks parentClosure;
            sqlParent->_collectParentClosure(parentClosure);
            if (parentClosure.contains(this))
            {   //  OOPS!
                throw tt3::db::api::IncompatibleInstanceException(sqlParent->type());
            }
        }
        //  Save, THEN cache
        _saveFkParent(fkParent);
        _fkParent = fkParent;
        //  We're done with the changes
        transaction.commit();   //  may throw

        //  ...schedule change notifications....
        _database->_changeNotifier.post(
            new tt3::db::api::ObjectModifiedNotification(
                _database, type(), _oid));
        //  TODO post change notification to the database
        //  TODO "modified" changes for old & new parent PublicTask
        //  ...and we're done
    }
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
        bool requireCommentOnStart,
        bool requireCommentOnStop,
        bool fullScreenReminder,
        tt3::db::api::IActivityType * activityType,
        tt3::db::api::IWorkload * workload,
        bool completed,
        bool requireCommentOnCompletion
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

    Workload * sqlWorkload = nullptr;
    if (workload != nullptr)
    {
        sqlWorkload = dynamic_cast<Workload*>(workload);
        if (sqlWorkload == nullptr ||
            sqlWorkload->_database != _database ||
            !sqlWorkload->_isLive)
        {   //  OOPS!
            throw tt3::db::api::IncompatibleInstanceException(workload->type());
        }
    }

    //  Display names must be unique
    //  SQL "UNIQUE displayname" constraint would take care of
    //  that, but try for a better (non-SQL) error message
    if (_findChild(displayName) != nullptr)
    {   //  OOPS!
        throw tt3::db::api::AlreadyExistsException(
            tt3::db::api::ObjectTypes::PublicTask::instance(),
            "displayName",
            displayName);
    }

    //  Begin transaction for the changes
    Transaction transaction(_database);  //  may throw

    //  Do the work - create [objects] row..
    Database::_ObjIds objIds = _database->_createObject(tt3::db::api::ObjectTypes::PublicTask::instance()); //  may throw
    //  ...then [activities] row...
    std::unique_ptr<Statement> stat
    {   _database->createStatement(
            "INSERT INTO [activities]"
            "       ([pk],"
            "        [fk_parent],[fk_owner],[fk_type],[fk_workload],"
            "        [displayname],[description],[timeout],"
            "        [requirecommentonstart],"
            "        [requirecommentonstop],"
            "        [fullscreenreminder],"
            "        [completed],[requirecommentoncompletion])"
            "       VALUES(?,?,?,?,?,?,?,?,?,?,?,?,?)") };
    stat->setIntParameter(0, std::get<0>(objIds));
    stat->setIntParameter(1, _pk);
    stat->setNullParameter(2);
    (sqlActivityType != nullptr) ?
        stat->setIntParameter(3, sqlActivityType->_pk) :
        stat->setNullParameter(3);
    (sqlWorkload != nullptr) ?
        stat->setIntParameter(4, sqlWorkload->_pk) :
        stat->setNullParameter(4);
    stat->setStringParameter(5, displayName);
    description.isEmpty() ?
        stat->setNullParameter(6) :
        stat->setStringParameter(6, description);
    timeout.has_value() ?
        stat->setTimeSpanParameter(7, timeout.value()) :
        stat->setNullParameter(7);
    stat->setBoolParameter(8, requireCommentOnStart);
    stat->setBoolParameter(9, requireCommentOnStop);
    stat->setBoolParameter(10, fullScreenReminder);
    stat->setBoolParameter(11, completed);
    stat->setBoolParameter(12, requireCommentOnCompletion);
    stat->execute();    //  may throw

    //  We're done with the changes
    transaction.commit();   //  may throw

    //  Create & register the PublicTask object...
    PublicTask * publicTask = new PublicTask(_database, std::get<0>(objIds));
    //  ...setting its cached properties to initial values
    publicTask->_oid = std::get<1>(objIds);
    publicTask->_displayName = displayName;
    publicTask->_description = description;
    publicTask->_timeout = timeout;
    publicTask->_requireCommentOnStart = requireCommentOnStart;
    publicTask->_requireCommentOnStop = requireCommentOnStop;
    publicTask->_fullScreenReminder = fullScreenReminder;
    publicTask->_fkActivityType =
        (sqlActivityType != nullptr) ?
            sqlActivityType->_pk :
            std::optional<qint64>();
    publicTask->_fkWorkload =
        (sqlWorkload != nullptr) ?
            sqlWorkload->_pk :
            std::optional<qint64>();
    publicTask->_requireCommentOnCompletion = requireCommentOnCompletion;
    publicTask->_completed = completed;
    publicTask->_fkParent = _pk;

    //  ...schedule change notifications...
    _database->_changeNotifier.post(
        new tt3::db::api::ObjectModifiedNotification(
            _database, this->type(), _oid));    //  Cache load may throw
    _database->_changeNotifier.post(
        new tt3::db::api::ObjectCreatedNotification(
            _database, publicTask->type(), publicTask->_oid));
    if (sqlActivityType != nullptr)
    {
        _database->_changeNotifier.post(
            new tt3::db::api::ObjectModifiedNotification(
                _database, sqlActivityType->type(), sqlActivityType->_oid)); //  Cache load may throw
    }
    if (sqlWorkload != nullptr)
    {
        _database->_changeNotifier.post(
            new tt3::db::api::ObjectModifiedNotification(
                _database, sqlWorkload->type(), sqlWorkload->_oid)); //  Cache load may throw
    }
    //  TODO post change notification to the database

    //  ...and we're done
    return publicTask;
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

    //  Child tasks - can't rely on SQL DELETE CASCADE
    //  PLUS we need delete notifications for children
    for (auto child : children())
    {
        child->destroy();
    }

    //  There's no _deleteCascade() at PublicActivity or Task level
    Activity::_deleteCascade();
}

bool PublicTask::_siblingExists(
        const QString & displayName
    ) const
{
    Q_ASSERT(_database->guard.isLockedByCurrentThread());
    Q_ASSERT(_isLive);
    Q_ASSERT(_database->_liveObjects.contains(_pk));

    if (_fkParent.value().has_value())  //  Cache load may throw
    {   //  We're looking for a child public task
        std::unique_ptr<Statement> stat
        {   _database->createStatement(
                "SELECT [pk]"
                "  FROM [activities]"
                " WHERE [displayname] = ?"
                "   AND [pk] <> ?"
                "   AND [fk_owner] IS NULL"         //  Public
                "   AND [completed] IS NOT NULL"    //  Task
                "   AND [fk_parent] = ?") };        //  with the same parent
        stat->setStringParameter(0, displayName);
        stat->setIntParameter(1, _pk);
        stat->setIntParameter(2, _fkParent.value().value());    //  Cache load may throw
        std::unique_ptr<ResultSet> rs
            { stat->executeQuery() };
        return rs->next();  //  row exists ?
    }
    else
    {   //  We're looking for a root public task
        std::unique_ptr<Statement> stat
        {   _database->createStatement(
                "SELECT [pk]"
                "  FROM [activities]"
                " WHERE [displayname] = ?"
                "   AND [pk] <> ?"
                "   AND [fk_owner] IS NULL"         //  Public
                "   AND [completed] IS NOT NULL"    //  Task
                "   AND [fk_parent] IS NULL") };    //  Root
        stat->setStringParameter(0, displayName);
        stat->setIntParameter(1, _pk);
        std::unique_ptr<ResultSet> rs
            { stat->executeQuery() };
        return rs->next();  //  row exists ?
    }
}

PublicTask * PublicTask::_findChild(
        const QString & displayName
    ) const
{
    Q_ASSERT(_database->guard.isLockedByCurrentThread());
    Q_ASSERT(_isLive);
    Q_ASSERT(_database->_liveObjects.contains(_pk));

    std::unique_ptr<Statement> stat
    {   _database->createStatement(
            "SELECT [pk]"
            "  FROM [activities]"
            " WHERE [displayname] = ?"
            "   AND [fk_owner] IS NULL"         //  Public
            "   AND [completed] IS NOT NULL"    //  Task
            "   AND [fk_parent] = ?") };        //  with this as parent
    stat->setStringParameter(0, displayName);
    stat->setIntParameter(1, _pk);
    std::unique_ptr<ResultSet> rs
        { stat->executeQuery() };
    if (rs->next())
    {   //  Row exists
        return _database->_getObject<PublicTask>(rs->intValue(0));
    }
    return nullptr;
}

void PublicTask::_collectParentClosure(PublicTasks & closure)
{
    Q_ASSERT(_database->guard.isLockedByCurrentThread());
    Q_ASSERT(_isLive);
    Q_ASSERT(_database->_liveObjects.contains(_pk));

    closure.insert(this);
    PublicTask * parentTask =
        _fkParent.value().has_value() ?  //  Cache load may throw
            _database->_getObject<PublicTask>(_fkParent.value().value()) :  //  Cache load may throw
            nullptr;
    if (parentTask != nullptr && !closure.contains(parentTask))
    {
        parentTask->_collectParentClosure(closure);
    }
}

//  End of tt3-db-sql/PublicTask.cpp
