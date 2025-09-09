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
Database::Database(DatabaseAddress * address, _Mode mode)
    :   _address(address),
        _validator(tt3::db::api::DefaultValidator::instance())
{
    Q_ASSERT(_address != nullptr);
    if (mode != _Mode::_Dead)
    {   //  ...because the DatabaseType holds that reference
        //  TODO this is UGLY!!!
        _address->addReference();
    }

    tt3::util::Lock lock(_guard);

    switch (mode)
    {
        case _Mode::_Create:
            //  Can't overwrite!
            if (QFile(_address->_path).exists())
            {   //  OOPS!
                delete _lockRefresher;
                throw tt3::db::api::AlreadyExistsException(
                    "XML file database", "location", _address->_path);
            }
            //  Need to save empty DB content, but
            //  obtain the lock first
            _lockRefresher = new _LockRefresher(this);
            try
            {
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
            catch (...)
            {   //  Cleanup & re-throw
                delete _lockRefresher;
                throw tt3::db::api::CustomDatabaseException("Unknown database error");
            }
            break;
        case _Mode::_Open:
            //  Need to load existing DB content, but
            //  obtain the lock first
            _lockRefresher = new _LockRefresher(this);
            try
            {
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
            catch (...)
            {   //  Cleanup & re-throw
                delete _lockRefresher;
                throw tt3::db::api::CustomDatabaseException("Unknown database error");
            }
            break;
        case _Mode::_Dead:
            //  We're creating a special "dead" database,
            //  where "dead" objects are parked until they
            //  lose their reference counts.
            //  We don't need a _lockRefresher
            _lockRefresher = nullptr;
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
    catch (...)
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
bool Database::isOpen() const
{
    tt3::util::Lock lock(_guard);

    return _lockRefresher != nullptr;
}

void Database::close()
{
    tt3::util::Lock lock(_guard);

    if (_lockRefresher == nullptr)
    {   //  Already closed
        return;
    }

    //  TODO Disconnect all slots from notification signals

    //  Save ?
    if (_needsSaving)
    {
        try
        {
            _save();    //  may throw!
        }
        catch (...)
        {   //  Cleanup & re-throw
            _markClosed();
            throw;
        }
    }

    //  Destroy all Object instances...
    for (User * user : _users.values())
    {
        user->destroy();    //  also destroys Accounts, Works, etc.
    }
    Q_ASSERT(_users.isEmpty());

    //  Done
    _markClosed();
}

//////////
//  tt3::db::api::IDatabase (associations)
auto Database::users(
    ) const -> tt3::db::api::Users
{
    tt3::util::Lock lock(_guard);

    return tt3::db::api::Users(_users.cbegin(), _users.cend());
}

auto Database::accounts(
    ) const -> tt3::db::api::Accounts
{
    tt3::util::Lock lock(_guard);

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

    return _findAccount(login);
}

auto Database::activityTypes(
    ) const -> tt3::db::api::ActivityTypes
{
    tt3::util::Lock lock(_guard);

    return tt3::db::api::ActivityTypes(_activityTypes.cbegin(), _activityTypes.cend());
}

auto Database::publicActivities(
    ) const -> tt3::db::api::PublicActivities
{
    throw tt3::db::api::CustomDatabaseException("Not yet implemented");
}

auto Database::publicActivitiesAndTasks(
    ) const -> tt3::db::api::PublicActivities
{
    throw tt3::db::api::CustomDatabaseException("Not yet implemented");
}

auto Database::publicTasks(
    ) const -> tt3::db::api::PublicTasks
{
    throw tt3::db::api::CustomDatabaseException("Not yet implemented");
}

auto Database::rootPublicTasks(
    ) const -> tt3::db::api::PublicTasks
{
    throw tt3::db::api::CustomDatabaseException("Not yet implemented");
}

auto Database::projects(
    ) const -> tt3::db::api::Projects
{
    throw tt3::db::api::CustomDatabaseException("Not yet implemented");
}

auto Database::rootProjects(
    ) const -> tt3::db::api::Projects
{
    throw tt3::db::api::CustomDatabaseException("Not yet implemented");
}

auto Database::workStreams(
    ) const -> tt3::db::api::WorkStreams
{
    throw tt3::db::api::CustomDatabaseException("Not yet implemented");
}

auto Database::beneficiaries(
    ) const -> tt3::db::api::Beneficiaries
{
    throw tt3::db::api::CustomDatabaseException("Not yet implemented");
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
    _ensureOpen();  //  may throw

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
    //  ...register it...
    Q_ASSERT(_liveObjects.contains(user->_oid));
    _needsSaving = true;
    //  ...schedule change notifications...
    _changeNotifier.post(
        new tt3::db::api::ObjectCreatedNotification(
            this, user->type(), user->_oid));
    //  ...and we're done
    return user;
}

auto Database::createActivityType(
        const QString & /*displayName*/,
        const QString & /*description*/
    ) -> tt3::db::api::IActivityType *
{
    throw tt3::db::api::CustomDatabaseException("Not yet implemented");
}

auto Database::createPublicActivity(
        const QString & /*displayName*/,
        const QString & /*description*/,
        const tt3::db::api::InactivityTimeout & /*timeout*/,
        bool /*requireCommentOnStart*/,
        bool /*requireCommentOnFinish*/,
        bool /*fullScreenReminder*/,
        tt3::db::api::IActivityType * /*activityType*/,
        tt3::db::api::IWorkload * /*workload*/
    ) -> tt3::db::api::IPublicActivity *
{
    throw tt3::db::api::CustomDatabaseException("Not yet implemented");
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
    throw tt3::db::api::CustomDatabaseException("Not yet implemented");
}

auto Database::createProject(
        const QString & /*displayName*/,
        const QString & /*description*/,
        const tt3::db::api::Beneficiaries & /*beneficiaries*/
    ) -> tt3::db::api::IProject *
{
    throw tt3::db::api::CustomDatabaseException("Not yet implemented");
}

auto Database::createWorkStream(
        const QString & /*displayName*/,
        const QString & /*description*/,
        const tt3::db::api::Beneficiaries & /*beneficiaries*/
    ) -> tt3::db::api::IWorkStream *
{
    throw tt3::db::api::CustomDatabaseException("Not yet implemented");
}

auto Database::createBeneficiary(
        const QString & /*displayName*/,
        const QString & /*description*/
    ) -> tt3::db::api::IBeneficiary *
{
    throw tt3::db::api::CustomDatabaseException("Not yet implemented");
}

//////////
//  Implementation helpers
void Database::_ensureOpen() const
{
    Q_ASSERT(_guard.isLockedByCurrentThread());

    if (_lockRefresher == nullptr)
    {   //  OOPS!
        throw tt3::db::api::DatabaseClosedException();
    }
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
    _ensureOpen();

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

//////////
//  Serialization
void Database::_save()
{
    Q_ASSERT(_guard.isLockedByCurrentThread());
    _ensureOpen();

    //  Make sure we're consistent
    _validate();

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
    _ensureOpen();

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

    //  Done loading - make sure we're consistent
    _validate();
}

QList<QDomElement> Database::_childElements(const QDomElement & parentElement, const QString & tagName)
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
    QSet<Object*> validatedObjects;

    //  Validate users (+ accounts, etc.)
    for (User * user : _users)
    {
        user->_validate(validatedObjects);
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
