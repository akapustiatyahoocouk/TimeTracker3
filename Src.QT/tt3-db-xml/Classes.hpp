//
//  tt3-db-xml/Classes.hpp - forward declarations and typedefs
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

namespace tt3::db::xml
{
    class DatabaseType;
    class DatabaseAddress;
    class Database;
    class DatabaseLock;

    class Object;
    class Principal;
    class User;
    class Account;
    class ActivityType;
    class Activity;
    class PublicActivity;
    class PublicTask;
    class PrivateActivity;
    class PrivateTask;
    class Work;
    class Event;
    class Workload;
    class Project;
    class WorkStream;
    class Beneficiary;

    using Objects = QSet<Object*>;
    using Users = QSet<User*>;
    using Accounts = QSet<Account*>;
    using ActivityTypes = QSet<ActivityType*>;
    using Activities = QSet<Activity*>;
    using PublicActivities = QSet<PublicActivity*>;
    using PublicTasks = QSet<PublicTask*>;
    using PrivateActivities = QSet<PrivateActivity*>;
    using PrivateTasks = QSet<PrivateTask*>;
    using Works = QSet<Work*>;
    using Events = QSet<Event*>;
    using Workloads = QSet<Workload*>;
    using Projects = QSet<Project*>;
    using WorkStreams = QSet<WorkStream*>;
    using Beneficiaries = QSet<Beneficiary*>;
}

//  End of tt3-db-xml/Classes.hpp
