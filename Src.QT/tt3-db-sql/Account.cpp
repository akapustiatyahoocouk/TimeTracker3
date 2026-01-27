//
//  tt3-db-sql/Account.cpp - tt3::db::sql::Account class implementation
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
Account::Account(
        Database * database,
        qint64 pk
    ) : Principal(database, pk),
        //  Cached properties
        _login([this] { _loadCachedProperties(); }),
        _passwordHash([this] { _loadCachedProperties(); }),
        _capabilities([this] { _loadCachedProperties(); }),
        _fkUser([this] { _loadCachedProperties(); })
{
}

Account::~Account()
{
}

//////////
//  tt3::db::api::IAccount (properties)
QString Account::login() const
{
    tt3::util::Lock _(_database->guard);
    _ensureLive();  //  may throw

    return _login;  //  Cache load may throw
}

void Account::setLogin(
        const QString & login
    )
{
    tt3::util::Lock _(_database->guard);
    _ensureLiveAndWritable();   //  may throw

    //  Validate parameters
    if (!_database->validator()->account()->isValidLogin(login))
    {
        throw tt3::db::api::InvalidPropertyValueException(
            type(),
            "login",
            login);
    }

    //  Logins must be unique per database
    auto existingAccount = _database->findAccount(login);
    if (existingAccount != nullptr && existingAccount != this)
    {   //  OOPS! Already there!
        throw tt3::db::api::AlreadyExistsException(
            type(),
            "login",
            login);
    }

    if (login != _login)    //  Cache load may throw
    {   //  Make the change...
        //  TODO wrap database access into a transaction
        _saveLogin(login);  //  may throw
        _login = login;
        //  ...schedule change notifications...
        _database->_changeNotifier.post(
            new tt3::db::api::ObjectModifiedNotification(
                _database, type(), _oid));
        //  TODO post change notification to the database
        //  ...and we're done
    }
}

QString Account::passwordHash() const
{
    tt3::util::Lock _(_database->guard);
    _ensureLive();  //  may throw

    return _passwordHash;   //  Cache load may throw
}

void Account::setPassword(
        const QString & password
    )
{
    tt3::util::Lock _(_database->guard);
    _ensureLiveAndWritable();   //  may throw

    //  Validate parameters
    if (!_database->validator()->account()->isValidPassword(password))
    {
        throw tt3::db::api::InvalidPropertyValueException(
            type(),
            "password",
            password);
    }

    //  Do the work
    std::unique_ptr<tt3::util::IMessageDigest::Builder> digestBuilder
        { tt3::util::StandardMessageDigests::Sha1::instance()->createBuilder() };
    digestBuilder->digestFragment(password);
    QString passwordHash = digestBuilder->digestAsString();

    if (passwordHash != _passwordHash)  //  Cache load may throw
    {   //  Make the change...
        //  TODO wrap database access into a transaction
        _savePasswordHash(passwordHash);    //  may throw
        _passwordHash = passwordHash;
        //  ...schedule change notifications...
        _database->_changeNotifier.post(
            new tt3::db::api::ObjectModifiedNotification(
                _database, type(), _oid));
        //  TODO post change notification to the database
        //  ...and we're done
    }
}

auto Account::capabilities(
    ) const -> tt3::db::api::Capabilities
{
    tt3::util::Lock _(_database->guard);
    _ensureLive();  //  may throw

    return _capabilities;   //  Cache load may throw
}

void Account::setCapabilities(
        tt3::db::api::Capabilities capabilities
    )
{
    tt3::util::Lock _(_database->guard);
    _ensureLiveAndWritable();   //  may throw

    if (capabilities != _capabilities)
    {   //  Make the change...
        //  TODO wrap database access into a transaction
        _saveCapabilities(capabilities);    //  may throw
        _capabilities = capabilities;
        //  ...schedule change notifications...
        _database->_changeNotifier.post(
            new tt3::db::api::ObjectModifiedNotification(
                _database, type(), _oid));
        //  TODO post change notification to the database
        //  ...and we're done
    }
}

//////////
//  tt3::db::api::IAccount (associations)
auto Account::user(
    ) const -> tt3::db::api::IUser *
{
    tt3::util::Lock _(_database->guard);
    _ensureLive();  //  may throw

    return _database->_getObject<User>(_fkUser);    //  Cache load may throw
}

auto Account::quickPicksList(
    ) const -> QList<tt3::db::api::IActivity*>
{
    tt3::util::Lock _(_database->guard);
    _ensureLive();  //  may throw

    //  TODO impement
    return QList<tt3::db::api::IActivity*>();
}

void Account::setQuickPicksList(
        const QList<tt3::db::api::IActivity*> & /*quickPicksList*/
    )
{
    tt3::util::Lock _(_database->guard);
    _ensureLiveAndWritable();   //  may throw

    throw tt3::util::NotImplementedError();
}

auto Account::works(
    ) const -> tt3::db::api::Works
{
    tt3::util::Lock _(_database->guard);
    _ensureLive();  //  may throw

    throw tt3::util::NotImplementedError();
}

auto Account::works(
        const QDateTime & /*from*/,
        const QDateTime & /*to*/
    ) const -> tt3::db::api::Works
{
    tt3::util::Lock _(_database->guard);
    _ensureLive();  //  may throw

    //  TODO implement
    return tt3::db::api::Works();
}

auto Account::events(
    ) const -> tt3::db::api::Events
{
    tt3::util::Lock _(_database->guard);
    _ensureLive();  //  may throw

    throw tt3::util::NotImplementedError();
}

auto Account::events(
        const QDateTime & /*from*/,
        const QDateTime & /*to*/
    ) const -> tt3::db::api::Events
{
    tt3::util::Lock _(_database->guard);
    _ensureLive();  //  may throw

    //  TODO implement
    return tt3::db::api::Events();
}

//////////
//  tt3::db::api::IAccount (life cycle)
auto Account::createWork(
        const QDateTime & /*startedAt*/,
        const QDateTime & /*finishedAt*/,
        tt3::db::api::IActivity * /*activity*/
    ) -> tt3::db::api::IWork *
{
    tt3::util::Lock _(_database->guard);
    _ensureLiveAndWritable();   //  may throw

    throw tt3::util::NotImplementedError();
}

auto Account::createEvent(
        const QDateTime & /*occurredAt*/,
        const QString & /*summary*/,
        const tt3::db::api::Activities & /*activities*/
    ) -> tt3::db::api::IEvent *
{
    tt3::util::Lock _(_database->guard);
    _ensureLiveAndWritable();   //  may throw

    throw tt3::util::NotImplementedError();
}

//////////
//  Cached properties
void Account::_invalidateCachedProperties()
{
    Principal::_invalidateCachedProperties();
    _login.invalidate();
    _passwordHash.invalidate();
    _capabilities.invalidate();
}

void Account::_loadCachedProperties()
{
    Q_ASSERT(_database->guard.isLockedByCurrentThread());

    std::unique_ptr<Statement> stat
    {   _database->createStatement(
        "SELECT [objects].[oid] AS [oid],"
        "       [accounts].[fk_user] AS [fk_user],"
        "       [accounts].[enabled] AS [enabled],"
        "       [accounts].[emailaddresses] AS [emailaddresses],"
        "       [accounts].[login] AS [login],"
        "       [accounts].[passwordhash] AS [passwordhash],"
        "       [accounts].[administrator] AS [administrator],"
        "       [accounts].[manageusers] AS [manageusers],"
        "       [accounts].[manageactivitytypes] AS [manageactivitytypes],"
        "       [accounts].[managebeneficiaries] AS [managebeneficiaries],"
        "       [accounts].[manageworkloads] AS [manageworkloads],"
        "       [accounts].[managepublicactivities] AS [managepublicactivities],"
        "       [accounts].[managepublictasks] AS [managepublictasks],"
        "       [accounts].[manageprivateactivities] AS [manageprivateactivities],"
        "       [accounts].[manageprivatetasks] AS [manageprivatetasks],"
        "       [accounts].[logwork] AS [logwork],"
        "       [accounts].[logevents] AS [logevents],"
        "       [accounts].[generatereports] AS [generatereports],"
        "       [accounts].[backupandrestore] AS [backupandrestore]"
        "  FROM [objects],[accounts]"
        " WHERE [objects].[pk] = ?"
        "   AND [accounts].[pk] = [objects].[pk]") };
    stat->setIntParameter(0, _pk);
    std::unique_ptr<ResultSet> rs
        { stat->executeQuery() };   //  may throw
    if (!rs->next())
    {   //  OOPS! Account row does not exist
        _makeDead();
        throw tt3::db::api::InstanceDeadException();
    }
    //  Account row exists and is now "current" in "rs"
    _oid = rs->oidValue("oid");
    _enabled = rs->boolValue("enabled");
    QStringList emailAddresses;
    if (!rs->isNull("emailaddresses"))
    {
        emailAddresses = rs->stringValue("emailaddresses").split("\n");
    }
    _emailAddresses = emailAddresses;
    _login = rs->stringValue("login");
    _passwordHash = rs->stringValue("passwordhash");

    tt3::db::api::Capabilities capabilities;
    if (rs->boolValue("administrator"))
    {
        capabilities |= tt3::db::api::Capability::Administrator;
    }
    if (rs->boolValue("manageusers"))
    {
        capabilities |= tt3::db::api::Capability::ManageUsers;
    }
    if (rs->boolValue("manageactivitytypes"))
    {
        capabilities |= tt3::db::api::Capability::ManageActivityTypes;
    }
    if (rs->boolValue("managebeneficiaries"))
    {
        capabilities |= tt3::db::api::Capability::ManageBeneficiaries;
    }
    if (rs->boolValue("manageworkloads"))
    {
        capabilities |= tt3::db::api::Capability::ManageWorkloads;
    }
    if (rs->boolValue("managepublicactivities"))
    {
        capabilities |= tt3::db::api::Capability::ManagePublicActivities;
    }
    if (rs->boolValue("managepublictasks"))
    {
        capabilities |= tt3::db::api::Capability::ManagePublicTasks;
    }
    if (rs->boolValue("manageprivateactivities"))
    {
        capabilities |= tt3::db::api::Capability::ManagePrivateActivities;
    }
    if (rs->boolValue("manageprivatetasks"))
    {
        capabilities |= tt3::db::api::Capability::ManagePrivateTasks;
    }
    if (rs->boolValue("logwork"))
    {
        capabilities |= tt3::db::api::Capability::LogWork;
    }
    if (rs->boolValue("logevents"))
    {
        capabilities |= tt3::db::api::Capability::LogEvents;
    }
    if (rs->boolValue("generatereports"))
    {
        capabilities |= tt3::db::api::Capability::GenerateReports;
    }
    if (rs->boolValue("backupandrestore"))
    {
        capabilities |= tt3::db::api::Capability::BackupAndRestore;
    }
    _capabilities = capabilities;

    _fkUser = rs->intValue("fk_user");
}

void Account::_saveLogin(const QString & login)
{
    Q_ASSERT(_database->guard.isLockedByCurrentThread());

    std::unique_ptr<Statement> stat
    {   _database->createStatement(
        "UPDATE [accounts]"
        "   SET [login] = ?"
        " WHERE [pk] = ?") };
    stat->setStringParameter(0, login);
    stat->setIntParameter(1, _pk);
    auto affectedRows = stat->execute();    //  may throw
    if (affectedRows == 0)
    {   //  OOPS! Row since deleted!
        _makeDead();
        throw tt3::db::api::InstanceDeadException();
    }
}

void Account::_savePasswordHash(const QString & passwordHash)
{
    Q_ASSERT(_database->guard.isLockedByCurrentThread());

    std::unique_ptr<Statement> stat
    {   _database->createStatement(
        "UPDATE [accounts]"
        "   SET [passwordhash] = ?"
        " WHERE [pk] = ?") };
    stat->setStringParameter(0, passwordHash);
    stat->setIntParameter(1, _pk);
    auto affectedRows = stat->execute();    //  may throw
    if (affectedRows == 0)
    {   //  OOPS! Row since deleted!
        _makeDead();
        throw tt3::db::api::InstanceDeadException();
    }
}

void Account::_saveCapabilities(tt3::db::api::Capabilities capabilities)
{
    Q_ASSERT(_database->guard.isLockedByCurrentThread());

    std::unique_ptr<Statement> stat
        {   _database->createStatement(
            "UPDATE [accounts]"
            "   SET [administrator] = ?,"
            "       [manageusers] = ?,"
            "       [manageactivitytypes] = ?,"
            "       [managebeneficiaries] = ?,"
            "       [manageworkloads] = ?,"
            "       [managepublicactivities] = ?,"
            "       [managepublictasks] = ?,"
            "       [manageprivateactivities] = ?,"
            "       [manageprivatetasks] = ?,"
            "       [logwork] = ?,"
            "       [logevents] = ?,"
            "       [generatereports] = ?,"
            "       [backupandrestore] = ?"
            " WHERE [pk] = ?") };
    stat->setBoolParameter(0, capabilities.contains(tt3::db::api::Capability::Administrator));
    stat->setBoolParameter(1, capabilities.contains(tt3::db::api::Capability::ManageUsers));
    stat->setBoolParameter(2, capabilities.contains(tt3::db::api::Capability::ManageActivityTypes));
    stat->setBoolParameter(3, capabilities.contains(tt3::db::api::Capability::ManageBeneficiaries));
    stat->setBoolParameter(4, capabilities.contains(tt3::db::api::Capability::ManageWorkloads));
    stat->setBoolParameter(5, capabilities.contains(tt3::db::api::Capability::ManagePublicActivities));
    stat->setBoolParameter(6, capabilities.contains(tt3::db::api::Capability::ManagePublicTasks));
    stat->setBoolParameter(7, capabilities.contains(tt3::db::api::Capability::ManagePrivateActivities));
    stat->setBoolParameter(8, capabilities.contains(tt3::db::api::Capability::ManagePrivateActivities));
    stat->setBoolParameter(9, capabilities.contains(tt3::db::api::Capability::LogWork));
    stat->setBoolParameter(10, capabilities.contains(tt3::db::api::Capability::LogEvents));
    stat->setBoolParameter(11, capabilities.contains(tt3::db::api::Capability::GenerateReports));
    stat->setBoolParameter(12, capabilities.contains(tt3::db::api::Capability::BackupAndRestore));
    stat->setIntParameter(13, _pk);
    auto affectedRows = stat->execute();    //  may throw
    if (affectedRows == 0)
    {   //  OOPS! Row since deleted!
        _makeDead();
        throw tt3::db::api::InstanceDeadException();
    }
}

//////////
//  Implementation helpers
void Account::_setPasswordHash(
        const QString & /*passwordHash*/
    )
{
    throw tt3::util::NotImplementedError();
}

void Account::_makeDead()
{
    Q_ASSERT(_database->guard.isLockedByCurrentThread());
    Q_ASSERT(_isLive);

    throw tt3::util::NotImplementedError();

    //  The rest is up to the base class
    Principal::_makeDead();
}

//  End of tt3-db-sql/Account.cpp
