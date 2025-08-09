//
//  tt3-ws/WorkspaceType.hpp - "Workspace type" ADT
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

namespace tt3::ws
{
    //////////
    //  A "workspace type" represents a specific storage technology
    //  used to keep data persistent.
    //  The set of available workspace types is generated
    //  automatically based on the available database types.
    class TT3_WS_PUBLIC WorkspaceType final
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(WorkspaceType)

        friend class WorkspaceTypeRegistry;

        //////////
        //  Construction/destruction - from friends only
    private:
        explicit WorkspaceType(tt3::db::api::IDatabaseType * databaseType);
        ~WorkspaceType();

        //////////
        //  Operation (general)
    public:
        //  The mnemonic identifier of this workspace type
        QString     mnemonic() const;

        //  The user-readable display name of this workspace type
        QString     displayName() const;

        //  The small (16x16) icon representing this workspace type.
        QIcon       smallIcon() const;

        //  The large (32x32) icon representing this workspace type.
        QIcon       largeIcon() const;

        //  TODO document
        bool        isOperational() const;
        QString     shortStatusReport() const;
        QString     fullStatusReport() const;

#if 0 //    TODO uncomment
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
        //  TODO document
        virtual IDatabase *         createDatabase(IDatabaseAddress * address) throws(DatabaseException) = 0;
        virtual IDatabase *         openDatabase(IDatabaseAddress * address) throws(DatabaseException) = 0;
        virtual void                destroyDatabase(IDatabaseAddress * address) throws(DatabaseException) = 0;
#endif

        //////////
        //  Implementation
    private:
        tt3::db::api::IDatabaseType *const  _databaseType;  //  nullptr == invalid
    };

    //////////
    //  The registry/manager of known workspace types
    class TT3_WS_PUBLIC WorkspaceTypeRegistry final
    {
        UTILITY_CLASS(WorkspaceTypeRegistry)

        //////////
        //  Operations
    public:
        //  TODO document
        static WorkspaceType *  findWorkspaceType(const QString & mnemonic);
        static WorkspaceTypes   allWorkspaceTypes();

        //////////
        //  Implementation
    private:
        static QMap<tt3::db::api::IDatabaseType*, WorkspaceType*>   _registry;

        //  Helpers
        static void             _collectWorkspaceTypes();
    };
}

//  End of tt3-ws/WorkspaceType.hpp
