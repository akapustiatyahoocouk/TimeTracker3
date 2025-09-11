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
    class TT3_WS_PUBLIC WorkspaceTypeImpl;
    class TT3_WS_PUBLIC WorkspaceAddressImpl;
    class TT3_WS_PUBLIC WorkspaceImpl;

    class TT3_WS_PUBLIC ObjectImpl;
    class TT3_WS_PUBLIC PrincipalImpl;
    class TT3_WS_PUBLIC UserImpl;
    class TT3_WS_PUBLIC AccountImpl;
    class TT3_WS_PUBLIC ActivityTypeImpl;
    class TT3_WS_PUBLIC ActivityImpl;

    //  Opaque pointer-like types (reference counted);
    //  can all be nullptr
    using WorkspaceType = WorkspaceTypeImpl*;
    using WorkspaceAddress = std::shared_ptr<WorkspaceAddressImpl>;
    using Workspace = std::shared_ptr<WorkspaceImpl>;

    using Object = std::shared_ptr<ObjectImpl>;
    using Principal = std::shared_ptr<PrincipalImpl>;
    using User = std::shared_ptr<UserImpl>;
    using Account = std::shared_ptr<AccountImpl>;
    using ActivityType = std::shared_ptr<ActivityTypeImpl>;
    using Activity = std::shared_ptr<ActivityImpl>;

    //  Collections
    using WorkspaceTypes = QSet<WorkspaceType>;
    using WorkspaceAddressesList = QList<WorkspaceAddress>;

    using Users = QSet<User>;
    using Accounts = QSet<Account>;
    using ActivityTypes = QSet<ActivityType>;
    using Activities = QSet<Activity>;

    //  Exceptins & notifications
    class TT3_WS_PUBLIC WorkspaceException;
    class TT3_WS_PUBLIC WorkspaceClosedNotification;
    class TT3_WS_PUBLIC ObjectCreatedNotification;
    class TT3_WS_PUBLIC ObjectDestroyedNotification;
    class TT3_WS_PUBLIC ObjectModifiedNotification;

    //  Helper types
    class TT3_WS_PUBLIC Credentials;
    class TT3_WS_PUBLIC WorkspaceTypeManager;

    //  We can piggyback on some DB API types
    using OpenMode = tt3::db::api::OpenMode;
    using ObjectType = tt3::db::api::IObjectType;
    using ObjectTypes = tt3::db::api::ObjectTypes;
    using Capabilities = tt3::db::api::Capabilities;

    using Oid = tt3::db::api::Oid;

    using InactivityTimeout = tt3::db::api::InactivityTimeout;
    using UiLocale = tt3::db::api::UiLocale;
}

//  End of tt3-ws/Classes.hpp
