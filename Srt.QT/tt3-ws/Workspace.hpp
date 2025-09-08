//
//  tt3-ws/Workspace.hpp - "Workspace" ADT support
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
#pragma once
#include "tt3-ws/API.hpp"

namespace tt3::ws
{
    //  A "workspace" is a connection to a
    //  persistent  container of business data.
    class TT3_WS_PUBLIC WorkspaceImpl final : public QObject
    {
        Q_OBJECT
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(WorkspaceImpl)

        friend class WorkspaceTypeImpl;
        friend std::shared_ptr<WorkspaceImpl>;

        friend class ObjectImpl;
        friend class PrincipalImpl;
        friend class UserImpl;
        friend class AccountImpl;

        //////////
        //  Construction/destruction - from friends only
    private:
        WorkspaceImpl(const WorkspaceAddress & address, tt3::db::api::IDatabase * database);
        virtual ~WorkspaceImpl();

        //////////
        //  Operations (general)
    public:
        //  The type of this Workspace. Can be safely
        //  obtained for both open and closed workspaces.
        auto        type(
                        ) const -> WorkspaceType;

        //  The address of this Workspace. Can be safely
        //  obtained for both open and closed workspaces.
        auto        address(
                        ) const -> WorkspaceAddress;

        //  The validator for this workspace.
        auto        validator(
                        ) const -> Validator *
        {
            return type()->validator();
        }

        //  Checks whether this Workspace is open or closed.
        bool        isOpen() const;

        /// Closes this Workspace; has no effect if already closed.
        ///
        /// \exception WorkspaceException
        ///     If an error occurs.
        void        close();

        //////////
        //  Operations (associations)
    public:
        /// \brief
        ///     Returns the set of all users in this workspace.
        /// \param credentials
        ///     The credentials of the service caller.
        /// \return
        ///     The set of all users in this workspace.
        /// \exception WorkspaceException
        ///     If an error occurs.
        auto        users(
                            const Credentials & credentials
                        ) const -> Users;

        /// \brief
        ///     Returns the set of all accounts of all users in this workspace.
        /// \param credentials
        ///     The credentials of the service caller.
        /// \return
        ///     The set of all accounts of all users in this workspace.
        /// \exception WorkspaceException
        ///     If an error occurs.
        auto        accounts(
                            const Credentials & credentials
                        ) const -> Accounts;

        /// \brief
        ///     Finds the account with the specified login.
        /// \param credentials
        ///     The credentials of the service caller.
        /// \param login
        ///     The login to look for.
        /// \return
        ///     The account with the specified login or
        ///     nullptr if not found.
        /// \exception WorkspaceException
        ///     If an error occurs.
        auto        findAccount(
                            const Credentials & credentials,
                            const QString & login
                        ) const -> Account;

        /// \brief
        ///     Returns the set of all activity types in this workspace.
        /// \param credentials
        ///     The credentials of the service caller.
        /// \return
        ///     The set of all activity types in this workspace.
        /// \exception WorkspaceException
        ///     If an error occurs.
        auto        activityTypes(
                            const Credentials & credentials
                        ) const -> ActivityTypes;

        //////////
        //  Operations (access control)
    public:
        /// \brief
        ///     Checks whether the specified Credentials allow ANY kind of
        ///     access to this woirkspace.
        /// \param credentials
        ///     The credentials to validate.
        /// \return
        ///     True if the specified Credentials allow ANY kind of
        ///     access to this woirkspace, else false.
        /// \exception WorkspaceException
        ///     If an error occurs.
        bool        canAccess(
                            const Credentials & credentials
                        ) const;

        /// \brief
        ///     Returns the capabilities that the specified credentials grant
        ///     for this workspace.
        /// \param credentials
        ///     The credentials to validate.
        /// \return
        ///     The capabilities that the specified credentials grant
        ///     for this workspace. If none, returns Capabilities::None.
        /// \exception WorkspaceException
        ///     If an error occurs.
        auto        capabilities(
                            const Credentials & credentials
                        ) const -> Capabilities;

        /// \brief
        ///     Checks if the specified credentials grant
        ///     the specified capability within this workspace.
        /// \details
        ///     IMPORTANT: Only a single capability flag can be checked
        //      for, not a combination of them
        /// \param credentials
        ///     The credentials to check.
        /// \param requiredCapability
        ///     The required capability.
        /// \return
        ///     True if the specified credentials grant the specified
        ///     capability within this workspace, false if not.
        /// \exception WorkspaceException
        ///     If an error occurs.
        bool        grantsCapability(
                            const Credentials & credentials,
                            Capabilities requiredCapability
                        ) const;

        /// \brief
        ///     If there exists an a) enabled account b) of an
        ///     enabled user c) with the spcified credentials,
        ///     returns it; otherwise returns nullptr.
        /// \param credentials
        ///     The credentials to perform the check for.
        /// \return
        ///     The enabled Account of an enabled User which matches
        ///     the specified Credentials; nullptr if not found.
        /// \exception WorkspaceException
        ///     If an error occurs.
        auto        tryLogin(
                            const Credentials & credentials
                        ) const -> Account;

        //////////
        //  Operations (life cycle)
    public:
        /// \brief
        ///     Creates a new User in this workspace.
        /// \param credentials
        ///     The credentials of the service caller.
        /// \param enabled
        ///     True to create initially enabled User, false for disabled.
        /// \param emailAddresses
        ///     The list of e-mail addresses for the new User.
        /// \param realName
        ///     The "real name" for the new User.
        /// \param inactivityTimeout
        ///     The inactivity timeout for the new User; absent == default.
        /// \param uiLocale
        ///     The preferred UI locale for the new User; absent == default.
        /// \return
        ///     The newly created User.
        /// \exception WorkspaceException
        ///     If an error occurs.
        auto        createUser(
                            const Credentials & credentials,
                            bool enabled,
                            const QStringList & emailAddresses,
                            const QString & realName,
                            const InactivityTimeout & inactivityTimeout,
                            const UiLocale & uiLocale
                        ) -> User;

        //////////
        //  Signals
        //  Clients are encourated to use "queued" connections.
    signals:
        /// \brief
        ///     Emitted after the workspace is closed.
        /// \param notification
        ///     The object specifying the source and details
        ///     of the changes made to a Workspace.
        void        workspaceClosed(
                            WorkspaceClosedNotification notification
                        );

        /// \brief
        ///     Emitted after a new object is created
        /// \param notification
        ///     The object specifying the source and details
        ///     of the changes made to a Workspace.
        void        objectCreated(
                            ObjectCreatedNotification notification
                        );

        /// \brief
        ///     Emitted after an object is destroyed
        /// \param notification
        ///     The object specifying the source and details
        ///     of the changes made to a Workspace.
        void        objectDestroyed(
                            ObjectDestroyedNotification notification
                        );

        /// \brief
        ///     Emitted after an object is modified
        /// \param notification
        ///     The object specifying the source and details
        ///     of the changes made to a Workspace.
        void        objectModified(
                            ObjectModifiedNotification notification
                        );

        //////////
        //  Implementation
    private:
        mutable tt3::util::Mutex    _guard;     //  for synchronizing all accesses to workspace

        const WorkspaceAddress      _address;
        tt3::db::api::IDatabase *   _database;  //  nullptr == workspace closed

        //  Access control "cache"
        static inline const int _AccessCacheSizeCap = 16;
        mutable QMap<Credentials, Capabilities> _goodCredentialsCache;
        mutable QSet<Credentials>               _badCredentialsCache;

        //  Object proxy cache
        mutable QMap<Oid, Object>   _proxyCache;

        //  Helpers
        void        _ensureOpen() const;    //  throws WorkspaceException
        void        _markClosed();
        auto        _validateAccessRights(  //  throws WorkspaceException
                            const Credentials & credentials
                        ) const -> Capabilities;

        auto        _getProxy(  //  throws WorkspaceException
                            tt3::db::api::IUser * dataUser
                        ) const -> User;
        auto        _getProxy(  //  throws WorkspaceException
                            tt3::db::api::IAccount * dataAccount
                        ) const -> Account;
        auto        _getProxy(  //  throws WorkspaceException
                            tt3::db::api::IActivityType * dataActivityType
                        ) const -> ActivityType;

        //////////
        //  Event handlers
    private slots:
        void        _onDatabaseClosed(
                            tt3::db::api::DatabaseClosedNotification notification
                        );
        void        _onObjectCreated(
                            tt3::db::api::ObjectCreatedNotification notification
                        );
        void        _onObjectDestroyed(
                            tt3::db::api::ObjectDestroyedNotification notification
                        );
        void        _onObjectModified(
                            tt3::db::api::ObjectModifiedNotification notification
                        );
    };

    //  The accessor for a "current" workspace pseudo-variable.
    //  Only one global static instance of this class exists,
    //  and other instances should NOT be constructed.
    //  Can be nullptr if there is no "current" workspace.
    class TT3_WS_PUBLIC CurrentWorkspace final : public QObject
    {
        Q_OBJECT
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(CurrentWorkspace)

        //////////
        //  Construction/destruction
    public:
        CurrentWorkspace();
        virtual ~CurrentWorkspace();
        \
        //////////
        //  Operators
        //  IMPORTANT: Changing the "current" workspace does
        //  NOT change its "open/closed" status.
    public:
        void        operator = (const Workspace & workspace);
        Workspace   operator -> () const;
                    operator Workspace() const;

        bool        operator == (nullptr_t null) const;
        bool        operator != (nullptr_t null) const;

        //////////
        //  Operations
    public:
        //  Swaps the specified Workspace with the "current" Workspace.
        //  Does NOT change the "open/closed" status of either one.
        void        swap(Workspace & other);

        //////////
        //  Signals
        //  Clients are encourated to use "queued" connections.
    signals:
        /// \brief
        ///     Emitted after the current workspace has changed.
        /// \param before
        ///     The workspace "current" before the change, nullptr == none.
        /// \param after
        ///     The workspace "current" after the change, nullptr == none.
        void        changed(Workspace before, Workspace after);

        //////////
        //  Implementation
    private:
        struct _Impl;

        //  Helpers
        static _Impl *  _impl();
    };

#if defined(TT3_WS_LIBRARY)
    //  Building tt3-ws
#else
    //  Building tt3-ws client
    Q_DECL_IMPORT CurrentWorkspace theCurrentWorkspace;
#endif
}

//  End of tt3-ws/Workspace.hpp
