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
        virtual qint64  size() const override { return _rows.size(); }
        virtual bool    next() override;
        virtual bool    isNull(int columnIndex) const override;
        virtual bool    isNull(const QString & columnName) const override;
        virtual bool    boolValue(int columnIndex, bool defaultValue = false) const override;
        virtual bool    boolValue(const QString & columnName, bool defaultValue = false) const override;
        virtual qint64  intValue(int columnIndex, qint64 defaultValue = 0) const override;
        virtual qint64  intValue(const QString & columnName, qint64 defaultValue = 0) const override;
        virtual QString stringValue(int columnIndex, const QString & defaultValue = "") const override;
        virtual QString stringValue(const QString & columnName, const QString & defaultValue = "") const override;

        virtual auto    timeSpanValue(int columnIndex, const tt3::util::TimeSpan & defaultValue = tt3::util::TimeSpan::Invalid) const -> tt3::util::TimeSpan override;
        virtual auto    timeSpanValue(const QString & columnName, const tt3::util::TimeSpan & defaultValue = tt3::util::TimeSpan::Invalid) const -> tt3::util::TimeSpan override;
        virtual Oid     oidValue(int columnIndex, const Oid & defaultValue = Oid::Invalid) const override;
        virtual Oid     oidValue(const QString & columnName, const Oid & defaultValue = Oid::Invalid) const override;

        //////////
        //  Implementation
    private:
        QList<QString>  _columns;   //  all-UPPERCASE column names
        mutable QMap<QString, int>  _columnIndices; //  COLUMN NAME UPPERCASED -> column index

        using _Cell = std::tuple<QString, bool>;    //  str-value, is-null
        using _Row = QList<_Cell>;
        using _Rows = QList<_Row>;
        _Rows           _rows;

        qint64          _currentRow = -1;   //  -1 == before 1st, _rowCount == after last

        //  Helpers
        int             _columnIndex(const QString & columnName) const;
    };
}

//  End of tt3-db-sqlite3/ResultSet.hpp
