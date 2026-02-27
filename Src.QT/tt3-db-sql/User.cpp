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
        //  Begin transaction for the changes
        Transaction transaction(_database); //  may throw
        //  Save, THEN cache
        _saveRealName(realName);    //  may throw
        _realName = realName;
        //  We're done with the changes
        transaction.commit();   //  may throw

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
        //  Begin transaction for the changes
        Transaction transaction(_database); //  may throw
        //  Save, THEN cache
        _saveInactivityTimeout(inactivityTimeout);  //  may throw
        _inactivityTimeout = inactivityTimeout;
        //  We're done with the changes
        transaction.commit();   //  may throw

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
        //  Begin transaction for the changes
        Transaction transaction(_database); //  may throw
        //  Save, THEN cache
        _saveUiLocale(uiLocale);    //  may throw
        _uiLocale = uiLocale;
        //  We're done with the changes
        transaction.commit();   //  may throw

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
    tt3::util::Lock _(_database->guard);
    _ensureLive();

    //  TODO cache PKs
    std::unique_ptr<Statement> stat
    {   _database->createStatement(
            "SELECT [pk]"
            "  FROM [accounts]"
            " WHERE [fk_user] = ?") };
    stat->setIntParameter(0, _pk);
    std::unique_ptr<ResultSet> rs
        { stat->executeQuery() };   //  may throw
    tt3::db::api::Accounts result;
    while (rs->next())
    {
        result.insert(_database->_getObject<Account>(rs->intValue(0)));
    }
    return result;
}

auto User::privateActivities(
    ) const -> tt3::db::api::PrivateActivities
{
    tt3::util::Lock _(_database->guard);
    _ensureLive();

    //  TODO cache PKs
    std::unique_ptr<Statement> stat
    {   _database->createStatement(
        "SELECT [pk]"
        "  FROM [activities]"
        " WHERE [fk_owner] = ?"         //  Private to this UserPublic}
        "   AND [completed] IS NULL") };//  Activity
    stat->setIntParameter(0, _pk);
    std::unique_ptr<ResultSet> rs
        { stat->executeQuery() };   //  may throw
    tt3::db::api::PrivateActivities result;
    while (rs->next())
    {
        result.insert(_database->_getObject<PrivateActivity>(rs->intValue(0)));
    }
    return result;
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
    tt3::util::Lock _(_database->guard);
    _ensureLive();

    //  TODO cache PKs
    std::unique_ptr<Statement> stat
    {   _database->createStatement(
        "SELECT [pk]"
        "  FROM [activities]"
        " WHERE [fk_owner] = ?"             //  Private to this UserPublic}
        "   AND [fk_parent] IS NULL"        //  Root
        "   AND [completed] IS NOT NULL") };//  Task
    stat->setIntParameter(0, _pk);
    std::unique_ptr<ResultSet> rs
        { stat->executeQuery() };   //  may throw
    tt3::db::api::PrivateTasks result;
    while (rs->next())
    {
        result.insert(_database->_getObject<PrivateTask>(rs->intValue(0)));
    }
    return result;
}

auto User::permittedWorkloads(
    ) const -> tt3::db::api::Workloads
{
    tt3::util::Lock _(_database->guard);
    _ensureLive();

    //  TODO cache PKs
    std::unique_ptr<Statement> stat
    {   _database->createStatement(
        "SELECT [fk_workload]"
        "  FROM [user_workloads]"
        " WHERE [fk_user] = ?") };
    stat->setIntParameter(0, _pk);
    std::unique_ptr<ResultSet> rs
        { stat->executeQuery() };   //  may throw
    tt3::db::api::Workloads result;
    while (rs->next())
    {
        result.insert(_database->_getWorkload(rs->intValue(0)));
    }
    return result;
}

void User::setPermittedWorkloads(
        const tt3::db::api::Workloads & workloads
    )
{
    tt3::util::Lock _(_database->guard);
    _ensureLiveAndWritable();   //  may throw

    //  Validate parameters
    if (workloads.contains(nullptr))
    {
        throw tt3::db::api::InvalidPropertyValueException(
            type(),
            "workloads",
            nullptr);
    }

    Workloads oldWorkloads =
        tt3::util::transform(
            permittedWorkloads(),   //  may throw
            [&](auto w)
            {
                Q_ASSERT(w != nullptr); //  should have been caught earlier!
                auto sqlWorkload = dynamic_cast<Workload*>(w);
                if (sqlWorkload == nullptr ||
                    sqlWorkload->_database != this->_database ||
                    !sqlWorkload->_isLive)
                {   //  OOPS!
                    throw tt3::db::api::IncompatibleInstanceException(w->type());
                }
                return sqlWorkload;
            });
    Workloads newWorkloads =
        tt3::util::transform(
            workloads,
            [&](auto w)
            {
                Q_ASSERT(w != nullptr); //  should have been caught earlier!
                auto sqlWorkload = dynamic_cast<Workload*>(w);
                if (sqlWorkload == nullptr ||
                    sqlWorkload->_database != this->_database ||
                    !sqlWorkload->_isLive)
                {   //  OOPS!
                    throw tt3::db::api::IncompatibleInstanceException(w->type());
                }
                return sqlWorkload;
            });
    if (oldWorkloads != newWorkloads)
    {   //  Make the changes
        //  Begin transaction for the changes
        Transaction transaction(_database); //  may throw
        //  Remove Workloads that are no longer relevant
        for (auto workload : std::as_const(oldWorkloads))
        {
            if (!newWorkloads.contains(workload))
            {
                std::unique_ptr<Statement> stat
                {   _database->createStatement(
                    "DELETE FROM [user_workloads]"
                    " WHERE [fk_user] = ?"
                    "   AND [fk_workload] = ?") };
                stat->setIntParameter(0, _pk);
                stat->setIntParameter(1, workload->_pk);
                stat->execute();    //  may throw
                //  Schedule change notifications
                _database->_changeNotifier.post(
                    new tt3::db::api::ObjectModifiedNotification(
                        _database, workload->type(), workload->_oid));  //  Cache load may throw
            }
        }
        //  Add wWorkloads that are new
        for (auto workload : std::as_const(newWorkloads))
        {
            if (!oldWorkloads.contains(workload))
            {
                std::unique_ptr<Statement> stat
                {   _database->createStatement(
                    "INSERT INTO [user_workloads]"
                    "       ([fk_user],[fk_workload])"
                    "       SELECT ?,?"
                    "        WHERE NOT EXISTS ("
                    "           SELECT 1"
                    "             FROM [user_workloads]"
                    "            WHERE [fk_user] = ?"
                    "              AND [fk_workload] = ?)") };
                stat->setIntParameter(0, _pk);
                stat->setIntParameter(1, workload->_pk);
                stat->setIntParameter(2, _pk);
                stat->setIntParameter(3, workload->_pk);
                stat->execute();    //  may throw
                //  Schedule change notifications
                _database->_changeNotifier.post(
                    new tt3::db::api::ObjectModifiedNotification(
                        _database, workload->type(), workload->_oid));  //  Cache load may throw
            }
        }
        //  We're done with the changes
        transaction.commit();   //  may throw
        //  Schedule change notifications
        _database->_changeNotifier.post(
            new tt3::db::api::ObjectModifiedNotification(
                _database, type(), _oid));  //  Cache load may throw
        //  TODO affected Workloads are Modified
    }
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

    //  Begin transaction for the changes
    Transaction transaction(_database); //  may throw

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
    stat->setIntParameter(0, std::get<0>(objIds));
    stat->setIntParameter(1, _pk);
    stat->setBoolParameter(2, enabled);
    emailAddresses.isEmpty() ?
        stat->setNullParameter(3) :
        stat->setStringParameter(3, emailAddresses.join('\n'));
    stat->setStringParameter(4, login);
    stat->setStringParameter(5, passwordHash);
    stat->setBoolParameter(6, capabilities.contains(tt3::db::api::Capability::Administrator));
    stat->setBoolParameter(7, capabilities.contains(tt3::db::api::Capability::ManageUsers));
    stat->setBoolParameter(8, capabilities.contains(tt3::db::api::Capability::ManageActivityTypes));
    stat->setBoolParameter(9, capabilities.contains(tt3::db::api::Capability::ManageBeneficiaries));
    stat->setBoolParameter(10, capabilities.contains(tt3::db::api::Capability::ManageWorkloads));
    stat->setBoolParameter(11, capabilities.contains(tt3::db::api::Capability::ManagePublicActivities));
    stat->setBoolParameter(12, capabilities.contains(tt3::db::api::Capability::ManagePublicTasks));
    stat->setBoolParameter(13, capabilities.contains(tt3::db::api::Capability::ManagePrivateActivities));
    stat->setBoolParameter(14, capabilities.contains(tt3::db::api::Capability::ManagePrivateTasks));
    stat->setBoolParameter(15, capabilities.contains(tt3::db::api::Capability::LogWork));
    stat->setBoolParameter(16, capabilities.contains(tt3::db::api::Capability::LogEvents));
    stat->setBoolParameter(17, capabilities.contains(tt3::db::api::Capability::GenerateReports));
    stat->setBoolParameter(18, capabilities.contains(tt3::db::api::Capability::BackupAndRestore));
    stat->execute();    //  may throw

    //  We're done with the changes
    transaction.commit();   //  may throw

    //  Create & register the Account object...
    Account * account = new Account(_database, std::get<0>(objIds));
    //  ...setting its cached properties to initial values
    account->_oid = std::get<1>(objIds);
    account->_enabled = enabled;
    account->_emailAddresses = emailAddresses;
    account->_login = login;
    account->_passwordHash = passwordHash;
    account->_capabilities = capabilities;
    account->_fkUser = _pk;

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
    tt3::util::Lock _(_database->guard);
    _ensureLiveAndWritable();

    //  Validate parameters
    if (!_database->validator()->publicActivity()->isValidDisplayName(displayName))
    {
        throw tt3::db::api::InvalidPropertyValueException(
            tt3::db::api::ObjectTypes::PublicActivity::instance(),
            "displayName",
            displayName);
    }
    if (!_database->validator()->publicActivity()->isValidDescription(description))
    {
        throw tt3::db::api::InvalidPropertyValueException(
            tt3::db::api::ObjectTypes::PublicActivity::instance(),
            "description",
            description);
    }
    if (timeout.has_value() &&
        !_database->validator()->publicActivity()->isValidTimeout(timeout))
    {
        throw tt3::db::api::InvalidPropertyValueException(
            tt3::db::api::ObjectTypes::PublicActivity::instance(),
            "timeout",
            timeout.value());
    }

    ActivityType * sqlActivityType = nullptr;
    if (activityType != nullptr)
    {
        sqlActivityType = dynamic_cast<ActivityType*>(activityType);
        if (sqlActivityType == nullptr ||
            sqlActivityType->_database != _database ||
            !sqlActivityType->_isLive)
        {   //  OOPS!
            throw tt3::db::api::IncompatibleInstanceException(activityType->type());
        }
    }

    Workload * sqlWorkload = nullptr;
    if (workload != nullptr)
    {
        sqlWorkload = dynamic_cast<Workload*>(workload);
        if (sqlWorkload == nullptr ||
            sqlWorkload->_database != _database ||
            !sqlWorkload->_isLive)
        {   //  OOPS!
            throw tt3::db::api::IncompatibleInstanceException(workload->type());
        }
    }

    //  Display names must be unique
    //  SQL "UNIQUE displayname" constraint would take care of
    //  that, but try for a better (non-SQL) error message
    if (_findPrivateActivity(displayName) != nullptr)
    {   //  OOPS!
        throw tt3::db::api::AlreadyExistsException(
            tt3::db::api::ObjectTypes::PrivateActivity::instance(),
            "displayName",
            displayName);
    }

    //  Begin transaction for the changes
    Transaction transaction(_database);  //  may throw

    //  Do the work - create [objects] row..
    Database::_ObjIds objIds = _database->_createObject(tt3::db::api::ObjectTypes::PrivateActivity::instance()); //  may throw
    //  ...then [activities] row...
    std::unique_ptr<Statement> stat
    {   _database->createStatement(
        "INSERT INTO [activities]"
        "       ([pk],"
        "        [fk_parent],[fk_owner],[fk_type],[fk_workload],"
        "        [displayname],[description],[timeout],"
        "        [requirecommentonstart],"
        "        [requirecommentonstop],"
        "        [fullscreenreminder],"
        "        [completed],[requirecommentoncompletion])"
        "       VALUES(?,?,?,?,?,?,?,?,?,?,?,?,?)") };
    stat->setIntParameter(0, std::get<0>(objIds));
    stat->setNullParameter(1);
    stat->setIntParameter(2, _pk);
    (sqlActivityType != nullptr) ?
        stat->setIntParameter(3, sqlActivityType->_pk) :
        stat->setNullParameter(3);
    (sqlWorkload != nullptr) ?
        stat->setIntParameter(4, sqlWorkload->_pk) :
        stat->setNullParameter(4);
    stat->setStringParameter(5, displayName);
    description.isEmpty() ?
        stat->setNullParameter(6) :
        stat->setStringParameter(6, description);
    timeout.has_value() ?
        stat->setTimeSpanParameter(7, timeout.value()) :
        stat->setNullParameter(7);
    stat->setBoolParameter(8, requireCommentOnStart);
    stat->setBoolParameter(9, requireCommentOnStop);
    stat->setBoolParameter(10, fullScreenReminder);
    stat->setNullParameter(11);
    stat->setNullParameter(12);
    stat->execute();    //  may throw

    //  We're done with the changes
    transaction.commit();   //  may throw

    //  Create & register the PrivateActivity object...
    PrivateActivity * privateActivity = new PrivateActivity(_database, std::get<0>(objIds));
    //  ...setting its cached properties to initial values
    privateActivity->_oid = std::get<1>(objIds);
    privateActivity->_displayName = displayName;
    privateActivity->_description = description;
    privateActivity->_timeout = timeout;
    privateActivity->_requireCommentOnStart = requireCommentOnStart;
    privateActivity->_requireCommentOnStop = requireCommentOnStop;
    privateActivity->_fullScreenReminder = fullScreenReminder;
    privateActivity->_fkActivityType =
        (sqlActivityType != nullptr) ?
            sqlActivityType->_pk :
            std::optional<qint64>();
    privateActivity->_fkWorkload =
        (sqlWorkload != nullptr) ?
            sqlWorkload->_pk :
            std::optional<qint64>();
    privateActivity->_fkOwner = _pk;

    //  ...schedule change notifications...
    _database->_changeNotifier.post(
        new tt3::db::api::ObjectCreatedNotification(
            _database, privateActivity->type(), privateActivity->_oid));    //  Cache load may throw
    if (sqlActivityType != nullptr)
    {
        _database->_changeNotifier.post(
            new tt3::db::api::ObjectModifiedNotification(
                _database, sqlActivityType->type(), sqlActivityType->_oid)); //  Cache load may throw
    }
    if (sqlWorkload != nullptr)
    {
        _database->_changeNotifier.post(
            new tt3::db::api::ObjectModifiedNotification(
                _database, sqlWorkload->type(), sqlWorkload->_oid)); //  Cache load may throw
    }
    //  TODO post change notification to the database

    //  ...and we're done
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
    tt3::util::Lock _(_database->guard);
    _ensureLiveAndWritable();   //  may throw

    //  Validate parameters
    if (!_database->validator()->publicTask()->isValidDisplayName(displayName))
    {
        throw tt3::db::api::InvalidPropertyValueException(
            tt3::db::api::ObjectTypes::PublicTask::instance(),
            "displayName",
            displayName);
    }
    if (!_database->validator()->publicTask()->isValidDescription(description))
    {
        throw tt3::db::api::InvalidPropertyValueException(
            tt3::db::api::ObjectTypes::PublicTask::instance(),
            "description",
            description);
    }
    if (timeout.has_value() &&
        !_database->validator()->publicTask()->isValidTimeout(timeout))
    {
        throw tt3::db::api::InvalidPropertyValueException(
            tt3::db::api::ObjectTypes::PublicTask::instance(),
            "timeout",
            timeout.value());
    }

    ActivityType * sqlActivityType = nullptr;
    if (activityType != nullptr)
    {
        sqlActivityType = dynamic_cast<ActivityType*>(activityType);
        if (sqlActivityType == nullptr ||
            sqlActivityType->_database != _database ||
            !sqlActivityType->_isLive)
        {   //  OOPS!
            throw tt3::db::api::IncompatibleInstanceException(activityType->type());
        }
    }

    Workload * sqlWorkload = nullptr;
    if (workload != nullptr)
    {
        sqlWorkload = dynamic_cast<Workload*>(workload);
        if (sqlWorkload == nullptr ||
            sqlWorkload->_database != _database ||
            !sqlWorkload->_isLive)
        {   //  OOPS!
            throw tt3::db::api::IncompatibleInstanceException(workload->type());
        }
    }

    //  Display names must be unique
    //  SQL "UNIQUE displayname" constraint would take care of
    //  that, but try for a better (non-SQL) error message
    if (_findRootPrivateTask(displayName) != nullptr)
    {   //  OOPS!
        throw tt3::db::api::AlreadyExistsException(
            tt3::db::api::ObjectTypes::PrivateTask::instance(),
            "displayName",
            displayName);
    }

    //  Begin transaction for the changes
    Transaction transaction(_database);  //  may throw

    //  Do the work - create [objects] row..
    Database::_ObjIds objIds = _database->_createObject(tt3::db::api::ObjectTypes::PublicTask::instance()); //  may throw
    //  ...then [activities] row...
    std::unique_ptr<Statement> stat
    {   _database->createStatement(
        "INSERT INTO [activities]"
        "       ([pk],"
        "        [fk_parent],[fk_owner],[fk_type],[fk_workload],"
        "        [displayname],[description],[timeout],"
        "        [requirecommentonstart],"
        "        [requirecommentonstop],"
        "        [fullscreenreminder],"
        "        [completed],[requirecommentoncompletion])"
        "       VALUES(?,?,?,?,?,?,?,?,?,?,?,?,?)") };
    stat->setIntParameter(0, std::get<0>(objIds));
    stat->setNullParameter(1);
    stat->setIntParameter(2, _pk);
    (sqlActivityType != nullptr) ?
        stat->setIntParameter(3, sqlActivityType->_pk) :
        stat->setNullParameter(3);
    (sqlWorkload != nullptr) ?
        stat->setIntParameter(4, sqlWorkload->_pk) :
        stat->setNullParameter(4);
    stat->setStringParameter(5, displayName);
    description.isEmpty() ?
        stat->setNullParameter(6) :
        stat->setStringParameter(6, description);
    timeout.has_value() ?
        stat->setTimeSpanParameter(7, timeout.value()) :
        stat->setNullParameter(7);
    stat->setBoolParameter(8, requireCommentOnStart);
    stat->setBoolParameter(9, requireCommentOnStop);
    stat->setBoolParameter(10, fullScreenReminder);
    stat->setBoolParameter(11, completed);
    stat->setBoolParameter(12, requireCommentOnCompletion);
    stat->execute();    //  may throw

    //  We're done with the changes
    transaction.commit();   //  may throw

    //  Create & register the PrivateTask object...
    PrivateTask * privateTask = new PrivateTask(_database, std::get<0>(objIds));
    //  ...setting its cached properties to initial values
    privateTask->_oid = std::get<1>(objIds);
    privateTask->_displayName = displayName;
    privateTask->_description = description;
    privateTask->_timeout = timeout;
    privateTask->_requireCommentOnStart = requireCommentOnStart;
    privateTask->_requireCommentOnStop = requireCommentOnStop;
    privateTask->_fullScreenReminder = fullScreenReminder;
    privateTask->_fkActivityType =
        (sqlActivityType != nullptr) ?
            sqlActivityType->_pk :
            std::optional<qint64>();
    privateTask->_fkWorkload =
        (sqlWorkload != nullptr) ?
            sqlWorkload->_pk :
            std::optional<qint64>();
    privateTask->_fkOwner = _pk;
    privateTask->_requireCommentOnCompletion = requireCommentOnCompletion;
    privateTask->_completed = completed;
    privateTask->_fkParent = std::optional<qint64>();

    //  ...schedule change notifications...
    _database->_changeNotifier.post(
        new tt3::db::api::ObjectCreatedNotification(
            _database, privateTask->type(), privateTask->_oid));    //  Cache load may throw
    if (sqlActivityType != nullptr)
    {
        _database->_changeNotifier.post(
            new tt3::db::api::ObjectModifiedNotification(
                _database, sqlActivityType->type(), sqlActivityType->_oid)); //  Cache load may throw
    }
    if (sqlWorkload != nullptr)
    {
        _database->_changeNotifier.post(
            new tt3::db::api::ObjectModifiedNotification(
                _database, sqlWorkload->type(), sqlWorkload->_oid)); //  Cache load may throw
    }
    //  TODO post change notification to the database

    //  ...and we're done
    return privateTask;
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
    Q_ASSERT(_database->guard.isLockedByCurrentThread());

    std::unique_ptr<Statement> stat
        {   _database->createStatement(
            "SELECT [objects].[oid] AS [oid],"
            "       [users].[enabled] AS [enabled],"
            "       [users].[emailaddresses] AS [emailaddresses],"
            "       [users].[realname] AS [realname],"
            "       [users].[inactivitytimeout] AS [inactivitytimeout],"
            "       [users].[uilocale] AS [uilocale]"
            "  FROM [objects],[users]"
            " WHERE [objects].[pk] = ?"
            "   AND [users].[pk] = [objects].[pk]") };
    stat->setIntParameter(0, _pk);
    std::unique_ptr<ResultSet> rs
        { stat->executeQuery() };   //  may throw
    if (!rs->next())
    {   //  OOPS! User row does not exist
        _makeDead();
        throw tt3::db::api::InstanceDeadException();
    }
    //  User row exists and is now "current" in "rs"
    _oid = rs->oidValue("oid");
    _enabled = rs->boolValue("enabled");
    QStringList emailAddresses;
    if (!rs->isNull("emailaddresses"))
    {
        emailAddresses = rs->stringValue("emailaddresses").split("\n");
    }
    _emailAddresses = emailAddresses;
    _realName = rs->stringValue("realname");

    tt3::db::api::InactivityTimeout inactivityTimeout;
    if (!rs->isNull("inactivitytimeout"))
    {
        inactivityTimeout = rs->timeSpanValue("inactivitytimeout");
    }
    _inactivityTimeout = inactivityTimeout;

    tt3::db::api::UiLocale uiLocale;
    if (!rs->isNull("uilocale"))
    {
        uiLocale = QLocale(rs->stringValue("uilocale"));
    }
    _uiLocale = uiLocale;
}

void User::_saveRealName(const QString & realName)
{
    Q_ASSERT(_database->guard.isLockedByCurrentThread());

    std::unique_ptr<Statement> stat
    {   _database->createStatement(
            "UPDATE [users]"
            "   SET [realname] = ?"
            " WHERE [pk] = ?") };
    stat->setStringParameter(0, realName);
    stat->setIntParameter(1, _pk);
    auto affectedRows = stat->execute();    //  may throw
    if (affectedRows == 0)
    {   //  OOPS! Row since deleted!
        _makeDead();
        throw tt3::db::api::InstanceDeadException();
    }
}

void User::_saveInactivityTimeout(const tt3::db::api::InactivityTimeout & inactivityTimeout)
{
    Q_ASSERT(_database->guard.isLockedByCurrentThread());

    std::unique_ptr<Statement> stat
    {   _database->createStatement(
        "UPDATE [users]"
        "   SET [inactivitytimeout] = ?"
        " WHERE [pk] = ?") };
    inactivityTimeout.has_value() ?
        stat->setTimeSpanParameter(0, inactivityTimeout.value()) :
        stat->setNullParameter(0);
    stat->setIntParameter(1, _pk);
    auto affectedRows = stat->execute();    //  may throw
    if (affectedRows == 0)
    {   //  OOPS! Row since deleted!
        _makeDead();
        throw tt3::db::api::InstanceDeadException();
    }
}

void User::_saveUiLocale(const tt3::db::api::UiLocale & uiLocale)
{
    Q_ASSERT(_database->guard.isLockedByCurrentThread());

    std::unique_ptr<Statement> stat
    {   _database->createStatement(
        "UPDATE [users]"
        "   SET [uilocale] = ?"
        " WHERE [pk] = ?") };
    uiLocale.has_value() ?
        stat->setStringParameter(0, tt3::util::toString(uiLocale.value())) :
        stat->setNullParameter(0);
    stat->setIntParameter(1, _pk);
    auto affectedRows = stat->execute();    //  may throw
    if (affectedRows == 0)
    {   //  OOPS! Row since deleted!
        _makeDead();
        throw tt3::db::api::InstanceDeadException();
    }
}

//////////
//  Implementation helpers
void User::_deleteCascade()
{
    Q_ASSERT(_database->guard.isLockedByCurrentThread());
    Q_ASSERT(_isLive);
    Q_ASSERT(_database->_liveObjects.contains(_pk));

    //  TODO private activities and tasks
    //  TODO Accounts
    throw tt3::util::NotImplementedError();
}

void User::_removeFromDatabase()
{
    Q_ASSERT(_database->guard.isLockedByCurrentThread());
    Q_ASSERT(_isLive);
    Q_ASSERT(_database->_liveObjects.contains(_pk));

    std::unique_ptr<Statement> stat
        {   _database->createStatement(
            "DELETE FROM [users]"
            " WHERE [pk] = ?") };
    stat->setIntParameter(0, _pk);
    stat->execute();    //  may throw
    Principal::_removeFromDatabase();
}

auto User::_findPrivateActivity(
        const QString & displayName
    ) const -> PrivateActivity *
{
    Q_ASSERT(_database->guard.isLockedByCurrentThread());
    Q_ASSERT(_isLive);
    Q_ASSERT(_database->_liveObjects.contains(_pk));

    //  TODO cache PKs
    std::unique_ptr<Statement> stat
    {   _database->createStatement(
        "SELECT [pk]"
        "  FROM [activities]"
        " WHERE [displayname] = ?"
        "   AND [fk_owner] = ?"         //  Private to this User
        "   AND [completed] IS NULL") };//  Activity
    stat->setStringParameter(0, displayName);
    stat->setIntParameter(1, _pk);
    std::unique_ptr<ResultSet> rs
        { stat->executeQuery() };
    if (rs->next())
    {   //  Got it!
        return _database->_getObject<PrivateActivity>(rs->intValue(0));
    }
    return nullptr;
}

auto User::_findRootPrivateTask(
        const QString & displayName
    ) const -> PrivateTask *
{
    Q_ASSERT(_database->guard.isLockedByCurrentThread());
    Q_ASSERT(_isLive);
    Q_ASSERT(_database->_liveObjects.contains(_pk));

    //  TODO cache PKs
    std::unique_ptr<Statement> stat
        {   _database->createStatement(
            "SELECT [pk]"
            "  FROM [activities]"
            " WHERE [displayname] = ?"
            "   AND [fk_owner] = ?"             //  Private to this User
            "   AND [fk_parent] IS NULL"        //  Root
            "   AND [completed] IS NOT NULL") };//  Task
    stat->setStringParameter(0, displayName);
    stat->setIntParameter(1, _pk);
    std::unique_ptr<ResultSet> rs
        { stat->executeQuery() };
    if (rs->next())
    {   //  Got it!
        return _database->_getObject<PrivateTask>(rs->intValue(0));
    }
    return nullptr;
}

//  End of tt3-db-sql/User.cpp
