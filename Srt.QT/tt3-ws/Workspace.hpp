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
        WorkspaceType       type() const;
        WorkspaceAddress    address() const;
        bool                isOpen();
        void                close() throws(WorkspaceException);

        //////////
        //  Implementation
    private:
        tt3::util::Mutex    _guard {};  //  for synchronizing all accesses to workspace

        const WorkspaceAddress  _address;
        tt3::db::api::IDatabase *   _database;  //  nullptr == workspace closed
    };
}

//  End of tt3-ws/Workspace.hpp
