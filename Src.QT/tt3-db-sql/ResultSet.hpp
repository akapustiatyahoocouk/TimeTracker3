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
        //  Types
    public:
        using Oid = tt3::db::api::Oid;

        //////////
        //  Construction/destruction
    protected:
        /// \brief
        ///     Constructs the result set.
        ResultSet() = default;
    public:
        /// \brief
        ///     The class destructor.
        virtual ~ResultSet() = default;

        //////////
        //  Operations
    public:
        /// \brief
        ///     Returns the number of rows in this result set.
        /// \return
        ///     The number of rows in this result set.
        virtual qint64  size() const = 0;

        //  TODO document
        virtual bool    next() = 0;
        virtual bool    isNull(int columnIndex) const = 0;
        virtual bool    isNull(const QString & columnName) const = 0;
        virtual bool    boolValue(int columnIndex, bool defaultValue = false) const = 0;
        virtual bool    boolValue(const QString & columnName, bool defaultValue = false) const = 0;
        virtual qint64  intValue(int columnIndex, qint64 defaultValue = 0) const = 0;
        virtual qint64  intValue(const QString & columnName, qint64 defaultValue = 0) const = 0;
        virtual QString stringValue(int columnIndex, const QString & defaultValue = "") const = 0;
        virtual QString stringValue(const QString & columnName, const QString & defaultValue = "") const = 0;

        virtual auto    timeSpanValue(int columnIndex, const tt3::util::TimeSpan & defaultValue = tt3::util::TimeSpan::Invalid) const -> tt3::util::TimeSpan = 0;
        virtual auto    timeSpanValue(const QString & columnName, const tt3::util::TimeSpan & defaultValue = tt3::util::TimeSpan::Invalid) const -> tt3::util::TimeSpan = 0;
        virtual Oid     oidValue(int columnIndex, const Oid & defaultValue = Oid::Invalid) const = 0;
        virtual Oid     oidValue(const QString & columnName, const Oid & defaultValue = Oid::Invalid) const = 0;
    };
}

//  End of tt3-db-sqlite3/ResultSet.hpp
