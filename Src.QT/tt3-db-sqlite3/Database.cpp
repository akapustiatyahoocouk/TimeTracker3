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

    tt3::util::Lock _(guard);

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
                throw tt3::db::api::CustomDatabaseException(
                    address->_path + ": " + SQLite3::errstr(err));
            }
            Q_ASSERT(_connection != nullptr);
            _isReadOnly = false;
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
                throw tt3::db::api::CustomDatabaseException(
                    address->_path + ": " + SQLite3::errstr(err));
            }
            Q_ASSERT(_connection != nullptr);
            _isReadOnly = true;
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
                throw tt3::db::api::CustomDatabaseException(
                    address->_path + ": " + SQLite3::errstr(err));
            }
            Q_ASSERT(_connection != nullptr);
            _isReadOnly = (SQLite3::db_readonly(_connection, "main") != 0);
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

    //  Run the init/validate script
    if (_connection != nullptr)
    {   //  ...except on DEAD databases
        QString script;
        if (openMode == _OpenMode::_Create)
        {   //  Create schema script
            QFile file(":/tt3-db-sqlite3/Resources/CreateDatabase.sql");
            if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
            {   //  OOPS!
                QString errorMessage = "Could not open resource file " + file.fileName();
                throw tt3::db::api::CustomDatabaseException(errorMessage);
            }
            QTextStream in(&file);
            script = in.readAll();
            file.close();
        }
        else if (openMode != _OpenMode::_Dead)
        {   //  TODO else validate schema script
        }
        try
        {
            executeScript(script);    //  may throw
        }
        catch (...)
        {   //  OOPS! Cleanup & re-throw
            SQLite3::close(_connection); //  suppress errors
            throw;
        }
    }
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
    tt3::util::Lock _(guard);

    return _connection != nullptr;
}

bool Database::isReadOnly() const
{
    return _isReadOnly;
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

void Database::beginTransaction()
{
    tt3::util::Lock _(guard);
    ensureOpen();

    if (_transactionInProgress)
    {   //  OOPS! TODO throw
    }
    char * errmsg = nullptr;
    int err = SQLite3::exec(
        _connection,
        "BEGIN IMMEDIATE TRANSACTION",
        nullptr,
        nullptr,
        &errmsg);
    if (err != SQLITE_OK)
    {   //  OOPS!
        QString errorMessage =
            (errmsg != nullptr) ?
                errmsg :
                SQLite3::errstr(err);
        if (errmsg != nullptr)
        {
            SQLite3::free(errmsg);
        }
        throw tt3::db::api::CustomDatabaseException(errorMessage);
    }
    _transactionInProgress = true;
}

void Database::commitTransaction()
{
    tt3::util::Lock _(guard);
    ensureOpen();

    if (!_transactionInProgress)
    {   //  OOPS! TODO throw
    }
    char * errmsg = nullptr;
    int err = SQLite3::exec(
        _connection,
        "COMMIT TRANSACTION",
        nullptr,
        nullptr,
        &errmsg);
    if (err != SQLITE_OK)
    {   //  OOPS!
        QString errorMessage =
            (errmsg != nullptr) ?
                errmsg :
                SQLite3::errstr(err);
        if (errmsg != nullptr)
        {
            SQLite3::free(errmsg);
        }
        throw tt3::db::api::CustomDatabaseException(errorMessage);
    }
    _transactionInProgress = false;
}

void Database::rollbackTransaction()
{
    tt3::util::Lock _(guard);
    ensureOpen();

    if (!_transactionInProgress)
    {   //  OOPS! TODO throw
    }
    char * errmsg = nullptr;
    int err = SQLite3::exec(
        _connection,
        "ROLLBACK TRANSACTION",
        nullptr,
        nullptr,
        &errmsg);
    if (err != SQLITE_OK)
    {   //  OOPS!
        QString errorMessage =
            (errmsg != nullptr) ?
                errmsg :
                SQLite3::errstr(err);
        if (errmsg != nullptr)
        {
            SQLite3::free(errmsg);
        }
        throw tt3::db::api::CustomDatabaseException(errorMessage);
    }
    _transactionInProgress = false;
}

bool Database::isTransactionInProgress() const
{
    tt3::util::Lock _(guard);
    ensureOpen();

    return _transactionInProgress;
}

qint64 Database::executeInsert(const QString & sql)
{
    tt3::util::Lock _(guard);
    ensureOpenAndWritable();

    char * errmsg = nullptr;
    int err = SQLite3::exec(
        _connection,
        sql.toUtf8(),
        nullptr,
        nullptr,
        &errmsg);
    if (err != SQLITE_OK)
    {   //  OOPS!
        QString errorMessage =
            (errmsg != nullptr) ?
                errmsg :
                SQLite3::errstr(err);
        if (errmsg != nullptr)
        {
            SQLite3::free(errmsg);
        }
        throw tt3::db::api::CustomDatabaseException(errorMessage);
    }
    //  Need affected rows count
    return SQLite3::last_insert_rowid(_connection);
}

qint64 Database::executeUpdate(const QString & sql)
{
    tt3::util::Lock _(guard);
    ensureOpenAndWritable();

    char * errmsg = nullptr;
    int err = SQLite3::exec(
        _connection,
        sql.toUtf8(),
        nullptr,
        nullptr,
        &errmsg);
    if (err != SQLITE_OK)
    {   //  OOPS!
        QString errorMessage =
            (errmsg != nullptr) ?
                errmsg :
                SQLite3::errstr(err);
        if (errmsg != nullptr)
        {
            SQLite3::free(errmsg);
        }
        throw tt3::db::api::CustomDatabaseException(errorMessage);
    }
    //  Need affected rows count
    return SQLite3::changes64(_connection);
}

qint64 Database::executeDelete(const QString & sql)
{
    tt3::util::Lock _(guard);
    ensureOpenAndWritable();

    char * errmsg = nullptr;
    int err = SQLite3::exec(
        _connection,
        sql.toUtf8(),
        nullptr,
        nullptr,
        &errmsg);
    if (err != SQLITE_OK)
    {   //  OOPS!
        QString errorMessage =
            (errmsg != nullptr) ?
                errmsg :
                SQLite3::errstr(err);
        if (errmsg != nullptr)
        {
            SQLite3::free(errmsg);
        }
        throw tt3::db::api::CustomDatabaseException(errorMessage);
    }
    //  Need affected rows count
    return SQLite3::changes64(_connection);
}

auto Database::executeSelect(const QString & sql) -> tt3::db::sql::ResultSet *
{
    tt3::util::Lock _(guard);
    ensureOpenAndWritable();

    std::unique_ptr<ResultSet> rs { new ResultSet() };
    char * errmsg = nullptr;
    int err = SQLite3::exec(
        _connection,
        sql.toUtf8(),
        _selectCallback,
        rs.get(),
        &errmsg);
    if (err != SQLITE_OK)
    {   //  OOPS!
        QString errorMessage =
            (errmsg != nullptr) ?
                errmsg :
                SQLite3::errstr(err);
        if (errmsg != nullptr)
        {
            SQLite3::free(errmsg);
        }
        throw tt3::db::api::CustomDatabaseException(errorMessage);
    }
    return rs.release();
}

void Database::execute(const QString & sql)
{
    tt3::util::Lock _(guard);
    ensureOpen();

    char * errmsg = nullptr;
    int err = SQLite3::exec(
        _connection,
        sql.toUtf8(),
        nullptr,
        nullptr,
        &errmsg);
    if (err != SQLITE_OK)
    {   //  OOPS!
        QString errorMessage =
            (errmsg != nullptr) ?
                errmsg :
                SQLite3::errstr(err);
        if (errmsg != nullptr)
        {
            SQLite3::free(errmsg);
        }
        throw tt3::db::api::CustomDatabaseException(errorMessage);
    }
}

void Database::ensureOpen() const
{
    Q_ASSERT(guard.isLockedByCurrentThread());

    if (_connection == nullptr)
    {   //  OOPS!
        throw tt3::db::api::DatabaseClosedException();
    }
}

void Database::ensureOpenAndWritable() const
{
    Q_ASSERT(guard.isLockedByCurrentThread());

    if (_connection == nullptr)
    {   //  OOPS!
        throw tt3::db::api::DatabaseClosedException();
    }
    if (_isReadOnly)
    {   //  OOPS!
        throw tt3::db::api::AccessDeniedException();
    }
}

//////////
//  Implementation helpers
int Database::_selectCallback(void * cbData, int argc, char ** argv,char ** colNames)
{
    auto resultSet = static_cast<ResultSet*>(cbData);
    if (resultSet->_rowCount == 0)
    {   //  Set up column names ONCE
        for (int i = 0; i < argc; i++)
        {
            resultSet->_columns.append(QString(colNames[i]).toUpper());
        }
    }
#ifndef Q_NODEBUG
    else
    {   //  Make sure column names match
        Q_ASSERT(resultSet->_columns.size() == argc);
        for (int i = 0; i < argc; i++)
        {
            Q_ASSERT(resultSet->_columns[i] == QString(colNames[i]).toUpper());
        }
    }
#endif
    //  Add new row
    ResultSet::_Row row;
    for (int i = 0; i < argc; i++)
    {
        row.append(argv[i]);
    }
    resultSet->_rows.append(row);
    return 0;   //  keep going
}

//  End of tt3-db-sqlite3/Database.cpp
