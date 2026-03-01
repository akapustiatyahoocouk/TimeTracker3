//
//  tt3-db-sql/Beneficiary.cpp - tt3::db::xml::Beneficiary class implementation
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
Beneficiary::Beneficiary(
        Database * database,
        qint64 pk
    ) : Object(database, pk),
        //  Cached propertirs
        _displayName([this] { _loadCachedProperties(); }),
        _description([this] { _loadCachedProperties(); })
{
}

Beneficiary::~Beneficiary()
{
}

//////////
//  tt3::db::api::IBeneficiary (properties)
QString Beneficiary::displayName() const
{
    tt3::util::Lock _(_database->guard);
    _ensureLive();  //  may throw

    return _displayName;    //  Cache load may throw
}

void Beneficiary::setDisplayName(
        const QString & displayName
    )
{
    tt3::util::Lock _(_database->guard);
    _ensureLiveAndWritable();   //  may throw

    //  Validate parameters
    if (!_database->validator()->beneficiary()->isValidDisplayName(displayName))
    {
        throw tt3::db::api::InvalidPropertyValueException(
            type(),
            "displayName",
            displayName);
    }
    if (displayName != _displayName)    //  Cache load may throw
    {   //  Make the change (but no duplication)
        if (_database->_beneficiaryExists(displayName))
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
                _database, type(), _oid));  //  Cache load may throw
        //  TODO post change notification to the database
        //  ...and we're done
    }
}

QString Beneficiary::description() const
{
    tt3::util::Lock _(_database->guard);
    _ensureLive();  //  may throw

    return _description;    //  Cache load may throw
}

void Beneficiary::setDescription(
        const QString & description
    )
{
    tt3::util::Lock _(_database->guard);
    _ensureLiveAndWritable();   //  may throw

    //  Validate parameters
    if (!_database->validator()->beneficiary()->isValidDescription(description))
    {
        throw tt3::db::api::InvalidPropertyValueException(
            type(),
            "description",
            description);
    }
    if (description != _description)
    {   //  Make the change
        //  Begin transaction for the changes
        Transaction transaction(_database); //  may throw
        //  Save, THEN cache
        _saveDescription(description);  //  may throw
        _description = description;
        //  We're done with the changes
        transaction.commit();   //  may throw

        //  ...schedule change notifications....
        _database->_changeNotifier.post(
            new tt3::db::api::ObjectModifiedNotification(
                _database, type(), _oid));  //  Cache load may throw
        //  TODO post change notification to the database
        //  ...and we're done
    }
}

//////////
//  tt3::db::api::IBeneficiary (associations)
auto Beneficiary::workloads(
    ) const -> tt3::db::api::Workloads
{
    tt3::util::Lock _(_database->guard);
    _ensureLive();

    //  TODO cache PKs
    std::unique_ptr<Statement> stat
    {   _database->createStatement(
            "SELECT [fk_workload]"
            "  FROM [workload_beneficiaries]"
            " WHERE [fk_beneficiary] = ?") };
    stat->setIntParameter(0, _pk);
    std::unique_ptr<ResultSet> rs
        { stat->executeQuery() };   //  may throw
    tt3::db::api::Workloads result;
    while (rs->next())
    {
        result.insert(_database->_getWorkload(rs->intValue(0)));
    }
    return result;
}

void Beneficiary::setWorkloads(
    const tt3::db::api::Workloads & workloads
    )
{
    tt3::util::Lock _(_database->guard);
    _ensureLiveAndWritable();   //  may throw
    //  Validate parameters
    if (workloads.contains(nullptr))
    {
        throw tt3::db::api::InvalidPropertyValueException(
            type(),
            "workloads",
            nullptr);
    }

    Workloads oldWorkloads =
        tt3::util::transform(
            this->workloads(),    //  may throw
            [&](auto w)
            {
                Q_ASSERT(w != nullptr); //  should have been caught earlier!
                auto sqlWorkload = dynamic_cast<Workload*>(w);
                if (sqlWorkload == nullptr ||
                    sqlWorkload->_database != this->_database ||
                    !sqlWorkload->_isLive)
                {   //  OOPS!
                    throw tt3::db::api::IncompatibleInstanceException(w->type());
                }
                return sqlWorkload;
            });
    Workloads newWorkloads =
        tt3::util::transform(
            workloads,
            [&](auto w)
            {
                Q_ASSERT(w != nullptr); //  should have been caught earlier!
                auto sqlWorkload = dynamic_cast<Workload*>(w);
                if (sqlWorkload == nullptr ||
                    sqlWorkload->_database != this->_database ||
                    !sqlWorkload->_isLive)
                {   //  OOPS!
                    throw tt3::db::api::IncompatibleInstanceException(w->type());
                }
                return sqlWorkload;
            });
    if (oldWorkloads != newWorkloads)
    {   //  Make the changes
        //  Begin transaction for the changes
        Transaction transaction(_database); //  may throw
        //  Remove Workloads that are no longer relevant
        for (auto workload : std::as_const(oldWorkloads))
        {
            if (!newWorkloads.contains(workload))
            {
                std::unique_ptr<Statement> stat
                {   _database->createStatement(
                        "DELETE FROM [workload_beneficiaries]"
                        " WHERE [fk_beneficiary] = ?"
                        "   AND [fk_workload] = ?") };
                stat->setIntParameter(0, _pk);
                stat->setIntParameter(1, workload->_pk);
                stat->execute();    //  may throw
                //  Schedule change notifications
                _database->_changeNotifier.post(
                    new tt3::db::api::ObjectModifiedNotification(
                        _database, workload->type(), workload->_oid));  //  Cache load may throw
            }
        }
        //  Add wWorkloads that are new
        for (auto workload : std::as_const(newWorkloads))
        {
            if (!oldWorkloads.contains(workload))
            {
                std::unique_ptr<Statement> stat
                {   _database->createStatement(
                        "INSERT INTO [workload_beneficiaries]"
                        "       ([fk_workload],[fk_beneficiary])"
                        "       SELECT ?,?"
                        "        WHERE NOT EXISTS ("
                        "           SELECT 1"
                        "             FROM [workload_beneficiaries]"
                        "            WHERE [fk_workload] = ?"
                        "              AND [fk_beneficiary] = ?)") };
                stat->setIntParameter(0, workload->_pk);
                stat->setIntParameter(1, _pk);
                stat->setIntParameter(2, workload->_pk);
                stat->setIntParameter(3, _pk);
                stat->execute();    //  may throw
                //  Schedule change notifications
                _database->_changeNotifier.post(
                    new tt3::db::api::ObjectModifiedNotification(
                        _database, workload->type(), workload->_oid));  //  Cache load may throw
            }
        }
        //  We're done with the changes
        transaction.commit();   //  may throw
        //  Schedule change notifications
        _database->_changeNotifier.post(
            new tt3::db::api::ObjectModifiedNotification(
                _database, type(), _oid));  //  Cache load may throw
    }
}

void Beneficiary::addWorkload(
        tt3::db::api::IWorkload * workload
    )
{
    tt3::util::Lock _(_database->guard);
    _ensureLiveAndWritable();   //  may throw

    //  Validate parameters
    if (workload == nullptr)
    {
        throw tt3::db::api::InvalidPropertyValueException(
            type(),
            "workload",
            nullptr);
    }

    Workload * sqlWorkload = dynamic_cast<Workload*>(workload);
    if (sqlWorkload == nullptr ||
        sqlWorkload->_database != this->_database ||
        !sqlWorkload->_isLive)
    {   //  OOPS!
        throw tt3::db::api::IncompatibleInstanceException(workload->type());
    }
    //  Make the changes UNCONDITIONALLY (to avoid database race)
    std::unique_ptr<Statement> stat
    {   _database->createStatement(
            "INSERT INTO [workload_beneficiaries]"
            "       ([fk_workload],[fk_beneficiary])"
            "       SELECT ?,?"
            "        WHERE NOT EXISTS ("
            "           SELECT 1"
            "             FROM [workload_beneficiaries]"
            "            WHERE [fk_workload] = ?"
            "              AND [[fk_beneficiary]] = ?)") };
    stat->setIntParameter(0, sqlWorkload->_pk);
    stat->setIntParameter(1, _pk);
    stat->setIntParameter(2, sqlWorkload->_pk);
    stat->setIntParameter(3, _pk);
    stat->execute();    //  may throw
    //  Schedule change notifications...
    _database->_changeNotifier.post(
        new tt3::db::api::ObjectModifiedNotification(
            _database, type(), _oid));  //  Cache load may throw
    _database->_changeNotifier.post(
        new tt3::db::api::ObjectModifiedNotification(
            _database, sqlWorkload->type(), sqlWorkload->_oid));    //  Cache load may throw
    //  ...and we're done
}

void Beneficiary::removeWorkload(
        tt3::db::api::IWorkload * workload
    )
{
    tt3::util::Lock _(_database->guard);
    _ensureLiveAndWritable();   //  may throw

    //  Validate parameters
    if (workload == nullptr)
    {
        throw tt3::db::api::InvalidPropertyValueException(
            type(),
            "workload",
            nullptr);
    }

    Workload * sqlWorkload = dynamic_cast<Workload*>(workload);
    if (sqlWorkload == nullptr ||
        sqlWorkload->_database != this->_database ||
        !sqlWorkload->_isLive)
    {   //  OOPS!
        throw tt3::db::api::IncompatibleInstanceException(workload->type());
    }
    //  Make the changes UNCONDITIONALLY (to avoid database race)
    std::unique_ptr<Statement> stat
    {   _database->createStatement(
            "DELETE FROM [workload_beneficiaries]"
            " WHERE [fk_workload] = ?"
            "   AND [fk_beneficiary] = ?") };
    stat->setIntParameter(0, sqlWorkload->_pk);
    stat->setIntParameter(1, _pk);
    stat->execute();    //  may throw
    //  Schedule change notifications...
    _database->_changeNotifier.post(
        new tt3::db::api::ObjectModifiedNotification(
            _database, type(), _oid));  //  Cache load may throw
    _database->_changeNotifier.post(
        new tt3::db::api::ObjectModifiedNotification(
            _database, sqlWorkload->type(), sqlWorkload->_oid));    //  Cache load may throw
    //  ...and we're done
}

//////////
//  Cached properties
void Beneficiary::_invalidateCachedProperties()
{
    Object::_invalidateCachedProperties();
    _displayName.invalidate();
    _description.invalidate();
}

void Beneficiary::_loadCachedProperties()
{
    Q_ASSERT(_database->guard.isLockedByCurrentThread());

    std::unique_ptr<Statement> stat
    {   _database->createStatement(
            "SELECT [objects].[oid] AS [oid],"
            "       [beneficiaries].[displayname] AS [displayname],"
            "       [beneficiaries].[description] AS [description]"
            "  FROM [objects],[beneficiaries]"
            " WHERE [objects].[pk] = ?"
            "   AND [beneficiaries].[pk] = [objects].[pk]") };
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

void Beneficiary::_saveDisplayName(const QString & displayName)
{
    Q_ASSERT(_database->guard.isLockedByCurrentThread());

    std::unique_ptr<Statement> stat
    {   _database->createStatement(
            "UPDATE [beneficiaries]"
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

void Beneficiary::_saveDescription(const QString & description)
{
    Q_ASSERT(_database->guard.isLockedByCurrentThread());

    std::unique_ptr<Statement> stat
    {   _database->createStatement(
            "UPDATE [beneficiaries]"
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
void Beneficiary::_deleteCascade()
{
    Q_ASSERT(_database->guard.isLockedByCurrentThread());
    Q_ASSERT(_isLive);
    Q_ASSERT(_database->_liveObjects.contains(_pk));

    //  Nothing here - can we make Object's
    //  _deleteCascade() non-abstract and kill this method ?
}

void Beneficiary::_removeFromDatabase()
{
    Q_ASSERT(_database->guard.isLockedByCurrentThread());
    Q_ASSERT(_isLive);
    Q_ASSERT(_database->_liveObjects.contains(_pk));

    //  Unlink Workloads (with notifications)
    try
    {   //  The associated workloads are affected...
        std::unique_ptr<Statement> stat1
        {   _database->createStatement(
                "SELECT [fk_workload]"
                "  FROM [workload_beneficiaries]"
                " WHERE [fk_beneficiary] = ?") };
        stat1->setIntParameter(0, _pk);
        std::unique_ptr<ResultSet> rs
            { stat1->executeQuery() };   //  may throw
        tt3::db::api::Workloads result;
        while (rs->next())
        {
            auto workload = _database->_getWorkload(rs->intValue(0));
            _database->_changeNotifier.post(
                new tt3::db::api::ObjectModifiedNotification(
                    _database, workload->type(), workload->_oid));  //  Cache load may throw
            //  TODO write notifications to the database
        }
        //  ...and link records must be deleted
        std::unique_ptr<Statement> stat2
        {   _database->createStatement(
                "DELETE FROM [workload_beneficiaries]"
                " WHERE [fk_beneficiary] = ?") };
        stat2->setIntParameter(0, _pk);
        stat2->executeQuery();  //  may throw
    }
    catch (...)
    {   //  Better a lost notification than a failed deletion
    }

    //  Delete [beneficiaries] row
    std::unique_ptr<Statement> stat
    {   _database->createStatement(
            "DELETE FROM [beneficiaries]"
            " WHERE [pk] = ?") };
    stat->setIntParameter(0, _pk);
    stat->execute();    //  may throw
    //  The rest is up to the base class
    Object::_removeFromDatabase();
}

//  End of tt3-db-sql/Beneficiary.cpp
