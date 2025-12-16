//
//  tt3-db-api/Classes.hpp - forward declarations and typedefs
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

namespace tt3::db::api
{
    class IDatabaseType;
    class IDatabaseAddress;
    class IDatabase;
    class IDatabaseLock;
    class IObject;
    class IPrincipal;
    class IUser;
    class IAccount;
    class IActivityType;
    class IActivity;
    class IPublicActivity;
    class IPrivateActivity;
    class ITask;
    class IPublicTask;
    class IPrivateTask;
    class IWork;
    class IEvent;
    class IWorkload;
    class IProject;
    class IWorkStream;
    class IBeneficiary;

    //  Collections
    using DatabaseTypes = QSet<IDatabaseType*>;
    using DatabaseAddresses= QSet<IDatabaseAddress*>;
    using Objects = QSet<IObject*>;
    using Users = QSet<IUser*>;
    using Accounts = QSet<IAccount*>;
    using ActivityTypes = QSet<IActivityType*>;
    using Activities = QSet<IActivity*>;
    using PublicActivities = QSet<IPublicActivity*>;
    using PublicTasks = QSet<IPublicTask*>;
    using PrivateActivities = QSet<IPrivateActivity*>;
    using PrivateTasks = QSet<IPrivateTask*>;
    using Works = QSet<IWork*>;
    using Events = QSet<IEvent*>;
    using Workloads = QSet<IWorkload*>;
    using Projects = QSet<IProject*>;
    using WorkStreams = QSet<IWorkStream*>;
    using Beneficiaries = QSet<IBeneficiary*>;

    //  Exceptins & notifications
    class DatabaseException;

    //  Helper types
    class Oid;
    class ChangeNotifier;

    using InactivityTimeout = std::optional<tt3::util::TimeSpan>;
    using UiLocale = std::optional<QLocale>;
}

//  End of tt3-db-api/Classes.hpp
