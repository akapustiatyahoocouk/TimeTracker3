//
//  tt3-ws/WorkspaceTypeImpl.hpp - "Workspace type" ADT support
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
    //  A "workspace type" represents a specific storage technology
    //  used to keep data persistent.
    //  The set of available workspace types is generated
    //  automatically based on the available database types.
    class TT3_WS_PUBLIC WorkspaceTypeImpl final
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(WorkspaceTypeImpl)

        friend class WorkspaceTypeManager;
        friend class WorkspaceAddressImpl;
        friend class WorkspaceImpl;

        //////////
        //  Construction/destruction - from friends only
    private:
        explicit WorkspaceTypeImpl(tt3::db::api::IDatabaseType * databaseType);
        ~WorkspaceTypeImpl();

        //////////
        //  Operation (general)
    public:
        //  The mnemonic identifier of this workspace type
        auto        mnemonic(
                        ) const -> tt3::util::Mnemonic;

        //  The user-readable display name of this workspace type
        auto        displayName(
                        ) const -> QString;

        //  The small (16x16) icon representing this workspace type.
        auto        smallIcon(
                        ) const -> QIcon;

        //  The large (32x32) icon representing this workspace type.
        auto        largeIcon(
                        ) const -> QIcon;

        //  Checks whether this workspace type is "operational"
        //  (i.e. can be used).
        bool        isOperational(
                        ) const;

        //  The short (1 line) status report for this workspace type.
        auto        shortStatusReport(
                        ) const -> QString;

        //  The long (multi-line) status report for this workspace type.
        //  Lines are separated by newline ('\n') character.
        auto        fullStatusReport(
                        ) const -> QString;

        //  The validator for workspaces of this type
        auto        validator(
                        ) const -> Validator *
        {
            return const_cast<Validator*>(&_validator);
        }

        //////////
        //  Operations (address handling)
    public:
        /// \brief
        ///     If this workspace type has a concept of a "default"
        ///     workspace, returns its address, else returns nullptr.
        /// \return
        ///     The address of the "default" workspace of this type;
        ///     nullptr if this workspace type has no concept of
        ///     a "default" workspace.
        auto        defaultWorkspaceAddress(
                        ) const -> WorkspaceAddress;

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

        /// \brief
        ///     Parses an external form of a workspace address of this type.
        /// \param externalForm
        ///     An external form of a workspace address of this type.
        /// \return
        ///     The parsed workspace address.
        /// \exception WorkspaceException
        ///     If address parsing fails for some reason.
        auto        parseWorkspaceAddress(
                            const QString & externalForm
                        ) -> WorkspaceAddress;

        //////////
        //  Operations (workspace)
    public:
        /// \brief
        ///     Creates a new workspace of this type at the specified address.
        /// \details
        ///     The newly created Worspace has a single administrator user
        ///     and account.
        /// \param address
        ///     The address for the new Workspace; must be of this type.
        /// \param adminUser
        ///     The "real name" for the admin User for the new Workspace.
        /// \param adminLogin
        ///     The login for the admin Account for the new Workspace.
        /// \param adminPassword
        ///     The password for the admin Account for the new Workspace.
        /// \return
        ///     The newly created Workspace.
        /// \exception WorkspaceException
        ///     If an error occurs.
        auto        createWorkspace(
                            const WorkspaceAddress & address,
                            const QString & adminUser,
                            const QString & adminLogin,
                            const QString & adminPassword
                        ) -> Workspace;

        /// \brief
        ///     Opens an existing workspace at the specified address.
        /// \param address
        ///     The address of an existing Workspace; must be of this type.
        /// \return
        ///     The newly open Workspace.
        /// \exception WorkspaceException
        ///     If an error occurs.
        auto        openWorkspace(
                            const WorkspaceAddress & address
                        ) -> Workspace;

        /// \brief
        ///     Destroys an existing workspace at the specified address.
        /// \details
        ///     The workspace must not currently be in use.
        ///     IMPORTANT: The specified credentials must permit Administrator
        ///     access to the workspace in question, or the workspace destruction
        ///     will fail wth an AccessDeniedException!
        /// \param credentials
        ///     The credentials that must grant an Administrator capability
        ///     for the destroyed Workspace (or else its destruction will fail).
        /// \param address
        ///     The address of an existing Workspace; must be of this type.
        /// \exception WorkspaceException
        ///     If an error occurs.
        void        destroyWorkspace(
                            const Credentials & credentials,
                            const WorkspaceAddress & address
                        );

        //////////
        //  Implementation
    private:
        tt3::db::api::IDatabaseType *const  _databaseType;  //  nullptr == invalid
        const Validator     _validator;
        tt3::util::Mutex    _cacheGuard;    //  used for all caches

        //  DB -> WS address cache, keys count as "references".
        QMap<tt3::db::api::IDatabaseAddress*,WorkspaceAddress>  _workspaceAddressCache;

        //  WS impl -> WS cache
        QMap<WorkspaceImpl*,Workspace>  _workspaceCache;

        //  Helpers
        WorkspaceAddress    _mapDatabaseAddress(tt3::db::api::IDatabaseAddress * databaseAddress);
        void                _pruneWorkspaceAddressCache();
        Workspace           _mapWorkspace(WorkspaceImpl * impl);
        void                _pruneWorkspaceCache();
    };

    //////////
    //  The registry/manager of known workspace types
    class TT3_WS_PUBLIC WorkspaceTypeManager final
    {
        UTILITY_CLASS(WorkspaceTypeManager)

        friend class WorkspaceAddressImpl;

        //////////
        //  Operations
    public:
        //  Finds a known workspace type by mnemonic;
        //  returns nullptr if mpy fpind.
        //  This is determined based on registered database types.
        static WorkspaceType    findWorkspaceType(const tt3::util::Mnemonic & mnemonic);

        //  The set of all known workspace types.
        //  This is determined based on registered database types.
        static WorkspaceTypes   allWorkspaceTypes();

        //////////
        //  Implementation
    private:
        struct _Impl;

        //  Helpers
        static _Impl *          _impl();
        //  TODO kill off static WorkspaceType    _findWorkspaceType(tt3::db::api::IDatabaseType * databaseType);
    };
}

//  End of tt3-ws/WorkspaceTypeImpl.hpp
