//
//  tt3-db-xml/Database.cpp - tt3::db::xml::Database class implementation
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
//  Construction/destruction
Database::Database(
        DatabaseAddress * address,
        _OpenMode openMode)
    :   _address(address),
        _validator(tt3::db::api::DefaultValidator::instance()),
        _needsSaving(false),
        _isOpen(true),
        _isReadOnly(openMode == _OpenMode::_OpenReadOnly),
        _nextSaveAt(QDateTime::currentDateTimeUtc().addMSecs(_SaveIntervalMs)),
        _lastSaveDurationMs(0),
        _saveTimer()
{
    Q_ASSERT(_address != nullptr);
    if (openMode != _OpenMode::_Dead)
    {   //  ...because the DatabaseType holds that reference
        _address->addReference();
    }

    tt3::util::Lock _(_guard);

    switch (openMode)
    {
        case _OpenMode::_Create:
            //  Can't overwrite!
            if (QFile(_address->_path).exists())
            {   //  OOPS!

                throw tt3::db::api::AlreadyExistsException(
                        DatabaseType::instance()->displayName(),
                        "location",
                        _address->_path);
            }
            //  Need to save empty DB content, but
            //  obtain the lock first
            try
            {
                _lockRefresher = new _LockRefresher(this);  //  may throw
                _save();    //  may throw
                _lockRefresher->start();
            }
            catch (const tt3::util::Exception & ex)
            {   //  Cleanup & re-throw
                qCritical() << ex;
                delete _lockRefresher;
                if (dynamic_cast<const tt3::db::api::DatabaseException*>(&ex) != nullptr)
                {   //  Can re-throw "as is"
                    throw;
                }
                throw tt3::db::api::CustomDatabaseException(ex.errorMessage());
            }
            break;
        case _OpenMode::_OpenReadOnly:
            //  Load database contents - no need for the DB lock
            try
            {
                Q_ASSERT(_lockRefresher == nullptr);
                _load();    //  may throw
            }
            catch (const tt3::util::Exception & ex)
            {   //  e.g. ParseException, etc.
                qCritical() << ex;
                //  Cleanup & re-throw
                if (dynamic_cast<const tt3::db::api::DatabaseException*>(&ex) != nullptr)
                {   //  Can re-throw "as is"
                    throw;
                }
                throw tt3::db::api::CustomDatabaseException(ex.errorMessage());
            }
            break;
        case _OpenMode::_OpenReadWrite:
            //  Need to load existing DB content, but
            //  obtain the lock first
            try
            {
                QFileInfo fileInfo(address->_path);
                if (fileInfo.isFile() && !fileInfo.isWritable())
                {
                    throw tt3::db::api::AccessDeniedException();
                }
                _lockRefresher = new _LockRefresher(this);  //  may throw
                _load();    //  may throw
                _lockRefresher->start();
            }
            catch (const tt3::util::Exception & ex)
            {   //  e.g. ParseException, etc.
                qCritical() << ex;
                //  Cleanup & re-throw
                delete _lockRefresher;
                if (dynamic_cast<const tt3::db::api::DatabaseException*>(&ex) != nullptr)
                {   //  Can re-throw "as is"
                    throw;
                }
                throw tt3::db::api::CustomDatabaseException(ex.errorMessage());
            }
            break;
        case _OpenMode::_Dead:
            //  We're creating a special "dead" database,
            //  where "dead" objects are parked until they
            //  lose their reference counts.
            //  We don't need a _lockRefresher
            Q_ASSERT(_lockRefresher == nullptr);
            break;
        default:
            Q_ASSERT(false);
    }
    Q_ASSERT(!_needsSaving);

    //  Start save timer IF this Database is writable
    if (_lockRefresher != nullptr)
    {
        _saveTimer.setInterval(1000);   //  ms
        QObject::connect(
            &_saveTimer,
            &QTimer::timeout,
            [&]()
            {
                tt3::util::Lock _(_guard);
                try
                {
                    _savePeriodically();    //  may throw
                }
                catch (const tt3::util::Exception & ex)
                {   //  OOPS! Log, but ignore
                    qCritical() << ex;
                }
            });
        _saveTimer.start();
    }
}

Database::~Database()
{
    //  Close if still open
    try
    {
        close();
    }
    catch (const tt3::util::Exception & ex)
    {   //  OOPS! Suppress, but log.
        qCritical() << ex;
    }

    //  All Objects should be in graveyard by now
    {
        tt3::util::Lock _(_guard);
        Q_ASSERT(_liveObjects.isEmpty());
        for (Object * object : _graveyard.values())
        {
            if (object->_referenceCount == 0)
            {
                delete object;
            }
            else
            {   //  ...but some of them may still have references -
                //  these shall become "orphans" without an associated
                //  database which are deleted when the last reference
                //  is lost.
                //  To achieve this, we move these objects to a hidden
                //  "dead" database.
                Database * deadDatabase = DatabaseType::instance()->_deadDatabase;
                _graveyard.remove(object->_oid);
                deadDatabase->_graveyard.insert(object->_oid, object);
                *const_cast<Database**>(&object->_database) = deadDatabase;
            }
        }
        Q_ASSERT(_graveyard.isEmpty());
    }

    //  We're done
    _address->removeReference();
}

//////////
//  tt3::db::api::IDatabase (general)
auto Database::type(
    ) const -> DatabaseType *
{   //  No need to synchronize
    return DatabaseType::instance();
}

auto Database::address(
    ) const -> DatabaseAddress *
{   //  No need to synchronize
    return _address;
}

auto Database::validator(
    ) const -> tt3::db::api::IValidator *
{   //  No need to synchronize
    return _validator;
}

bool Database::isOpen() const
{
    tt3::util::Lock _(_guard);

    return _isOpen;
}

bool Database::isReadOnly() const
{   //  No need to synchronize
    return _isReadOnly;
}

void Database::close()
{
    tt3::util::Lock _(_guard);

    if (!_isOpen)
    {   //  Already closed
        return;
    }
    _saveTimer.stop();

    //  All active locks become orpans
    for (DatabaseLock * databaseLock : qAsConst(_activeDatabaseLocks))
    {
        databaseLock->_database = nullptr;
    }
    _activeDatabaseLocks.clear();

    //  Emit "database is closed" change notification
    //  directly - all signal/slot connections wibb
    //  soon be broken
    tt3::db::api::DatabaseClosedNotification n(this);
    emit _changeNotifier.databaseClosed(n);

    //  Disconnect all slots from notification signals
    QObject::disconnect(
        &_changeNotifier,
        &tt3::db::api::ChangeNotifier::databaseClosed,
        nullptr, nullptr);
    QObject::disconnect(
        &_changeNotifier,
        &tt3::db::api::ChangeNotifier::objectCreated,
        nullptr, nullptr);
    QObject::disconnect(
        &_changeNotifier,
        &tt3::db::api::ChangeNotifier::objectDestroyed,
        nullptr, nullptr);
    QObject::disconnect(
        &_changeNotifier,
        &tt3::db::api::ChangeNotifier::objectModified,
        nullptr, nullptr);

    //  Save ?
    if (_needsSaving)
    {
        _needsSaving = false;
        try
        {
            _save();    //  may throw!
        }
        catch (const tt3::util::Exception & ex)
        {   //  Cleanup & re-throw
            qCritical() << ex;
            _markClosed();
            throw;
        }
    }

    //  Destroy all Object instances...
    //  (we pretend to be read/wrote for the duration -
    //  these changes will never be saved)
    bool wasReadOnly = _isReadOnly;
    _isReadOnly = false;
    for (User * user : _users.values())
    {
        user->destroy();    //  also destroys Accounts, Works, etc.
    }
    for (ActivityType * activityType : _activityTypes.values())
    {
        activityType->destroy();
    }
    for (PublicActivity * publicActivity : _publicActivities.values())
    {
        publicActivity->destroy();
    }
    for (PublicTask * publicTask : _rootPublicTasks.values())
    {
        publicTask->destroy();
    }
    for (Project * project : _rootProjects.values())
    {
        project->destroy();
    }
    for (WorkStream * workStream : _workStreams.values())
    {
        workStream->destroy();
    }
    for (Beneficiary * beneficiary : _beneficiaries.values())
    {
        beneficiary->destroy();
    }
    Q_ASSERT(_users.isEmpty());
    Q_ASSERT(_activityTypes.isEmpty());
    Q_ASSERT(_publicActivities.isEmpty());
    Q_ASSERT(_rootPublicTasks.isEmpty());
    Q_ASSERT(_rootProjects.isEmpty());
    Q_ASSERT(_workStreams.isEmpty());
    Q_ASSERT(_beneficiaries.isEmpty());
    _isReadOnly = wasReadOnly;

    //  Done
    _markClosed();
}

void Database::refresh()
{
    tt3::util::Lock _(_guard);

    _ensureOpen();  //  may throw
#ifdef Q_DEBUG
    _validate();    //  may throw
#endif
    //  ...otherwise an all-in-RAM database performs no caching
}

//////////
//  tt3::db::api::IDatabase (associations)
quint64 Database::objectCount(
    ) const
{
    tt3::util::Lock _(_guard);
    _ensureOpen();  //  may throw
    //  We assume database is consistent since last change

    return _liveObjects.size();
}

auto Database::users(
    ) const -> tt3::db::api::Users
{
    tt3::util::Lock _(_guard);
    _ensureOpen();  //  may throw
    //  We assume database is consistent since last change

    return tt3::db::api::Users(_users.cbegin(), _users.cend());
}

auto Database::accounts(
    ) const -> tt3::db::api::Accounts
{
    tt3::util::Lock _(_guard);
    _ensureOpen();  //  may throw
    //  We assume database is consistent since last change

    tt3::db::api::Accounts result;
    for (User * user : _users)
    {
        result += tt3::db::api::Accounts(user->_accounts.cbegin(), user->_accounts.cend());
    }
    return result;
}

auto Database::findAccount(
        const QString & login
    ) const -> tt3::db::api::IAccount *
{
    tt3::util::Lock _(_guard);
    _ensureOpen();  //  may throw
    //  We assume database is consistent since last change

    return _findAccount(login);
}

auto Database::activityTypes(
    ) const -> tt3::db::api::ActivityTypes
{
    tt3::util::Lock _(_guard);
    _ensureOpen();  //  may throw
    //  We assume database is consistent since last change

    return tt3::db::api::ActivityTypes(_activityTypes.cbegin(), _activityTypes.cend());
}

auto Database::findActivityType(
        const QString & displayName
    ) -> tt3::db::api::IActivityType *
{
    tt3::util::Lock _(_guard);
    _ensureOpen();  //  may throw
    //  We assume database is consistent since last change

    return _findActivityType(displayName);
}

auto Database::publicActivities(
    ) const -> tt3::db::api::PublicActivities
{
    tt3::util::Lock _(_guard);
    _ensureOpen();  //  may throw
    //  We assume database is consistent since last change

    return tt3::db::api::PublicActivities(_publicActivities.cbegin(), _publicActivities.cend());
}

auto Database::findPublicActivity(
        const QString & displayName
    ) -> tt3::db::api::IPublicActivity *
{
    tt3::util::Lock _(_guard);
    _ensureOpen();  //  may throw
    //  We assume database is consistent since last change

    return _findPublicActivity(displayName);
}

auto Database::publicActivitiesAndTasks(
    ) const -> tt3::db::api::PublicActivities
{
    tt3::util::Lock _(_guard);
    _ensureOpen();  //  may throw
    //  We assume database is consistent since last change

    PublicTasks closure;
    _collectPublicTasksClosure(closure, _rootPublicTasks);
    return
        tt3::db::api::PublicActivities(
            _publicActivities.cbegin(), _publicActivities.cend()) +
        tt3::db::api::PublicActivities(
           closure.cbegin(), closure.cend());
}

auto Database::publicTasks(
    ) const -> tt3::db::api::PublicTasks
{
    tt3::util::Lock _(_guard);
    _ensureOpen();  //  may throw
    //  We assume database is consistent since last change

    PublicTasks closure;
    _collectPublicTasksClosure(closure, _rootPublicTasks);
    return tt3::db::api::PublicTasks(closure.cbegin(), closure.cend());
}

auto Database::rootPublicTasks(
    ) const -> tt3::db::api::PublicTasks
{
    tt3::util::Lock _(_guard);
    _ensureOpen();  //  may throw
    //  We assume database is consistent since last change

    return tt3::db::api::PublicTasks(_rootPublicTasks.cbegin(), _rootPublicTasks.cend());
}

auto Database::projects(
    ) const -> tt3::db::api::Projects
{
    tt3::util::Lock _(_guard);
    _ensureOpen();  //  may throw
    //  We assume database is consistent since last change

    Projects closure;
    _collectProjectsClosure(closure, _rootProjects);
    return tt3::db::api::Projects(closure.cbegin(), closure.cend());
}

auto Database::rootProjects(
    ) const -> tt3::db::api::Projects
{
    tt3::util::Lock _(_guard);
    _ensureOpen();  //  may throw
    //  We assume database is consistent since last change

    return tt3::db::api::Projects(_rootProjects.cbegin(), _rootProjects.cend());
}

auto Database::workStreams(
    ) const -> tt3::db::api::WorkStreams
{
    tt3::util::Lock _(_guard);
    _ensureOpen();  //  may throw
    //  We assume database is consistent since last change

    return tt3::db::api::WorkStreams(_workStreams.cbegin(), _workStreams.cend());
}

auto Database::beneficiaries(
    ) const -> tt3::db::api::Beneficiaries
{
    tt3::util::Lock _(_guard);
    _ensureOpen();  //  may throw
    //  We assume database is consistent since last change

    return tt3::db::api::Beneficiaries(_beneficiaries.cbegin(), _beneficiaries.cend());
}

//////////
//  tt3::db::api::IDatabase (access control)
auto Database::tryLogin(
        const QString & login,
        const QString & password
    ) const -> tt3::db::api::IAccount *
{
    static tt3::util::IMessageDigest * sha1 = tt3::util::StandardMessageDigests::Sha1::instance();  //  idempotent

    tt3::util::Lock _(_guard);
    _ensureOpen();  //  may throw
    //  We assume database is consistent since last change

    std::unique_ptr<tt3::util::IMessageDigest::Builder> sha1Builder
        { sha1->createBuilder() };
    sha1Builder->digestFragment(password);
    QString passwordHash = sha1Builder->digestAsString();

    for (User * user : _users)
    {
        if (user->_enabled)
        {
            for (Account * account : qAsConst(user->_accounts))
            {
                if (account->_enabled && account->_login == login && account->_passwordHash == passwordHash)
                {
                    return account;
                }
            }
        }
    }
    return nullptr;
}

auto Database::login(
        const QString & login,
        const QString & password
    ) const -> tt3::db::api::IAccount *
{
    if (tt3::db::api::IAccount * account = tryLogin(login, password))
    {
        return account;
    }
    throw tt3::db::api::AccessDeniedException();
}

//////////
//  tt3::db::api::IDatabase (life cycle)
auto Database::createUser(
        bool enabled,
        const QStringList & emailAddresses,
        const QString & realName,
        const tt3::db::api::InactivityTimeout & inactivityTimeout,
        const tt3::db::api::UiLocale & uiLocale,
        const tt3::db::api::Workloads & permittedWorkloads
    ) -> tt3::db::api::IUser *
{
    tt3::util::Lock _(_guard);
    _ensureOpenAndWritable();   //  may throw
#ifdef Q_DEBUG
    _validate();    //  may throw
#endif

    //  Validate parameters
    if (!_validator->principal()->isValidEmailAddresses(emailAddresses))
    {
        throw tt3::db::api::InvalidPropertyValueException(
            tt3::db::api::ObjectTypes::User::instance(),
            "emailAddresses",
            emailAddresses.join(','));
    }
    if (!_validator->user()->isValidRealName(realName))
    {
        throw tt3::db::api::InvalidPropertyValueException(
            tt3::db::api::ObjectTypes::User::instance(),
            "realName",
            realName);
    }
    if (!_validator->user()->isValidInactivityTimeout(inactivityTimeout))
    {
        throw tt3::db::api::InvalidPropertyValueException(
            tt3::db::api::ObjectTypes::User::instance(),
            "inactivityTimeout",
            inactivityTimeout.value());
    }
    if (!_validator->user()->isValidUiLocale(uiLocale))
    {
        throw tt3::db::api::InvalidPropertyValueException(
            tt3::db::api::ObjectTypes::User::instance(),
            "uiLocale",
            uiLocale.value());
    }
    if (permittedWorkloads.contains(nullptr))
    {
        throw tt3::db::api::InvalidPropertyValueException(
            tt3::db::api::ObjectTypes::User::instance(),
            "workloads",
            nullptr);
    }

    Workloads xmlPermittedWorkloads =
        tt3::util::transform(
            permittedWorkloads,
            [&](auto w)
            {
                Q_ASSERT(w != nullptr); //  should have been caught earlier!
                auto xmlWorkload = dynamic_cast<Workload*>(w);
                if (xmlWorkload == nullptr ||
                    xmlWorkload->_database != this ||
                    !xmlWorkload->_isLive)
                {   //  OOPS!
                    throw tt3::db::api::IncompatibleInstanceException(w->type());
                }
                return xmlWorkload;
            });

    //  Do the work - create & initialize the User...
    User * user = new User(this, _generateOid()); //  registers with Database
    user->_enabled = enabled;
    user->_emailAddresses = emailAddresses;
    user->_realName = realName;
    user->_inactivityTimeout = inactivityTimeout;
    user->_uiLocale = uiLocale;
    for (Workload * xmlWorkload : qAsConst(xmlPermittedWorkloads))
    {
        user->_permittedWorkloads.insert(xmlWorkload);
        xmlWorkload->_assignedUsers.insert(user);
        user->addReference();
        xmlWorkload->addReference();
    }
    _markModified();
    //  ...schedule change notifications...
    _changeNotifier.post(
        new tt3::db::api::ObjectCreatedNotification(
            this, user->type(), user->_oid));
    for (Workload * xmlWorkload : qAsConst(xmlPermittedWorkloads))
    {
        _changeNotifier.post(
            new tt3::db::api::ObjectModifiedNotification(
                this, xmlWorkload->type(), xmlWorkload->_oid));
    }
    //  ...and we're done
#ifdef Q_DEBUG
    _validate();    //  may throw
#endif
    return user;
}

auto Database::createActivityType(
        const QString & displayName,
        const QString & description
    ) -> tt3::db::api::IActivityType *
{
    tt3::util::Lock _(_guard);
    _ensureOpenAndWritable();   //  may throw
#ifdef Q_DEBUG
    _validate();    //  may throw
#endif

    //  Validate parameters
    if (!_validator->activityType()->isValidDisplayName(displayName))
    {
        throw tt3::db::api::InvalidPropertyValueException(
            tt3::db::api::ObjectTypes::ActivityType::instance(),
            "displayName",
            displayName);
    }
    if (!_validator->activityType()->isValidDescription(description))
    {
        throw tt3::db::api::InvalidPropertyValueException(
            tt3::db::api::ObjectTypes::ActivityType::instance(),
            "description",
            description);
    }

    //  Display names must be unique
    if (_findActivityType(displayName) != nullptr)
    {   //  OOPS!
        throw tt3::db::api::AlreadyExistsException(
            tt3::db::api::ObjectTypes::ActivityType::instance(),
            "displayName",
            displayName);
    }

    //  Do the work - create & initialize the ActivityType...
    ActivityType * activityType = new ActivityType(this, _generateOid()); //  registers with Database
    activityType->_displayName = displayName;
    activityType->_description = description;
    _markModified();
    //  ...schedule change notifications...
    _changeNotifier.post(
        new tt3::db::api::ObjectCreatedNotification(
            this, activityType->type(), activityType->_oid));
    //  ...and we're done
#ifdef Q_DEBUG
    _validate();    //  may throw
#endif
    return activityType;
}

auto Database::createPublicActivity(
        const QString & displayName,
        const QString & description,
        const tt3::db::api::InactivityTimeout & timeout,
        bool requireCommentOnStart,
        bool requireCommentOnStop,
        bool fullScreenReminder,
        tt3::db::api::IActivityType * activityType,
        tt3::db::api::IWorkload * workload
    ) -> tt3::db::api::IPublicActivity *
{
    tt3::util::Lock _(_guard);
    _ensureOpenAndWritable();   //  may throw
#ifdef Q_DEBUG
    _validate();    //  may throw
#endif

    //  Validate parameters
    if (!_validator->publicActivity()->isValidDisplayName(displayName))
    {
        throw tt3::db::api::InvalidPropertyValueException(
            tt3::db::api::ObjectTypes::PublicActivity::instance(),
            "displayName",
            displayName);
    }
    if (!_validator->publicActivity()->isValidDescription(description))
    {
        throw tt3::db::api::InvalidPropertyValueException(
            tt3::db::api::ObjectTypes::PublicActivity::instance(),
            "description",
            description);
    }
    if (timeout.has_value() &&
        !_validator->publicActivity()->isValidTimeout(timeout))
    {
        throw tt3::db::api::InvalidPropertyValueException(
            tt3::db::api::ObjectTypes::PublicActivity::instance(),
            "timeout",
            timeout.value());
    }
    ActivityType * xmlActivityType = nullptr;
    if (activityType != nullptr)
    {
        xmlActivityType = dynamic_cast<ActivityType*>(activityType);
        if (xmlActivityType == nullptr ||
            xmlActivityType->_database != this ||
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
            xmlWorkload->_database != this ||
            !xmlWorkload->_isLive)
        {   //  OOPS!
            throw tt3::db::api::IncompatibleInstanceException(workload->type());
        }
    }

    //  Display names must be unique
    if (_findPublicActivity(displayName) != nullptr)
    {   //  OOPS!
        throw tt3::db::api::AlreadyExistsException(
            tt3::db::api::ObjectTypes::PublicActivity::instance(),
            "displayName",
            displayName);
    }

    //  Do the work - create & initialize the PublicActivity...
    PublicActivity * publicActivity = new PublicActivity(this, _generateOid()); //  registers with Database
    publicActivity->_displayName = displayName;
    publicActivity->_description = description;
    publicActivity->_timeout = timeout;
    publicActivity->_requireCommentOnStart = requireCommentOnStart;
    publicActivity->_requireCommentOnStop = requireCommentOnStop;
    publicActivity->_fullScreenReminder = fullScreenReminder;
    if (xmlActivityType != nullptr)
    {   //  Link with ActivityType
        publicActivity->_activityType = xmlActivityType;
        xmlActivityType->_activities.insert(publicActivity);
        xmlActivityType->addReference();
        publicActivity->addReference();
    }
    if (xmlWorkload != nullptr)
    {   //  Link with Workload
        publicActivity->_workload = xmlWorkload;
        xmlWorkload->_contributingActivities.insert(publicActivity);
        xmlWorkload->addReference();
        publicActivity->addReference();
    }
    _markModified();
    //  ...schedule change notifications...
    _changeNotifier.post(
        new tt3::db::api::ObjectCreatedNotification(
            this, publicActivity->type(), publicActivity->_oid));
    if (xmlActivityType != nullptr)
    {
        _changeNotifier.post(
            new tt3::db::api::ObjectModifiedNotification(
                this, xmlActivityType->type(), xmlActivityType->_oid));
    }
    if (xmlWorkload != nullptr)
    {
        _changeNotifier.post(
            new tt3::db::api::ObjectModifiedNotification(
                this, xmlWorkload->type(), xmlWorkload->_oid));
    }
    //  ...and we're done
#ifdef Q_DEBUG
    _validate();    //  may throw
#endif
    return publicActivity;
}

auto Database::createPublicTask(
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
    ) -> tt3::db::api::IPublicTask *
{
    tt3::util::Lock _(_guard);
    _ensureOpenAndWritable();   //  may throw
#ifdef Q_DEBUG
    _validate();    //  may throw
#endif

    //  Validate parameters
    if (!_validator->publicTask()->isValidDisplayName(displayName))
    {
        throw tt3::db::api::InvalidPropertyValueException(
            tt3::db::api::ObjectTypes::PublicTask::instance(),
            "displayName",
            displayName);
    }
    if (!_validator->publicTask()->isValidDescription(description))
    {
        throw tt3::db::api::InvalidPropertyValueException(
            tt3::db::api::ObjectTypes::PublicTask::instance(),
            "description",
            description);
    }
    if (timeout.has_value() &&
        !_validator->publicTask()->isValidTimeout(timeout))
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
            xmlActivityType->_database != this ||
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
            xmlWorkload->_database != this ||
            !xmlWorkload->_isLive)
        {   //  OOPS!
            throw tt3::db::api::IncompatibleInstanceException(workload->type());
        }
    }

    //  Display names must be unique
    if (_findRootPublicTask(displayName) != nullptr)
    {   //  OOPS!
        throw tt3::db::api::AlreadyExistsException(
            tt3::db::api::ObjectTypes::PublicTask::instance(),
            "displayName",
            displayName);
    }

    //  Do the work - create & initialize the PublicTask...
    PublicTask * publicTask = new PublicTask(this, _generateOid()); //  registers with Database
    publicTask->_displayName = displayName;
    publicTask->_description = description;
    publicTask->_timeout = timeout;
    publicTask->_requireCommentOnStart = requireCommentOnStart;
    publicTask->_requireCommentOnStop = requireCommentOnStop;
    publicTask->_fullScreenReminder = fullScreenReminder;
    publicTask->_completed = completed;
    publicTask->_requireCommentOnCompletion = requireCommentOnCompletion;
    if (xmlActivityType != nullptr)
    {   //  Link with ActivityType
        publicTask->_activityType = xmlActivityType;
        xmlActivityType->_activities.insert(publicTask);
        xmlActivityType->addReference();
        publicTask->addReference();
    }
    if (xmlWorkload != nullptr)
    {   //  Link with Workload
        publicTask->_workload = xmlWorkload;
        xmlWorkload->_contributingActivities.insert(publicTask);
        xmlWorkload->addReference();
        publicTask->addReference();
    }
    _markModified();
    //  ...schedule change notifications...
    _changeNotifier.post(
        new tt3::db::api::ObjectCreatedNotification(
            this, publicTask->type(), publicTask->_oid));
    if (xmlActivityType != nullptr)
    {
        _changeNotifier.post(
            new tt3::db::api::ObjectModifiedNotification(
                this, xmlActivityType->type(), xmlActivityType->_oid));
    }
    if (xmlWorkload != nullptr)
    {
        _changeNotifier.post(
            new tt3::db::api::ObjectModifiedNotification(
                this, xmlWorkload->type(), xmlWorkload->_oid));
    }
    //  ...and we're done
#ifdef Q_DEBUG
    _validate();    //  may throw
#endif
    return publicTask;
}

auto Database::createProject(
        const QString & displayName,
        const QString & description,
        const tt3::db::api::Beneficiaries & beneficiaries,
        bool completed
    ) -> tt3::db::api::IProject *
{
    tt3::util::Lock _(_guard);
    _ensureOpenAndWritable();   //  may throw
#ifdef Q_DEBUG
    _validate();    //  may throw
#endif

    //  Validate parameters
    if (!_validator->project()->isValidDisplayName(displayName))
    {
        throw tt3::db::api::InvalidPropertyValueException(
            tt3::db::api::ObjectTypes::Project::instance(),
            "displayName",
            displayName);
    }
    if (!_validator->project()->isValidDescription(description))
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

    Beneficiaries xmlBeneficiaries =
        tt3::util::transform(
            beneficiaries,
            [&](auto b)
            {
                Q_ASSERT(b != nullptr);   //  should have been caught earlier
                auto xmlBeneficiary = dynamic_cast<Beneficiary*>(b);
                if (xmlBeneficiary == nullptr ||
                    xmlBeneficiary->_database != this ||
                    !xmlBeneficiary->_isLive)
                {   //  OOPS!
                    throw tt3::db::api::IncompatibleInstanceException(b->type());
                }
                return xmlBeneficiary;
            });

    //  Display names must be unique
    if (_findRootProject(displayName) != nullptr)
    {   //  OOPS!
        throw tt3::db::api::AlreadyExistsException(
            tt3::db::api::ObjectTypes::Project::instance(),
            "displayName",
            displayName);
    }

    //  Do the work - create & initialize the Project...
    Project * project = new Project(this, _generateOid()); //  registers with Database
    project->_displayName = displayName;
    project->_description = description;
    project->_completed = completed;
    for (Beneficiary * xmlBeneficiary : qAsConst(xmlBeneficiaries))
    {   //  Link with Beneficiary
        project->_beneficiaries.insert(xmlBeneficiary);
        xmlBeneficiary->_workloads.insert(project);
        xmlBeneficiary->addReference();
        project->addReference();
    }
    _markModified();
    //  ...schedule change notifications...
    _changeNotifier.post(
        new tt3::db::api::ObjectCreatedNotification(
            this, project->type(), project->_oid));
    for (Beneficiary * xmlBeneficiary : qAsConst(xmlBeneficiaries))
    {
        _changeNotifier.post(
            new tt3::db::api::ObjectModifiedNotification(
                this, xmlBeneficiary->type(), xmlBeneficiary->_oid));
    }
    //  ...and we're done
#ifdef Q_DEBUG
    _validate();    //  may throw
#endif
    return project;
}

auto Database::createWorkStream(
        const QString & displayName,
        const QString & description,
        const tt3::db::api::Beneficiaries & beneficiaries
    ) -> tt3::db::api::IWorkStream *
{
    tt3::util::Lock _(_guard);
    _ensureOpenAndWritable();   //  may throw
#ifdef Q_DEBUG
    _validate();    //  may throw
#endif

    //  Validate parameters
    if (!_validator->workStream()->isValidDisplayName(displayName))
    {
        throw tt3::db::api::InvalidPropertyValueException(
            tt3::db::api::ObjectTypes::WorkStream::instance(),
            "displayName",
            displayName);
    }
    if (!_validator->workStream()->isValidDescription(description))
    {
        throw tt3::db::api::InvalidPropertyValueException(
            tt3::db::api::ObjectTypes::WorkStream::instance(),
            "description",
            description);
    }
    if (beneficiaries.contains(nullptr))
    {
        throw tt3::db::api::InvalidPropertyValueException(
            tt3::db::api::ObjectTypes::WorkStream::instance(),
            "beneficiaries",
            nullptr);
    }

    Beneficiaries xmlBeneficiaries =
        tt3::util::transform(
            beneficiaries,
            [&](auto b)
            {
                Q_ASSERT(b != nullptr);   //  should have been caught earlier
                auto xmlBeneficiary = dynamic_cast<Beneficiary*>(b);
                if (xmlBeneficiary == nullptr ||
                    xmlBeneficiary->_database != this ||
                    !xmlBeneficiary->_isLive)
                {   //  OOPS!
                    throw tt3::db::api::IncompatibleInstanceException(b->type());
                }
                return xmlBeneficiary;
            });

    //  Display names must be unique
    if (_findWorkStream(displayName) != nullptr)
    {   //  OOPS!
        throw tt3::db::api::AlreadyExistsException(
            tt3::db::api::ObjectTypes::WorkStream::instance(),
            "displayName",
            displayName);
    }

    //  Do the work - create & initialize the WorkStream...
    WorkStream * workStream = new WorkStream(this, _generateOid()); //  registers with Database
    workStream->_displayName = displayName;
    workStream->_description = description;
    for (Beneficiary * xmlBeneficiary : qAsConst(xmlBeneficiaries))
    {   //  Link with Beneficiary
        workStream->_beneficiaries.insert(xmlBeneficiary);
        xmlBeneficiary->_workloads.insert(workStream);
        xmlBeneficiary->addReference();
        workStream->addReference();
    }
    _markModified();
    //  ...schedule change notifications...
    _changeNotifier.post(
        new tt3::db::api::ObjectCreatedNotification(
            this, workStream->type(), workStream->_oid));
    for (Beneficiary * xmlBeneficiary : qAsConst(xmlBeneficiaries))
    {
        _changeNotifier.post(
            new tt3::db::api::ObjectModifiedNotification(
                this, xmlBeneficiary->type(), xmlBeneficiary->_oid));
    }
    //  ...and we're done
#ifdef Q_DEBUG
    _validate();    //  may throw
#endif
    return workStream;
}

auto Database::createBeneficiary(
        const QString & displayName,
        const QString & description,
        const tt3::db::api::Workloads & workloads
    ) -> tt3::db::api::IBeneficiary *
{
    tt3::util::Lock _(_guard);
    _ensureOpenAndWritable();   //  may throw
#ifdef Q_DEBUG
    _validate();    //  may throw
#endif

    //  Validate parameters
    if (!_validator->beneficiary()->isValidDisplayName(displayName))
    {
        throw tt3::db::api::InvalidPropertyValueException(
            tt3::db::api::ObjectTypes::Beneficiary::instance(),
            "displayName",
            displayName);
    }
    if (!_validator->beneficiary()->isValidDescription(description))
    {
        throw tt3::db::api::InvalidPropertyValueException(
            tt3::db::api::ObjectTypes::Beneficiary::instance(),
            "description",
            description);
    }
    if (workloads.contains(nullptr))
    {
        throw tt3::db::api::InvalidPropertyValueException(
            tt3::db::api::ObjectTypes::Beneficiary::instance(),
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
                    xmlWorkload->_database != this ||
                    !xmlWorkload->_isLive)
                {   //  OOPS!
                    throw tt3::db::api::IncompatibleInstanceException(w->type());
                }
                return xmlWorkload;
            });

    //  Display names must be unique
    if (_findBeneficiary(displayName) != nullptr)
    {   //  OOPS!
        throw tt3::db::api::AlreadyExistsException(
            tt3::db::api::ObjectTypes::Beneficiary::instance(),
            "displayName",
            displayName);
    }

    //  Do the work - create & initialize the Beneficiary...
    Beneficiary * beneficiary = new Beneficiary(this, _generateOid()); //  registers with Database
    beneficiary->_displayName = displayName;
    beneficiary->_description = description;
    for (Workload * xmlWorkload : qAsConst(xmlWorkloads))
    {   //  Link with Workload
        beneficiary->_workloads.insert(xmlWorkload);
        xmlWorkload->_beneficiaries.insert(beneficiary);
        xmlWorkload->addReference();
        beneficiary->addReference();
    }
    _markModified();
    //  ...schedule change notifications...
    _changeNotifier.post(
        new tt3::db::api::ObjectCreatedNotification(
            this, beneficiary->type(), beneficiary->_oid));
    for (Workload * xmlWorkload : qAsConst(xmlWorkloads))
    {
        _changeNotifier.post(
            new tt3::db::api::ObjectModifiedNotification(
                this, xmlWorkload->type(), xmlWorkload->_oid));
    }
    //  ...and we're done
#ifdef Q_DEBUG
    _validate();    //  may throw
#endif
    return beneficiary;
}

//////////
//  IDatabase (locking)
auto Database::lock(
        tt3::db::api::IDatabaseLock::LockType lockType,
        unsigned long timeoutMs
    ) -> tt3::db::api::IDatabaseLock *
{
    QDateTime startedAt = QDateTime::currentDateTimeUtc();
    QDateTime giveUpAt = startedAt.addMSecs(
        (timeoutMs > LONG_MAX) ? LONG_MAX : timeoutMs);

    QThread::yieldCurrentThread();  //  give other threads a chance
    tt3::util::Lock _(_guard);  //  lock manipulation needs to be guarded

    DatabaseLock * databaseLock;
    switch (lockType)
    {
        case tt3::db::api::IDatabaseLock::LockType::Read:
            _ensureOpen();   //  may throw
            //  Any existing DatabaseLock would have been
            //  placed by the same process.
             databaseLock = new DatabaseLock(this, lockType);
            break;
        case tt3::db::api::IDatabaseLock::LockType::Write:
            _ensureOpenAndWritable();   //  may throw
            //  Any existing DatabaseLock would have been
            //  placed by the same process.
            databaseLock = new DatabaseLock(this, lockType);
            break;
        default:    //  OOPS!
            Q_ASSERT(false);
            //  Be defensive in release mode
            databaseLock =
                new DatabaseLock(this, DatabaseLock::LockType::Read);
            break;
    }
    //  Register the lock & we're done
    _activeDatabaseLocks.insert(databaseLock);
    return databaseLock;
}

//////////
//  Implementation helpers
void Database::_ensureOpen() const
{
    Q_ASSERT(_guard.isLockedByCurrentThread());

    if (!_isOpen)
    {   //  OOPS!
        throw tt3::db::api::DatabaseClosedException();
    }
}

void Database::_ensureOpenAndWritable() const
{
    Q_ASSERT(_guard.isLockedByCurrentThread());

    if (!_isOpen)
    {   //  OOPS!
        throw tt3::db::api::DatabaseClosedException();
    }
    if (_isReadOnly)
    {   //  OOPS!
        throw tt3::db::api::AccessDeniedException();
    }
}

void Database::_markModified()
{
    Q_ASSERT(_guard.isLockedByCurrentThread());

    _needsSaving = true;
}

void Database::_markClosed()
{
    Q_ASSERT(_guard.isLockedByCurrentThread());

    if (_lockRefresher != nullptr)
    {
        _lockRefresher->requestStop();
        if (!_lockRefresher->wait(_LockRefresher::StaleTimeoutMin * 60 * 1000))
        {   /// OOPS! Force termination
            _lockRefresher->terminate();
            _lockRefresher->wait(ULONG_MAX);
        }
        delete _lockRefresher;
        _lockRefresher = nullptr;
    }
    _isOpen = false;
}

tt3::db::api::Oid Database::_generateOid()
{
    Q_ASSERT(_guard.isLockedByCurrentThread());

    for (; ; )
    {
        tt3::db::api::Oid oid = tt3::db::api::Oid::createRandom();
        if (!_liveObjects.contains(oid) && !_graveyard.contains(oid))
        {
            return oid;
        }
    }
}

void Database::_clearOrphanedDatabaseLocks()
{
    Q_ASSERT(_guard.isLockedByCurrentThread());

    for (DatabaseLock * databaseLock : _activeDatabaseLocks.values())
    {
        if (databaseLock->_database != this)
        {   //  No longer relevant!
            _activeDatabaseLocks.remove(databaseLock);
        }
    }
}

Account * Database::_findAccount(const QString & login) const
{
    Q_ASSERT(_guard.isLockedByCurrentThread());
    _ensureOpen();  //  may throw

    for (User * user : _users)
    {
        for (Account * account : qAsConst(user->_accounts))
        {
            if (account->_login == login)
            {
                return account;
            }
        }
    }
    return nullptr;
}

ActivityType * Database::_findActivityType(const QString & displayName) const
{
    Q_ASSERT(_guard.isLockedByCurrentThread());
    _ensureOpen();  //  may throw

    for (ActivityType * activityType : _activityTypes)
    {
        if (activityType->_displayName == displayName)
        {
            return activityType;
        }
    }
    return nullptr;
}

auto Database::_findPublicActivity(
        const QString & displayName
    ) const -> PublicActivity *
{
    Q_ASSERT(_guard.isLockedByCurrentThread());
    _ensureOpen();  //  may throw

    for (PublicActivity * publicActivity : _publicActivities)
    {
        if (publicActivity->_displayName == displayName)
        {
            return publicActivity;
        }
    }
    return nullptr;
}

auto Database::_findRootPublicTask(
        const QString & displayName
    ) const -> PublicTask *
{
    Q_ASSERT(_guard.isLockedByCurrentThread());
    _ensureOpen();  //  may throw

    for (PublicTask * publicTask : _rootPublicTasks)
    {
        if (publicTask->_displayName == displayName)
        {
            return publicTask;
        }
    }
    return nullptr;
}

auto Database::_findRootProject(
        const QString & displayName
    ) const -> Project *
{
    Q_ASSERT(_guard.isLockedByCurrentThread());
    _ensureOpen();  //  may throw

    for (Project * project : _rootProjects)
    {
        if (project->_displayName == displayName)
        {
            return project;
        }
    }
    return nullptr;
}

WorkStream * Database::_findWorkStream(const QString & displayName) const
{
    Q_ASSERT(_guard.isLockedByCurrentThread());
    _ensureOpen();  //  may throw

    for (WorkStream * workStream : _workStreams)
    {
        if (workStream->_displayName == displayName)
        {
            return workStream;
        }
    }
    return nullptr;
}

Beneficiary * Database::_findBeneficiary(const QString & displayName) const
{
    Q_ASSERT(_guard.isLockedByCurrentThread());
    _ensureOpen();  //  may throw

    for (Beneficiary * beneficiary : _beneficiaries)
    {
        if (beneficiary->_displayName == displayName)
        {
            return beneficiary;
        }
    }
    return nullptr;
}

void Database::_savePeriodically()
{
    Q_ASSERT(_guard.isLockedByCurrentThread());

    if (_needsSaving)
    {
        QDateTime now = QDateTime::currentDateTimeUtc();
        if (now >= _nextSaveAt)
        {   //  It's time to save
            _save();    //  may throw
            _needsSaving = false;
            QDateTime then = QDateTime::currentDateTimeUtc();
            _lastSaveDurationMs = now.msecsTo(then);
            _nextSaveAt = then.addMSecs(_SaveIntervalMs);
        }
    }
}

void Database::_collectPublicTasksClosure(PublicTasks & closure, const PublicTasks & addend) const
{
    Q_ASSERT(_guard.isLockedByCurrentThread());

    closure += addend;
    for (auto publicTask : addend)
    {   //  We rely on no-parent-child-loops!
        _collectPublicTasksClosure(closure, publicTask->_children);
    }
}

void Database::_collectProjectsClosure(Projects & closure, const Projects & addend) const
{
    Q_ASSERT(_guard.isLockedByCurrentThread());

    closure += addend;
    for (auto project : addend)
    {   //  We rely on no-parent-child-loops!
        _collectProjectsClosure(closure, project->_children);
    }
}

//////////
//  Serialization
void Database::_save()
{
    Q_ASSERT(_guard.isLockedByCurrentThread());
    _ensureOpen();  //  may throw

    //  Make sure we're consistent
    _validate();    //  may throw

    //  Create DOM document with a root node
    QDomDocument document;
    QDomProcessingInstruction xmlDeclaration = document.createProcessingInstruction("xml", "version='1.0' encoding='UTF-8' standalone='yes'");
    document.appendChild(xmlDeclaration);

    QDomElement rootElement = document.createElement("TT3");
    rootElement.setAttribute("FormatVersion", "1");
    document.appendChild(rootElement);

    _serializeAggregation(
        rootElement,
        "Users",
        _users);    //  + accounts, works, events, etc.
    _serializeAggregation(
        rootElement,
        "ActivityTypes",
        _activityTypes);
    _serializeAggregation(
        rootElement,
        "PublicActivities",
        _publicActivities);
    _serializeAggregation(
        rootElement,
        "PublicTasks",
        _rootPublicTasks);
    _serializeAggregation(
        rootElement,
        "Projects",
        _rootProjects);
    _serializeAggregation(
        rootElement,
        "WorkStreams",
        _workStreams);
    _serializeAggregation(
        rootElement,
        "Beneficiaries",
        _beneficiaries);

    //  Save DOM
    //  Use the renaming scheme for data safety:
    //  1.  Generate "old" and "new" XML file name
    //      from the "_address", making sure neither one exists.
    //  2.  Save XML to "new" file.
    //      If an error occurs, we throw, but the file
    //      at "_address" remains intact.
    //  3.  Rename "_address" file to "old" file.
    //      If an error occurs, delete the "new" file and throw.
    //      The file at "_address" remains intact.
    //  4.  Rename "new" file to "_address" file.
    //      If an error occurs, delete the "new" file
    //      and attempt to rename "old" file to "address"
    //      file, then throw.
    //  5.  Delete the "old" file.

    //  Step 1
    QFile file(_address->_path);
    QFile oldFile(_address->_path + ".old");
    QFile newFile(_address->_path + ".new");
    oldFile.remove();
    newFile.remove();
    //  Step 2
    if (!newFile.open(QIODevice::WriteOnly | QIODevice::Text))
    {   //  OOPS!
        throw tt3::db::api::CustomDatabaseException(newFile.fileName() + ": " +  newFile.errorString());
    }
    QTextStream stream(&newFile);
    document.save(stream, 4);
    newFile.close();
    //  Step 3
    {
        QFile f(file.fileName());
        if (f.exists() && !f.rename(oldFile.fileName()))  //  changes "f" on success
        {   //  OOPS! Cleanup & throw
            newFile.remove();
            throw tt3::db::api::CustomDatabaseException(f.fileName() + ": " +  f.errorString());
        }
    }
    //  Step 4
    {
        QFile f(newFile.fileName());
        if (!f.rename(file.fileName())) //  changes "f" on success
        {   //  OOPS! Cleanup & throw
            newFile.remove();
            if (!oldFile.rename(file.fileName()))   //  may fail
            {   //  No recovery possible, but we still can log
                qCritical() << oldFile.fileName() + ": " +  oldFile.errorString();
            }
            throw tt3::db::api::CustomDatabaseException(f.fileName() + ": " +  f.errorString());
        }
    }
    //  Step 5
    oldFile.remove();

    //  All done
    _needsSaving = false;
}

//////////
//  Deserialization
void Database::_load()
{
    Q_ASSERT(_guard.isLockedByCurrentThread());
    _ensureOpen();  //  may throw

    _deserializationMap.clear();
    //  Load XML DOM
    QDomDocument document;
    QFile file(_address->_path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {   //  OOPS!
        throw tt3::db::api::CustomDatabaseException(_address->displayForm() + ": " + file.errorString());
    }
    if (!document.setContent(&file))
    {   //  OOPS!
        throw tt3::db::api::DatabaseCorruptException(_address);
    }

    //  Validate root element
    QDomElement rootElement = document.documentElement();
    if (rootElement.isNull() ||
        rootElement.tagName() != "TT3" ||
        rootElement.attribute("FormatVersion") != "1")
    {   //  OOPS!
        throw tt3::db::api::DatabaseCorruptException(_address);
    }

    _deserializeAggregation<User>(
        rootElement,
        "Users",
        _users,
        [&](auto oid)
        {
            return new User(this, oid);
        }); //  + accounts, works, events, etc.
    _deserializeAggregation<ActivityType>(
        rootElement,
        "ActivityTypes",
        _activityTypes,
        [&](auto oid)
        {
            return new ActivityType(this, oid);
        });
    _deserializeAggregation<PublicActivity>(
        rootElement,
        "PublicActivities",
        _publicActivities,
        [&](auto oid)
        {
            return new PublicActivity(this, oid);
        });
    _deserializeAggregation<PublicTask>(
        rootElement,
        "PublicTasks",
        _rootPublicTasks,
        [&](auto oid)
        {
            return new PublicTask(this, oid);
        });
    _deserializeAggregation<Project>(
        rootElement,
        "Projects",
        _rootProjects,
        [&](auto oid)
        {
            return new Project(this, oid);
        });
    _deserializeAggregation<WorkStream>(
        rootElement,
        "WorkStreams",
        _workStreams,
        [&](auto oid)
        {
            return new WorkStream(this, oid);
        });
    _deserializeAggregation<Beneficiary>(
        rootElement,
        "Beneficiaries",
        _beneficiaries,
        [&](auto oid)
        {
            return new Beneficiary(this, oid);
        });

    //  Now we can do the asociations
    Q_ASSERT(_liveObjects.size() == _deserializationMap.size());
    for (Object * object : _deserializationMap.keys())
    {
        object->_deserializeAssociations(_deserializationMap[object]);
    }

    //  Done loading - make sure we're consistent
    _deserializationMap.clear();
    _validate();    //  may throw
}

QList<QDomElement> Database::_childElements(
        const QDomElement & parentElement,
        const QString & tagName
    )
{
    QList<QDomElement> result;
    for (QDomNode childNode = parentElement.firstChild();
         !childNode.isNull();
         childNode = childNode.nextSibling())
    {
        if (childNode.isElement())
        {
            QDomElement childElement = childNode.toElement();
            if (childElement.tagName() == tagName)
            {
                result.append(childElement);
            }
        }
    }
    return result;
}

auto Database::_childElement(
        const QDomElement & parentElement,
        const QString & tagName
    ) -> QDomElement
{
    QList<QDomElement> children = _childElements(parentElement, tagName);
    if (children.size() != 1)
    {   //  OOPS!
        throw tt3::db::api::DatabaseCorruptException(_address);
    }
    return children[0];
}

//////////
//  Validation
void Database::_validate()
{
    Objects validatedObjects;

    for (User * user : qAsConst(_users))
    {
        if (user == nullptr || !user->_isLive ||
            user->_database != this)
        {   //  OOPS!
            throw tt3::db::api::DatabaseCorruptException(this->_address);
        }
        user->_validate(validatedObjects);
    }
    for (ActivityType * activityType : qAsConst(_activityTypes))
    {
        if (activityType == nullptr || !activityType->_isLive ||
            activityType->_database != this)
        {   //  OOPS!
            throw tt3::db::api::DatabaseCorruptException(this->_address);
        }
        activityType->_validate(validatedObjects);
        if (activityType->_siblingExists(activityType->_displayName))
        {   //  OOPS!
            throw tt3::db::api::DatabaseCorruptException(this->_address);
        }
    }
    for (PublicActivity * publicActivity : qAsConst(_publicActivities))
    {
        if (publicActivity == nullptr || !publicActivity->_isLive ||
            publicActivity->_database != this ||
            dynamic_cast<PublicTask*>(publicActivity) != nullptr)
        {   //  OOPS!
            throw tt3::db::api::DatabaseCorruptException(this->_address);
        }
        publicActivity->_validate(validatedObjects);
        if (publicActivity->_siblingExists(publicActivity->_displayName))
        {   //  OOPS!
            throw tt3::db::api::DatabaseCorruptException(this->_address);
        }
    }
    for (PublicTask * publicTask : qAsConst(_rootPublicTasks))
    {
        if (publicTask == nullptr || !publicTask->_isLive ||
            publicTask->_database != this ||
            publicTask->_parent != nullptr)
        {   //  OOPS!
            throw tt3::db::api::DatabaseCorruptException(this->_address);
        }
        publicTask->_validate(validatedObjects);
        if (publicTask->_siblingExists(publicTask->_displayName))
        {   //  OOPS!
            throw tt3::db::api::DatabaseCorruptException(this->_address);
        }
    }
    for (Project * project : qAsConst(_rootProjects))
    {
        if (project == nullptr || !project->_isLive ||
            project->_database != this ||
            project->_parent != nullptr)
        {   //  OOPS!
            throw tt3::db::api::DatabaseCorruptException(this->_address);
        }
        project->_validate(validatedObjects);
        if (project->_siblingExists(project->_displayName))
        {   //  OOPS!
            throw tt3::db::api::DatabaseCorruptException(this->_address);
        }
    }
    for (WorkStream * workStream : qAsConst(_workStreams))
    {
        if (workStream == nullptr || !workStream->_isLive ||
            workStream->_database != this)
        {   //  OOPS!
            throw tt3::db::api::DatabaseCorruptException(this->_address);
        }
        workStream->_validate(validatedObjects);
        if (workStream->_siblingExists(workStream->_displayName))
        {   //  OOPS!
            throw tt3::db::api::DatabaseCorruptException(this->_address);
        }
    }
    for (Beneficiary * beneficiary : qAsConst(_beneficiaries))
    {
        if (beneficiary == nullptr || !beneficiary->_isLive ||
            beneficiary->_database != this)
        {   //  OOPS!
            throw tt3::db::api::DatabaseCorruptException(this->_address);
        }
        beneficiary->_validate(validatedObjects);
        if (beneficiary->_siblingExists(beneficiary->_displayName))
        {   //  OOPS!
            throw tt3::db::api::DatabaseCorruptException(this->_address);
        }
    }

    //  Final checks
    if (validatedObjects.size() != _liveObjects.size())
    {   //  OOPS!
        throw tt3::db::api::DatabaseCorruptException(_address);
    }
}

//////////
//  Database::_LockRefresher
Database::_LockRefresher::_LockRefresher(Database * database)
    :   _database(database),
        _lockFile(database->_address->_path + ".lock")
{
    //  Open/create the lock file
    if (_lockFile.exists())
    {   //  Can we take over the stale lock ?
        QFileInfo fileInfo(_lockFile);
        if (fileInfo.isFile())
        {
            if (!_lockFile.open(QIODevice::ReadWrite))
            {   //  OOPS! Can't!
                throw tt3::db::api::CustomDatabaseException(_lockFile.errorString());
            }
            QDateTime lastModifiedAt = fileInfo.lastModified(QTimeZone::UTC);
            QDateTime utcNow = QDateTime::currentDateTimeUtc();
            if (lastModifiedAt.secsTo(utcNow) < StaleTimeoutMin * 60)
            {   //  OOPS! Lock too young
                throw tt3::db::api::DatabaseInUseException(_database->_address);
            }
            //  Lock is stale - take it over
            _lockFile.setFileTime(utcNow, QFileDevice::FileTime::FileModificationTime);
        }
        else
        {   //  OOPS! Can't take over a non-file
            throw tt3::db::api::DatabaseCorruptException(_database->_address);
        }
    }
    else
    {
        if (!_lockFile.open(QIODevice::NewOnly))
        {   //  OOPS! Can't!
            throw tt3::db::api::CustomDatabaseException(_lockFile.errorString());
        }
    }
}

Database::_LockRefresher::~_LockRefresher()
{
    _lockFile.close();
    _lockFile.remove();
}

void Database::_LockRefresher::run()
{
    const int WaitChunkMs = 1000;

    QDateTime waitChunkStartedAt = QDateTime::currentDateTimeUtc();
    while (!_stopRequested)
    {
        msleep(WaitChunkMs);
        QDateTime utcNow = QDateTime::currentDateTimeUtc();
        if (waitChunkStartedAt.secsTo(utcNow) < RefreshIntervalMin)
        {   //  Keep waiting
            continue;
        }
        //  It's time to update the last modification time of the lock file
        _lockFile.setFileTime(utcNow, QFileDevice::FileTime::FileModificationTime);
    }
}

//  End of tt3-db-xml/Database.cpp
