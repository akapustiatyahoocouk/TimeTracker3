//
//  tt3-db-sql/ResultSet.hpp - SQL Result Set
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

namespace tt3::db::sql
{
    /// \class ResultSet tt3-db-sql/API.hpp
    /// \brief The result set of a SELECT query.
    class TT3_DB_SQL_PUBLIC ResultSet
    {
        TT3_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(ResultSet)

        //////////
        //  Construction/destruction
    protected:
        /// \brief
        ///     Constructs the result set.
        ResultSet();
    public:
        /// \brief
        ///     The class destructor.
        virtual ~ResultSet();

        //////////
        //  Operations
    public:
        /// \brief
        ///     Returns the number of rows in this result set.
        /// \return
        ///     The number of rows in this result set.
        virtual qint64  size() const = 0;
    };
}

//  End of tt3-db-sqlite3/ResultSet.hpp
