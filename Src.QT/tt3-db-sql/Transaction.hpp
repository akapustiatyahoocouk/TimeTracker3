//
//  tt3-db-sql/Transaction.hpp - SQL transaction control
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
    /// \class Transaction tt3-db-sql/API.hpp
    /// \brief The SQL transaction in progress!
    class TT3_DB_SQL_PUBLIC Transaction final
    {
        TT3_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(Transaction)

        //////////
        //  Construction/destruction
    public:
        /// \brief
        ///     Begins a Transaction in the specified database.
        /// \param database
        ///     The database to begin transaction in.
        /// \exception DatabaseException
        ///     If another transaction is already underway.
        explicit Transaction(Database * database);

        /// \brief
        ///     If this Transaction has been neither committed nor
        ///     rolled back, performs a rollback. Intended to be
        ///     used diring exception propagation deom database
        ///     modification code.
        ~Transaction();

        //////////
        //  Operations
    public:
        /// \brief
        ///     Commits the transaction; has no effect
        ///     if already committed.
        /// \exception DatabaseException
        ///     If an error occurs.
        void            commit();

        /// \brief
        ///     Rolls back the transaction; has no effect
        ///     if already rolled back.
        /// \exception DatabaseException
        ///     If an error occurs.
        void            rollback();

        //////////
        //  Implementation
    private:
        Database *  _database;
        bool        _committed = false;
        bool        _rollbacked = false;
    };
}

//  End of tt3-db-sqlite3/Transaction.hpp
