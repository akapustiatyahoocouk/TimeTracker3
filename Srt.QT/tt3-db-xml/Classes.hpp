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
    class TT3_DB_XML_PUBLIC DatabaseType;
    class TT3_DB_XML_PUBLIC DatabaseAddress;
    class TT3_DB_XML_PUBLIC Database;

    class TT3_DB_XML_PUBLIC Object;
    class TT3_DB_XML_PUBLIC Principal;
    class TT3_DB_XML_PUBLIC User;
    class TT3_DB_XML_PUBLIC Account;
    class TT3_DB_XML_PUBLIC ActivityType;
    class TT3_DB_XML_PUBLIC Activity;
    class TT3_DB_XML_PUBLIC PublicActivity;
    class TT3_DB_XML_PUBLIC PublicTask;
    class TT3_DB_XML_PUBLIC PrivateActivity;
    class TT3_DB_XML_PUBLIC PrivateTask;
    class TT3_DB_XML_PUBLIC Work;
    class TT3_DB_XML_PUBLIC Event;
    class TT3_DB_XML_PUBLIC Workload;
    class TT3_DB_XML_PUBLIC Project;
    class TT3_DB_XML_PUBLIC WorkStream;
    class TT3_DB_XML_PUBLIC Beneficiary;

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
