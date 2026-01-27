//
//  tt3-db-sql/Database.cpp - tt3::db::sql::Database class implementation
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
//  tt3::db::api::IDatabase (associations)
quint64 Database::objectCount(
    ) const
{
    throw tt3::util::NotImplementedError();
}

auto Database::findObjectByOid(
        const tt3::db::api::Oid & /*oid*/
    ) const -> tt3::db::api::IObject *
{
    throw tt3::util::NotImplementedError();
}

auto Database::users(
    ) const -> tt3::db::api::Users
{
    throw tt3::util::NotImplementedError();
}

auto Database::accounts(
    ) const -> tt3::db::api::Accounts
{
    throw tt3::util::NotImplementedError();
}

auto Database::findAccount(
        const QString & login
    ) const -> tt3::db::api::IAccount *
{
    tt3::util::Lock _(guard);
    ensureOpen();

    std::unique_ptr<Statement> stat
    {   createStatement(
            "SELECT [pk]"
            "  FROM [accounts]"
            " WHERE [login] = ?") };
    stat->setParameter(0, login);
    std::unique_ptr<ResultSet> rs
        { stat->executeQuery() };
    if (rs->next())
    {   //  Got it!
        return _getObject<Account>(rs->value(0, -1));
    }
    return nullptr;
}

auto Database::activityTypes(
    ) const -> tt3::db::api::ActivityTypes
{
    throw tt3::util::NotImplementedError();
}

auto Database::findActivityType(
        const QString & /*displayName*/
    ) -> tt3::db::api::IActivityType *
{
    throw tt3::util::NotImplementedError();
}

auto Database::publicActivities(
    ) const -> tt3::db::api::PublicActivities
{
    throw tt3::util::NotImplementedError();
}

auto Database::findPublicActivity(
        const QString & /*displayName*/
    ) -> tt3::db::api::IPublicActivity *
{
    throw tt3::util::NotImplementedError();
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
    tt3::util::Lock _(guard);
    ensureOpen();

    std::unique_ptr<tt3::util::IMessageDigest::Builder> digestBuilder
        { tt3::util::StandardMessageDigests::Sha1::instance()->createBuilder() };
    digestBuilder->digestFragment(password);
    QString passwordHash = digestBuilder->digestAsString();

    std::unique_ptr<Statement> stat
    {   createStatement(
            "SELECT [accounts].[pk]"
            "  FROM [users],[accounts]"
            " WHERE [accounts].[fk_user] = [users].[pk]"
            "   AND [accounts].[login] = ?"
            "   AND [accounts].[passwordhash] = ?"
            "   AND [users].[enabled] = 'Y'"
            "   AND [accounts].[enabled] = 'Y'") };
    stat->setParameter(0, login);
    stat->setParameter(1, passwordHash);
    std::unique_ptr<ResultSet> rs
        { stat->executeQuery() };
    if (rs->next())
    {
        return _getObject<Account>(rs->value(0, -1));
    }
    return nullptr;
}

auto Database::login(
        const QString & login,
        const QString & password
    ) const -> tt3::db::api::IAccount *
{
    if (auto account = tryLogin(login, password))
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
    tt3::util::Lock _(guard);
    ensureOpenAndWritable();    //  may throw

    //  Validate parameters
    if (!validator()->principal()->isValidEmailAddresses(emailAddresses))
    {
        throw tt3::db::api::InvalidPropertyValueException(
            tt3::db::api::ObjectTypes::User::instance(),
            "emailAddresses",
            emailAddresses.join(','));
    }
    if (!validator()->user()->isValidRealName(realName))
    {
        throw tt3::db::api::InvalidPropertyValueException(
            tt3::db::api::ObjectTypes::User::instance(),
            "realName",
            realName);
    }
    if (!validator()->user()->isValidInactivityTimeout(inactivityTimeout))
    {
        throw tt3::db::api::InvalidPropertyValueException(
            tt3::db::api::ObjectTypes::User::instance(),
            "inactivityTimeout",
            inactivityTimeout.value());
    }
    if (!validator()->user()->isValidUiLocale(uiLocale))
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

    /*  TODO
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
    */

    User * user = nullptr;
    try
    {
        beginTransaction(); //  may throw

        //  Do the work - create [objects] row..
        _ObjIds objIds = _createObject(tt3::db::api::ObjectTypes::User::instance()); //  may throw
        //  ...then [users] row...
        std::unique_ptr<Statement> stat
        {   createStatement(
                "INSERT INTO [users]"
                "       ([pk],[enabled],[emailaddresses],"
                "        [realname],[inactivitytimeout],[uilocale])"
                "       VALUES(?,?,?,?,?,?)") };
        stat->setParameter(0, std::get<0>(objIds));
        stat->setParameter(1, enabled);
        if (emailAddresses.isEmpty())
        {
            stat->setParameter(2, nullptr);
        }
        else
        {
            stat->setParameter(2, emailAddresses.join('\n'));
        }
        stat->setParameter(3, realName);
        if (inactivityTimeout.has_value())
        {
            stat->setParameter(4, inactivityTimeout.value());
        }
        else
        {
            stat->setParameter(4, nullptr);
        }
        if (uiLocale.has_value())
        {
            stat->setParameter(5, tt3::util::toString(uiLocale.value()));
        }
        else
        {
            stat->setParameter(5, nullptr);
        }
        stat->execute();    //  may throw

        //  Associate User with Workloads
        /*  TODO
        for (Workload * xmlWorkload : std::as_const(xmlPermittedWorkloads))
        {
            user->_permittedWorkloads.insert(xmlWorkload);
            xmlWorkload->_assignedUsers.insert(user);
            user->addReference();
            xmlWorkload->addReference();
        }
        */

        commitTransaction();//  may throw

        //  Create & register the User object...
        user = new User(this, std::get<0>(objIds));
        //  ...setting its cached properties to initial values
        user->_oid = std::get<1>(objIds);
        user->_enabled = enabled;
        user->_emailAddresses = emailAddresses;
        user->_realName = realName;
        user->_inactivityTimeout = inactivityTimeout;
        user->_uiLocale = uiLocale;
    }
    catch (...)
    {   //  OOPS! Cleanup, then re-throw
        rollbackTransaction();  //  may throw, but at this point who cares?
        throw;
    }
    //  ...schedule change notifications...
    _changeNotifier.post(
        new tt3::db::api::ObjectCreatedNotification(
            this, user->type(), user->_oid));
    //  TODO post change notification to the database
    /*  TODO
    for (Workload * xmlWorkload : std::as_const(xmlPermittedWorkloads))
    {
        _changeNotifier.post(
            new tt3::db::api::ObjectModifiedNotification(
                this, xmlWorkload->type(), xmlWorkload->_oid));
        //  TODO post change notification to the database
    }
    */
    //  ...and we're done
    return user;
}

auto Database::createActivityType(
        const QString & /*displayName*/,
        const QString & /*description*/
    ) -> tt3::db::api::IActivityType *
{
    throw tt3::util::NotImplementedError();
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
    throw tt3::util::NotImplementedError();
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
    throw tt3::util::NotImplementedError();
}

auto Database::createProject(
        const QString & displayName,
        const QString & description,
        const tt3::db::api::Beneficiaries & beneficiaries,
        bool completed
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
        const QString & displayName,
        const QString & description,
        const tt3::db::api::Workloads & workloads
    ) -> tt3::db::api::IBeneficiary *
{
    throw tt3::util::NotImplementedError();
}

//////////
//  IDatabase (locking)
auto Database::lock(
        tt3::db::api::IDatabaseLock::LockType lockType,
        unsigned long timeoutMs
    ) -> tt3::db::api::IDatabaseLock *
{
    throw tt3::util::NotImplementedError();
}

//////////
//  Operations (database specifics)
bool Database::isIdentifierStart(const QChar & c) const
{
    return (c >= 'a' && c <= 'z') ||
           (c >= 'A' && c <= 'Z') ||
           (c == '_');
}

bool Database::isIdentifierChar(const QChar & c) const
{
    return (c >= 'a' && c <= 'z') ||
           (c >= 'A' && c <= 'Z') ||
           (c >= '0' && c <= '9') ||
           (c == '_');
}

bool Database::isIdentifier(const QString & word) const
{
    if (word.length() == 0)
    {
        return false;
    }
    if (!isIdentifierStart(word[0]))
    {
        return false;
    }
    for (int i = 0; i < word.length(); i++)
    {
        if (!isIdentifierChar(word[i]))
        {
            return false;
        }
    }
    return true;
}

void Database::executeScript(const QString & sql)
{
    tt3::util::Lock _(guard);

    qsizetype scan = 0;     //  the next statement starts here
    QChar quote = '\x00';   //  The opening quote currently in effect
    while (scan < sql.length())
    {
        Q_ASSERT(quote.unicode() == 0); //  No quote at the beginning og a statement
        //  Fetch the next SQL statement
        QString statement;
        qsizetype prescan = scan;
        while (prescan < sql.length())
        {
            if (sql[prescan] == ';' && quote.isNull())
            {   //  End of statement
                prescan++;  //  skip ;
                break;
            }
            if ((sql[prescan] == '\'' ||
                 sql[prescan] == '"' ||
                 sql[prescan] == '`' ||
                 sql[prescan] == '[') &&
                quote.isNull())
            {   //  An opening quote
                quote = sql[prescan++];
                statement += quote;
                continue;
            }
            if ((sql[prescan] == '\'' ||
                 sql[prescan] == '"' ||
                 sql[prescan] == '`') &&
                quote == sql[prescan])
            {   //  A symmetric closing quote
                statement += sql[prescan++];
                quote = '\x00';
                continue;
            }
            if (sql[prescan] == ']' &&
                quote == '[')
            {   //  An asymmetric closing quote
                statement += ']';
                prescan++;
                quote = '\x00';
                continue;
            }
            if (sql[prescan] == '-' &&
                prescan + 1 < sql.length() && sql[prescan + 1] == '-' &&
                quote.isNull())
            {   //  -- ... \n comment
                prescan += 2;   //  Skip --
                while (prescan < sql.length() && sql[prescan] != '\n')
                {   //  Skip until '\n'
                    prescan++;
                }
                if (prescan < sql.length())
                {   //  Skip '\n'
                    prescan++;
                }
                statement += '\n';
                continue;
            }
            if (sql[prescan] == '/' &&
                prescan + 1 < sql.length() && sql[prescan + 1] == '*' &&
                quote.isNull())
            {   //  /* ... */ comment
                prescan += 2;   //  Skip /*
                bool commentClosed = false;
                while (prescan < sql.length())
                {
                    if (sql[prescan] == '*' &&
                        prescan + 1 < sql.length() && sql[prescan + 1] == '/')
                    {   //  Skip */
                        prescan += 2;
                        commentClosed = true;
                        break;
                    }
                    prescan++;  //  skip comment character
                }
                if (!commentClosed)
                {   //  OOPS! TODO throw SQL syntax error
                }
                continue;
            }
            if (sql[prescan].isNull())
            {   //  Be defensive!
                statement += ' ';
                prescan++;
                continue;
            }
            //  Just a character
            statement += sql[prescan++];
        }
        if (quote.unicode() != 0)
        {   //  OOPS! TODO throw SQL syntax error
        }
        //  We have a statement
        if (!statement.trimmed().isEmpty())
        {   //  Quotes are still not database-specific, etc...
            qDebug() << statement;
            //  ...but createStatement() will normalize everything
            std::unique_ptr<Statement> stat
                { createStatement(statement) }; //  may throw
            stat->execute();    //  may throw
        }
        //  Advance & keep going
        scan = prescan;
    }
}

auto Database::createStatement(const QString & sqlTemplate) const -> Statement *
{
    return new Statement(const_cast<Database*>(this), sqlTemplate);
}

//////////
//  Implementation helpers
Database::_ObjIds Database::_createObject(tt3::db::api::IObjectType * objectType)
{
    Q_ASSERT(guard.isLockedByCurrentThread());

    std::unique_ptr<Statement> stat1
    {   createStatement(
            "SELECT *"
            "  FROM [objects]"
            " WHERE [oid] = ?") };
    for (int n = 0; n < 100; n++)
    {
        auto oid = tt3::db::api::Oid::createRandom();
        //  Does the OID already exist ?
        stat1->setParameter(0, oid);
        std::unique_ptr<ResultSet> rs
            { stat1->executeQuery() };  //  may throw
        if (rs->size() > 0)
        {   //  OOPS! Already used, however unlikely
            continue;
        }
        //  Insert [objects] row
        std::unique_ptr<Statement> stat2
        {   createStatement(
                "INSERT INTO [objects]"
                "       ([oid],[type])"
                " VALUES(?,?)") };
        stat2->setParameter(0, oid);
        stat2->setParameter(1, objectType->mnemonic().toString());
        qint64 pk = stat2->execute();   //  may throw
        //  All done
        return std::make_tuple(pk, oid);
    }
    //  This is insane! Give up!
    //  TODO throw
    return std::make_tuple(-1, tt3::db::api::Oid::Invalid);
}

//  End of tt3-db-sql/Database.cpp
