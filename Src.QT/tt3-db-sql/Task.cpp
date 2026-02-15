//
//  tt3-db-sql/Task.cpp - tt3::db::sql::Task class implementation
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
Task::Task(Database * database, qint64 pk)
    :   Activity(database, pk),
        //  Cached properties
        _requireCommentOnCompletion([this] { _loadCachedProperties(); }),
        _completed([this] { _loadCachedProperties(); }),
        _fkParent([this] { _loadCachedProperties(); })
{
}

Task::~Task()
{
}

//////////
//  tt3::db::api::ITask (properties)
bool Task::completed() const
{
    tt3::util::Lock _(_database->guard);
    _ensureLive();  //  may throw

    return _completed;  //  Cache load may throw
}

void Task::setCompleted(
        bool completed
    )
{
    tt3::util::Lock _(_database->guard);
    _ensureLiveAndWritable();   //  may throw

    if (completed != _completed)    //  Cache load may throw
    {   //  Make the change...
        //  Begin transaction for the changes
        Transaction transaction(_database); //  may throw
        //  Save, THEN cache
        _saveCompleted(completed);    //  may throw
        _completed = completed;
        //  We're done with the changes
        transaction.commit();   //  may throw

        //  ...schedule change notifications....
        _database->_changeNotifier.post(
            new tt3::db::api::ObjectModifiedNotification(
                _database, type(), _oid));
        //  TODO post change notification to the database
        //  ...and we're done
    }
}

bool Task::requireCommentOnCompletion(
    ) const
{
    tt3::util::Lock _(_database->guard);
    _ensureLive();  //  may throw

    return _requireCommentOnCompletion; //  Cache load may throw
}

void Task::setRequireCommentOnCompletion(
        bool requireCommentOnCompletion
    )
{
    tt3::util::Lock _(_database->guard);
    _ensureLiveAndWritable();   //  may throw

    if (requireCommentOnCompletion != _requireCommentOnCompletion)  //  Cache load may throw
    {   //  Make the change...
        //  Begin transaction for the changes
        Transaction transaction(_database); //  may throw
        //  Save, THEN cache
        _saveRequireCommentOnCompletion(requireCommentOnCompletion);    //  may throw
        _requireCommentOnCompletion = requireCommentOnCompletion;
        //  We're done with the changes
        transaction.commit();   //  may throw

        //  ...schedule change notifications....
        _database->_changeNotifier.post(
            new tt3::db::api::ObjectModifiedNotification(
                _database, type(), _oid));
        //  TODO post change notification to the database
        //  ...and we're done
    }
}

//////////
//  Cached properties
void Task::_invalidateCachedProperties()
{
    Activity::_invalidateCachedProperties();
    _requireCommentOnCompletion.invalidate();
    _completed.invalidate();
    _fkParent.invalidate();
}

void Task::_saveRequireCommentOnCompletion(bool requireCommentOnCompletion)
{
    Q_ASSERT(_database->guard.isLockedByCurrentThread());

    std::unique_ptr<Statement> stat
    {   _database->createStatement(
        "UPDATE [activities]"
        "   SET [requirecommentoncompletion] = ?"
        " WHERE [pk] = ?") };
    stat->setBoolParameter(0, requireCommentOnCompletion);
    stat->setIntParameter(1, _pk);
    auto affectedRows = stat->execute();    //  may throw
    if (affectedRows == 0)
    {   //  OOPS! Row since deleted!
        _makeDead();
        throw tt3::db::api::InstanceDeadException();
    }
}

void Task::_saveCompleted(bool completed)
{
    Q_ASSERT(_database->guard.isLockedByCurrentThread());

    std::unique_ptr<Statement> stat
    {   _database->createStatement(
        "UPDATE [activities]"
        "   SET [completed] = ?"
        " WHERE [pk] = ?") };
    stat->setBoolParameter(0, completed);
    stat->setIntParameter(1, _pk);
    auto affectedRows = stat->execute();    //  may throw
    if (affectedRows == 0)
    {   //  OOPS! Row since deleted!
        _makeDead();
        throw tt3::db::api::InstanceDeadException();
    }
}

void Task::_saveFkParent(const std::optional<qint64> & fkParent)
{
    Q_ASSERT(_database->guard.isLockedByCurrentThread());

    std::unique_ptr<Statement> stat
    {   _database->createStatement(
        "UPDATE [activities]"
        "   SET [fk_parent] = ?"
        " WHERE [pk] = ?") };
    fkParent.has_value() ?
        stat->setIntParameter(0, fkParent.value()) :
        stat->setNullParameter(0);
    stat->setIntParameter(1, _pk);
    auto affectedRows = stat->execute();    //  may throw
    if (affectedRows == 0)
    {   //  OOPS! Row since deleted!
        _makeDead();
        throw tt3::db::api::InstanceDeadException();
    }
}

//  End of tt3-db-sql/User.cpp
