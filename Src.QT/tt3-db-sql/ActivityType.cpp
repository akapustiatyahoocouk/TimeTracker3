//
//  tt3-db-sql/ActivityType.cpp - tt3::db::sql::ActivityType class implementation
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
ActivityType::ActivityType(Database * database, qint64 pk)
    :   Object(database, pk),
        //  Cached propertirs
        _displayName([this] { _loadCachedProperties(); }),
        _description([this] { _loadCachedProperties(); })
{
}

ActivityType::~ActivityType()
{
}

//////////
//  tt3::db::api::IActivityType (properties)
QString ActivityType::displayName() const
{
    tt3::util::Lock _(_database->guard);
    _ensureLive();  //  may throw

    return _displayName;    //  Cache load may throw
}

void ActivityType::setDisplayName(
        const QString & displayName
    )
{
    tt3::util::Lock _(_database->guard);
    _ensureLiveAndWritable();   //  may throw

    //  Validate parameters
    if (!_database->validator()->activityType()->isValidDisplayName(displayName))
    {
        throw tt3::db::api::InvalidPropertyValueException(
            type(),
            "displayName",
            displayName);
    }

    if (displayName != _displayName)    //  Cache load may throw
    {   //  Make the change...
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

QString ActivityType::description() const
{
    tt3::util::Lock _(_database->guard);
    _ensureLive();  //  may throw

    return _description;    //  Cache load may throw
}

void ActivityType::setDescription(
        const QString & description
    )
{
    tt3::util::Lock _(_database->guard);
    _ensureLiveAndWritable();   //  may throw

    //  Validate parameters
    if (!_database->validator()->activityType()->isValidDescription(description))
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

//////////
//  tt3::db::api::IActivityType (associations)
auto ActivityType::activities(
    ) const -> tt3::db::api::Activities
{
    throw tt3::util::NotImplementedError();
}

//////////
//  Cached properties
void ActivityType::_invalidateCachedProperties()
{
    Object::_invalidateCachedProperties();
    _displayName.invalidate();
    _description.invalidate();
}

void ActivityType::_loadCachedProperties()
{
    Q_ASSERT(_database->guard.isLockedByCurrentThread());

    std::unique_ptr<Statement> stat
        {   _database->createStatement(
            "SELECT [objects].[oid] AS [oid],"
            "       [activitytypes].[displayname] AS [displayname],"
            "       [activitytypes].[description] AS [description]"
            "  FROM [objects],[activitytypes]"
            " WHERE [objects].[pk] = ?"
            "   AND [activitytypes].[pk] = [objects].[pk]") };
    stat->setIntParameter(0, _pk);
    std::unique_ptr<ResultSet> rs
        { stat->executeQuery() };   //  may throw
    if (!rs->next())
    {   //  OOPS! User row does not exist
        _makeDead();
        throw tt3::db::api::InstanceDeadException();
    }
    //  ActivityType row exists and is now "current" in "rs"
    _oid = rs->oidValue("oid");
    _displayName = rs->stringValue("displayname");
    _description = rs->stringValue("description");
}

void ActivityType::_saveDisplayName(const QString & displayName)
{
    Q_ASSERT(_database->guard.isLockedByCurrentThread());

    std::unique_ptr<Statement> stat
    {   _database->createStatement(
        "UPDATE [activitytypes]"
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

void ActivityType::_saveDescription(const QString & description)
{
    Q_ASSERT(_database->guard.isLockedByCurrentThread());

    std::unique_ptr<Statement> stat
    {   _database->createStatement(
        "UPDATE [activitytypes]"
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

//////////
//  Implementation helpers
void ActivityType::_deleteCascade()
{   //  TODO all Activitoies associated with this ActivityType
    //  must lose their associations
    //  TODO and notifications issued too
}

void ActivityType::_removeFromDatabase()
{
    Q_ASSERT(_database->guard.isLockedByCurrentThread());
    Q_ASSERT(_isLive);
    Q_ASSERT(_database->_liveObjects.contains(_pk));

    std::unique_ptr<Statement> stat
        {   _database->createStatement(
            "DELETE FROM [activitytypes]"
            " WHERE [pk] = ?") };
    stat->setIntParameter(0, _pk);
    stat->execute();    //  may throw
    Object::_removeFromDatabase();
}

//  End of tt3-db-sql/ActivityType.cpp
