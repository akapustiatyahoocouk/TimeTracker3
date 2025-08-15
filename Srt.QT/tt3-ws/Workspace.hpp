//
//  tt3-ws/Workspace.hpp - "Workspace" ADT
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
    //  A "workspace" is a connection to a
    //  persistent  container of business data.
    class TT3_WS_PUBLIC Workspace final
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(Workspace)

        friend class WorkspaceType;

        //////////
        //  Construction/destruction - from friends only
    private:
        Workspace(const WorkspaceAddress & address, tt3::db::api::IDatabase * database);
    public:
        ~Workspace();

        //////////
        //  Operations (general)
    public:
        //  TODO document
        WorkspaceType *     type() const;
        WorkspaceAddress    address() const;
        bool                isOpen();
        void                close() throws(WorkspaceException);

        //  The validator for this workspace
        Validator * validator() const { return type()->validator(); }

        //////////
        //  Implementation
    private:
        tt3::util::Mutex    _guard {};  //  for synchronizing all accesses to workspace

        const WorkspaceAddress  _address;
        tt3::db::api::IDatabase *   _database;  //  nullptr == workspace closed
    };

    //////////
    //  The accessor for a "current" workspace.
    //  Only one global static instance of this class
    //  exists, and other instances should NOT be
    //  constructed.
    class TT3_WS_PUBLIC CurrentWorkspace final
    {
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
        void                operator = (const WorkspacePtr & workspace);
        Workspace *         operator -> () const;
                            operator WorkspacePtr() const;

        bool                operator == (nullptr_t null) const;
        bool                operator != (nullptr_t null) const;

        //////////
        //  Operations
    public:
        //  TODO document
        Workspace *         get() const;    //  can yield nullptr
        void                swap(WorkspacePtr & other);

        //////////
        //  Implementation
    private:
        static int          _instanceCount; //  ...to disallow a second instance
        static WorkspacePtr _currentWorkspace;
    };

#if defined(TT3_WS_LIBRARY)
    //  Building tt3-ws
#else
    //  Building tt3-ws client
    Q_DECL_IMPORT CurrentWorkspace theCurrentWorkspace;
#endif
}

//  End of tt3-ws/Workspace.hpp
