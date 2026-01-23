//
//  tt3-db-sql/Statement.hpp - SQL statement
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
    /// \class SqlStatement tt3-db-sql/API.hpp
    /// \brief The generic SQL statement.
    class TT3_DB_SQL_PUBLIC Statement
    {
        TT3_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(Statement)

        //////////
        //  Construction/destruction - from friends only
    public: //  TODO really ?
        Statement(
                Database * database,
                const QString & sqlTemplate
            );
        virtual ~Statement();

        //////////
        //  Operations
    public:
        /// \brief
        ///     Returns the Database to which this Statement belongs.
        /// \return
        ///     The Database to which this Statement belongs.
        Database *      database() const;

        /// \brief
        ///     Returns the SQL template from which this Statement
        ///     was created.
        /// \details
        ///     This is the SQL statement with ? placeholders
        ///     representing 0-based positional parameters.
        /// \return
        ///     The SQL template from which this Statement
        ///     was created.
        QString         sqlTemplate() const;   //  with ? placeholders)

        //  TODO document
        void            resetParameters();
        void            resetParameter(int index);
        void            setParameter(int index, nullptr_t);
        void            setParameter(int index, bool value);
        void            setParameter(int index, const QString & value);
        void            setParameter(int index, const QDateTime & value);
        void            setParameter(int index, const tt3::util::TimeSpan & value);
        void            setParameter(int index, const QUuid & value);

        void            execute();
        ResultSet *     executeQuery();

        //////////
        //  Implementation
    private:
        Database *const _database;
        QString         _sqlTemplate;   //  with ? placeholders

        //  The SQL template broken into N textual pieces
        //  and N-1 textually represented parameters.
        //  All quoting and parameter formatting is database
        //  type - specific
        QStringList     _literalFragments;  //  [N]
        QStringList     _parameters;        //  [N-1], "" == parameter not specified

        //  The SQL statement, assembled as a NUL-terminated UTF-8
        QByteArray      _sqlAsUtf8; //  re-assembled when parameter(s) change

        //  Result sets - all live instances for this statement
        QList<ResultSet*>   _liveResultSets;
    };
}

//  End of tt3-db-sqlite3/Statement.hpp
