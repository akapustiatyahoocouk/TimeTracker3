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

        friend class WorkspaceTypeManager;

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

        //  Checks whether this workspace type is "operational"
        //  (i.e. can be used).
        bool        isOperational() const;

        //  The short (1 line) status report for this workspace type.
        QString     shortStatusReport() const;

        //  The long (multi-line) status report for this workspace type.
        //  Lines are separated by newline ('\n') character.
        QString     fullStatusReport() const;

        //  The validator for workspaces of this type
        Validator * validator() const { return const_cast<Validator*>(&_validator); }

        //////////
        //  Operations (address handling)
    public:
        WorkspaceAddress    defaultWorkspaceAddress() const;

        //  Prompts the user to interactively specify an address for a new
        //  workspace of this type, using the specified widget as a parent
        //  for all modal dialogs that may be involved in address choosing.
        //  Returns the workspace address chosen by the user or invalid
        //  workspace address if the user has chosen to cancel the dialog.
        WorkspaceAddress    enterNewWorkspaceAddress(QWidget * parent);

        //  Prompts the user to interactively specify an address of an existing
        //  workspace of this type, using the specified widget as a parent
        //  for all modal dialogs that may be involved in address choosing.
        //  Returns the workspace address chosen by the user or invalid
        //  workspace address if the user has chosen to cancel the dialog.
        WorkspaceAddress    enterExistingWorkspaceAddress(QWidget * parent);

        //  Parses an external form of a workspace address of this type.
        //  Returns the parsed workspace address or throws WorkspaceException if
        //  the address parsing fails for some reason
        WorkspaceAddress    parseWorkspaceAddress(const QString & externalForm) throws(WorkspaceException);

        //////////
        //  Operations (workspace)
    public:
        //  Creates a new workspace of this typre at the specigied
        //  address, with  a single administrator user and account.
        //  Throws WorkspaceException if an error occurs.
        WorkspacePtr        createWorkspace(const WorkspaceAddress & address,
                                     const QString & adminUser,
                                     const QString adminLogin, const QString & adminPassword) throws(WorkspaceException);

        //  Opens an existing workspace at the specified address.
        //  Throws WorkspaceException if an error occurs.
        WorkspacePtr        openWorkspace(const WorkspaceAddress & address) throws(WorkspaceException);

        //  Destroys an existing workspace at the specified address.
        //  The workspace must not currently be in use.
        //  Throws WorkspaceException if an error occurs.
        void                destroyWorkspace(const WorkspaceAddress & address) throws(WorkspaceException);

        //////////
        //  Implementation
    private:
        tt3::db::api::IDatabaseType *const  _databaseType;  //  nullptr == invalid
        const Validator     _validator;
    };

    //////////
    //  The registry/manager of known workspace types
    class TT3_WS_PUBLIC WorkspaceTypeManager final
    {
        UTILITY_CLASS(WorkspaceTypeManager)

        friend class WorkspaceAddress;

        //////////
        //  Operations
    public:
        //  Finds a known workspace type by mnemonic;
        //  returns nullptr if mpy fpind.
        //  This is determined based on registered database types.
        static WorkspaceType *  findWorkspaceType(const QString & mnemonic);

        //  The set of all known workspace types.
        //  This is determined based on registered database types.
        static WorkspaceTypes   allWorkspaceTypes();

        //////////
        //  Implementation
    private:
        static tt3::util::Mutex _guard;
        static QMap<tt3::db::api::IDatabaseType*, WorkspaceType*>   _registry;

        //  Helpers
        static void             _collectWorkspaceTypes();
        static WorkspaceType *  _findWorkspaceType(tt3::db::api::IDatabaseType * databaseType);
    };
}

//  End of tt3-ws/WorkspaceType.hpp
