//
//  tt3-db-sql/Project.cpp - tt3::db::sql::Project class implementation
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
Project::Project(
        Database * database,
        qint64 pk
    ) : Workload(database, pk),
        //  Cached propertirs
        _completed([this] { _loadCachedProperties(); }),
        _fkParent([this] { _loadCachedProperties(); })
{
}

Project::~Project()
{
}

//////////
//  tt3::db::api::IProject (properties)
bool Project::completed() const
{
    tt3::util::Lock _(_database->guard);
    _ensureLive();  //  may throw

    return _completed;  //  Cache load may throw
}

void Project::setCompleted(
        bool completed
    )
{
    tt3::util::Lock _(_database->guard);
    _ensureLiveAndWritable();   //  may throw

    if (completed != _completed)    //  Cache load may throw
    {   //  Make the change
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

//////////
//  tt3::db::api::IProject (associations)
auto Project::parent(
    ) const -> IProject *
{
    tt3::util::Lock _(_database->guard);
    _ensureLive();  //  may throw

    return _fkParent.value().has_value() ?  //  Cache load may throw
               _database->_getObject<Project>(_fkParent.value().value()) :  //  Cache load may throw
               nullptr;
}

void Project::setParent(
        IProject * parent
    )
{
    tt3::util::Lock _(_database->guard);
    _ensureLiveAndWritable();   //  may throw

    Project * sqlParent = nullptr;
    if (parent != nullptr)
    {
        sqlParent = dynamic_cast<Project*>(parent);
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
        //  ...ensuring we're not creating a oarent/child loop...
        if (sqlParent != nullptr)
        {
            Projects parentClosure;
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

auto Project::children(
    ) const -> tt3::db::api::Projects
{
    tt3::util::Lock _(_database->guard);
    _ensureLive();  //  may throw

    //  TODO cache PKs
    std::unique_ptr<Statement> stat
    {   _database->createStatement(
        "SELECT [pk]"
        "  FROM [workloads]"
        " WHERE [fk_parent] = ?") };
    stat->setIntParameter(0, _pk);
    std::unique_ptr<ResultSet> rs
        { stat->executeQuery() };   //  may throw
    tt3::db::api::Projects result;
    while (rs->next())
    {
        result.insert(_database->_getObject<Project>(rs->intValue(0)));
    }
    return result;
}

//////////
//  tt3::db::api::IProject (life cycle)
auto Project::createChild(
        const QString & displayName,
        const QString & description,
        const tt3::db::api::Beneficiaries & beneficiaries,
        bool completed
    ) -> tt3::db::api::IProject *
{
    tt3::util::Lock _(_database->guard);
    _ensureLiveAndWritable();   //  may throw

    //  Validate parameters
    if (!_database->validator()->project()->isValidDisplayName(displayName))
    {
        throw tt3::db::api::InvalidPropertyValueException(
            tt3::db::api::ObjectTypes::Project::instance(),
            "displayName",
            displayName);
    }
    if (!_database->validator()->project()->isValidDescription(description))
    {
        throw tt3::db::api::InvalidPropertyValueException(
            tt3::db::api::ObjectTypes::Project::instance(),
            "description",
            description);
    }
    if (beneficiaries.contains(nullptr))
    {
        throw tt3::db::api::InvalidPropertyValueException(
            tt3::db::api::ObjectTypes::Project::instance(),
            "beneficiaries",
            nullptr);
    }

    /*  TODO
    Beneficiaries xmlBeneficiaries;
    for (tt3::db::api::IBeneficiary * beneficiary : beneficiaries)
    {
        Q_ASSERT(beneficiary != nullptr);   //  should have been caught earlier
        auto xmlBeneficiary = dynamic_cast<Beneficiary*>(beneficiary);
        if (xmlBeneficiary == nullptr ||
            xmlBeneficiary->_database != this->_database ||
            !xmlBeneficiary->_isLive)
        {   //  OOPS!
            throw tt3::db::api::IncompatibleInstanceException(beneficiary->type());
        }
        xmlBeneficiaries.insert(xmlBeneficiary);
    }
    */

    //  Display names must be unique.
    //  SQL "UNIQUE displayname" constraint would take care of
    //  that, but try for a better (non-SQL) error message
    if (_childExists(displayName))
    {   //  OOPS!
        throw tt3::db::api::AlreadyExistsException(
            tt3::db::api::ObjectTypes::Project::instance(),
            "displayName",
            displayName);
    }

    //  Begin transaction for the changes
    Transaction transaction(_database);  //  may throw

    //  Do the work - create [objects] row..
    Database::_ObjIds objIds = _database->_createObject(tt3::db::api::ObjectTypes::Project::instance()); //  may throw
    //  ...then [worklooads] row...
    std::unique_ptr<Statement> stat
    {   _database->createStatement(
        "INSERT INTO [workloads]"
        "       ([pk],[fk_parent],[displayname],[description],[completed])"
        "       VALUES(?,?,?,?,?)") };
    stat->setIntParameter(0, std::get<0>(objIds));
    stat->setIntParameter(1, _pk);
    stat->setStringParameter(2, displayName);
    description.isEmpty() ?
        stat->setNullParameter(3) :
        stat->setStringParameter(3, description);
    stat->setBoolParameter(4, completed);
    stat->execute();    //  may throw

    //  We're done with the changes
    transaction.commit();   //  may throw

    //  Create & register the Project object...
    Project * project = new Project(_database, std::get<0>(objIds));
    //  ...setting its cached properties to initial values
    project->_oid = std::get<1>(objIds);
    project->_displayName = displayName;
    project->_description = description;
    project->_fkParent = _pk;
    project->_completed = completed;

    //  ...schedule change notifications...
    _database->_changeNotifier.post(
        new tt3::db::api::ObjectModifiedNotification(
            _database, this->type(), this->_oid));
    _database->_changeNotifier.post(
        new tt3::db::api::ObjectCreatedNotification(
            _database, project->type(), project->_oid));
    //  TODO post change notification to the database
    //  ...and we're done
    return project;
}

//////////
//  Cached properties
void Project::_invalidateCachedProperties()
{
    Workload::_invalidateCachedProperties();
    _completed.invalidate();
    _fkParent.invalidate();
}

void Project::_loadCachedProperties()
{
    Q_ASSERT(_database->guard.isLockedByCurrentThread());

    std::unique_ptr<Statement> stat
        {   _database->createStatement(
            "SELECT [objects].[oid] AS [oid],"
            "       [workloads].[displayname] AS [displayname],"
            "       [workloads].[description] AS [description],"
            "       [workloads].[completed] AS [completed],"
            "       [workloads].[fk_parent] AS [fk_parent]"
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
    _completed = rs->boolValue("completed");
    _fkParent =
        rs->isNull("fk_parent") ?
            std::optional<qint64>() :
            rs->intValue("fk_parent");
}

void Project::_saveCompleted(bool completed)
{
    Q_ASSERT(_database->guard.isLockedByCurrentThread());

    std::unique_ptr<Statement> stat
    {   _database->createStatement(
        "UPDATE [workloads]"
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

void Project::_saveFkParent(const std::optional<qint64> & fkParent)
{
    Q_ASSERT(_database->guard.isLockedByCurrentThread());

    std::unique_ptr<Statement> stat
    {   _database->createStatement(
        "UPDATE [workloads]"
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

//////////
//  Implementation helpers
bool Project::_siblingExists(const QString & displayName) const
{
    Q_ASSERT(_database->guard.isLockedByCurrentThread());
    Q_ASSERT(_isLive);
    Q_ASSERT(_database->_liveObjects.contains(_pk));

    if (_fkParent.value().has_value())  //  Cache load may throw
    {   //  We're looking for a child project
        std::unique_ptr<Statement> stat
        {   _database->createStatement(
            "SELECT [pk]"
            "  FROM [workloads]"
            " WHERE [displayname] = ?"
            "   AND [pk] <> ?"
            "   AND [completed] IS NOT NULL"    //  Project
            "   AND [fk_parent] = ?") };        //  with the same parent
        stat->setStringParameter(0, displayName);
        stat->setIntParameter(1, _pk);
        stat->setIntParameter(2, _fkParent.value().value());    //  Cache load may throw
        std::unique_ptr<ResultSet> rs
            { stat->executeQuery() };
        return rs->next();  //  row exists ?
    }
    else
    {   //  We're looking for a root project
        std::unique_ptr<Statement> stat
        {   _database->createStatement(
            "SELECT [pk]"
            "  FROM [workloads]"
            " WHERE [displayname] = ?"
            "   AND [pk] <> ?"
            "   AND [completed] IS NOT NULL"    //  Project
            "   AND [fk_parent] IS NULL") };    //  Root
        stat->setStringParameter(0, displayName);
        stat->setIntParameter(1, _pk);
        std::unique_ptr<ResultSet> rs
            { stat->executeQuery() };
        return rs->next();  //  row exists ?
    }
}

void Project::_deleteCascade()
{
    Q_ASSERT(_database->guard.isLockedByCurrentThread());
    Q_ASSERT(_isLive);
    Q_ASSERT(_database->_liveObjects.contains(_pk));

    //  Child projects - can't rely on SQL DELETE CASCADE
    //  PLUS we need delete notifications for children
    for (auto child : children())
    {
        child->destroy();
    }

    //  The rest is up to the base class
    Workload::_deleteCascade();
}

bool Project::_childExists(const QString & displayName) const
{
    Q_ASSERT(_database->guard.isLockedByCurrentThread());
    Q_ASSERT(_isLive);
    Q_ASSERT(_database->_liveObjects.contains(_pk));

    std::unique_ptr<Statement> stat
    {   _database->createStatement(
        "SELECT [pk]"
        "  FROM [workloads]"
        " WHERE [displayname] = ?"
        "   AND [completed] IS NOT NULL"    //  Project
        "   AND [fk_parent] = ?") };        //  with this as parent
    stat->setStringParameter(0, displayName);
    stat->setIntParameter(1, _pk);
    std::unique_ptr<ResultSet> rs
        { stat->executeQuery() };
    return rs->next();  //  row exists ?
}

void Project::_collectParentClosure(Projects & closure)
{
    Q_ASSERT(_database->guard.isLockedByCurrentThread());
    Q_ASSERT(_isLive);
    Q_ASSERT(_database->_liveObjects.contains(_pk));

    closure.insert(this);
    Project * parentProject =
        _fkParent.value().has_value() ?  //  Cache load may throw
            _database->_getObject<Project>(_fkParent.value().value()) :  //  Cache load may throw
            nullptr;
    if (parentProject != nullptr && !closure.contains(parentProject))
    {
        parentProject->_collectParentClosure(closure);
    }
}

//  End of tt3-db-sql/Project.cpp
