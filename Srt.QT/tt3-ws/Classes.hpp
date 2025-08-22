//
//  tt3-ws/Classes.hpp - forward declarations and typedefs
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
    class TT3_WS_PUBLIC Credentials;
    class TT3_WS_PUBLIC WorkspaceType;
    class TT3_WS_PUBLIC WorkspaceTypeManager;
    class TT3_WS_PUBLIC WorkspaceAddress;
    class TT3_WS_PUBLIC Workspace;

    class TT3_WS_PUBLIC WorkspaceClosedNotification;

    using WorkspacePtr = std::shared_ptr<Workspace>;

    using WorkspaceTypes = QSet<WorkspaceType*>;
    using WorkspaceAddressesList = QList<WorkspaceAddress>;

    //  We can piggyback on some DB API types
    using Capabilities = tt3::db::api::Capabilities;
}

//  End of tt3-ws/Classes.hpp
