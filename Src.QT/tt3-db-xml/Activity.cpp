//
//  tt3-db-xml/Activity.cpp - tt3::db::xml::Activity class implementation
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
Activity::Activity(
        Database * database,
        tt3::db::api::Oid oid
    ) : Object(database, oid)
{
}

Activity::~Activity()
{
}

//////////
//  tt3::db::api::IActivity (general)
QString Activity::displayName() const
{
    tt3::util::Lock _(_database->_guard);
    _ensureLive();  //  may throw
    //  We assume database is consistent since last change

    return _displayName;
}

void Activity::setDisplayName(
        const QString & displayName
    )
{
    tt3::util::Lock _(_database->_guard);
    _ensureLiveAndWritable();   //  may throw
#ifdef Q_DEBUG
    _database->_validate(); //  may throw
#endif

    //  Validate parameters
    if (!_database->_validator->activity()->isValidDisplayName(displayName))
    {
        throw tt3::db::api::InvalidPropertyValueException(
            type(),
            "displayName",
            displayName);
    }
    if (displayName != _displayName)
    {   //  Make the change (but no duplication)...
        if (_siblingExists(displayName))
        {
            throw tt3::db::api::AlreadyExistsException(
                type(),
                "displayName",
                displayName);
        }
        _displayName = displayName;
        _database->_markModified();
        //  ...schedule change notifications...
        _database->_changeNotifier.post(
            new tt3::db::api::ObjectModifiedNotification(
                _database, type(), _oid));
        //  ...and we're done
#ifdef Q_DEBUG
        _database->_validate(); //  may throw
#endif
    }
}

QString Activity::description() const
{
    tt3::util::Lock _(_database->_guard);
    _ensureLive();  //  may throw
    //  We assume database is consistent since last change

    return _description;
}

void Activity::setDescription(
        const QString & description
    )
{
    tt3::util::Lock _(_database->_guard);
    _ensureLiveAndWritable();   //  may throw
#ifdef Q_DEBUG
    _database->_validate(); //  may throw
#endif

    //  Validate parameters
    if (!_database->_validator->activity()->isValidDescription(description))
    {
        throw tt3::db::api::InvalidPropertyValueException(
            type(),
            "description",
            description);
    }
    if (description != _description)
    {   //  Make the change...
        _description = description;
        _database->_markModified();
        //  ...schedule change notifications...
        _database->_changeNotifier.post(
            new tt3::db::api::ObjectModifiedNotification(
                _database, type(), _oid));
        //  ...and we're done
#ifdef Q_DEBUG
        _database->_validate(); //  may throw
#endif
    }
}

auto Activity::timeout(
    ) const -> tt3::db::api::InactivityTimeout
{
    tt3::util::Lock _(_database->_guard);
    _ensureLive();  //  may throw
    //  We assume database is consistent since last change

    return _timeout;
}

void Activity::setTimeout(
        const tt3::db::api::InactivityTimeout & timeout
    )
{
    tt3::util::Lock _(_database->_guard);
    _ensureLiveAndWritable();   //  may throw
#ifdef Q_DEBUG
    _database->_validate(); //  may throw
#endif

    //  Validate parameters
    if (timeout.has_value() &&
        !_database->_validator->activity()->isValidTimeout(timeout.value()))
    {
        throw tt3::db::api::InvalidPropertyValueException(
            type(),
            "timeout",
            timeout.value());
    }
    if (timeout != _timeout)
    {   //  Make the change...
        _timeout = timeout;
        _database->_markModified();
        //  ...schedule change notifications...
        _database->_changeNotifier.post(
            new tt3::db::api::ObjectModifiedNotification(
                _database, type(), _oid));
        //  ...and we're done
#ifdef Q_DEBUG
        _database->_validate(); //  may throw
#endif
    }
}

bool Activity::requireCommentOnStart(
    ) const
{
    tt3::util::Lock _(_database->_guard);
    _ensureLive();  //  may throw
    //  We assume database is consistent since last change

    return _requireCommentOnStart;
}

void Activity::setRequireCommentOnStart(
        bool requireCommentOnStart
    )
{
    tt3::util::Lock _(_database->_guard);
    _ensureLiveAndWritable();   //  may throw
#ifdef Q_DEBUG
    _database->_validate(); //  may throw
#endif

    if (requireCommentOnStart != _requireCommentOnStart)
    {   //  Make the change...
        _requireCommentOnStart = requireCommentOnStart;
        _database->_markModified();
        //  ...schedule change notifications...
        _database->_changeNotifier.post(
            new tt3::db::api::ObjectModifiedNotification(
                _database, type(), _oid));
        //  ...and we're done
#ifdef Q_DEBUG
        _database->_validate(); //  may throw
#endif
    }
}

bool Activity::requireCommentOnStop(
    ) const
{
    tt3::util::Lock _(_database->_guard);
    _ensureLive();  //  may throw
    //  We assume database is consistent since last change

    return _requireCommentOnStop;
}

void Activity::setRequireCommentOnStop(
        bool requireCommentOnStop
    )
{
    tt3::util::Lock _(_database->_guard);
    _ensureLiveAndWritable();   //  may throw
#ifdef Q_DEBUG
    _database->_validate(); //  may throw
#endif

    if (requireCommentOnStop != _requireCommentOnStop)
    {   //  Make the change...
        _requireCommentOnStop = requireCommentOnStop;
        _database->_markModified();
        //  ...schedule change notifications...
        _database->_changeNotifier.post(
            new tt3::db::api::ObjectModifiedNotification(
                _database, type(), _oid));
        //  ...and we're done
#ifdef Q_DEBUG
        _database->_validate(); //  may throw
#endif
    }
}

bool Activity::fullScreenReminder(
    ) const
{
    tt3::util::Lock _(_database->_guard);
    _ensureLive();  //  may throw
    //  We assume database is consistent since last change

    return _fullScreenReminder;
}

void Activity::setFullScreenReminder(
        bool fullScreenReminder
    )
{
    tt3::util::Lock _(_database->_guard);
    _ensureLiveAndWritable();   //  may throw
#ifdef Q_DEBUG
    _database->_validate(); //  may throw
#endif

    if (fullScreenReminder != _fullScreenReminder)
    {   //  Make the change...
        _fullScreenReminder = fullScreenReminder;
        _database->_markModified();
        //  ...schedule change notifications...
        _database->_changeNotifier.post(
            new tt3::db::api::ObjectModifiedNotification(
                _database, type(), _oid));
        //  ...and we're done
#ifdef Q_DEBUG
        _database->_validate(); //  may throw
#endif
    }
}

//////////
//  tt3::db::api::IActivity (associations)
auto Activity::activityType(
    ) const -> tt3::db::api::IActivityType *
{
    tt3::util::Lock _(_database->_guard);
    _ensureLive();  //  may throw
    //  We assume database is consistent since last change

    return _activityType;
}

void Activity::setActivityType(
        tt3::db::api::IActivityType * activityType
    )
{
    tt3::util::Lock _(_database->_guard);
    _ensureLiveAndWritable();   //  may throw
#ifdef Q_DEBUG
    _database->_validate(); //  may throw
#endif

    ActivityType * xmlActivityType = nullptr;
    if (activityType != nullptr)
    {
        xmlActivityType = dynamic_cast<ActivityType*>(activityType);
        if (xmlActivityType == nullptr ||
            !xmlActivityType->_isLive ||
            xmlActivityType->_database != this->_database)
        {   //  OOPS!
            throw tt3::db::api::IncompatibleInstanceException(activityType->type());
        }
    }
    if (xmlActivityType != _activityType)
    {   //  Make the change...
        if (_activityType != nullptr)
        {
            _activityType->_activities.remove(this);
            this->removeReference();
            _activityType->removeReference();
            _database->_changeNotifier.post(
                new tt3::db::api::ObjectModifiedNotification(
                    _database, _activityType->type(), _activityType->_oid));
        }
        _activityType = xmlActivityType;
        if (_activityType != nullptr)
        {
            _activityType->_activities.insert(this);
            this->addReference();
            _activityType->addReference();
            _database->_changeNotifier.post(
                new tt3::db::api::ObjectModifiedNotification(
                    _database, _activityType->type(), _activityType->_oid));
        }
        _database->_markModified();
        //  ...schedule change notifications...
        _database->_changeNotifier.post(
            new tt3::db::api::ObjectModifiedNotification(
                _database, type(), _oid));
        //  ...and we're done
#ifdef Q_DEBUG
        _database->_validate(); //  may throw
#endif
    }
}

auto Activity::workload(
    ) const -> tt3::db::api::IWorkload *
{
    tt3::util::Lock _(_database->_guard);
    _ensureLive();  //  may throw
    //  We assume database is consistent since last change

    return _workload;
}

void Activity::setWorkload(
        tt3::db::api::IWorkload * workload
    )
{
    tt3::util::Lock _(_database->_guard);
    _ensureLiveAndWritable();   //  may throw
#ifdef Q_DEBUG
    _database->_validate(); //  may throw
#endif

    Workload * xmlWorkload = nullptr;
    if (workload != nullptr)
    {
        xmlWorkload = dynamic_cast<Workload*>(workload);
        if (xmlWorkload == nullptr ||
            !xmlWorkload->_isLive ||
            xmlWorkload->_database != this->_database)
        {   //  OOPS!
            throw tt3::db::api::IncompatibleInstanceException(workload->type());
        }
    }
    if (xmlWorkload != _workload)
    {   //  Make the change...
        if (_workload != nullptr)
        {
            _workload->_contributingActivities.remove(this);
            this->removeReference();
            _workload->removeReference();
            _database->_changeNotifier.post(
                new tt3::db::api::ObjectModifiedNotification(
                    _database, _workload->type(), _workload->_oid));
        }
        _workload = xmlWorkload;
        if (_workload != nullptr)
        {
            _workload->_contributingActivities.insert(this);
            this->addReference();
            _workload->addReference();
            _database->_changeNotifier.post(
                new tt3::db::api::ObjectModifiedNotification(
                    _database, _workload->type(), _workload->_oid));
        }
        _database->_markModified();
        //  ...schedule change notifications...
        _database->_changeNotifier.post(
            new tt3::db::api::ObjectModifiedNotification(
                _database, type(), _oid));
        //  ...and we're done
#ifdef Q_DEBUG
        _database->_validate(); //  may throw
#endif
    }
}

auto Activity::works(
    ) const -> tt3::db::api::Works
{
    tt3::util::Lock _(_database->_guard);
    _ensureLive();  //  may throw
    //  We assume database is consistent since last change

    return tt3::db::api::Works(_works.cbegin(), _works.cend());
}

auto Activity::events(
    ) const -> tt3::db::api::Events
{
    tt3::util::Lock _(_database->_guard);
    _ensureLive();  //  may throw
    //  We assume database is consistent since last change

    return tt3::db::api::Events(_events.cbegin(), _events.cend());
}

//////////
//  Implementation helpers
void Activity::_makeDead()
{
    Q_ASSERT(_database->_guard.isLockedByCurrentThread());
    Q_ASSERT(_isLive);

    //  Destroy aggregated objects
    for (Work * work : _works.values())
    {
        work->destroy();
    }
    Q_ASSERT(_works.isEmpty());
    for (Event * event : _events.values())
    {
        event->destroy();
    }
    Q_ASSERT(_events.isEmpty());

    //  Break associations
    if (_activityType != nullptr)
    {
        Q_ASSERT(_activityType->_activities.contains(this));
        _activityType->_activities.remove(this);
        this->removeReference();
        _activityType->removeReference();
        _activityType = nullptr;
    }
    if (_workload != nullptr)
    {
        Q_ASSERT(_workload->_contributingActivities.contains(this));
        _workload->_contributingActivities.remove(this);
        this->removeReference();
        _workload->removeReference();
        _workload = nullptr;
    }
    //  Remove from all "quick pick" lists
    for (User * user : qAsConst(_database->_users))
    {
        for (Account * account : qAsConst(user->_accounts))
        {
            if (account->_quickPicksList.contains(this))
            {
                Q_ASSERT(account->_quickPicksList.count(this) == 1);
                account->_quickPicksList.removeOne(this);
                this->removeReference();
                //  The association is one-directional!
            }
        }
    }

    //  The rest is up to the base class
    Object::_makeDead();
}

//////////
//  Serialization
void Activity::_serializeProperties(
        QDomElement & objectElement
    ) const
{
    Object::_serializeProperties(objectElement);

    objectElement.setAttribute("DisplayName", _displayName);
    objectElement.setAttribute("Description", _description);
    if (_timeout.has_value())
    {
        objectElement.setAttribute("Timeout", tt3::util::toString(_timeout.value()));
    }
    objectElement.setAttribute("RequireCommentOnStart", tt3::util::toString(_requireCommentOnStart));
    objectElement.setAttribute("RequireCommentOnStop", tt3::util::toString(_requireCommentOnStop));
    objectElement.setAttribute("FullScreenReminder", tt3::util::toString(_fullScreenReminder));
}

void Activity::_serializeAggregations(
        QDomElement & objectElement
    ) const
{
    Object::_serializeAggregations(objectElement);
    //  Works and Events are serialized via Account
}

void Activity::_serializeAssociations(
        QDomElement & objectElement
    ) const
{
    Object::_serializeAssociations(objectElement);

    _database->_serializeAssociation(
        objectElement,
        "ActivityType",
        _activityType);
    _database->_serializeAssociation(
        objectElement,
        "Workload",
        _workload);
    _database->_serializeAssociation(
        objectElement,
        "Works",
        _works);
    _database->_serializeAssociation(
        objectElement,
        "Events",
        _events);
}

void Activity::_deserializeProperties(
        const QDomElement & objectElement
    )
{
    Object::_deserializeProperties(objectElement);

    _displayName = objectElement.attribute("DisplayName");
    _description = objectElement.attribute("Description");
    if (objectElement.hasAttribute("Timeout"))
    {
        _timeout =
            tt3::util::fromString<tt3::util::TimeSpan>(
                objectElement.attribute("Timeout"));
    }
    _requireCommentOnStart =
        tt3::util::fromString(
            objectElement.attribute("RequireCommentOnStart"),
            _requireCommentOnStart);
    _requireCommentOnStop =
        tt3::util::fromString(
            objectElement.attribute("RequireCommentOnStop"),
            _requireCommentOnStop);
    _fullScreenReminder =
        tt3::util::fromString(
            objectElement.attribute("FullScreenReminder"),
            _fullScreenReminder);
}

void Activity::_deserializeAggregations(
        const QDomElement & objectElement
    )
{
    Object::_deserializeAggregations(objectElement);
    //  Works and Events are deserialized via Account
}

void Activity::_deserializeAssociations(
        const QDomElement & objectElement
    )
{
    Object::_deserializeAssociations(objectElement);

    _database->_deserializeAssociation(
        objectElement,
        "ActivityType",
        _activityType);
    _database->_deserializeAssociation(
        objectElement,
        "Workload",
        _workload);
    _database->_deserializeAssociation(
        objectElement,
        "Works",
        _works);
    _database->_deserializeAssociation(
        objectElement,
        "Events",
        _events);
}

//////////
//  Validation
void Activity::_validate(
        Objects & validatedObjects
    )
{
    Object::_validate(validatedObjects);

    //  Validate properties
    if (!_database->_validator->activity()->isValidDisplayName(_displayName))
    {   //  OOPS!
        throw tt3::db::api::DatabaseCorruptException(_database->_address);
    }
    if (!_database->_validator->activity()->isValidDescription(_description))
    {   //  OOPS!
        throw tt3::db::api::DatabaseCorruptException(_database->_address);
    }
    if (_timeout.has_value() &&
        !_database->_validator->activity()->isValidTimeout(_timeout.value()))
    {   //  OOPS!
        throw tt3::db::api::DatabaseCorruptException(_database->_address);
    }

    //  Validate aggregations

    //  Validate associations
    if (_activityType != nullptr)
    {
        if (_activityType->_database != this->_database ||
            !_activityType->_isLive ||
            !_activityType->_activities.contains(this))
        {   //  OOPS!
            throw tt3::db::api::DatabaseCorruptException(_database->_address);
        }
    }
    if (_workload != nullptr)
    {
        if (_workload->_database != this->_database ||
            !_workload->_isLive ||
            !_workload->_contributingActivities.contains(this))
        {   //  OOPS!
            throw tt3::db::api::DatabaseCorruptException(_database->_address);
        }
    }
    for (Work * work : qAsConst(_works))
    {
        if (work == nullptr || !work->_isLive ||
            work->_database != _database ||
            work->_activity != this)
        {   //  OOPS!
            throw tt3::db::api::DatabaseCorruptException(_database->_address);
        }
        //  Works themselves are validated via Account
    }
    for (Event * event : qAsConst(_events))
    {
        if (event == nullptr || !event->_isLive ||
            event->_database != _database ||
            !event->_activities.contains(this))
        {   //  OOPS!
            throw tt3::db::api::DatabaseCorruptException(_database->_address);
        }
        //  Events themselves are validated via Account
    }
}

//  End of tt3-db-xml/Activity.cpp
