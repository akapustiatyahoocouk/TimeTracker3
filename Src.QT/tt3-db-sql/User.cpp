//
//  tt3-db-sql/User.cpp - tt3::db::sql::User class implementation
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
User::User(Database * database, qint64 pk)
    :   Principal(database, pk),
        //  Cached propertirs
        _realName([this] { _loadCachedProperties(); }),
        _inactivityTimeout([this] { _loadCachedProperties(); }),
        _uiLocale([this] { _loadCachedProperties(); })
{
}

User::~User()
{
}

//////////
//  tt3::db::api::IUser (properties)
QString User::realName() const
{
    tt3::util::Lock _(_database->guard);
    _ensureLive();  //  may throw

    return _realName;   //  Cache load may throw
}

void User::setRealName(
        const QString & realName
    )
{
    tt3::util::Lock _(_database->guard);
    _ensureLiveAndWritable();   //  may throw

    //  Validate parameters
    if (!_database->validator()->user()->isValidRealName(realName))
    {
        throw tt3::db::api::InvalidPropertyValueException(
            type(),
            "realName",
            realName);
    }

    if (realName != _realName)  //  Cache load may throw
    {   //  Make the change...
        //  TODO wrap database access into a transaction
        _saveRealName(realName);    //  may throw
        _realName = realName;
        //  ...schedule change notifications....
        _database->_changeNotifier.post(
            new tt3::db::api::ObjectModifiedNotification(
                _database, type(), _oid));
        //  TODO post change notification to the database
        //  ...and we're done
    }
}

auto User::inactivityTimeout(
    ) const -> tt3::db::api::InactivityTimeout
{
    tt3::util::Lock _(_database->guard);
    _ensureLive();  //  may throw

    return _inactivityTimeout;  //  Cache load may throw
}

void User::setInactivityTimeout(
        const tt3::db::api::InactivityTimeout & inactivityTimeout
    )
{
    tt3::util::Lock _(_database->guard);
    _ensureLiveAndWritable();   //  may throw

    //  Validate parameters
    if (inactivityTimeout.has_value() &&
        !_database->validator()->user()->isValidInactivityTimeout(inactivityTimeout.value()))
    {
        throw tt3::db::api::InvalidPropertyValueException(
            type(),
            "inactivityTimeout",
            inactivityTimeout.value());
    }

    if (inactivityTimeout != _inactivityTimeout.value())    //  Cache load may throw
    {   //  Make the change...
        //  TODO wrap database access into a transaction
        _saveInactivityTimeout(inactivityTimeout);  //  may throw
        _inactivityTimeout = inactivityTimeout;
        //  ...schedule change notifications...
        _database->_changeNotifier.post(
            new tt3::db::api::ObjectModifiedNotification(
                _database, type(), _oid));
        //  TODO post change notification to the database
        //  ...and we're done
    }
}

auto User::uiLocale(
    ) const -> tt3::db::api::UiLocale
{
    tt3::util::Lock _(_database->guard);
    _ensureLive();  //  may throw

    return _uiLocale;   //  Cache load may throw
}

void User::setUiLocale(
        const tt3::db::api::UiLocale & uiLocale
    )
{
    tt3::util::Lock _(_database->guard);
    _ensureLiveAndWritable();   //  may throw

    //  Validate parameters
    if (uiLocale.has_value() &&
        !_database->validator()->user()->isValidUiLocale(uiLocale.value()))
    {
        throw tt3::db::api::InvalidPropertyValueException(
            type(),
            "uiLocale",
            uiLocale.value());
    }

    if (uiLocale != _uiLocale.value())  //  Cache load may throw
    {   //  Make the change...
        //  TODO wrap database access into a transaction
        _saveUiLocale(uiLocale);    //  may throw
        _uiLocale = uiLocale;
        //  ...schedule change notifications...
        _database->_changeNotifier.post(
            new tt3::db::api::ObjectModifiedNotification(
                _database, type(), _oid));
        //  TODO post change notification to the database
        //  ...and we're done
    }
}

//////////
//  tt3::db::api::IUser (associations)
auto User::accounts(
    ) const -> tt3::db::api::Accounts
{
    throw tt3::util::NotImplementedError();
}

auto User::privateActivities(
    ) const -> tt3::db::api::PrivateActivities
{
    throw tt3::util::NotImplementedError();
}

auto User::privateActivitiesAndTasks(
    ) const -> tt3::db::api::PrivateActivities
{
    throw tt3::util::NotImplementedError();
}

auto User::privateTasks(
    ) const -> tt3::db::api::PrivateTasks
{
    throw tt3::util::NotImplementedError();
}

auto User::rootPrivateTasks(
    ) const -> tt3::db::api::PrivateTasks
{
    throw tt3::util::NotImplementedError();
}

auto User::permittedWorkloads(
    ) const -> tt3::db::api::Workloads
{
    throw tt3::util::NotImplementedError();
}

void User::setPermittedWorkloads(
        const tt3::db::api::Workloads & /*workloads*/
    )
{
    throw tt3::util::NotImplementedError();
}

void User::addPermittedWorkload(
        tt3::db::api::IWorkload * /*workload*/
    )
{
    throw tt3::util::NotImplementedError();
}

void User::removePermittedWorkload(
        tt3::db::api::IWorkload * /*workload*/
    )
{
    throw tt3::util::NotImplementedError();
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
    tt3::util::Lock _(_database->guard);
    _ensureLiveAndWritable();

    //  Validate parameters
    if (!_database->validator()->principal()->isValidEmailAddresses(emailAddresses))
    {
        throw tt3::db::api::InvalidPropertyValueException(
            tt3::db::api::ObjectTypes::Account::instance(),
            "emailAddresses",
            emailAddresses.join(','));
    }
    if (!_database->validator()->account()->isValidLogin(login))
    {
        throw tt3::db::api::InvalidPropertyValueException(
            tt3::db::api::ObjectTypes::Account::instance(),
            "login",
            login);
    }
    if (!_database->validator()->account()->isValidPassword(password))
    {
        throw tt3::db::api::InvalidPropertyValueException(
            tt3::db::api::ObjectTypes::Account::instance(),
            "password",
            password);
    }

    //  Logins must be unique per database.
    //  SQL "UNIQUE login" constraint would take care of
    //  that, but try for a better (non-SQL) error message
    if (_database->findAccount(login) != nullptr)
    {   //  OOPS! Already there!
        throw tt3::db::api::AlreadyExistsException(
            tt3::db::api::ObjectTypes::Account::instance(),
            "login",
            login);
    }

    std::unique_ptr<tt3::util::IMessageDigest::Builder> digestBuilder
        { tt3::util::StandardMessageDigests::Sha1::instance()->createBuilder() };
    digestBuilder->digestFragment(password);
    QString passwordHash = digestBuilder->digestAsString();

    Account * account = nullptr;
    try
    {
        _database->beginTransaction(); //  may throw

        //  Do the work - create [objects] row..
        Database::_ObjIds objIds = _database->_createObject(tt3::db::api::ObjectTypes::Account::instance());//  may throw
        //  ...then [users] row...
        std::unique_ptr<Statement> stat
            {   _database->createStatement(
                "INSERT INTO [accounts]"
                "       ([pk],[fk_user],[enabled],[emailaddresses],"
                "        [login],[passwordhash],"
                "        [administrator],"
                "        [manageusers],"
                "        [manageactivitytypes],"
                "        [managebeneficiaries],"
                "        [manageworkloads],"
                "        [managepublicactivities],"
                "        [managepublictasks],"
                "        [manageprivateactivities],"
                "        [manageprivatetasks],"
                "        [logwork],"
                "        [logevents],"
                "        [generatereports],"
                "        [backupandrestore])"
                "       VALUES(?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)") };
        stat->setParameter(0, std::get<0>(objIds));
        stat->setParameter(1, _pk);
        stat->setParameter(2, enabled);
        if (emailAddresses.isEmpty())
        {
            stat->setParameter(3, nullptr);
        }
        else
        {
            stat->setParameter(3, emailAddresses.join('\n'));
        }
        stat->setParameter(4, login);
        stat->setParameter(5, passwordHash);
        stat->setParameter(6, capabilities.contains(tt3::db::api::Capability::Administrator));
        stat->setParameter(7, capabilities.contains(tt3::db::api::Capability::ManageUsers));
        stat->setParameter(8, capabilities.contains(tt3::db::api::Capability::ManageActivityTypes));
        stat->setParameter(9, capabilities.contains(tt3::db::api::Capability::ManageBeneficiaries));
        stat->setParameter(10, capabilities.contains(tt3::db::api::Capability::ManageWorkloads));
        stat->setParameter(11, capabilities.contains(tt3::db::api::Capability::ManagePublicActivities));
        stat->setParameter(12, capabilities.contains(tt3::db::api::Capability::ManagePublicTasks));
        stat->setParameter(13, capabilities.contains(tt3::db::api::Capability::ManagePrivateActivities));
        stat->setParameter(14, capabilities.contains(tt3::db::api::Capability::ManagePrivateTasks));
        stat->setParameter(15, capabilities.contains(tt3::db::api::Capability::LogWork));
        stat->setParameter(16, capabilities.contains(tt3::db::api::Capability::LogEvents));
        stat->setParameter(17, capabilities.contains(tt3::db::api::Capability::GenerateReports));
        stat->setParameter(18, capabilities.contains(tt3::db::api::Capability::BackupAndRestore));
        stat->execute();    //  may throw

        _database->commitTransaction();//  may throw

        //  Create & register the Account object...
        account = new Account(_database, std::get<0>(objIds));
        //  ...setting its cached properties to initial values
        account->_oid = std::get<1>(objIds);
        account->_enabled = enabled;
        account->_emailAddresses = emailAddresses;
        account->_login = login;
        account->_passwordHash = passwordHash;
        account->_capabilities = capabilities;
    }
    catch (...)
    {   //  OOPS! Cleanup, then re-throw
        _database->rollbackTransaction();  //  may throw, but at this point who cares?
        throw;
    }
    //  ...schedule change notifications...
    _database->_changeNotifier.post(
        new tt3::db::api::ObjectModifiedNotification(
            _database, this->type(), this->_oid));
    _database->_changeNotifier.post(
        new tt3::db::api::ObjectCreatedNotification(
            _database, account->type(), account->_oid));
    //  TODO post change notifications to the database
    //  ...and we're done
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
    throw tt3::util::NotImplementedError();
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
    throw tt3::util::NotImplementedError();
}

//////////
//  Cached properties
void User::_invalidateCachedProperties()
{
    Principal::_invalidateCachedProperties();
    _realName.invalidate();
    _inactivityTimeout.invalidate();
    _uiLocale.invalidate();
}

void User::_loadCachedProperties()
{
    throw tt3::util::NotImplementedError();
}

void User::_saveRealName(const QString & realName)
{
    throw tt3::util::NotImplementedError();
}

void User::_saveInactivityTimeout(const tt3::db::api::InactivityTimeout & inactivityTimeout)
{
    throw tt3::util::NotImplementedError();
}

void User::_saveUiLocale(const tt3::db::api::UiLocale & uiLocale)
{
    throw tt3::util::NotImplementedError();
}

//////////
//  Implementation helpers
void User::_makeDead()
{
    Q_ASSERT(_database->guard.isLockedByCurrentThread());
    Q_ASSERT(_isLive);

    throw tt3::util::NotImplementedError();

    //  The rest is up to the base class
    Principal::_makeDead();
}

//  End of tt3-db-sql/User.cpp
