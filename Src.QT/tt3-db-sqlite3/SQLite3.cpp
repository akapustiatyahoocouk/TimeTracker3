//
//  tt3-db-sqlite3/SQLite3.cpp - tt3::db::sqlite3::SQLite3 class implementation
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

struct SQLite3::_Impl
{
    _Impl()
        :   libSqlite3("sqlite3")
    {
        if (!libSqlite3.load())
        {   //  OOPS!
            errorMessage = "Failed to load sqlite3 library:" + libSqlite3.errorString();
            qCritical() << errorMessage;
            loaded = false;
            return; //  Don't resolve symols
        }

#define LIBSQLITE3_RESOLVE(sym)                                         \
        Q_ASSERT(sizeof(QFunctionPointer) == sizeof(uintptr_t));        \
        Q_ASSERT(sizeof(decltype(sym)) == sizeof(uintptr_t));           \
        sym = reinterpret_cast<decltype(sym)>(                          \
                reinterpret_cast<uintptr_t>(libSqlite3.resolve(#sym))); \
        if (sym == nullptr)                                             \
        {                                                               \
            errorMessage = "Cannot resolve " #sym " in libsqlite3";     \
            loaded = false;                                             \
        }

        LIBSQLITE3_RESOLVE(sqlite3_open_v2)
        LIBSQLITE3_RESOLVE(sqlite3_close)
        LIBSQLITE3_RESOLVE(sqlite3_errstr)
        LIBSQLITE3_RESOLVE(sqlite3_db_readonly)
        LIBSQLITE3_RESOLVE(sqlite3_exec)
#undef LIBSQLITE3_RESOLVE
    }

    QLibrary    libSqlite3;
    QString     errorMessage;   //  "" = loaded successfully
    bool        loaded = true;
    //  Symbols = nullptr when not defined
    int         (*sqlite3_open_v2)(
                        const char * filename,   /* Database filename (UTF-8) */
                        ::sqlite3 ** ppDb,         /* OUT: SQLite db handle */
                        int flags,              /* Flags */
                        const char *zVfs        /* Name of VFS module to use */
                    ) = nullptr;
    int         (*sqlite3_close)(::sqlite3 * db);
    const char *(*sqlite3_errstr)(int err);
    int         (*sqlite3_db_readonly)(::sqlite3 * db, const char * zDbName);
    int         (*sqlite3_exec)(
                        ::sqlite3 * db,
                        const char * sql,
                        int (*callback)(void*,int,char**,char**),
                        void * cbData,
                        char ** errmsg
                    );
};

//////////
//  Operations
int SQLite3::open_v2(const char * filename, ::sqlite3 ** ppDb, int flags)
{
    _Impl * impl = _impl();
    if (impl->loaded)
    {
        return impl->sqlite3_open_v2(filename, ppDb, flags, nullptr);
    }
    throw tt3::db::api::CustomDatabaseException(impl->errorMessage);
}

int SQLite3::close(::sqlite3 * db)
{
    _Impl * impl = _impl();
    if (impl->loaded)
    {
        return impl->sqlite3_close(db);
    }
    throw tt3::db::api::CustomDatabaseException(impl->errorMessage);
}

const char * SQLite3::errstr(int err)
{
    _Impl * impl = _impl();
    if (impl->loaded)
    {
        return impl->sqlite3_errstr(err);
    }
    throw tt3::db::api::CustomDatabaseException(impl->errorMessage);
}

int SQLite3::db_readonly(::sqlite3 * db, const char * zDbName)
{
    _Impl * impl = _impl();
    if (impl->loaded)
    {
        return impl->sqlite3_db_readonly(db, zDbName);
    }
    throw tt3::db::api::CustomDatabaseException(impl->errorMessage);
}

int SQLite3::exec(::sqlite3 * db,
        const char * sql,
        int (*callback)(void*,int,char**,char**),
        void * cbData,
        char ** errmsg
    )
{
    _Impl * impl = _impl();
    if (impl->loaded)
    {
        return impl->sqlite3_exec(db, sql, callback, cbData, errmsg);
    }
    throw tt3::db::api::CustomDatabaseException(impl->errorMessage);
}

//////////
//  Implementation helpers
SQLite3::_Impl * SQLite3::_impl()
{
    static _Impl impl;
    return &impl;
}

//  End of tt3-db-sqlite3/SQLite3.cpp
