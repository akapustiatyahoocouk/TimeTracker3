//
//  tt3-db-sql/Workload.cpp - tt3::db::sql::Workload class implementation
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
Workload::Workload(
        Database * database,
        qint64 pk
    ) : Object(database, pk),
        //  Cached propertirs
        _displayName([this] { _loadCachedProperties(); }),
        _description([this] { _loadCachedProperties(); })
{
}

Workload::~Workload()
{
}

//////////
//  tt3::db::api::IWorkload (properties)
QString Workload::displayName() const
{
    tt3::util::Lock _(_database->guard);
    _ensureLive();  //  may throw

    return _displayName;    //  Cache load may throw
}

void Workload::setDisplayName(
        const QString & displayName
    )
{
    tt3::util::Lock _(_database->guard);
    _ensureLiveAndWritable();   //  may throw

    //  Validate parameters
    if (!_database->validator()->workload()->isValidDisplayName(displayName))
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

QString Workload::description() const
{
    tt3::util::Lock _(_database->guard);
    _ensureLive();  //  may throw

    return _description;    //  Cache load may throw
}

void Workload::setDescription(
        const QString & description
    )
{
    tt3::util::Lock _(_database->guard);
    _ensureLiveAndWritable();   //  may throw

    //  Validate parameters
    if (!_database->validator()->workload()->isValidDescription(description))
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
//  tt3::db::api::IWorkload (associations)
auto Workload::contributingActivities(
    ) const -> tt3::db::api::Activities
{
    tt3::util::Lock _(_database->guard);
    _ensureLive();  //  may throw

    return tt3::db::api::Activities();  //  TODO implement properly
}

auto Workload::beneficiaries(
    ) const -> tt3::db::api::Beneficiaries
{
    tt3::util::Lock _(_database->guard);
    _ensureLive();  //  may throw

    return tt3::db::api::Beneficiaries();   //  TODO implement properly
}

void Workload::setBeneficiaries(
        const tt3::db::api::Beneficiaries & beneficiaries
    )
{
    tt3::util::Lock _(_database->guard);
    _ensureLiveAndWritable();   //  may throw

    Q_ASSERT(beneficiaries.isEmpty());  //  TODO implement properly
}

void Workload::addBeneficiary(
        tt3::db::api::IBeneficiary * /*beneficiary*/
    )
{
    tt3::util::Lock _(_database->guard);
    _ensureLiveAndWritable();   //  may throw

    throw tt3::util::NotImplementedError();
}

void Workload::removeBeneficiary(
        tt3::db::api::IBeneficiary * /*beneficiary*/
    )
{
    tt3::util::Lock _(_database->guard);
    _ensureLiveAndWritable();   //  may throw

    throw tt3::util::NotImplementedError();
}

auto Workload::assignedUsers(
    ) const -> tt3::db::api::Users
{
    tt3::util::Lock _(_database->guard);
    _ensureLive();  //  may throw

    return tt3::db::api::Users();   //  TODO implement properly
}

void Workload::setAssignedUsers(
        const tt3::db::api::Users & /*users*/
    )
{
    tt3::util::Lock _(_database->guard);
    _ensureLiveAndWritable();   //  may throw

    throw tt3::util::NotImplementedError();
}

void Workload::addAssignedUser(
        tt3::db::api::IUser * /*user*/
    )
{
    tt3::util::Lock _(_database->guard);
    _ensureLiveAndWritable();   //  may throw

    throw tt3::util::NotImplementedError();
}

void Workload::removeAssignedUser(
        tt3::db::api::IUser * /*user*/
    )
{
    tt3::util::Lock _(_database->guard);
    _ensureLiveAndWritable();   //  may throw

    throw tt3::util::NotImplementedError();
}

//////////
//  Cached properties
void Workload::_invalidateCachedProperties()
{
    Object::_invalidateCachedProperties();
    _displayName.invalidate();
    _description.invalidate();
}

void Workload::_saveDisplayName(const QString & displayName)
{
    Q_ASSERT(_database->guard.isLockedByCurrentThread());

    std::unique_ptr<Statement> stat
    {   _database->createStatement(
        "UPDATE [workloads]"
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

void Workload::_saveDescription(const QString & description)
{
    Q_ASSERT(_database->guard.isLockedByCurrentThread());

    std::unique_ptr<Statement> stat
    {   _database->createStatement(
        "UPDATE [workloads]"
        "   SET [description] = ?"
        " WHERE [pk] = ?") };
    description.isEmpty() ?
        stat->setNullParameter(0) :
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
void Workload::_deleteCascade()
{
    Q_ASSERT(_database->guard.isLockedByCurrentThread());
    Q_ASSERT(_isLive);
    Q_ASSERT(_database->_liveObjects.contains(_pk));

    //  Nothing here - can we make Object's
    //  _deleteCascade() non-abstract and kill this method ?
}

void Workload::_removeFromDatabase()
{
    Q_ASSERT(_database->guard.isLockedByCurrentThread());
    Q_ASSERT(_isLive);
    Q_ASSERT(_database->_liveObjects.contains(_pk));

    //  TODO break associations
    throw tt3::util::NotImplementedError();
}

//  End of tt3-db-sql/Workload.cpp
