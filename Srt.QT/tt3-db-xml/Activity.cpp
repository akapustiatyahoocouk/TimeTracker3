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
        tt3::db::api::Oid oid)
    :   Object(database, oid)
{
}

Activity::~Activity()
{
}

//////////
//  tt3::db::api::IActivity (general)
auto Activity::displayName(
    ) const -> QString
{
    tt3::util::Lock lock(_database->_guard);
    _ensureLive();  //  may throw
    //  We assume database is consistent since last change

    return _displayName;
}

void Activity::setDisplayName(
        const QString & displayName
    )
{
    tt3::util::Lock lock(_database->_guard);
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

auto Activity::description(
    ) const -> QString
{
    tt3::util::Lock lock(_database->_guard);
    _ensureLive();  //  may throw
    //  We assume database is consistent since last change

    return _description;
}

void Activity::setDescription(
        const QString & description
    )
{
    tt3::util::Lock lock(_database->_guard);
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
    tt3::util::Lock lock(_database->_guard);
    _ensureLive();  //  may throw
    //  We assume database is consistent since last change

    return _timeout;
}

void Activity::setTimeout(
        const tt3::db::api::InactivityTimeout & timeout
    )
{
    tt3::util::Lock lock(_database->_guard);
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
    tt3::util::Lock lock(_database->_guard);
    _ensureLive();  //  may throw
    //  We assume database is consistent since last change

    return _requireCommentOnStart;
}

void Activity::setRequireCommentOnStart(
        bool requireCommentOnStart
    )
{
    tt3::util::Lock lock(_database->_guard);
    _ensureLiveAndWritable();   //  may throw
#ifdef Q_DEBUG
    _database->_validate(); //  may throw
#endif

    //  Validate parameters
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

bool Activity::requireCommentOnFinish(
    ) const
{
    tt3::util::Lock lock(_database->_guard);
    _ensureLive();  //  may throw
    //  We assume database is consistent since last change

    return _requireCommentOnFinish;
}

void Activity::setRequireCommentOnFinish(
        bool requireCommentOnFinish
    )
{
    tt3::util::Lock lock(_database->_guard);
    _ensureLiveAndWritable();   //  may throw
#ifdef Q_DEBUG
    _database->_validate(); //  may throw
#endif

    if (requireCommentOnFinish != _requireCommentOnFinish)
    {   //  Make the change...
        _requireCommentOnFinish = requireCommentOnFinish;
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
    tt3::util::Lock lock(_database->_guard);
    _ensureLive();  //  may throw
    //  We assume database is consistent since last change

    return _fullScreenReminder;
}

void Activity::setFullScreenReminder(
        bool fullScreenReminder
    )
{
    tt3::util::Lock lock(_database->_guard);
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
    tt3::util::Lock lock(_database->_guard);
    _ensureLive();  //  may throw
    //  We assume database is consistent since last change

    return _activityType;
}

void Activity::setActivityType(
        tt3::db::api::IActivityType * activityType
    )
{
    tt3::util::Lock lock(_database->_guard);
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
    tt3::util::Lock lock(_database->_guard);
    _ensureLive();  //  may throw
    //  We assume database is consistent since last change

    return _workload;
}

void Activity::setWorkload(
        tt3::db::api::IWorkload * workload
    )
{
    tt3::util::Lock lock(_database->_guard);
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
    throw tt3::util::NotImplementedError();
}

auto Activity::events(
    ) const -> tt3::db::api::Events
{
    throw tt3::util::NotImplementedError();
}

//////////
//  Serialization
void Activity::_serializeProperties(
        QDomElement & objectElement
    )
{
    Object::_serializeProperties(objectElement);

    objectElement.setAttribute("DisplayName", _displayName);
    objectElement.setAttribute("Description", _description);
    if (_timeout.has_value())
    {
        objectElement.setAttribute("Timeout", tt3::util::toString(_timeout.value()));
    }
    objectElement.setAttribute("RequireCommentOnStart", tt3::util::toString(_requireCommentOnStart));
    objectElement.setAttribute("RequireCommentOnFinish", tt3::util::toString(_requireCommentOnFinish));
    objectElement.setAttribute("FullScreenReminder", tt3::util::toString(_fullScreenReminder));
}

void Activity::_serializeAggregations(
        QDomElement & objectElement
    )
{
    Object::_serializeAggregations(objectElement);
}

void Activity::_serializeAssociations(
        QDomElement & objectElement
    )
{
    Object::_serializeAssociations(objectElement);

    if (_activityType != nullptr)
    {
        objectElement.setAttribute("ActivityType", tt3::util::toString(_activityType->_oid));
    }
    //  TODO    Workload *      _workload = nullptr;    //  counts as "references" uness nullptr
    //  TODO    Works           _works;     //  count as "references"
    //  TODO    Events          _events;    //  count as "references"
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
    _requireCommentOnStart = tt3::util::fromString<bool>(objectElement.attribute("RequireCommentOnStart"));
    _requireCommentOnFinish = tt3::util::fromString<bool>(objectElement.attribute("RequireCommentOnFinish"));
    _fullScreenReminder = tt3::util::fromString<bool>(objectElement.attribute("FullScreenReminder"));
}

void Activity::_deserializeAggregations(
        const QDomElement & objectElement
    )
{
    Object::_deserializeAggregations(objectElement);
}

void Activity::_deserializeAssociations(
        const QDomElement & objectElement
    )
{
    Object::_deserializeAssociations(objectElement);

    if (objectElement.hasAttribute("ActivityType"))
    {
        _activityType =
            _database->_getObject<ActivityType*>(
                tt3::util::fromString<tt3::db::api::Oid>(
                    objectElement.attribute("ActivityType")));
        _activityType->addReference();
    }
    //  TODO    Workload *      _workload = nullptr;    //  counts as "references" uness nullptr
    //  TODO    Works           _works;     //  count as "references"
    //  TODO    Events          _events;    //  count as "references"
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
    //  TODO works, events
}

//  End of tt3-db-xml/Activity.cpp
