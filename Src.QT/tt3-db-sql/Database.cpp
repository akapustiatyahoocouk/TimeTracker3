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
        const tt3::db::api::Oid & oid
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
    throw tt3::util::NotImplementedError();
}

auto Database::activityTypes(
    ) const -> tt3::db::api::ActivityTypes
{
    throw tt3::util::NotImplementedError();
}

auto Database::findActivityType(
        const QString & displayName
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
        const QString & displayName
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
    throw tt3::util::NotImplementedError();
}

auto Database::login(
        const QString & login,
        const QString & password
    ) const -> tt3::db::api::IAccount *
{
    throw tt3::util::NotImplementedError();
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
    throw tt3::util::NotImplementedError();
}

auto Database::createActivityType(
        const QString & displayName,
        const QString & description
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
        const QString & displayName,
        const QString & description,
        const tt3::db::api::Beneficiaries & beneficiaries
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
                statement += quote;
                quote = '\x00';
                continue;
            }
            if (sql[prescan] == ']' &&
                quote == '[')
            {   //  An asymmetric closing quote
                statement += ']';
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
    }
}

//  End of tt3-db-sql/Database.cpp
