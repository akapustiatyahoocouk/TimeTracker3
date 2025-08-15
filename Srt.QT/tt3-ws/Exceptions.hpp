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
    //  TODO define proper workspace-level exception
    //  classes, with WorkspaceException::from(exception)
    //  to translate any exception to WorkspaceException
    using WorkspaceException = tt3::db::api::DatabaseException;
    using InvalidWorkspaceAddressException = tt3::db::api::InvalidDatabaseAddressException;
    using WorkspaceInUseException = tt3::db::api::DatabaseInUseException;
    using WorkspaceCorruptException = tt3::db::api::DatabaseCorruptException;
    using WorkspaceClosedException = tt3::db::api::DatabaseClosedException;
    using AccessDeniedException = tt3::db::api::AccessDeniedException;
}

//  End of tt3-ws/DateTime.hpp
