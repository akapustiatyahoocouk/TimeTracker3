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
    //////////
    //  A "database type" represents a specific storage technology
    //  used to keep data persistent.
    //  Concrete database types will normally be singletons
    //  registered by plugins.
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
        virtual QString     mnemonic() const = 0;

        //  The user-readable display name of this database type
        virtual QString     displayName() const = 0;

        //  The small (16x16) icon representing this database type.
        virtual QIcon       smallIcon() const = 0;

        //  The large (32x32) icon representing this database type.
        virtual QIcon       largeIcon() const = 0;

        //  Checks whether this database type is operationsl (i.e.
        //  can be used) - e.g. a;; required drivers are present, etc.
        virtual bool        isOperational() const = 0;

        //  The short (1 line) status report for this database type.
        virtual QString     shortStatusReport() const = 0;

        //  The longer (mulyi-line) status report for this database
        //  type with newline '\n' character separating the lines.
        virtual QString     fullStatusReport() const = 0;

        //  The validator for databases of this type
        virtual IValidator *validator() const = 0;

        //////////
        //  Operations (address handling)
    public:
        //  If this dataase type has a concept of a "default" database,
        //  returns its address; otherwise returns nullptr.
        virtual IDatabaseAddress *  defaultDatabaseAddress() const = 0;

        //  Prompts the user to interactively specify an address for a new
        //  database of this type, using the specified widget as a parent
        //  for all modal dialogs that may be involved in address choosing.
        //  Returns the database address chosen by the user or nullptr if
        //  the user has chosen to cancel the dialog.
        virtual IDatabaseAddress *  enterNewDatabaseAddress(QWidget * parent) = 0;

        //  Prompts the user to interactively specify an address of an existing
        //  database of this type, using the specified widget as a parent
        //  for all modal dialogs that may be involved in address choosing.
        //  Returns the database address chosen by the user or nullptr if
        //  the user has chosen to cancel the dialog.
        virtual IDatabaseAddress *  enterExistingDatabaseAddress(QWidget * parent) = 0;

        //  Parses an external form of a database address of this type.
        //  Returns the parsed database address or throws DatabaseException if
        //  the address parsing fails for some reason
        virtual IDatabaseAddress *  parseDatabaseAddress(const QString & externalForm) throws(DatabaseException) = 0;

        //////////
        //  Operations (databases)
    public:
        //  Creates a new, initially empty, database at the specified address.
        //  Throws DatabaseException if an error occurs.
        virtual IDatabase *         createDatabase(IDatabaseAddress * address) throws(DatabaseException) = 0;

        //  Opens an existing database at the specified address.
        //  Throws DatabaseException if an error occurs.
        virtual IDatabase *         openDatabase(IDatabaseAddress * address) throws(DatabaseException) = 0;

        //  Destroys an existing database at the specified address.
        //  The database must not currently be in use.
        //  Throws DatabaseException if an error occurs.
        virtual void                destroyDatabase(IDatabaseAddress * address) throws(DatabaseException) = 0;
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
        static bool             registerDatabaseType(IDatabaseType * databaseType);

        //  Finds a registered database type by its mnemonic;
        //  returns nullptr if not found.
        static IDatabaseType *  findDatabaseType(const QString & mnemonic);

        //  Returns the set of all registered database types.
        static DatabaseTypes    allDatabaseTypes();

        //////////
        //  Implementation
    private:
        static QMap<QString, IDatabaseType*>    _registry;  //  key == mnemonic
    };
}

//  End of tt3-db-api/DatabaseType.hpp
