//
//  tt3-db-sqlite3/ResultSet.hpp - "SQLite3 result set"
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
    /// \class ResultSet tt3-db-sqlite3/API.hpp
    /// \brief The result set from a SQLite3 query.
    class TT3_DB_SQLITE3_PUBLIC ResultSet final
        :   public virtual tt3::db::sql::ResultSet
    {
        TT3_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(ResultSet)

        friend class Database;

        //////////
        //  Construction/destruction(from DB type only)
    private:
        ResultSet() = default;
    public:
        /// \brief
        ///     The class destructor.
        virtual ~ResultSet() = default;

        //////////
        //  tt3::db::sql::ResultSet
    public:
        virtual qint64  size() const override { return _rowCount; }
        virtual bool    next() override;
        virtual bool    isNull(int columnIndex) const override;
        virtual bool    isNull(const QString & columnName) const override;

        //////////
        //  Implementation
    private:
        qint64          _rowCount = 0;
        QList<QString>  _columns;   //  all-UPPERCASE column names
        mutable QMap<QString, int>  _columnIndices; //  COLUMN NAME UPPERCASED -> column index

        qint64          _currentRow = -1;   //  -1 == before 1st, _rowCount == after last

        using _Row = QList<QString>;    //  Values as SQL constants, e.g. 1, 'abc' or NULL
        using _Rows = QList<_Row>;

        _Rows           _rows;

        //  Helpers
        bool            _ensureCurrentRow() const;
        int             _columnIndex(const QString & columnName) const;
    };
}

//  End of tt3-db-sqlite3/ResultSet.hpp
