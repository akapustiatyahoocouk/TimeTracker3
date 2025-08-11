//
//  tt3-ws/Exceptions.hpp - Workspace - level exceptions
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
    using WorkspaceException = tt3::db::api::DatabaseException;
    using InvalidWorkspaceAddressException = tt3::db::api::InvalidDatabaseAddressException;
    using WorkspaceInUseException = tt3::db::api::DatabaseInUseException;
    using WorkspaceCorruptException = tt3::db::api::DatabaseCorruptException;
    using WorkspaceClosedException = tt3::db::api::DatabaseClosedException;

    //  Thrown when a workspace service is access-denied
    class TT3_WS_PUBLIC AccessDeniedException : public WorkspaceException
    {
        //////////
        //  Construction/destruction/assignment
    public:
        AccessDeniedException()
            :   WorkspaceException("Access denied") {}

        //////////
        //  QException
    public:
        virtual AccessDeniedException *
                        clone() const { return new AccessDeniedException(*this); }
        virtual void    raise() const { throw *this; }
    };
}

//  End of tt3-ws/DateTime.hpp
