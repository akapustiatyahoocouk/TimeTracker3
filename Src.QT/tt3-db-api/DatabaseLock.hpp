//
//  tt3-db-api/DatabaseLock.hpp - "database lock" ADT
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

namespace tt3::db::api
{
    /// \class IDatabaseLock tt3-db-api/API.hpp
    /// \brief Represents a whole-database lock.
    /// \details
    ///     While the database is "locked" by one process,
    ///     another process may place a non-onflicting lock
    ///     on it. While the database is "locked" (and regardless
    ///     of the lock type) thr datbase object recycling is
    ///     temporarily suspended, even if the object becomes
    ///     Old. This means that all pointers to objects that
    ///     reside in the database remain valid until the lock
    ///     is released.
    ///     Database locks are useful for backup, restore and
    ///     reporting purposes.
    ///
    /// *   If a Database has no Locks placed upon it, then
    ///     all operations by any thread of any process accessing
    ///     the database are permitted (subject to the Database
    ///     itself being read-only).
    /// *   If a Database has only Read locks placed on it,
    ///     then these can belong to any thread of any process.
    ///     Any thread of any process can read from the database;
    ///     threads that attempt to write to the database will stall
    ///     will stall until all Write locks are released.
    /// *   If a atabase has at least one Write lock placed on it, then
    ///     all its locks must belong to the same thread of the same Process.
    ///     Only this thread will be allowed to read and write from/to
    ///     the Database; all other threads and processes will stall
    ///     until all Write locks are released.
    ///
    /// If a database is closed (or deleted) while it is
    /// "locked", all locks become "orphan locks" which are
    /// not associated with any database and whose deletion
    /// will have no side effects whatsoever.
    class TT3_DB_API_PUBLIC IDatabaseLock
    {
        //////////
        //  Types
    public:
        /// \brief
        ///     The supported lock types.
        enum class LockType
        {
            ReadOnly,   ///< The lock owner can read from database, but not write to it.
            ReadWrite   ///< The lock owner can read and write from/to the database.
        };

        //////////
        //  This is an interface
    protected:
        /// \brief
        ///     The empty [interface] constructor.
        IDatabaseLock() = default;

    public:
        /// \brief
        ///     The empty [interface] constructor.
        /// \details
        ///     Releases this lock.
        virtual ~IDatabaseLock() = default;

        //////////
        //  Operations
    public:
        /// \brief
        ///     Returns the type of this lock.
        /// \return
        ///     The type of this lock.
        virtual LockType    lockType() const = 0;
    };
}

//  End of tt3-db-api/DatabaseLock.hpp
