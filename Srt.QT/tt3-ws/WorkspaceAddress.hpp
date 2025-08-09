//
//  tt3-ws/WorkspaceAddress.hpp - "Workspace address" ADT
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
    //  UA workspace address uniquely specifies the
    //  location of a workspace of a certain type.
    //  In addition, a WorkspaceAddress instance can
    //  be "invalid", i.e. guaranteed not to refer to
    //  any existing workspace.
    class TT3_WS_PUBLIC WorkspaceAddress final
    {
        //////////
        //  Construction/destruction/assignment
    private:
        explicit WorkspaceAddress(tt3::db::api::IDatabaseAddress * databaseAddress);
    public:
        WorkspaceAddress();
        WorkspaceAddress(const WorkspaceAddress & src);
        WorkspaceAddress(WorkspaceAddress && src);
        ~WorkspaceAddress();

        WorkspaceAddress &  operator = (const WorkspaceAddress & src);
        WorkspaceAddress &  operator = (WorkspaceAddress && src);

        //////////
        //  Operators
    public:
        //  Impose equality-by-workspace-location
        bool            operator == (const WorkspaceAddress & op2) const;
        bool            operator != (const WorkspaceAddress & op2) const;
        //  Impose an arbitrary stable ordering
        bool            operator <  (const WorkspaceAddress & op2) const;
        bool            operator <= (const WorkspaceAddress & op2) const;
        bool            operator >  (const WorkspaceAddress & op2) const;
        bool            operator >= (const WorkspaceAddress & op2) const;

        //////////
        //  Operations (general)
    public:
        //  Checks if this workspace address is valid.
        bool            isValid() const;

        //  The workspace type to which this workspace address belongs
        //  or nullptr is this workspace address is invalid.
        WorkspaceType * workspaceType() const;

        //  The user-readable form of this workspace address.
        QString         displayForm() const;

        //  The external (re-parsable) form of this workspace address.
        QString         externalForm() const;

        //////////
        //  Implementation
    private:
        tt3::db::api::IDatabaseAddress *    _databaseAddress;   //  nullptr == invalid
    };
}

//  End of tt3-ws/WorkspaceAddress.hpp
