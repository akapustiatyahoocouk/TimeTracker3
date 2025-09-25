//
//  tt3-db-xml/PublicTask.cpp - tt3::db::xml::PublicTask class implementation
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
PublicTask::PublicTask(
        Database * database,
        tt3::db::api::Oid oid)
    :   Activity(database, oid),
        PublicActivity(database, oid),
        Task(database, oid),
        _parent(nullptr)
{
    //  Register PublicTask with parent
    _database->_rootPublicTasks.insert(this);
    this->addReference();

    //  Undo the damage done by PublicActivity constructor
    Q_ASSERT(_database->_publicActivities.contains(this));
    _database->_publicActivities.remove(this);
    this->removeReference();
}

PublicTask::PublicTask(
        PublicTask * parent,
        tt3::db::api::Oid oid
    ) : Activity(parent->_database, oid),
        PublicActivity(parent->_database, oid),
        Task(parent->_database, oid),
        _parent(parent)
{
    //  Register PublicTask with parent
    _parent->_children.insert(this);
    this->addReference();
    _parent->addReference();
}

PublicTask::~PublicTask()
{
    Q_ASSERT(!_database->_rootPublicTasks.contains(this));
    Q_ASSERT(_parent == nullptr);
}

//////////
//  tt3::db::api::IObject (life cycle)
void PublicTask::destroy()
{
    tt3::util::Lock lock(_database->_guard);
    _ensureLiveAndWritable();   //  may throw
#ifdef Q_DEBUG
    _database->_validate(); //  may throw
#endif

    //  Destroy aggregated objects
    for (PublicTask * child : _children.values())
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
//  Implementation helpers
bool PublicTask::_siblingExists(
        const QString & /*displayName*/
    ) const
{
    Q_ASSERT(_database->_guard.isLockedByCurrentThread());
    Q_ASSERT(_isLive);

    Q_ASSERT(false);    //  TODO propery
    return true;
}

void PublicTask::_markDead()
{
    Q_ASSERT(_database->_guard.isLockedByCurrentThread());
    Q_ASSERT(_isLive);

    Q_ASSERT(_children.isEmpty());

    //  Remove from "live" caches
    if (_parent == nullptr)
    {
        Q_ASSERT(_database->_rootPublicTasks.contains(this));
        _database->_rootPublicTasks.remove(this);
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
    Activity::_markDead();
}

//////////
//  Serialization
void PublicTask::_serializeProperties(
        QDomElement & objectElement
    )
{
    PublicActivity::_serializeProperties(objectElement);
    Task::_serializeProperties(objectElement);
}

void PublicTask::_serializeAggregations(
        QDomElement & objectElement
    )
{
    PublicActivity::_serializeAggregations(objectElement);
    Task::_serializeAggregations(objectElement);
}

void PublicTask::_serializeAssociations(
        QDomElement & objectElement
    )
{
    PublicActivity::_serializeAssociations(objectElement);
    Task::_serializeAssociations(objectElement);
}

void PublicTask::_deserializeProperties(
        const QDomElement & objectElement
    )
{
    PublicActivity::_deserializeProperties(objectElement);
    Task::_deserializeProperties(objectElement);
}

void PublicTask::_deserializeAggregations(
        const QDomElement & objectElement
    )
{
    PublicActivity::_deserializeAggregations(objectElement);
    Task::_deserializeAggregations(objectElement);
}

void PublicTask::_deserializeAssociations(
        const QDomElement & objectElement
    )
{
    PublicActivity::_deserializeAssociations(objectElement);
    Task::_deserializeAssociations(objectElement);
}

//////////
//  Validation
void PublicTask::_validate(
       Objects & validatedObjects
    )
{
    PublicActivity::_validate(validatedObjects);
    Task::_validate(validatedObjects);

    //  Validate properties

    //  Validate aggregations
    if (_parent == nullptr)
    {
        if (!_database->_rootPublicTasks.contains(this))
        {   //  OOPS!
            throw tt3::db::api::DatabaseCorruptException(_database->_address);
        }
    }
    else
    {
        Q_ASSERT(false);    //  TODO properly
        //  TODO there must be no parent/child loops
    }

    //  Validate associations
}

//  End of tt3-db-xml/User.cpp
