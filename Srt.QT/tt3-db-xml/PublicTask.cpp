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

    //  Undo the damage done by PublicActivity constructor
    Q_ASSERT(_database->_publicActivities.contains(this));
    _database->_publicActivities.remove(this);
    this->removeReference();
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
//  tt3::db::api::IPublicTask (associations)
auto PublicTask::parent(
    ) const -> IPublicTask *
{
    tt3::util::Lock lock(_database->_guard);
    _ensureLive();  //  may throw
    //  We assume database is consistent since last change

    return _parent;
}

void PublicTask::setParent(
        IPublicTask * /*parent*/
    )
{
    throw tt3::util::NotImplementedError();
}

auto PublicTask::children(
    ) const -> tt3::db::api::PublicTasks
{
    tt3::util::Lock lock(_database->_guard);
    _ensureLive();  //  may throw
    //  We assume database is consistent since last change

    return tt3::db::api::PublicTasks(_children.cbegin(), _children.cend());
}

//////////
//  tt3::db::api::IPublicTask (life cycle)
auto PublicTask::createChild(
        const QString & displayName,
        const QString & description,
        const tt3::db::api::InactivityTimeout & timeout,
        bool requireCommentOnStart,
        bool requireCommentOnFinish,
        bool fullScreenReminder,
        tt3::db::api::IActivityType * activityType,
        tt3::db::api::IWorkload * workload,
        bool completed,
        bool requireCommentOnCompletion
    ) -> tt3::db::api::IPublicTask *
{
    tt3::util::Lock lock(_database->_guard);
    _ensureLiveAndWritable(); //  may throw
#ifdef Q_DEBUG
    _database->_validate();    //  may throw
#endif

    //  Validate parameters
    if (!_database->_validator->publicTask()->isValidDisplayName(displayName))
    {
        throw tt3::db::api::InvalidPropertyValueException(
            tt3::db::api::ObjectTypes::PublicTask::instance(),
            "displayName",
            displayName);
    }
    if (!_database->_validator->publicTask()->isValidDescription(description))
    {
        throw tt3::db::api::InvalidPropertyValueException(
            tt3::db::api::ObjectTypes::PublicTask::instance(),
            "description",
            description);
    }
    if (timeout.has_value() &&
        !_database->_validator->publicTask()->isValidTimeout(timeout))
    {
        throw tt3::db::api::InvalidPropertyValueException(
            tt3::db::api::ObjectTypes::PublicTask::instance(),
            "timeout",
            timeout.value());
    }
    ActivityType * xmlActivityType = nullptr;
    if (activityType != nullptr)
    {
        xmlActivityType = dynamic_cast<ActivityType*>(activityType);
        if (xmlActivityType == nullptr ||
            xmlActivityType->_database != _database ||
            !xmlActivityType->_isLive)
        {   //  OOPS!
            throw tt3::db::api::IncompatibleInstanceException(activityType->type());
        }
    }
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

    //  Display names must be unique
    if (_findChild(displayName) != nullptr)
    {   //  OOPS!
        throw tt3::db::api::AlreadyExistsException(
            tt3::db::api::ObjectTypes::PublicTask::instance(),
            "displayName",
            displayName);
    }

    //  Do the work - create & initialize the PublicTask...
    PublicTask * child = new PublicTask(this, _database->_generateOid()); //  registers with Database
    child->_displayName = displayName;
    child->_description = description;
    child->_timeout = timeout;
    child->_requireCommentOnStart = requireCommentOnStart;
    child->_requireCommentOnFinish = requireCommentOnFinish;
    child->_fullScreenReminder = fullScreenReminder;
    child->_completed = completed;
    child->_requireCommentOnCompletion = requireCommentOnCompletion;
    if (xmlActivityType != nullptr)
    {   //  Link with ActivityType
        child->_activityType = xmlActivityType;
        xmlActivityType->_activities.insert(child);
        xmlActivityType->addReference();
        child->addReference();
    }
    if (xmlWorkload != nullptr)
    {   //  Link with Workload
        child->_workload = xmlWorkload;
        xmlWorkload->_contributingActivities.insert(child);
        xmlWorkload->addReference();
        child->addReference();
    }
    _database->_markModified();
    //  ...schedule change notifications...
    _database->_changeNotifier.post(
        new tt3::db::api::ObjectCreatedNotification(
            _database, child->type(), child->_oid));
    if (xmlActivityType != nullptr)
    {
        _database->_changeNotifier.post(
            new tt3::db::api::ObjectModifiedNotification(
                _database, xmlActivityType->type(), xmlActivityType->_oid));
    }
    if (xmlWorkload != nullptr)
    {
        _database->_changeNotifier.post(
            new tt3::db::api::ObjectModifiedNotification(
                _database, xmlWorkload->type(), xmlWorkload->_oid));
    }
    //  ...and we're done
#ifdef Q_DEBUG
    _database->_validate();    //  may throw
#endif
    return child;
}

//////////
//  Implementation helpers
bool PublicTask::_siblingExists(
        const QString & displayName
    ) const
{
    Q_ASSERT(_database->_guard.isLockedByCurrentThread());
    Q_ASSERT(_isLive);

    PublicTask * sibling =
        (_parent == nullptr) ?
            _database->_findRootPublicTask(displayName) :
            _parent->_findChild(displayName);
    return sibling != nullptr && sibling != this;
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

PublicTask * PublicTask::_findChild(
        const QString & displayName
    ) const
{
    Q_ASSERT(_database->_guard.isLockedByCurrentThread());

    for (PublicTask * child : _children)
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
void PublicTask::_serializeProperties(
        QDomElement & objectElement
    ) const
{
    PublicActivity::_serializeProperties(objectElement);
    Task::_serializeProperties(objectElement);
}

void PublicTask::_serializeAggregations(
        QDomElement & objectElement
    ) const
{
    PublicActivity::_serializeAggregations(objectElement);
    Task::_serializeAggregations(objectElement);

    _database->_serializeAggregation(
        objectElement,
        "Children",
        _children);
}

void PublicTask::_serializeAssociations(
        QDomElement & objectElement
    ) const
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

    _database->_deserializeAggregation<PublicTask>(
        objectElement,
        "Children",
        _children,
        [&](auto oid)
        {
            return new PublicTask(this, oid);
        });
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
        if (!_parent->_isLive ||
            _parent->_database != this->_database ||
            !_parent->_children.contains(this))
        {   //  OOPS!
            throw tt3::db::api::DatabaseCorruptException(_database->_address);
        }
        Q_ASSERT(false);    //  TODO properly
        //  TODO there must be no parent/child loops
    }
    for (PublicTask * child : _children)
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

//  End of tt3-db-xml/User.cpp
