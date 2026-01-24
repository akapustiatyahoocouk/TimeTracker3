//
//  tt3-db-sqlite3/SQLite3.hpp - SQLite3 API that binds at runtime
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

namespace tt3::db::sqlite3
{
    /// \class SQLite3 tt3-db-sqlite3/API.hpp
    /// \brief The SQLite3 AI that binds at runtime, not linktime.
    class TT3_DB_SQLITE3_PUBLIC SQLite3 final
    {
        TT3_UTILITY_CLASS(SQLite3)

        //////////
        //  Operations
    public:
        static int          open_v2(const char * filename, ::sqlite3 ** ppDb, int flags);
        static int          close(::sqlite3 * db);
        static const char * errstr(int err);
        static int          db_readonly(::sqlite3 * db, const char * zDbName);
        static int          exec(::sqlite3 * db,
                                    const char * sql,
                                    int (*callback)(void*,int,char**,char**),
                                    void * cbData,
                                    char ** errmsg
                                );


        //////////
        //  Implementtion
    private:
        struct _Impl;

        //  Helpers
        static _Impl *      _impl();
    };
}

//  End of tt3-db-sqlite3/SQLite3.hpp
