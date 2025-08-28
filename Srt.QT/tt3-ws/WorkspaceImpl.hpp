//
//  tt3-ws/WorkspaceImpl.hpp - "Workspace" ADT implemenyayion
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
    //////////
    //  A "workspace" is a connection to a
    //  persistent  container of business data.
    class TT3_WS_PUBLIC WorkspaceImpl final : public QObject
    {
        Q_OBJECT
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(WorkspaceImpl)

        friend class WorkspaceTypeImpl;
        friend std::shared_ptr<WorkspaceImpl>;

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

        //  Checks whether this Workspace is open or closed.
        bool                isOpen();

        //  Closes this Workspace; has no effect ig already closed.
        //  Throws WorkspaceException if an error occurs.
        void                close() throws(WorkspaceException);

        //  The validator for this workspace
        Validator *         validator() const { return type()->validator(); }

        //////////
        //  Operations (access control)
    public:
        //  Checks whether the specified Credentials allow ANY kind of
        //  access to this woirkspace
        bool            canAccess(const Credentials & credentials) const throws(WorkspaceException);

        //  Returns the capabilities that the specified credentials grant
        //  for thos workspace. If none, returns Capabilities::None.
        Capabilities    capabilities(const Credentials & credentials) const throws(WorkspaceException);

        //////////
        //  Signals
    signals:
        void        workspaceClosed(WorkspaceClosedNotification notification);

        //////////
        //  Implementation
    private:
        mutable tt3::util::Mutex    _guard {};  //  for synchronizing all accesses to workspace

        const WorkspaceAddress      _address;
        tt3::db::api::IDatabase *   _database;  //  nullptr == workspace closed

        //  Acces control "cache" - 1 entry is enough, as workspace
        //  services will typically be invoked with the same Credentials
        using CredentialsPtr = Credentials*;
        mutable CredentialsPtr  _accessCacheKey = nullptr;  //  mullptr == value not cached
        mutable Capabilities    _accessCacheValue = Capabilities::None;

        //  Helpers
        void                _ensureOpen() const throws(WorkspaceException);
        void                _markClosed();

        //////////
        //  Event handlers
    private slots:
        void                _onDatabaseClosed(tt3::db::api::DatabaseClosedNotification notification);
        void                _onObjectCreated(tt3::db::api::ObjectCreatedNotification notification);
        void                _onObjectDestroyed(tt3::db::api::ObjectDestroyedNotification notification);
        void                _onObjectModified(tt3::db::api::ObjectModifiedNotification notification);
    };

    //////////
    //  The accessor for a "current" workspace.
    //  Only one global static instance of this class
    //  exists, and other instances should NOT be
    //  constructed.
    //  Can be nullptr if there is no "current" workspace.
    class TT3_WS_PUBLIC CurrentWorkspace final : public QObject
    {
        Q_OBJECT
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(CurrentWorkspace)

        //////////
        //  Construction/destruction
    public:
        CurrentWorkspace();
        ~CurrentWorkspace();
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
        void                swap(Workspace & other);

        //////////
        //  Signals
    signals:
        void                currentWorkspaceChanged();

        //////////
        //  Implementation
    private:
        static int          _instanceCount; //  ...to disallow a second instance
        static Workspace    _currentWorkspace;
    };

#if defined(TT3_WS_LIBRARY)
    //  Building tt3-ws
#else
    //  Building tt3-ws client
    Q_DECL_IMPORT CurrentWorkspace theCurrentWorkspace;
#endif
}

//  End of tt3-ws/WorkspaceImpl.hpp
