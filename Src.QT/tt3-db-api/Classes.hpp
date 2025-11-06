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
    class TT3_DB_API_PUBLIC IDatabaseType;
    class TT3_DB_API_PUBLIC IDatabaseAddress;
    class TT3_DB_API_PUBLIC IDatabase;
    class TT3_DB_API_PUBLIC IDatabaseLock;
    class TT3_DB_API_PUBLIC IObject;
    class TT3_DB_API_PUBLIC IPrincipal;
    class TT3_DB_API_PUBLIC IUser;
    class TT3_DB_API_PUBLIC IAccount;
    class TT3_DB_API_PUBLIC IActivityType;
    class TT3_DB_API_PUBLIC IActivity;
    class TT3_DB_API_PUBLIC IPublicActivity;
    class TT3_DB_API_PUBLIC IPrivateActivity;
    class TT3_DB_API_PUBLIC ITask;
    class TT3_DB_API_PUBLIC IPublicTask;
    class TT3_DB_API_PUBLIC IPrivateTask;
    class TT3_DB_API_PUBLIC IWork;
    class TT3_DB_API_PUBLIC IEvent;
    class TT3_DB_API_PUBLIC IWorkload;
    class TT3_DB_API_PUBLIC IProject;
    class TT3_DB_API_PUBLIC IWorkStream;
    class TT3_DB_API_PUBLIC IBeneficiary;

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
    class TT3_DB_API_PUBLIC DatabaseException;

    //  Helper types
    class TT3_DB_API_PUBLIC Oid;
    class TT3_DB_API_PUBLIC ChangeNotifier;

    using InactivityTimeout = std::optional<tt3::util::TimeSpan>;
    using UiLocale = std::optional<QLocale>;
}

//  End of tt3-db-api/Classes.hpp
