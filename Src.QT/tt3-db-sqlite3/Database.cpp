//
//  tt3-db-sqlite3/Database.cpp - tt3::db::sqlite3::Database class implementation
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
#include "tt3-db-sqlite3/API.hpp"
using namespace tt3::db::sqlite3;

//////////
//  Construction/destruction
Database::Database(
        DatabaseAddress * address,
        _OpenMode openMode
    ) : _address(address),
        _validator(tt3::db::api::DefaultValidator::instance()),
        _keywords
        {
            "ABORT",
            "ACTION",
            "ADD",
            "AFTER",
            "ALL",
            "ALTER",
            "ALWAYS",
            "ANALYZE",
            "AND",
            "AS",
            "ASC",
            "ATTACH",
            "AUTOINCREMENT",
            "BEFORE",
            "BEGIN",
            "BETWEEN",
            "BY",
            "CASCADE",
            "CASE",
            "CAST",
            "CHECK",
            "COLLATE",
            "COLUMN",
            "COMMIT",
            "CONFLICT",
            "CONSTRAINT",
            "CREATE",
            "CROSS",
            "CURRENT",
            "CURRENT_DATE",
            "CURRENT_TIME",
            "CURRENT_TIMESTAMP",
            "DATABASE",
            "DEFAULT",
            "DEFERRABLE",
            "DEFERRED",
            "DELETE",
            "DESC",
            "DETACH",
            "DISTINCT",
            "DO",
            "DROP",
            "EACH",
            "ELSE",
            "END",
            "ESCAPE",
            "EXCEPT",
            "EXCLUDE",
            "EXCLUSIVE",
            "EXISTS",
            "EXPLAIN",
            "FAIL",
            "FILTER",
            "FIRST",
            "FOLLOWING",
            "FOR",
            "FOREIGN",
            "FROM",
            "FULL",
            "GENERATED",
            "GLOB",
            "GROUP",
            "GROUPS",
            "HAVING",
            "IF",
            "IGNORE",
            "IMMEDIATE",
            "IN",
            "INDEX",
            "INDEXED",
            "INITIALLY",
            "INNER",
            "INSERT",
            "INSTEAD",
            "INTERSECT",
            "INTO",
            "IS",
            "ISNULL",
            "JOIN",
            "KEY",
            "LAST",
            "LEFT",
            "LIKE",
            "LIMIT",
            "MATCH",
            "MATERIALIZED",
            "NATURAL",
            "NO",
            "NOT",
            "NOTHING",
            "NOTNULL",
            "NULL",
            "NULLS",
            "OF",
            "OFFSET",
            "ON",
            "OR",
            "ORDER",
            "OTHERS",
            "OUTER",
            "OVER",
            "PARTITION",
            "PLAN",
            "PRAGMA",
            "PRECEDING",
            "PRIMARY",
            "QUERY",
            "RAISE",
            "RANGE",
            "RECURSIVE",
            "REFERENCES",
            "REGEXP",
            "REINDEX",
            "RELEASE",
            "RENAME",
            "REPLACE",
            "RESTRICT",
            "RETURNING",
            "RIGHT",
            "ROLLBACK",
            "ROW",
            "ROWS",
            "SAVEPOINT",
            "SELECT",
            "SET",
            "TABLE",
            "TEMP",
            "TEMPORARY",
            "THEN",
            "TIES",
            "TO",
            "TRANSACTION",
            "TRIGGER",
            "UNBOUNDED",
            "UNION",
            "UNIQUE",
            "UPDATE",
            "USING",
            "VACUUM",
            "VALUES",
            "VIEW",
            "VIRTUAL",
            "WHEN",
            "WHERE",
            "WINDOW",
            "WITH",
            "WITHOUT"
        }
{
    Q_ASSERT(_address != nullptr);
    if (openMode != _OpenMode::_Dead)
    {   //  ...because the DatabaseType holds that reference
        _address->addReference();
    }

    tt3::util::Lock _(_guard);

    int err;
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
            //  Do the opening
            err = SQLite3::open_v2(
                address->_path.toUtf8(),
                &_connection,
                SQLITE_OPEN_PRIVATECACHE | SQLITE_OPEN_FULLMUTEX |
                SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE);
            if (err != SQLITE_OK)
            {   //  OOPS!
                qCritical() << SQLite3::errstr(err);
                throw tt3::db::api::CustomDatabaseException(
                    address->_path + ": " + SQLite3::errstr(err));
            }
            Q_ASSERT(_connection != nullptr);
            //  TODO create schema
            break;
        case _OpenMode::_OpenReadOnly:
            //  File must exist
            if (!QFileInfo(_address->_path).isFile())
            {   //  OOPS!
                throw tt3::db::api::DoesNotExistException(
                    DatabaseType::instance()->displayName(),
                    "location",
                    _address->_path);
            }
            //  Do the opening
            err = SQLite3::open_v2(
                address->_path.toUtf8(),
                &_connection,
                SQLITE_OPEN_PRIVATECACHE | SQLITE_OPEN_FULLMUTEX |
                SQLITE_OPEN_READONLY);
            if (err != SQLITE_OK)
            {   //  OOPS!
                qCritical() << SQLite3::errstr(err);
                throw tt3::db::api::CustomDatabaseException(
                    address->_path + ": " + SQLite3::errstr(err));
            }
            Q_ASSERT(_connection != nullptr);
            //  TODO validate schema
            break;
        case _OpenMode::_OpenReadWrite:
            //  File must exist
            if (!QFileInfo(_address->_path).isFile())
            {   //  OOPS!
                throw tt3::db::api::DoesNotExistException(
                    DatabaseType::instance()->displayName(),
                    "location",
                    _address->_path);
            }
            //  Do the opening
            err = SQLite3::open_v2(
                address->_path.toUtf8(),
                &_connection,
                SQLITE_OPEN_PRIVATECACHE | SQLITE_OPEN_FULLMUTEX |
                SQLITE_OPEN_READWRITE);
            if (err != SQLITE_OK)
            {   //  OOPS!
                qCritical() << SQLite3::errstr(err);
                throw tt3::db::api::CustomDatabaseException(
                    address->_path + ": " + SQLite3::errstr(err));
            }
            Q_ASSERT(_connection != nullptr);
            //  TODO validate schema
            break;
        case _OpenMode::_Dead:
            //  We're creating a special "dead" database,
            //  where "dead" objects are parked until they
            //  lose their reference counts.
            Q_ASSERT(_connection == nullptr);
            break;
        default:
            Q_ASSERT(false);
    }
    //  TODO set the _isReadOnly flag
}

Database::~Database()
{
    //  TODO synchronize ?
    //  Close if still open
    if (_connection != nullptr)
    {
        int err = SQLite3::close(_connection); //  suppress errors
        if (err != SQLITE_OK)
        {   //  OOPS! Log, but can't throw
            qCritical() << SQLite3::errstr(err);
        }
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
    throw tt3::util::NotImplementedError();
}

bool Database::isReadOnly() const
{
    throw tt3::util::NotImplementedError();
}

void Database::close()
{
    throw tt3::util::NotImplementedError();
}

void Database::refresh()
{
    throw tt3::util::NotImplementedError();
}

//////////
//  tt3::db::sql::Database (database specifics)
bool Database::isKeyword(const QString & word) const
{
    return _keywords.contains(word.toUpper());
}

QString Database::quoteIdentifier(const QString & identifier) const
{
    if (isKeyword(identifier))
    {   //  Use standard SQL quotes
        return '"' + identifier + '"';
    }
    return identifier;
}

//  End of tt3-db-sqlite3/Database.cpp
