//
//  tt3-db-api/DatabaseType.hpp - "database type" ADT
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
    /// \brief
    ///     The database open mode
    enum class OpenMode
    {
        ReadOnly,   ///< Open as read-only, even when writable.
        ReadWrite,  ///< Open as writable, fail if read-only.
        Default     ///< Open as read/write is possible, as read-only if not.
    };

    /// \class IDatabaseType tt3-db-api/API.hpp
    /// \brief
    ///     Represents a specific storage technology
    ///     used to keep data persistent.
    /// \details
    ///     Concrete database types will normally be singletons
    ///     registered by plugins.
    class TT3_DB_API_PUBLIC IDatabaseType
    {
        //////////
        //  This is an interface
    protected:
        IDatabaseType() = default;
        virtual ~IDatabaseType() = default;

        //////////
        //  Operation (general)
    public:
        //  The mnemonic identifier of this database type
        virtual auto    mnemonic(
                            ) const
                            -> tt3::util::Mnemonic = 0;

        //  The user-readable display name of this database type
        virtual auto    displayName(
                            ) const
                            -> QString = 0;

        //  The small (16x16) icon representing this database type.
        virtual auto    smallIcon(
                            ) const
                            -> QIcon = 0;

        //  The large (32x32) icon representing this database type.
        virtual auto    largeIcon(
                            ) const
                            -> QIcon = 0;

        //  Checks whether this database type is operationsl (i.e.
        //  can be used) - e.g. a;; required drivers are present, etc.
        virtual bool    isOperational() const = 0;

        //  The short (1 line) status report for this database type.
        virtual auto    shortStatusReport(
                            ) const
                            -> QString = 0;

        //  The longer (mulyi-line) status report for this database
        //  type with newline '\n' character separating the lines.
        virtual auto    fullStatusReport(
                            ) const
                            -> QString = 0;

        //  The validator for databases of this type
        virtual auto    validator(
                            ) const
                            -> IValidator * = 0;

        //////////
        //  Operations (address handling)
    public:
        //  If this dataase type has a concept of a "default" database,
        //  returns its address; otherwise returns nullptr.
        virtual auto    defaultDatabaseAddress(
                            ) const
                            -> IDatabaseAddress * = 0;

        //  Prompts the user to interactively specify an address for a new
        //  database of this type, using the specified widget as a parent
        //  for all modal dialogs that may be involved in address choosing.
        //  Returns the database address chosen by the user or nullptr if
        //  the user has chosen to cancel the dialog.
        virtual auto    enterNewDatabaseAddress(
                                QWidget * parent
                            ) -> IDatabaseAddress * = 0;

        //  Prompts the user to interactively specify an address of an existing
        //  database of this type, using the specified widget as a parent
        //  for all modal dialogs that may be involved in address choosing.
        //  Returns the database address chosen by the user or nullptr if
        //  the user has chosen to cancel the dialog.
        virtual auto    enterExistingDatabaseAddress(
                                QWidget * parent
                            ) -> IDatabaseAddress * = 0;

        /// Parses an external form of a database address of this type.
        ///
        /// \param externalForm
        ///     The external form of a database address of this type.
        /// \return
        ///     The parsed database address.
        /// \exception DatabaseException
        ///     If the address parsing fails.
        virtual auto    parseDatabaseAddress(
                                const QString & externalForm
                            ) -> IDatabaseAddress * = 0;

        //////////
        //  Operations (databases)
    public:
        /// Creates a new, initially empty, database at the specified address.
        ///
        /// \param address
        ///     The address to create a new database at.
        /// \return
        ///     The newly created database (initially empty).
        /// \exception DatabaseException
        ///     If an error occurs.
        virtual auto    createDatabase(
                                IDatabaseAddress * address
                            ) -> IDatabase * = 0;

        /// Opens an existing database at the specified address.
        ///
        /// \param address
        ///     The address to open an existing database at.
        /// \param openMode
        ///     The database open mode.
        /// \return
        ///     The newly opened database.
        /// \exception DatabaseException
        ///     If an error occurs.
        virtual auto    openDatabase(
                                IDatabaseAddress * address,
                                OpenMode openMode
                            ) -> IDatabase * = 0;

        /// Destroys an existing database at the specified address.
        /// The database must not currently be in use.
        ///
        /// \param address
        ///     The address to destroy an existing database at.
        /// \exception DatabaseException
        ///     If an error occurs.
        virtual void    destroyDatabase(
                                IDatabaseAddress * address
                            ) = 0;
    };

    //////////
    //  The manager of known database types
    class TT3_DB_API_PUBLIC DatabaseTypeManager final
    {
        UTILITY_CLASS(DatabaseTypeManager)

        //////////
        //  Operations
    public:
        //  Registers the specified database type; returns
        //  true on success, false on failure.
        static bool     registerDatabaseType(
                                IDatabaseType * databaseType
                            );

        //  Finds a registered database type by its mnemonic;
        //  returns nullptr if not found.
        static auto     findDatabaseType(
                                const tt3::util::Mnemonic & mnemonic
                            )
                            -> IDatabaseType *;

        //  Returns the set of all registered database types.
        static auto     allDatabaseTypes()
                            -> DatabaseTypes;

        //////////
        //  Implementation
    private:
        struct _Impl;

        //  Helpers
        static _Impl *  _impl();
    };
}

//  End of tt3-db-api/DatabaseType.hpp
