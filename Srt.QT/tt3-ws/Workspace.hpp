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
        WorkspaceType       type() const;

        //  The address of this Workspace. Can be safely
        //  obtained for both open and closed workspaces.
        WorkspaceAddress    address() const;

        //  The validator for this workspace.
        Validator *         validator() const { return type()->validator(); }

        //  Checks whether this Workspace is open or closed.
        bool                isOpen();

        //  Closes this Workspace; has no effect ig already closed.
        //  Throws WorkspaceException if an error occurs.
        void                close() throws(WorkspaceException);

        //////////
        //  Operations (associations)
    public:
        //  The set of all users in this workspace.
        //  Throws WorkspaceException if an error occurs.
        Users           users(const Credentials & credentials) const throws(WorkspaceException);

        //  The set of all accounts of all users in this workspace.
        //  Throws WorkspaceException if an error occurs.
        Accounts        accounts(const Credentials & credentials) const throws(WorkspaceException);

        //  The account with the specified login.
        //  Throws WorkspaceException if an error occurs.
        Account         findAccount(const Credentials & credentials, const QString & login) const throws(WorkspaceException);

        //////////
        //  Operations (access control)
    public:
        //  Checks whether the specified Credentials allow ANY kind of
        //  access to this woirkspace
        //  Throws WorkspaceException if an error occurs.
        bool            canAccess(const Credentials & credentials) const throws(WorkspaceException);

        //  Returns the capabilities that the specified credentials grant
        //  for this workspace. If none, returns Capabilities::None.
        //  Throws WorkspaceException if an error occurs.
        Capabilities    capabilities(const Credentials & credentials) const throws(WorkspaceException);

        //////////
        //  Operations (life cycle)
    public:
        //  Creates a new User in this database.
        //  Throws WorkspaceException if an error occurs.
        User            createUser(const Credentials & credentials,
                            bool enabled, const QStringList & emailAddresses,
                            const QString & realName,
                            const InactivityTimeout & inactivityTimeout,
                            const UiLocale & uiLocale) throws(WorkspaceException);

        //////////
        //  Signals
    signals:
        //  Emitted after the workspace is closed.
        void        workspaceClosed(WorkspaceClosedNotification notification);

        //  Emitted after a new object is created
        void        objectCreared(ObjectCreatedNotification notification);

        //  Emitted after an object is destroyed
        void        objectDestroyed(ObjectDestroyedNotification notification);

        //  Emitted after an object is modified
        void        objectModified(ObjectModifiedNotification notification);

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
        void                _ensureOpen() const throws(WorkspaceException);
        void                _markClosed();
        Capabilities        _validateAccessRights(const Credentials & credentials) const throws(WorkspaceException);

        User                _getProxy(tt3::db::api::IUser * dataUser) const;
        Account             _getProxy(tt3::db::api::IAccount * dataAccount) const;

        //////////
        //  Event handlers
    private slots:
        void                _onDatabaseClosed(tt3::db::api::DatabaseClosedNotification notification);
        void                _onObjectCreated(tt3::db::api::ObjectCreatedNotification notification);
        void                _onObjectDestroyed(tt3::db::api::ObjectDestroyedNotification notification);
        void                _onObjectModified(tt3::db::api::ObjectModifiedNotification notification);
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
        void                operator = (const Workspace & workspace);
        Workspace           operator -> () const;
                            operator Workspace() const;

        bool                operator == (nullptr_t null) const;
        bool                operator != (nullptr_t null) const;

        //////////
        //  Operations
    public:
        //  Swaps the specified Workspace with the "current" Workspace.
        //  Does NOT change the "open/closed" status of either one.
        void                swap(Workspace & other);

        //////////
        //  Signals
    signals:
        //  Emitted after the current workspace has changed.
        void                changed(Workspace before, Workspace after);

        //////////
        //  Implementation
    private:
        static std::atomic<int>     _instanceCount; //  ...to disallow a second instance
        mutable tt3::util::Mutex    _currentWorkspaceGuard;
        Workspace                   _currentWorkspace = nullptr;
    };

#if defined(TT3_WS_LIBRARY)
    //  Building tt3-ws
#else
    //  Building tt3-ws client
    Q_DECL_IMPORT CurrentWorkspace theCurrentWorkspace;
#endif
}

//  End of tt3-ws/Workspace.hpp
