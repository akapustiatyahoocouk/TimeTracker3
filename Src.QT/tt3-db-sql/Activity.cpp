//
//  tt3-db-sql/Activity.cpp - tt3::db::sql::Activity class implementation
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
Activity::Activity(
        Database * database,
        qint64 pk
    ) : Object(database, pk),
        //  Cached propertirs
        _displayName([this] { _loadCachedProperties(); }),
        _description([this] { _loadCachedProperties(); }),
        _timeout([this] { _loadCachedProperties(); }),
        _requireCommentOnStart([this] { _loadCachedProperties(); }),
        _requireCommentOnStop([this] { _loadCachedProperties(); }),
        _fullScreenReminder([this] { _loadCachedProperties(); }),
        _fkActivityType([this] { _loadCachedProperties(); })
{
}

Activity::~Activity()
{
}

//////////
//  tt3::db::api::IActivity (general)
QString Activity::displayName() const
{
    tt3::util::Lock _(_database->guard);
    _ensureLive();  //  may throw

    return _displayName;    //  Cache load may throw
}

void Activity::setDisplayName(
        const QString & displayName
    )
{
    tt3::util::Lock _(_database->guard);
    _ensureLiveAndWritable();   //  may throw

    //  Validate parameters
    if (!_database->validator()->activity()->isValidDisplayName(displayName))
    {
        throw tt3::db::api::InvalidPropertyValueException(
            type(),
            "displayName",
            displayName);
    }
    if (displayName != _displayName)    //  Cache load may throw
    {   //  Make the change (but no duplication)...
        if (_siblingExists(displayName))
        {
            throw tt3::db::api::AlreadyExistsException(
                type(),
                "displayName",
                displayName);
        }

        //  Begin transaction for the changes
        Transaction transaction(_database); //  may throw
        //  Save, THEN cache
        _saveDisplayName(displayName);    //  may throw
        _displayName = displayName;
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

QString Activity::description() const
{
    tt3::util::Lock _(_database->guard);
    _ensureLive();  //  may throw

    return _description;    //  Cache load may throw
}

void Activity::setDescription(
        const QString & description
    )
{
    tt3::util::Lock _(_database->guard);
    _ensureLiveAndWritable();   //  may throw

    //  Validate parameters
    if (!_database->validator()->activity()->isValidDescription(description))
    {
        throw tt3::db::api::InvalidPropertyValueException(
            type(),
            "description",
            description);
    }
    if (description != _description)    //  Cache load may throw
    {   //  Make the change...
        //  Begin transaction for the changes
        Transaction transaction(_database); //  may throw
        //  Save, THEN cache
        _saveDescription(description);    //  may throw
        _description = description;
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

auto Activity::timeout(
    ) const -> tt3::db::api::InactivityTimeout
{
    tt3::util::Lock _(_database->guard);
    _ensureLive();  //  may throw

    return _timeout;    //  Cache load may throw
}

void Activity::setTimeout(
        const tt3::db::api::InactivityTimeout & timeout
    )
{
    tt3::util::Lock _(_database->guard);
    _ensureLiveAndWritable();   //  may throw

    //  Validate parameters
    if (timeout.has_value() &&
        !_database->validator()->activity()->isValidTimeout(timeout.value()))
    {
        throw tt3::db::api::InvalidPropertyValueException(
            type(),
            "timeout",
            timeout.value());
    }
    if (timeout != _timeout.value())    //  Cache load may throw
    {   //  Make the change...
        //  Begin transaction for the changes
        Transaction transaction(_database); //  may throw
        //  Save, THEN cache
        _saveTimeout(timeout);    //  may throw
        _timeout = timeout;
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

bool Activity::requireCommentOnStart(
    ) const
{
    tt3::util::Lock _(_database->guard);
    _ensureLive();  //  may throw

    return _requireCommentOnStart;  //  Cache load may throw
}

void Activity::setRequireCommentOnStart(
        bool requireCommentOnStart
    )
{
    tt3::util::Lock _(_database->guard);
    _ensureLiveAndWritable();   //  may throw

    if (requireCommentOnStart != _requireCommentOnStart)    //  Cache load may throw
    {   //  Make the change...
        //  Begin transaction for the changes
        Transaction transaction(_database); //  may throw
        //  Save, THEN cache
        _saveRequireCommentOnStart(requireCommentOnStart);    //  may throw
        _requireCommentOnStart = requireCommentOnStart;
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

bool Activity::requireCommentOnStop(
    ) const
{
    tt3::util::Lock _(_database->guard);
    _ensureLive();  //  may throw

    return _requireCommentOnStop;   //  Cache load may throw
}

void Activity::setRequireCommentOnStop(
        bool requireCommentOnStop
    )
{
    tt3::util::Lock _(_database->guard);
    _ensureLiveAndWritable();   //  may throw

    if (requireCommentOnStop != _requireCommentOnStop)  //  Cache load may throw
    {   //  Make the change...
        //  Begin transaction for the changes
        Transaction transaction(_database); //  may throw
        //  Save, THEN cache
        _saveRequireCommentOnStop(requireCommentOnStop);    //  may throw
        _requireCommentOnStop = requireCommentOnStop;
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

bool Activity::fullScreenReminder(
    ) const
{
    tt3::util::Lock _(_database->guard);
    _ensureLive();  //  may throw

    return _fullScreenReminder; //  Cache load may throw
}

void Activity::setFullScreenReminder(
        bool fullScreenReminder
    )
{
    tt3::util::Lock _(_database->guard);
    _ensureLiveAndWritable();   //  may throw

    if (fullScreenReminder != _fullScreenReminder)  //  Cache load may throw
    {   //  Make the change...
        //  Begin transaction for the changes
        Transaction transaction(_database); //  may throw
        //  Save, THEN cache
        _saveFullScreenReminder(fullScreenReminder);    //  may throw
        _fullScreenReminder = fullScreenReminder;
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
//  tt3::db::api::IActivity (associations)
auto Activity::activityType(
    ) const -> tt3::db::api::IActivityType *
{
    tt3::util::Lock _(_database->guard);
    _ensureLive();  //  may throw

    std::optional<qint64> fkActivityType = _fkActivityType; //  Cache load may throw
    return fkActivityType.has_value() ?
                _database->_getObject<ActivityType>(fkActivityType.value()) :
               nullptr;
}

void Activity::setActivityType(
        tt3::db::api::IActivityType * activityType
    )
{
    tt3::util::Lock _(_database->guard);
    _ensureLiveAndWritable();   //  may throw

    ActivityType * sqlActivityType = nullptr;
    if (activityType != nullptr)
    {
        sqlActivityType = dynamic_cast<ActivityType*>(activityType);
        if (sqlActivityType == nullptr ||
            !sqlActivityType->_isLive ||
            sqlActivityType->_database != this->_database)
        {   //  OOPS!
            throw tt3::db::api::IncompatibleInstanceException(activityType->type());
        }
    }

    std::optional<qint64> fkActivityType;
    if (sqlActivityType != nullptr)
    {
        fkActivityType = sqlActivityType->_pk;
    }
    if (fkActivityType != _fkActivityType.value())  //  Cache load may throw
    {   //  Make the change...
        //  Begin transaction for the changes
        Transaction transaction(_database); //  may throw
        //  Save, THEN cache
        _saveFkActivityType(fkActivityType);    //  may throw
        _fkActivityType = fkActivityType;
        //  We're done with the changes
        transaction.commit();   //  may throw

        //  ...schedule change notifications....
        _database->_changeNotifier.post(
            new tt3::db::api::ObjectModifiedNotification(
                _database, type(), _oid));
        //  TODO post change notification to the database
        //  TODO "modified" changes for old & new ActivityTypes
        //  ...and we're done
    }
}

auto Activity::workload(
    ) const -> tt3::db::api::IWorkload *
{
    tt3::util::Lock _(_database->guard);
    _ensureLive();  //  may throw
    //  We assume database is consistent since last change

    return nullptr; //  TODO implement
}

void Activity::setWorkload(
        tt3::db::api::IWorkload * /*workload*/
    )
{
    throw tt3::util::NotImplementedError();
}

auto Activity::works(
    ) const -> tt3::db::api::Works
{
    tt3::util::Lock _(_database->guard);
    _ensureLive();  //  may throw
    //  We assume database is consistent since last change

    return tt3::db::api::Works();   //  TODO implement
}

auto Activity::events(
    ) const -> tt3::db::api::Events
{
    tt3::util::Lock _(_database->guard);
    _ensureLive();  //  may throw
    //  We assume database is consistent since last change

    return tt3::db::api::Events();  //  TODO implement
}

//////////
//  Cached properties
void Activity::_invalidateCachedProperties()
{
    Object::_invalidateCachedProperties();
    _displayName.invalidate();
    _description.invalidate();
    _timeout.invalidate();
    _requireCommentOnStart.invalidate();
    _requireCommentOnStop.invalidate();
    _fullScreenReminder.invalidate();
    _fkActivityType.invalidate();
    //  TODO other associations
}

void Activity::_saveDisplayName(const QString & displayName)
{
    Q_ASSERT(_database->guard.isLockedByCurrentThread());

    std::unique_ptr<Statement> stat
    {   _database->createStatement(
        "UPDATE [activities]"
        "   SET [displayname] = ?"
        " WHERE [pk] = ?") };
    stat->setStringParameter(0, displayName);
    stat->setIntParameter(1, _pk);
    auto affectedRows = stat->execute();    //  may throw
    if (affectedRows == 0)
    {   //  OOPS! Row since deleted!
        _makeDead();
        throw tt3::db::api::InstanceDeadException();
    }
}

void Activity::_saveDescription(const QString & description)
{
    Q_ASSERT(_database->guard.isLockedByCurrentThread());

    std::unique_ptr<Statement> stat
    {   _database->createStatement(
        "UPDATE [activities]"
        "   SET [description] = ?"
        " WHERE [pk] = ?") };
    stat->setStringParameter(0, description);
    stat->setIntParameter(1, _pk);
    auto affectedRows = stat->execute();    //  may throw
    if (affectedRows == 0)
    {   //  OOPS! Row since deleted!
        _makeDead();
        throw tt3::db::api::InstanceDeadException();
    }
}

void Activity::_saveTimeout(const tt3::db::api::InactivityTimeout & timeout)
{
    Q_ASSERT(_database->guard.isLockedByCurrentThread());

    std::unique_ptr<Statement> stat
    {   _database->createStatement(
        "UPDATE [activities]"
        "   SET [timeout] = ?"
        " WHERE [pk] = ?") };
    timeout.has_value() ?   //  TODO use the same trick where SQL parameters are NULLable
        stat->setTimeSpanParameter(0, timeout.value()) :
        stat->setNullParameter(0);
    stat->setIntParameter(1, _pk);
    auto affectedRows = stat->execute();    //  may throw
    if (affectedRows == 0)
    {   //  OOPS! Row since deleted!
        _makeDead();
        throw tt3::db::api::InstanceDeadException();
    }
}

void Activity::_saveRequireCommentOnStart(bool requireCommentOnStart)
{
    Q_ASSERT(_database->guard.isLockedByCurrentThread());

    std::unique_ptr<Statement> stat
    {   _database->createStatement(
        "UPDATE [activities]"
        "   SET [requirecommentonstart] = ?"
        " WHERE [pk] = ?") };
    stat->setBoolParameter(0, requireCommentOnStart);
    stat->setIntParameter(1, _pk);
    auto affectedRows = stat->execute();    //  may throw
    if (affectedRows == 0)
    {   //  OOPS! Row since deleted!
        _makeDead();
        throw tt3::db::api::InstanceDeadException();
    }
}

void Activity::_saveRequireCommentOnStop(bool requireCommentOnStop)
{
    Q_ASSERT(_database->guard.isLockedByCurrentThread());

    std::unique_ptr<Statement> stat
    {   _database->createStatement(
        "UPDATE [activities]"
        "   SET [requirecommentonstop] = ?"
        " WHERE [pk] = ?") };
    stat->setBoolParameter(0, requireCommentOnStop);
    stat->setIntParameter(1, _pk);
    auto affectedRows = stat->execute();    //  may throw
    if (affectedRows == 0)
    {   //  OOPS! Row since deleted!
        _makeDead();
        throw tt3::db::api::InstanceDeadException();
    }
}

void Activity::_saveFullScreenReminder(bool fullScreenReminder)
{
    Q_ASSERT(_database->guard.isLockedByCurrentThread());

    std::unique_ptr<Statement> stat
    {   _database->createStatement(
        "UPDATE [activities]"
        "   SET [fullscreenreminder] = ?"
        " WHERE [pk] = ?") };
    stat->setBoolParameter(0, fullScreenReminder);
    stat->setIntParameter(1, _pk);
    auto affectedRows = stat->execute();    //  may throw
    if (affectedRows == 0)
    {   //  OOPS! Row since deleted!
        _makeDead();
        throw tt3::db::api::InstanceDeadException();
    }
}

void Activity::_saveFkActivityType(const std::optional<qint64> & fkActivityType)
{
    Q_ASSERT(_database->guard.isLockedByCurrentThread());

    std::unique_ptr<Statement> stat
    {   _database->createStatement(
        "UPDATE [activities]"
        "   SET [fk_type] = ?"
        " WHERE [pk] = ?") };
    if (fkActivityType.has_value())
    {
        stat->setBoolParameter(0, fkActivityType.value());
    }
    else
    {
        stat->setNullParameter(0);
    }
    stat->setIntParameter(1, _pk);
    auto affectedRows = stat->execute();    //  may throw
    if (affectedRows == 0)
    {   //  OOPS! Row since deleted!
        _makeDead();
        throw tt3::db::api::InstanceDeadException();
    }
}

//////////
//  Implementatuion helpers
void Activity::_deleteCascade()
{
    Q_ASSERT(_database->guard.isLockedByCurrentThread());
    Q_ASSERT(_isLive);
    Q_ASSERT(_database->_liveObjects.contains(_pk));

    //  TODO works and events
    //  TODO remove from quick picks lists
    //  TODO etc.
    throw tt3::util::NotImplementedError();
}

void Activity::_removeFromDatabase()
{
    Q_ASSERT(_database->guard.isLockedByCurrentThread());
    Q_ASSERT(_isLive);
    Q_ASSERT(_database->_liveObjects.contains(_pk));

    std::unique_ptr<Statement> stat
        {   _database->createStatement(
            "DELETE FROM [activities]"
            " WHERE [pk] = ?") };
    stat->setIntParameter(0, _pk);
    stat->execute();    //  may throw
    Object::_removeFromDatabase();
}

//  End of tt3-db-sql/Activity.cpp
