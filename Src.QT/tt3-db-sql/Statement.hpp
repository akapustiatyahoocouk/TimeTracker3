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
    /// \class Statement tt3-db-sql/API.hpp
    /// \brief The generic SQL statement; NOT thread-safe!
    class TT3_DB_SQL_PUBLIC Statement
    {
        TT3_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(Statement)

        friend class Database;

        //////////
        //  Types
    public:
        /// \brief The SQL Statement category
        enum class Category
        {
            Select, ///< SELECT...
            Insert, ///< INSERT...
            Update, ///< UPDATE...
            Delete, ///< DELETE...
            Other   ///< SET..., etc.
        };

        //////////
        //  Construction/destruction
    protected:
        /// \brief
        ///     Constructs the statement.
        /// \param database
        ///     The database where the statement will be executed.
        /// \param sqlTemplate
        ///     The SQL template from which this Statement
        ///     is created, with ? as 0-based positional
        ///     parameter placeholders.
        /// \exception DatabaseException
        ///     If an error occurs (e.g. SQL syntax error).
        Statement(
                Database * database,
                const QString & sqlTemplate
            );
    public:
        /// \brief
        ///     The class destructor.
        virtual ~Statement();

        //////////
        //  Operations
    public:
        /// \brief
        ///     Returns the Database to which this Statement belongs.
        /// \return
        ///     The Database to which this Statement belongs.
        Database *      database() const { return _database; }

        /// \brief
        ///     Returns the SQL template from which this Statement
        ///     was created.
        /// \details
        ///     This is the SQL statement with ? placeholders
        ///     representing 0-based positional parameters.
        /// \return
        ///     The SQL template from which this Statement
        ///     was created.
        QString         sqlTemplate() const { return _sqlTemplate; }

        /// \brief
        ///     Returns thie Statement's category.
        /// \return
        ///     Thie Statement's category.
        Category        category() const { return _category; }

        //  TODO document
        virtual void    resetParameters();
        virtual void    resetParameter(int index);
        virtual void    setParameter(int index, nullptr_t);
        virtual void    setParameter(int index, bool value);
        virtual void    setParameter(int index, qint64 value);
        virtual void    setParameter(int index, const QString & value);
        virtual void    setParameter(int index, const QDateTime & value);
        virtual void    setParameter(int index, const tt3::util::TimeSpan & value);
        virtual void    setParameter(int index, const tt3::db::api::Oid & value);

        qint64          execute();  //  lastinsertID for INSERT, affected rows count for UPDATE/DELETE, etc.
        ResultSet *     executeQuery(); //  never nullptr, but may be empty

        //////////
        //  Implementation
    private:
        Database *const _database;
        const QString   _sqlTemplate;   //  with ? placeholders
        const Category  _category;

        //  The SQL template broken into N textual pieces
        //  and N-1 textually represented parameters in between.
        //  All quoting and parameter formatting is database
        //  type - specific
        QStringList     _literalFragments;  //  [N]
        QStringList     _parameters;        //  [N-1], "" == parameter not specified

        //  The SQL statement, assembled as a NUL-terminated UTF-8
        QString         _sql;   //  re-assembled when parameter(s) change

        //  Result sets - all live instances for this statement
        QList<ResultSet*>   _liveResultSets;

        //  Helpers
        static Category _categorize(const QString & sqlTemplate);
        static void     _consumeIdentifier(
                                QString & accumulator,
                                QString & identifier,
                                Database * database
                            );
        void            _prepareSql();
    };
}

//  End of tt3-db-sqlite3/Statement.hpp
