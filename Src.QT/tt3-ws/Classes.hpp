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
    class WorkspaceTypeImpl;
    class WorkspaceAddressImpl;
    class WorkspaceImpl;

    class ObjectImpl;
    class PrincipalImpl;
    class UserImpl;
    class AccountImpl;
    class ActivityTypeImpl;
    class ActivityImpl;
    class PublicActivityImpl;
    class PrivateActivityImpl;
    class TaskImpl;
    class PublicTaskImpl;
    class PrivateTaskImpl;
    class WorkImpl;
    class EventImpl;
    class WorkloadImpl;
    class ProjectImpl;
    class WorkStreamImpl;
    class BeneficiaryImpl;

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
    using PublicActivity = std::shared_ptr<PublicActivityImpl>;
    using PrivateActivity = std::shared_ptr<PrivateActivityImpl>;
    using Task = std::shared_ptr<TaskImpl>;
    using PublicTask = std::shared_ptr<PublicTaskImpl>;
    using PrivateTask = std::shared_ptr<PrivateTaskImpl>;
    using Work = std::shared_ptr<WorkImpl>;
    using Event = std::shared_ptr<EventImpl>;
    using Workload = std::shared_ptr<WorkloadImpl>;
    using Project = std::shared_ptr<ProjectImpl>;
    using WorkStream = std::shared_ptr<WorkStreamImpl>;
    using Beneficiary = std::shared_ptr<BeneficiaryImpl>;

    //  Collections
    using WorkspaceTypes = QSet<WorkspaceType>;
    using WorkspaceAddressesList = QList<WorkspaceAddress>;

    using Users = QSet<User>;
    using Accounts = QSet<Account>;
    using ActivityTypes = QSet<ActivityType>;
    using Activities = QSet<Activity>;
    using PublicActivities = QSet<PublicActivity>;
    using PrivateActivities = QSet<PrivateActivity>;
    using Tasks = QSet<Task>;
    using PublicTasks = QSet<PublicTask>;
    using PrivateTasks = QSet<PrivateTask>;
    using Works = QSet<Work>;
    using Events = QSet<Event>;
    using Workloads = QSet<Workload>;
    using Projects = QSet<Project>;
    using WorkStreams = QSet<WorkStream>;
    using Beneficiaries = QSet<Beneficiary>;

    //  Exceptins & notifications
    class WorkspaceException;
    class WorkspaceClosedNotification;
    class ObjectCreatedNotification;
    class ObjectDestroyedNotification;
    class ObjectModifiedNotification;

    //  Helper types
    class Credentials;
    class BackupCredentials;
    class RestoreCredentials;
    class ReportCredentials;
    class WorkspaceTypeManager;

    //  We can piggyback on some DB API types
    using OpenMode = tt3::db::api::OpenMode;
    using ObjectType = tt3::db::api::IObjectType;
    using ObjectTypes = tt3::db::api::ObjectTypes;
    using Capability = tt3::db::api::Capability;
    using Capabilities = tt3::db::api::Capabilities;

    using Oid = tt3::db::api::Oid;

    using InactivityTimeout = tt3::db::api::InactivityTimeout;
    using UiLocale = tt3::db::api::UiLocale;
}

//  End of tt3-ws/Classes.hpp
