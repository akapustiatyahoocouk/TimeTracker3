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
        _isReadOnly(openMode == _OpenMode::_OpenReadOnly)
{
    Q_ASSERT(_address != nullptr);
    if (openMode != _OpenMode::_Dead)
    {   //  ...because the DatabaseType holds that reference
        //  TODO this is UGLY!!!
        _address->addReference();
    }

    tt3::util::Lock lock(_guard);

    switch (openMode)
    {
        case _OpenMode::_Create:
            //  Can't overwrite!
            if (QFile(_address->_path).exists())
            {   //  OOPS!
                throw tt3::db::api::AlreadyExistsException(
                    "XML file database", "location", _address->_path);
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
}

Database::~Database()
{
    //  Close if still open
    try
    {
        close();
    }
    catch (const tt3::util::Exception &)
    {   //  OOPS! TODO log?
    }

    //  All Objects should be in graveyard by now
    {
        tt3::util::Lock lock(_guard);
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
    tt3::util::Lock lock(_guard);

    return _isOpen;
}

bool Database::isReadOnly() const
{   //  No need to synchronize
    return _isReadOnly;
}

void Database::close()
{
    tt3::util::Lock lock(_guard);

    if (!_isOpen)
    {   //  Already closed
        return;
    }

    //  TODO Disconnect all slots from notification signals

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
            qDebug() << ex.errorMessage();
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
    Q_ASSERT(_users.isEmpty());
    Q_ASSERT(_activityTypes.isEmpty());
    Q_ASSERT(_publicActivities.isEmpty());
    _isReadOnly = wasReadOnly;

    //  Done
    _markClosed();
}

void Database::refresh()
{
    tt3::util::Lock lock(_guard);

    _ensureOpen();  //  may throw
#ifdef Q_DEBUG  //  TODO add as pre/post condition to all public XML DB methods!
    _validate();    //  may throw
#endif
    //  ...otherwise an all-in-RAM database performs no caching
}

//////////
//  tt3::db::api::IDatabase (associations)
auto Database::users(
    ) const -> tt3::db::api::Users
{
    tt3::util::Lock lock(_guard);
    _ensureOpen();  //  may throw
    //  We assume database is consistent since last change

    return tt3::db::api::Users(_users.cbegin(), _users.cend());
}

auto Database::accounts(
    ) const -> tt3::db::api::Accounts
{
    tt3::util::Lock lock(_guard);
    _ensureOpen();  //  may throw
    //  We assume database is consistent since last change

    tt3::db::api::Accounts result;
    for (User * user : _users)
    {
        result += tt3::db::api::Accounts(user->_accounts.begin(), user->_accounts.end());
    }
    return result;
}

auto Database::findAccount(
        const QString & login
    ) const -> tt3::db::api::IAccount *
{
    tt3::util::Lock lock(_guard);
    _ensureOpen();  //  may throw
    //  We assume database is consistent since last change

    return _findAccount(login);
}

auto Database::activityTypes(
    ) const -> tt3::db::api::ActivityTypes
{
    tt3::util::Lock lock(_guard);
    _ensureOpen();  //  may throw
    //  We assume database is consistent since last change

    return tt3::db::api::ActivityTypes(_activityTypes.cbegin(), _activityTypes.cend());
}

auto Database::publicActivities(
    ) const -> tt3::db::api::PublicActivities
{
    tt3::util::Lock lock(_guard);
    _ensureOpen();  //  may throw
    //  We assume database is consistent since last change

    return tt3::db::api::PublicActivities(_publicActivities.cbegin(), _publicActivities.cend());
}

auto Database::publicActivitiesAndTasks(
    ) const -> tt3::db::api::PublicActivities
{
    throw tt3::util::NotImplementedError();
}

auto Database::publicTasks(
    ) const -> tt3::db::api::PublicTasks
{
    throw tt3::util::NotImplementedError();
}

auto Database::rootPublicTasks(
    ) const -> tt3::db::api::PublicTasks
{
    throw tt3::util::NotImplementedError();
}

auto Database::projects(
    ) const -> tt3::db::api::Projects
{
    throw tt3::util::NotImplementedError();
}

auto Database::rootProjects(
    ) const -> tt3::db::api::Projects
{
    throw tt3::util::NotImplementedError();
}

auto Database::workStreams(
    ) const -> tt3::db::api::WorkStreams
{
    throw tt3::util::NotImplementedError();
}

auto Database::beneficiaries(
    ) const -> tt3::db::api::Beneficiaries
{
    throw tt3::util::NotImplementedError();
}

//////////
//  tt3::db::api::IDatabase (access control)
auto Database::tryLogin(
        const QString & login,
        const QString & password
    ) const -> tt3::db::api::IAccount *
{
    static tt3::util::IMessageDigest * sha1 = tt3::util::Sha1MessageDigest::instance();  //  idempotent

    tt3::util::Lock lock(_guard);
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
            for (Account * account : user->_accounts)
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
        const tt3::db::api::UiLocale & uiLocale
    ) -> tt3::db::api::IUser *
{
    tt3::util::Lock lock(_guard);
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

    //  Do the work - create & initialize the User...
    User * user = new User(this, _generateOid()); //  registers with Database
    user->_enabled = enabled;
    user->_emailAddresses = emailAddresses;
    user->_realName = realName;
    user->_inactivityTimeout = inactivityTimeout;
    user->_uiLocale = uiLocale;
    _markModified();
    //  ...schedule change notifications...
    _changeNotifier.post(
        new tt3::db::api::ObjectCreatedNotification(
            this, user->type(), user->_oid));
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
    tt3::util::Lock lock(_guard);
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
        bool requireCommentOnFinish,
        bool fullScreenReminder,
        tt3::db::api::IActivityType * activityType,
        tt3::db::api::IWorkload * workload
    ) -> tt3::db::api::IPublicActivity *
{
    tt3::util::Lock lock(_guard);
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
            throw tt3::db::api::IncompatibleInstanceException(
                    tt3::db::api::ObjectTypes::ActivityType::instance());
        }
    }
    if (workload != nullptr)
    {
        Q_ASSERT(false);    //  TODO validate workload similarly to activityType
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
    publicActivity->_requireCommentOnFinish = requireCommentOnFinish;
    publicActivity->_fullScreenReminder = fullScreenReminder;
    if (xmlActivityType != nullptr)
    {   //  Link with ActivityType
        publicActivity->_activityType = xmlActivityType;
        xmlActivityType->_activities.insert(publicActivity);
        xmlActivityType->addReference();
        publicActivity->addReference();
    }
    Q_ASSERT(workload == nullptr);   // TODO Link with Workload
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
    //  TODO same notification for associated Workload
    //  ...and we're done
#ifdef Q_DEBUG
    _validate();    //  may throw
#endif
    return publicActivity;
}

auto Database::createPublicTask(
        const QString & /*displayName*/,
        const QString & /*description*/,
        const tt3::db::api::InactivityTimeout & /*timeout*/,
        bool /*requireCommentOnStart*/,
        bool /*requireCommentOnFinish*/,
        bool /*fullScreenReminder*/,
        tt3::db::api::IActivityType * /*activityType*/,
        tt3::db::api::IWorkload * /*workload*/,
        bool /*completed*/,
        bool /*requireCommentOnCompletion*/
    ) -> tt3::db::api::IPublicTask *
{
    throw tt3::util::NotImplementedError();
}

auto Database::createProject(
        const QString & /*displayName*/,
        const QString & /*description*/,
        const tt3::db::api::Beneficiaries & /*beneficiaries*/
    ) -> tt3::db::api::IProject *
{
    throw tt3::util::NotImplementedError();
}

auto Database::createWorkStream(
        const QString & /*displayName*/,
        const QString & /*description*/,
        const tt3::db::api::Beneficiaries & /*beneficiaries*/
    ) -> tt3::db::api::IWorkStream *
{
    throw tt3::util::NotImplementedError();
}

auto Database::createBeneficiary(
        const QString & /*displayName*/,
        const QString & /*description*/
    ) -> tt3::db::api::IBeneficiary *
{
    throw tt3::util::NotImplementedError();
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
    //  TODO save periodically
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
        //  Schedule "database is closed" change notification
        //  TODO keep? kill? _changeNotifier.post(new tt3::db::api::DatabaseClosedNotification(this));
        tt3::db::api::DatabaseClosedNotification n(this);
        _changeNotifier.databaseClosed(n);
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

Account * Database::_findAccount(const QString & login) const
{
    Q_ASSERT(_guard.isLockedByCurrentThread());
    _ensureOpen();  //  may throw

    for (User * user : _users)
    {
        for (Account * account : user->_accounts)
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

    //  Serialize users (+ accounts, etc.)
    QDomElement usersElement = document.createElement("Users");
    rootElement.appendChild(usersElement);
    for (User * user : _users)
    {   //  TODO try to sort by OID to reduce changes
        QDomElement userElement = document.createElement("User");
        usersElement.appendChild(userElement);
        //  Serialize user features
        user->_serializeProperties(userElement);
        user->_serializeAggregations(userElement);
        user->_serializeAssociations(userElement);
    }

    //  Serialize activity types
    QDomElement activityTypesElement = document.createElement("ActivityTypes");
    rootElement.appendChild(activityTypesElement);
    for (ActivityType * activityType : _activityTypes)
    {   //  TODO try to sort by OID to reduce changes
        QDomElement activityTypeElement = document.createElement("ActivityType");
        activityTypesElement.appendChild(activityTypeElement);
        //  Serialize activity type features
        activityType->_serializeProperties(activityTypeElement);
        activityType->_serializeAggregations(activityTypeElement);
        activityType->_serializeAssociations(activityTypeElement);
    }

    //  Serialize public activities
    QDomElement publicActivitiesElement = document.createElement("PublicActivities");
    rootElement.appendChild(publicActivitiesElement);
    for (PublicActivity * publicActivity : _publicActivities)
    {   //  TODO try to sort by OID to reduce changes
        QDomElement publicActivityElement = document.createElement("PublicActivity");
        publicActivitiesElement.appendChild(publicActivityElement);
        //  Serialize public activity features
        publicActivity->_serializeProperties(publicActivityElement);
        publicActivity->_serializeAggregations(publicActivityElement);
        publicActivity->_serializeAssociations(publicActivityElement);
    }

    //  Save DOM
    QFile file(_address->_path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {   //  OOPS!
        throw tt3::db::api::CustomDatabaseException(_address->displayForm() + ": " +  file.errorString());
    }
    QTextStream stream(&file);
    document.save(stream, 4);
    file.close();
    _needsSaving = false;
}

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

    //  Process users (+ nested Accounts, etc.)
    QDomElement usersElement = _childElement(rootElement, "Users");  //  may throw
    for (QDomElement userElement : _childElements(usersElement, "User"))
    {
        tt3::db::api::Oid oid = tt3::util::fromString<tt3::db::api::Oid>(userElement.attribute("OID", ""));
        if (!oid.isValid() || _liveObjects.contains(oid))
        {   //  OOPS!
            throw tt3::db::api::DatabaseCorruptException(_address);
        }
        User * user = new User(this, oid);
        user->_deserializeProperties(userElement);
        user->_deserializeAggregations(userElement);
    }

    //  Process activity types
    QDomElement activityTypesElement = _childElement(rootElement, "ActivityTypes");  //  may throw
    for (QDomElement activityTypeElement : _childElements(activityTypesElement, "ActivityType"))
    {
        tt3::db::api::Oid oid = tt3::util::fromString<tt3::db::api::Oid>(activityTypeElement.attribute("OID", ""));
        if (!oid.isValid() || _liveObjects.contains(oid))
        {   //  OOPS!
            throw tt3::db::api::DatabaseCorruptException(_address);
        }
        ActivityType * activityType = new ActivityType(this, oid);
        activityType->_deserializeProperties(activityTypeElement);
        activityType->_deserializeAggregations(activityTypeElement);
    }

    //  Process public activities
    QDomElement publicActivitiesElement = _childElement(rootElement, "PublicActivities");  //  may throw
    for (QDomElement publicActivityElement : _childElements(publicActivitiesElement, "PublicActivity"))
    {
        tt3::db::api::Oid oid = tt3::util::fromString<tt3::db::api::Oid>(publicActivityElement.attribute("OID", ""));
        if (!oid.isValid() || _liveObjects.contains(oid))
        {   //  OOPS!
            throw tt3::db::api::DatabaseCorruptException(_address);
        }
        PublicActivity * publicActivity = new PublicActivity(this, oid);
        publicActivity->_deserializeProperties(publicActivityElement);
        publicActivity->_deserializeAggregations(publicActivityElement);
    }

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

    for (User * user : _users)
    {
        if (user == nullptr || !user->_isLive ||
            user->_database != this)
        {   //  OOPS!
            throw tt3::db::api::DatabaseCorruptException(this->_address);
        }
        user->_validate(validatedObjects);
    }
    for (ActivityType * activityType : _activityTypes)
    {
        if (activityType == nullptr || !activityType->_isLive ||
            activityType->_database != this)
        {   //  OOPS!
            throw tt3::db::api::DatabaseCorruptException(this->_address);
        }
        activityType->_validate(validatedObjects);
    }
    for (PublicActivity * publicActivity : _publicActivities)
    {
        if (publicActivity == nullptr || !publicActivity->_isLive ||
            publicActivity->_database != this)
        {   //  OOPS!
            throw tt3::db::api::DatabaseCorruptException(this->_address);
        }
        publicActivity->_validate(validatedObjects);
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
