//
//  tt3-db-xml/Project.cpp - tt3::db::xml::Project class implementation
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
#include "tt3-db-xml/API.hpp"
using namespace tt3::db::xml;

//////////
//  Construction/destruction (from DB type only)
Project::Project(
        Database * database,
        tt3::db::api::Oid oid
    ) : Workload(database, oid),
        _parent(nullptr)
{
    //  Register Project with parent
    _database->_rootProjects.insert(this);
    this->addReference();
}

Project::Project(
        Project * parent,
        tt3::db::api::Oid oid
    ) : Workload(parent->_database, oid),
        _parent(parent)
{
    //  Register Project with parent
    _parent->_children.insert(this);
    this->addReference();
    _parent->addReference();
}

Project::~Project()
{
    Q_ASSERT(!_database->_rootProjects.contains(this));
    Q_ASSERT(_parent == nullptr);
}

//////////
//  tt3::db::api::IObject (life cycle)
void Project::destroy()
{
    tt3::util::Lock lock(_database->_guard);
    _ensureLiveAndWritable();   //  may throw
#ifdef Q_DEBUG
    _database->_validate(); //  may throw
#endif

    //  Destroy aggregated objects
    for (Project * child : _children.values())
    {
        child->destroy();
    }
    Q_ASSERT(_children.isEmpty());

    _markDead();

#ifdef Q_DEBUG
    _database->_validate(); //  may throw
#endif
}

//////////
//  tt3::db::api::IProject (properties)
bool Project::completed(
    ) const
{
    tt3::util::Lock lock(_database->_guard);
    _ensureLive();  //  may throw
    //  We assume database is consistent since last change

    return _completed;
}

void Project::setCompleted(
        bool /*completed*/
    )
{
    throw tt3::util::NotImplementedError();
}

//////////
//  tt3::db::api::IProject (associations)
auto Project::parent(
    ) const -> IProject *
{
    tt3::util::Lock lock(_database->_guard);
    _ensureLive();  //  may throw
    //  We assume database is consistent since last change

    return _parent;
}

void Project::setParent(
        IProject * /*parent*/
    )
{
    throw tt3::util::NotImplementedError();
}

auto Project::children(
    ) const -> tt3::db::api::Projects
{
    tt3::util::Lock lock(_database->_guard);
    _ensureLive();  //  may throw
    //  We assume database is consistent since last change

    return tt3::db::api::Projects(_children.cbegin(), _children.cend());
}

//////////
//  tt3::db::api::IProject (life cycle)
auto Project::createChild(
        const QString & /*displayName*/,
        const QString & /*description*/,
        const tt3::db::api::Beneficiaries & /*beneficiaries*/,
        bool /*completed*/
    ) -> tt3::db::api::IProject *
{
    throw tt3::util::NotImplementedError();
}

//////////
//  Implementation helpers
bool Project::_siblingExists(
        const QString & displayName
    ) const
{
    Q_ASSERT(_database->_guard.isLockedByCurrentThread());
    Q_ASSERT(_isLive);

    Project * sibling =
        (_parent == nullptr) ?
            _database->_findRootProject(displayName) :
            _parent->_findChild(displayName);
    return sibling != nullptr && sibling != this;
}

void Project::_markDead()
{
    Q_ASSERT(_database->_guard.isLockedByCurrentThread());
    Q_ASSERT(_isLive);

    Q_ASSERT(_children.isEmpty());

    //  Remove from "live" caches
    if (_parent == nullptr)
    {
        Q_ASSERT(_database->_rootProjects.contains(this));
        _database->_rootProjects.remove(this);
        this->removeReference();
    }
    else
    {
        Q_ASSERT(_parent->_children.contains(this));
        _parent->_children.remove(this);
        this->removeReference();
        _parent->removeReference();
        _parent = nullptr;
    }

    //  The rest is up to the base class
    Workload::_markDead();
}

Project * Project::_findChild(
        const QString & displayName
    ) const
{
    Q_ASSERT(_database->_guard.isLockedByCurrentThread());

    for (Project * child : _children)
    {
        if (child->_displayName == displayName)
        {
            return child;
        }
    }
    return nullptr;
}

//////////
//  Serialization
void Project::_serializeProperties(
        QDomElement & objectElement
    ) const
{
    Workload::_serializeProperties(objectElement);
    objectElement.setAttribute("Completed", tt3::util::toString(_completed));
}

void Project::_serializeAggregations(
        QDomElement & objectElement
    ) const
{
    Workload::_serializeAggregations(objectElement);

    _database->_serializeAggregation(
        objectElement,
        "Children",
        _children);
}

void Project::_serializeAssociations(
        QDomElement & objectElement
    ) const
{
    Workload::_serializeAssociations(objectElement);
}

void Project::_deserializeProperties(
        const QDomElement & objectElement
    )
{
    Workload::_deserializeProperties(objectElement);
    _completed = tt3::util::fromString<bool>(objectElement.attribute("Completed"));
}

void Project::_deserializeAggregations(
        const QDomElement & objectElement
    )
{
    Workload::_deserializeAggregations(objectElement);

    _database->_deserializeAggregation<Project>(
        objectElement,
        "Children",
        _children,
        [&](auto oid)
        {
            return new Project(this, oid);
        });
}

void Project::_deserializeAssociations(
        const QDomElement & objectElement
    )
{
    Workload::_deserializeAssociations(objectElement);
}

//////////
//  Validation
void Project::_validate(
        Objects & validatedObjects
    )
{
    Workload::_validate(validatedObjects);

    //  Validate properties

    //  Validate aggregations
    if (_parent == nullptr)
    {
        if (!_database->_rootProjects.contains(this))
        {   //  OOPS!
            throw tt3::db::api::DatabaseCorruptException(_database->_address);
        }
    }
    else
    {
        if (!_parent->_isLive ||
            _parent->_database != this->_database ||
            !_parent->_children.contains(this))
        {   //  OOPS!
            throw tt3::db::api::DatabaseCorruptException(_database->_address);
        }
        //  TODO there must be no parent/child loops
    }
    for (Project * child : _children)
    {
        if (child == nullptr || !child->_isLive ||
            child->_database != _database ||
            child->_parent != this)
        {   //  OOPS!
            throw tt3::db::api::DatabaseCorruptException(_database->_address);
        }
        child->_validate(validatedObjects);
    }

    //  Validate associations
}

//  End of tt3-db-xml/Project.cpp
