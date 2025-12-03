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
    ///     registered by Components during their initialization.
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
        /// \brief
        ///     Returns the mnemonic identifier of this database type.
        /// \return
        ///     The mnemonic identifier of this database type.
        virtual auto    mnemonic(
                            ) const -> tt3::util::Mnemonic = 0;

        /// \brief
        ///     Returns the user-readable display name of this database type.
        /// \return
        ///     The user-readable display name of this database type.
        virtual QString displayName() const = 0;

        /// \brief
        ///     Returns the small (16x16) icon representing this database type.
        /// \return
        ///     The small (16x16) icon representing this database type.
        virtual QIcon   smallIcon() const = 0;

        /// \brief
        ///     Returns the large (32x32) icon representing this database type.
        /// \return
        ///     The large (32x32) icon representing this database type.
        virtual QIcon   largeIcon() const = 0;

        /// \brief
        ///     Checks whether this database type is operational
        ///     and can be used - all required drivers are present, etc.
        /// \return
        ///     True if this database type is operationsl and
        ///     can be used, else false.
        virtual bool    isOperational() const = 0;

        /// \brief
        ///     Returns the short (1 line) status report for this database type.
        /// \return
        ///     The short (1 line) status report for this database type.
        virtual QString shortStatusReport() const = 0;

        /// \brief
        ///     Returns the longer (mulyi-line) status report
        ///     for this database type.
        /// \return
        ///     The longer (mulyi-line) status report for this
        ///     database type with newline '\\n' character
        ///     separating the lines.
        virtual QString fullStatusReport() const = 0;

        /// \brief
        ///     Returns the validator for databases of this type
        /// \return
        ///     The validator for databases of this type
        virtual auto    validator(
                            ) const -> IValidator * = 0;

        //////////
        //  Operations (address handling)
    public:
        /// \brief
        ///     If this dataase type has a concept of a "default" database,
        ///     returns its address.
        /// \return
        ///     The address of the "default" database of this type or
        ///     nullptr if thia database type has no concept of a
        ///     "default" database.
        virtual auto    defaultDatabaseAddress(
                            ) const -> IDatabaseAddress * = 0;

        /// \brief
        ///     Prompts the user to interactively specify an address
        ///     for a new database of this type.
        /// \param parent
        ///     The parent widget for all modal dialogs that may be
        ///     involved in address choosing.
        /// \return
        ///     The database address chosen by the user or nullptr if
        ///     the user has chosen to cancel the dialog.
        virtual auto    enterNewDatabaseAddress(
                                QWidget * parent
                            ) -> IDatabaseAddress * = 0;

        /// \brief
        ///     Prompts the user to interactively specify an address
        ///     of an existing database of this type.
        /// \param parent
        ///     The parent widget for all modal dialogs that may be
        ///     involved in address choosing.
        /// \return
        ///     The database address chosen by the user or nullptr if
        ///     the user has chosen to cancel the dialog.
        virtual auto    enterExistingDatabaseAddress(
                                QWidget * parent
                            ) -> IDatabaseAddress * = 0;

        /// \brief
        ///     Parses an external form of a database address of this type.
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
        /// \brief
        ///     Creates a new, initially empty, database at the specified address.
        /// \param address
        ///     The address to create a new database at.
        /// \return
        ///     The newly created database (initially empty).
        /// \exception DatabaseException
        ///     If an error occurs.
        virtual auto    createDatabase(
                                IDatabaseAddress * address
                            ) -> IDatabase * = 0;

        /// \brief
        ///     Opens an existing database at the specified address.
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

        /// \brief
        ///     Destroys an existing database at the specified address.
        /// \details
        ///     The database must not currently be in use.
        /// \param address
        ///     The address to destroy an existing database at.
        /// \exception DatabaseException
        ///     If an error occurs.
        virtual void    destroyDatabase(
                                IDatabaseAddress * address
                            ) = 0;
    };

    /// \class DatabaseTypeManager tt3-db-api/API.hpp
    /// \brief The manager of known database types.
    class TT3_DB_API_PUBLIC DatabaseTypeManager final
    {
        TT3_UTILITY_CLASS(DatabaseTypeManager)

        //////////
        //  Operations
    public:
        /// \brief
        ///     Registers the specified database type; returns
        ///     true on success, false on failure.
        /// \param databaseType
        ///     The database type to register.
        /// \return
        ///     True on success, false on failure.
        ///     Repeated registration of the same database type
        ///     is considered as a "success".
        static bool     registerDatabaseType(
                                IDatabaseType * databaseType
                            );

        /// \brief
        ///     Un-registers the specified database type.
        /// \param databaseType
        ///     The database type to un-register.
        /// \return
        ///     True on success, false on failure.
        static bool     unregisterDatabaseType(
                                IDatabaseType * databaseType
                            );

        /// \brief
        ///     Finds a registered database type by its mnemonic.
        /// \param mnemonic
        ///     The mnemonic to look for.
        /// \return
        ///     The registered database type with the required
        ///     mnemonic; nullptr if not found.
        static auto     findDatabaseType(
                                const tt3::util::Mnemonic & mnemonic
                            ) -> IDatabaseType *;

        /// \brief
        ///     Returns the set of all registered database types.
        /// \return
        ///     Rhe set of all registered database types.
        static auto     allDatabaseTypes(
                            ) -> DatabaseTypes;

        //////////
        //  Implementation
    private:
        struct _Impl;

        //  Helpers
        static _Impl *  _impl();
    };
}

//  End of tt3-db-api/DatabaseType.hpp
