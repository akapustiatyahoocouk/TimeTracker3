//
//  tt3-db-xml/User.cpp - tt3::db::xml::User class implementation
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
User::User(Database * database, tt3::db::api::Oid oid)
    :   Principal(database, oid)
{
    //  Register User with parent
    _database->_users.insert(this);
    this->addReference();
}

User::~User()
{
    Q_ASSERT(!_database->_users.contains(this));
    Q_ASSERT(_accounts.isEmpty());
    Q_ASSERT(_permittedWorkloads.isEmpty());
}

//////////
//  tt3::db::api::IUser (properties)
QString User::realName() const
{
    tt3::util::Lock _(_database->_guard);
    _ensureLive();  //  may throw
    //  We assume database is consistent since last change

    return _realName;
}

void User::setRealName(
        const QString & realName
    )
{
    tt3::util::Lock _(_database->_guard);
    _ensureLiveAndWritable();   //  may throw
#ifndef Q_NODEBUG
    _database->_validate(); //  may throw
#endif

    //  Validate parameters
    if (!_database->_validator->user()->isValidRealName(realName))
    {
        throw tt3::db::api::InvalidPropertyValueException(
            type(),
            "realName",
            realName);
    }
    if (realName != _realName)
    {   //  Make the change...
        _realName = realName;
        _database->_markModified();
        //  ...schedule change notifications....
        _database->_changeNotifier.post(
            new tt3::db::api::ObjectModifiedNotification(
                _database, type(), _oid));
        //  ...and we're done
#ifndef Q_NODEBUG
        _database->_validate(); //  may throw
#endif
    }
}

auto User::inactivityTimeout(
    ) const -> tt3::db::api::InactivityTimeout
{
    tt3::util::Lock _(_database->_guard);
    _ensureLive();  //  may throw
    //  We assume database is consistent since last change

    return _inactivityTimeout;
}

void User::setInactivityTimeout(
        const tt3::db::api::InactivityTimeout & inactivityTimeout
    )
{
    tt3::util::Lock _(_database->_guard);
    _ensureLiveAndWritable();   //  may throw
#ifndef Q_NODEBUG
    _database->_validate(); //  may throw
#endif

    //  Validate parameters
    if (inactivityTimeout.has_value() &&
        !_database->_validator->user()->isValidInactivityTimeout(inactivityTimeout.value()))
    {
        throw tt3::db::api::InvalidPropertyValueException(
            type(),
            "inactivityTimeout",
            inactivityTimeout.value());
    }
    if (inactivityTimeout != _inactivityTimeout)
    {   //  Make the change...
        _inactivityTimeout = inactivityTimeout;
        _database->_markModified();
        //  ...schedule change notifications...
        _database->_changeNotifier.post(
            new tt3::db::api::ObjectModifiedNotification(
                _database, type(), _oid));
        //  ...and we're done
#ifndef Q_NODEBUG
        _database->_validate(); //  may throw
#endif
    }
}

auto User::uiLocale(
    ) const -> tt3::db::api::UiLocale
{
    tt3::util::Lock _(_database->_guard);
    _ensureLive();  //  may throw
    //  We assume database is consistent since last change

    return _uiLocale;
}

void User::setUiLocale(
        const tt3::db::api::UiLocale & uiLocale
    )
{
    tt3::util::Lock _(_database->_guard);
    _ensureLiveAndWritable();   //  may throw
#ifndef Q_NODEBUG
    _database->_validate(); //  may throw
#endif

    //  Validate parameters
    if (uiLocale.has_value() &&
        !_database->_validator->user()->isValidUiLocale(uiLocale.value()))
    {
        throw tt3::db::api::InvalidPropertyValueException(
            type(),
            "uiLocale",
            uiLocale.value());
    }
    if (uiLocale != _uiLocale)
    {   //  Make the change...
        _uiLocale = uiLocale;
        _database->_markModified();
        //  ...schedule change notifications...
        _database->_changeNotifier.post(
            new tt3::db::api::ObjectModifiedNotification(
                _database, type(), _oid));
        //  ...and we're done
#ifndef Q_NODEBUG
        _database->_validate(); //  may throw
#endif
    }
}

//////////
//  tt3::db::api::IUser (associations)
auto User::accounts(
    ) const -> tt3::db::api::Accounts
{
    tt3::util::Lock _(_database->_guard);
    _ensureLive();  //  may throw
    //  We assume database is consistent since last change

    return tt3::db::api::Accounts(_accounts.cbegin(), _accounts.cend());
}

auto User::privateActivities(
    ) const -> tt3::db::api::PrivateActivities
{
    tt3::util::Lock _(_database->_guard);
    _ensureLive();  //  may throw
    //  We assume database is consistent since last change

    return tt3::db::api::PrivateActivities(_privateActivities.cbegin(), _privateActivities.cend());
}

auto User::privateActivitiesAndTasks(
    ) const -> tt3::db::api::PrivateActivities
{
    tt3::util::Lock _(_database->_guard);
    _ensureLive();  //  may throw
    //  We assume database is consistent since last change

    PrivateTasks closure;
    _collectPrivateTasksClosure(closure, _rootPrivateTasks);
    return
        tt3::db::api::PrivateActivities(
            _privateActivities.cbegin(), _privateActivities.cend()) +
        tt3::db::api::PrivateActivities(
            closure.cbegin(), closure.cend());
}

auto User::privateTasks(
    ) const -> tt3::db::api::PrivateTasks
{
    tt3::util::Lock _(_database->_guard);
    _ensureLive();  //  may throw
    //  We assume database is consistent since last change

    PrivateTasks closure;
    _collectPrivateTasksClosure(closure, _rootPrivateTasks);
    return tt3::db::api::PrivateTasks(closure.cbegin(), closure.cend());
}

auto User::rootPrivateTasks(
    ) const -> tt3::db::api::PrivateTasks
{
    tt3::util::Lock _(_database->_guard);
    _ensureLive();  //  may throw
    //  We assume database is consistent since last change

    return tt3::db::api::PrivateTasks(_rootPrivateTasks.cbegin(), _rootPrivateTasks.cend());
}

auto User::permittedWorkloads(
    ) const -> tt3::db::api::Workloads
{
    tt3::util::Lock _(_database->_guard);
    _ensureLive();  //  may throw
    //  We assume database is consistent since last change

    return tt3::db::api::Workloads(_permittedWorkloads.cbegin(), _permittedWorkloads.cend());
}

void User::setPermittedWorkloads(
        const tt3::db::api::Workloads & workloads
    )
{
    tt3::util::Lock _(_database->_guard);
    _ensureLiveAndWritable();   //  may throw
#ifndef Q_NODEBUG
    _database->_validate(); //  may throw
#endif

    //  Validate parameters
    if (workloads.contains(nullptr))
    {
        throw tt3::db::api::InvalidPropertyValueException(
            type(),
            "workloads",
            nullptr);
    }

    Workloads xmlWorkloads =
        tt3::util::transform(
            workloads,
            [&](auto w)
            {
                Q_ASSERT(w != nullptr); //  should have been caught earlier!
                auto xmlWorkload = dynamic_cast<Workload*>(w);
                if (xmlWorkload == nullptr ||
                    xmlWorkload->_database != this->_database ||
                    !xmlWorkload->_isLive)
                {   //  OOPS!
                    throw tt3::db::api::IncompatibleInstanceException(w->type());
                }
                return xmlWorkload;
            });
    if (xmlWorkloads != _permittedWorkloads)
    {   //  Make the changes
        Workloads addedWorkloads = xmlWorkloads - _permittedWorkloads;
        Workloads removedWorkloads = _permittedWorkloads - xmlWorkloads;
        //  link the added workloads...
        for (Workload * xmlWorkload : std::as_const(addedWorkloads))
        {
            _permittedWorkloads.insert(xmlWorkload);
            xmlWorkload->_assignedUsers.insert(this);
            xmlWorkload->addReference();
            this->addReference();
        }
        //  ...un-link the removed workloads...
        for (Workload * xmlWorkload : std::as_const(removedWorkloads))
        {
            _permittedWorkloads.remove(xmlWorkload);
            xmlWorkload->_assignedUsers.remove(this);
            xmlWorkload->removeReference();
            this->removeReference();
        }
        //  ...ensure the changes are saved...
        _database->_markModified();
        //  ...schedule change notifications...
        _database->_changeNotifier.post(
            new tt3::db::api::ObjectModifiedNotification(
                _database, type(), _oid));
        for (Workload * xmlWorkload : addedWorkloads + removedWorkloads)
        {
            _database->_changeNotifier.post(
                new tt3::db::api::ObjectModifiedNotification(
                    _database, xmlWorkload->type(), xmlWorkload->_oid));
        }
        //  ...and we're done
#ifndef Q_NODEBUG
        _database->_validate(); //  may throw
#endif
    }
}

void User::addPermittedWorkload(
        tt3::db::api::IWorkload * workload
    )
{
    tt3::util::Lock _(_database->_guard);
    _ensureLiveAndWritable();   //  may throw
#ifndef Q_NODEBUG
    _database->_validate(); //  may throw
#endif

    //  Validate parameters
    if (workload == nullptr)
    {
        throw tt3::db::api::InvalidPropertyValueException(
            type(),
            "workload",
            nullptr);
    }

    Workload * xmlWorkload = dynamic_cast<Workload*>(workload);
    if (xmlWorkload == nullptr ||
        xmlWorkload->_database != this->_database ||
        !xmlWorkload->_isLive)
    {   //  OOPS!
        throw tt3::db::api::IncompatibleInstanceException(workload->type());
    }
    if (!_permittedWorkloads.contains(xmlWorkload))
    {   //  Make the changes
        Q_ASSERT(!xmlWorkload->_assignedUsers.contains(this));
        _permittedWorkloads.insert(xmlWorkload);
        xmlWorkload->_assignedUsers.insert(this);
        xmlWorkload->addReference();
        this->addReference();
        //  ...ensure the changes are saved...
        _database->_markModified();
        //  ...schedule change notifications...
        _database->_changeNotifier.post(
            new tt3::db::api::ObjectModifiedNotification(
                _database, type(), _oid));
        _database->_changeNotifier.post(
            new tt3::db::api::ObjectModifiedNotification(
                _database, xmlWorkload->type(), xmlWorkload->_oid));
        //  ...and we're done
#ifndef Q_NODEBUG
        _database->_validate(); //  may throw
#endif
    }
}

void User::removePermittedWorkload(
        tt3::db::api::IWorkload * workload
    )
{
    tt3::util::Lock _(_database->_guard);
    _ensureLiveAndWritable();   //  may throw
#ifndef Q_NODEBUG
    _database->_validate(); //  may throw
#endif

    //  Validate parameters
    if (workload == nullptr)
    {
        throw tt3::db::api::InvalidPropertyValueException(
            type(),
            "workload",
            nullptr);
    }

    Workload * xmlWorkload = dynamic_cast<Workload*>(workload);
    if (xmlWorkload == nullptr ||
        xmlWorkload->_database != this->_database ||
        !xmlWorkload->_isLive)
    {   //  OOPS!
        throw tt3::db::api::IncompatibleInstanceException(workload->type());
    }
    if (_permittedWorkloads.contains(xmlWorkload))
    {   //  Make the changes
        Q_ASSERT(xmlWorkload->_assignedUsers.contains(this));
        _permittedWorkloads.remove(xmlWorkload);
        xmlWorkload->_assignedUsers.remove(this);
        xmlWorkload->removeReference();
        this->removeReference();
        //  ...ensure the changes are saved...
        _database->_markModified();
        //  ...schedule change notifications...
        _database->_changeNotifier.post(
            new tt3::db::api::ObjectModifiedNotification(
                _database, type(), _oid));
        _database->_changeNotifier.post(
            new tt3::db::api::ObjectModifiedNotification(
                _database, xmlWorkload->type(), xmlWorkload->_oid));
        //  ...and we're done
#ifndef Q_NODEBUG
        _database->_validate(); //  may throw
#endif
    }
}

//////////
//  tt3::db::api::IUser  (life cycle)
auto User::createAccount(
        bool enabled,
        const QStringList & emailAddresses,
        const QString & login,
        const QString & password,
        tt3::db::api::Capabilities capabilities
    ) -> tt3::db::api::IAccount *
{
    tt3::util::Lock _(_database->_guard);
    _ensureLiveAndWritable();   //  may throw
#ifndef Q_NODEBUG
    _database->_validate(); //  may throw
#endif

    //  Validate parameters
    if (!_database->_validator->principal()->isValidEmailAddresses(emailAddresses))
    {
        throw tt3::db::api::InvalidPropertyValueException(
            tt3::db::api::ObjectTypes::Account::instance(),
            "emailAddresses",
            emailAddresses.join(','));
    }
    if (!_database->_validator->account()->isValidLogin(login))
    {
        throw tt3::db::api::InvalidPropertyValueException(
            tt3::db::api::ObjectTypes::Account::instance(),
            "login",
            login);
    }
    if (!_database->_validator->account()->isValidPassword(password))
    {
        throw tt3::db::api::InvalidPropertyValueException(
            tt3::db::api::ObjectTypes::Account::instance(),
            "password",
            password);
    }

    //  Logins must be unique per database
    if (_database->_findAccount(login) != nullptr)
    {   //  OOPS! Already there!
        throw tt3::db::api::AlreadyExistsException(
            tt3::db::api::ObjectTypes::Account::instance(),
            "login",
            login);
    }

    //  Do the work - create & initialize the Account...
    std::unique_ptr<tt3::util::IMessageDigest::Builder> digestBuilder
        { tt3::util::StandardMessageDigests::Sha1::instance()->createBuilder() };
    digestBuilder->digestFragment(password);
    QString passwordHash = digestBuilder->digestAsString();

    Account * account = new Account(this, _database->_generateOid()); //  registers with User
    account->_enabled = enabled;
    account->_emailAddresses = emailAddresses;
    account->_login = login;
    account->_passwordHash = passwordHash;
    account->_capabilities = capabilities;
    _database->_markModified();
    //  ...schedule change notifications...
    _database->_changeNotifier.post(
        new tt3::db::api::ObjectModifiedNotification(
            _database, type(), _oid));
    _database->_changeNotifier.post(
        new tt3::db::api::ObjectCreatedNotification(
            _database, account->type(), account->_oid));
    //  ...and we're done
#ifndef Q_NODEBUG
    _database->_validate(); //  may throw
#endif
    return account;
}

auto User::createPrivateActivity(
        const QString & displayName,
        const QString & description,
        const tt3::db::api::InactivityTimeout & timeout,
        bool requireCommentOnStart,
        bool requireCommentOnStop,
        bool fullScreenReminder,
        tt3::db::api::IActivityType * activityType,
        tt3::db::api::IWorkload * workload
    ) -> tt3::db::api::IPrivateActivity *
{
    tt3::util::Lock _(_database->_guard);
    _ensureLiveAndWritable();   //  may throw
#ifndef Q_NODEBUG
    _database->_validate(); //  may throw
#endif

    //  Validate parameters
    if (!_database->_validator->privateActivity()->isValidDisplayName(displayName))
    {
        throw tt3::db::api::InvalidPropertyValueException(
            tt3::db::api::ObjectTypes::PrivateActivity::instance(),
            "displayName",
            displayName);
    }
    if (!_database->_validator->privateActivity()->isValidDescription(description))
    {
        throw tt3::db::api::InvalidPropertyValueException(
            tt3::db::api::ObjectTypes::PrivateActivity::instance(),
            "description",
            description);
    }
    if (timeout.has_value() &&
        !_database->_validator->privateActivity()->isValidTimeout(timeout))
    {
        throw tt3::db::api::InvalidPropertyValueException(
            tt3::db::api::ObjectTypes::PrivateActivity::instance(),
            "timeout",
            timeout.value());
    }
    ActivityType * xmlActivityType = nullptr;
    if (activityType != nullptr)
    {
        xmlActivityType = dynamic_cast<ActivityType*>(activityType);
        if (xmlActivityType == nullptr ||
            xmlActivityType->_database != this->_database ||
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
            xmlWorkload->_database != this->_database ||
            !xmlWorkload->_isLive)
        {   //  OOPS!
            throw tt3::db::api::IncompatibleInstanceException(workload->type());
        }
    }

    //  Display names must be unique
    if (_findPrivateActivity(displayName) != nullptr)
    {   //  OOPS!
        throw tt3::db::api::AlreadyExistsException(
            tt3::db::api::ObjectTypes::PrivateActivity::instance(),
            "displayName",
            displayName);
    }

    //  Do the work - create & initialize the PrivateActivity...
    PrivateActivity * privateActivity =
        new PrivateActivity(this, _database->_generateOid());   //  registers with User
    privateActivity->_displayName = displayName;
    privateActivity->_description = description;
    privateActivity->_timeout = timeout;
    privateActivity->_requireCommentOnStart = requireCommentOnStart;
    privateActivity->_requireCommentOnStop = requireCommentOnStop;
    privateActivity->_fullScreenReminder = fullScreenReminder;
    if (xmlActivityType != nullptr)
    {   //  Link with ActivityType
        privateActivity->_activityType = xmlActivityType;
        xmlActivityType->_activities.insert(privateActivity);
        xmlActivityType->addReference();
        privateActivity->addReference();
    }
    if (xmlWorkload != nullptr)
    {   //  Link with Workload
        privateActivity->_workload = xmlWorkload;
        xmlWorkload->_contributingActivities.insert(privateActivity);
        xmlWorkload->addReference();
        privateActivity->addReference();
    }
    _database->_markModified();
    //  ...schedule change notifications...
    _database->_changeNotifier.post(
        new tt3::db::api::ObjectModifiedNotification(
            _database, this->type(), this->_oid));
    _database->_changeNotifier.post(
        new tt3::db::api::ObjectCreatedNotification(
            _database, privateActivity->type(), privateActivity->_oid));
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
#ifndef Q_NODEBUG
    _database->_validate();    //  may throw
#endif
    return privateActivity;
}

auto User::createPrivateTask(
        const QString & displayName,
        const QString & description,
        const tt3::db::api::InactivityTimeout & timeout,
        bool requireCommentOnStart,
        bool requireCommentOnStop,
        bool fullScreenReminder,
        tt3::db::api::IActivityType * activityType,
        tt3::db::api::IWorkload * workload,
        bool completed,
        bool requireCommentOnCompletion
    ) -> tt3::db::api::IPrivateTask *
{
    tt3::util::Lock _(_database->_guard);
    _ensureLiveAndWritable();   //  may throw
#ifndef Q_NODEBUG
    _database->_validate(); //  may throw
#endif

    //  Validate parameters
    if (!_database->_validator->privateTask()->isValidDisplayName(displayName))
    {
        throw tt3::db::api::InvalidPropertyValueException(
            tt3::db::api::ObjectTypes::PrivateTask::instance(),
            "displayName",
            displayName);
    }
    if (!_database->_validator->privateTask()->isValidDescription(description))
    {
        throw tt3::db::api::InvalidPropertyValueException(
            tt3::db::api::ObjectTypes::PrivateTask::instance(),
            "description",
            description);
    }
    if (timeout.has_value() &&
        !_database->_validator->privateTask()->isValidTimeout(timeout))
    {
        throw tt3::db::api::InvalidPropertyValueException(
            tt3::db::api::ObjectTypes::PrivateTask::instance(),
            "timeout",
            timeout.value());
    }
    ActivityType * xmlActivityType = nullptr;
    if (activityType != nullptr)
    {
        xmlActivityType = dynamic_cast<ActivityType*>(activityType);
        if (xmlActivityType == nullptr ||
            xmlActivityType->_database != this->_database ||
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
            xmlWorkload->_database != this->_database ||
            !xmlWorkload->_isLive)
        {   //  OOPS!
            throw tt3::db::api::IncompatibleInstanceException(workload->type());
        }
    }

    //  Display names must be unique
    if (_findRootPrivateTask(displayName) != nullptr)
    {   //  OOPS!
        throw tt3::db::api::AlreadyExistsException(
            tt3::db::api::ObjectTypes::PrivateTask::instance(),
            "displayName",
            displayName);
    }

    //  Do the work - create & initialize the PrivateTask...
    PrivateTask * privateTask =
        new PrivateTask(this, _database->_generateOid());   //  registers with User
    privateTask->_displayName = displayName;
    privateTask->_description = description;
    privateTask->_timeout = timeout;
    privateTask->_requireCommentOnStart = requireCommentOnStart;
    privateTask->_requireCommentOnStop = requireCommentOnStop;
    privateTask->_fullScreenReminder = fullScreenReminder;
    privateTask->_completed = completed;
    privateTask->_requireCommentOnCompletion = requireCommentOnCompletion;
    if (xmlActivityType != nullptr)
    {   //  Link with ActivityType
        privateTask->_activityType = xmlActivityType;
        xmlActivityType->_activities.insert(privateTask);
        xmlActivityType->addReference();
        privateTask->addReference();
    }
    if (xmlWorkload != nullptr)
    {   //  Link with Workload
        privateTask->_workload = xmlWorkload;
        xmlWorkload->_contributingActivities.insert(privateTask);
        xmlWorkload->addReference();
        privateTask->addReference();
    }
    _database->_markModified();
    //  ...schedule change notifications...
    _database->_changeNotifier.post(
        new tt3::db::api::ObjectModifiedNotification(
            _database, this->type(), this->_oid));
    _database->_changeNotifier.post(
        new tt3::db::api::ObjectCreatedNotification(
            _database, privateTask->type(), privateTask->_oid));
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
#ifndef Q_NODEBUG
    _database->_validate();    //  may throw
#endif
    return privateTask;
}

//////////
//  Implementation helpers
void User::_makeDead()
{
    Q_ASSERT(_database->_guard.isLockedByCurrentThread());
    Q_ASSERT(_isLive);

    //  Destroy aggregated objects
    for (Account * account : _accounts.values())
    {
        account->destroy();
    }
    Q_ASSERT(_accounts.isEmpty());

    for(PrivateActivity * privateActivity : _privateActivities.values())
    {
        privateActivity->destroy();
    }
    Q_ASSERT(_privateActivities.isEmpty());

    for(PrivateTask * privateTask : _rootPrivateTasks.values())
    {
        privateTask->destroy();
    }
    Q_ASSERT(_rootPrivateTasks.isEmpty());

    //  Remove from "live" caches
    Q_ASSERT(_database->_users.contains(this));
    _database->_users.remove(this);
    this->removeReference();

    //  The rest is up to the base class
    Principal::_makeDead();
}

auto User::_findPrivateActivity(
        const QString & displayName
    ) const -> PrivateActivity *
{
    for (PrivateActivity * privateActivity : _privateActivities)
    {
        if (privateActivity->_displayName == displayName)
        {
            return privateActivity;
        }
    }
    return nullptr;
}

auto User::_findRootPrivateTask(
        const QString & displayName
    ) const -> PrivateTask *
{
    for (PrivateTask * privateTask : _rootPrivateTasks)
    {
        if (privateTask->_displayName == displayName)
        {
            return privateTask;
        }
    }
    return nullptr;
}

void User::_collectPrivateTasksClosure(PrivateTasks & closure, const PrivateTasks & addend) const
{
    Q_ASSERT(_database->_guard.isLockedByCurrentThread());

    closure += addend;
    for (auto privateTask : addend)
    {   //  We rely on no-parent-child-loops!
        _collectPrivateTasksClosure(closure, privateTask->_children);
    }
}

//////////
//  Serialization
void User::_serializeProperties(
        QDomElement & objectElement
    ) const
{
    Principal::_serializeProperties(objectElement);

    objectElement.setAttribute("RealName", _realName);
    if (_inactivityTimeout.has_value())
    {
        objectElement.setAttribute("InactivityTimeout", tt3::util::toString(_inactivityTimeout.value()));
    }
    if (_uiLocale.has_value())
    {
        objectElement.setAttribute("UiLocale", tt3::util::toString(_uiLocale.value()));
    }
}

void User::_serializeAggregations(
        QDomElement & objectElement
    ) const
{
    Principal::_serializeAggregations(objectElement);

    _database->_serializeAggregation(
        objectElement,
        "Accounts",
        _accounts);
    _database->_serializeAggregation(
        objectElement,
        "PrivateActivities",
        _privateActivities);
    _database->_serializeAggregation(
        objectElement,
        "PrivateTasks",
        _rootPrivateTasks);
}

void User::_serializeAssociations(
        QDomElement & objectElement
    ) const
{
    Principal::_serializeProperties(objectElement);

    _database->_serializeAssociation(
        objectElement,
        "PermittedWorkloads",
        _permittedWorkloads);
}

void User::_deserializeProperties(
        const QDomElement & objectElement
    )
{
    Principal::_deserializeProperties(objectElement);

    _realName = objectElement.attribute("RealName");
    if (objectElement.hasAttribute("InactivityTimeout"))
    {
        _inactivityTimeout =
            tt3::util::fromString<tt3::util::TimeSpan>(
                objectElement.attribute("InactivityTimeout"));
    }
    if (objectElement.hasAttribute("UiLocale"))
    {
        _uiLocale =
            tt3::util::fromString<QLocale>(
                objectElement.attribute("UiLocale"));
    }
}

void User::_deserializeAggregations(
        const QDomElement & objectElement
    )
{
    Principal::_deserializeAggregations(objectElement);

    _database->_deserializeAggregation<Account>(
        objectElement,
        "Accounts",
        _accounts,
        [&](auto oid)
        {
            return new Account(this, oid);
        });
    _database->_deserializeAggregation<PrivateActivity>(
        objectElement,
        "PrivateActivities",
        _privateActivities,
        [&](auto oid)
        {
            return new PrivateActivity(this, oid);
        });
    _database->_deserializeAggregation<PrivateTask>(
        objectElement,
        "PrivateTasks",
        _rootPrivateTasks,
        [&](auto oid)
        {
            return new PrivateTask(this, oid);
        });
}

void User::_deserializeAssociations(
        const QDomElement & objectElement
    )
{
    Principal::_deserializeProperties(objectElement);

    _database->_deserializeAssociation(
        objectElement,
        "PermittedWorkloads",
        _permittedWorkloads);
}

//////////
//  Validation
void User::_validate(
        Objects & validatedObjects
    )
{
    Principal::_validate(validatedObjects);

    //  Validate properties
    if (!_database->_validator->user()->isValidRealName(_realName))
    {   //  OOPS!
        throw tt3::db::api::DatabaseCorruptException(_database->_address);
    }
    if (_inactivityTimeout.has_value() &&
        !_database->_validator->user()->isValidInactivityTimeout(_inactivityTimeout.value()))
    {   //  OOPS!
        throw tt3::db::api::DatabaseCorruptException(_database->_address);
    }
    if (_uiLocale.has_value() &&
        !_database->_validator->user()->isValidUiLocale(_uiLocale.value()))
    {   //  OOPS!
        throw tt3::db::api::DatabaseCorruptException(_database->_address);
    }

    //  Validate aggregations
    if (!_database->_users.contains(this))
    {   //  OOPS!
        throw tt3::db::api::DatabaseCorruptException(_database->_address);
    }
    for (Account * account : std::as_const(_accounts))
    {
        if (account == nullptr || !account->_isLive ||
            account->_database != _database ||
            account->_user != this)
        {   //  OOPS!
            throw tt3::db::api::DatabaseCorruptException(_database->_address);
        }
        account->_validate(validatedObjects);
    }
    for (PrivateActivity * privateActivity : std::as_const(_privateActivities))
    {
        if (privateActivity == nullptr || !privateActivity->_isLive ||
            privateActivity->_database != _database ||
            privateActivity->_owner != this)
        {   //  OOPS!
            throw tt3::db::api::DatabaseCorruptException(_database->_address);
        }
        privateActivity->_validate(validatedObjects);
    }
    for (PrivateTask * privateTask : std::as_const(_rootPrivateTasks))
    {
        if (privateTask == nullptr || !privateTask->_isLive ||
            privateTask->_database != _database ||
            privateTask->_owner != this)
        {   //  OOPS!
            throw tt3::db::api::DatabaseCorruptException(_database->_address);
        }
        privateTask->_validate(validatedObjects);
    }

    //  Validate associations
    for (Workload * workload : std::as_const(_permittedWorkloads))
    {
        if (workload == nullptr ||
            workload->_database != this->_database || !workload->_isLive ||
            !workload->_assignedUsers.contains(this))
        {   //  OOPS!
            throw tt3::db::api::DatabaseCorruptException(_database->_address);
        }
    }
}

//  End of tt3-db-xml/User.cpp
