//
//  tt3-ws/Workspace.hpp - "Workspace" ADT support
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
#pragma once
#include "tt3-ws/API.hpp"

namespace tt3::ws
{
    /// \class WorkspaceImpl tt3-ws/API.hpp
    /// \brief A connection to a persistent container of data.
    class TT3_WS_PUBLIC WorkspaceImpl final
        :   public QObject
    {
        Q_OBJECT
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(WorkspaceImpl)

        friend class WorkspaceTypeImpl;
        friend std::shared_ptr<WorkspaceImpl>;

        friend class ObjectImpl;
        friend class PrincipalImpl;
        friend class UserImpl;
        friend class AccountImpl;
        friend class ActivityTypeImpl;
        friend class ActivityImpl;
        friend class PublicActivityImpl;
        friend class PrivateActivityImpl;
        friend class TaskImpl;
        friend class PublicTaskImpl;
        friend class PrivateTaskImpl;
        friend class WorkloadImpl;
        friend class ProjectImpl;
        friend class WorkStreamImpl;
        friend class BeneficiaryImpl;
        friend class WorkImpl;
        friend class EventImpl;

        //////////
        //  Construction/destruction - from friends only
    private:
        WorkspaceImpl(
                WorkspaceAddress address,
                tt3::db::api::IDatabase * database
            );
        ~WorkspaceImpl();

        //////////
        //  Operations (general)
    public:
        /// \brief
        ///     Returns the type of this Workspace.
        /// \details
        ///     Can be safely obtained for both open and
        ///     closed workspaces.
        /// \return
        ///     The type of this Workspace.
        auto        type(
                        ) const -> WorkspaceType;

        /// \brief
        ///     Returns the address of this Workspace.
        /// \details
        ///     Can be safely obtained for both open and
        ///     closed workspaces.
        /// \return
        ///     The address of this Workspace.
        auto        address(
                        ) const -> WorkspaceAddress;

        /// \brief
        ///     Returns the validator for this workspace.
        /// \details
        ///     Can be safely obtained for both open and
        ///     closed workspaces (although WHY?)
        /// \return
        ///     The validator for this workspace.
        auto        validator(
                        ) const -> Validator *
        {
            return type()->validator();
        }

        /// \brief
        ///     Checks whether this Workspace is open or closed.
        /// \return
        ///     True if this Workspace is open, false if closed.
        bool        isOpen() const;

        /// \brief
        ///     Checks whether this Workspace is open (or used
        ///     to be, while open) read-only.
        /// \return
        ///     True if this Workspace is open (or used
        ///     to be, while open) read-only, else false.
        bool        isReadOnly() const;

        /// \brief
        ///     Closes this Workspace.
        /// \details
        ///     Has no effect if already closed.
        /// \exception WorkspaceException
        ///     If an error occurs.
        void        close();

        /// \brief
        ///     If the Workspace performs any sort of cacing
        ///     internally, drops all caches so that all
        ///     subsequent accesses reflect the actual
        ///     persistent data in the underlying database.
        /// \exception WorkspaceException
        ///     If an error occurs.
        void        refresh();

        //////////
        //  Operations (associations)
    public:
        /// \brief
        ///     Returns the set of all users in this workspace.
        /// \param credentials
        ///     The credentials of the service caller.
        /// \return
        ///     The set of all users in this workspace.
        /// \exception WorkspaceException
        ///     If an error occurs.
        auto        users(
                            const Credentials & credentials
                        ) const -> Users;

        /// \brief
        ///     Returns the set of all accounts of all users in this workspace.
        /// \param credentials
        ///     The credentials of the service caller.
        /// \return
        ///     The set of all accounts of all users in this workspace.
        /// \exception WorkspaceException
        ///     If an error occurs.
        auto        accounts(
                            const Credentials & credentials
                        ) const -> Accounts;

        /// \brief
        ///     Finds the account with the specified login.
        /// \param credentials
        ///     The credentials of the service caller.
        /// \param login
        ///     The login to look for.
        /// \return
        ///     The account with the specified login or
        ///     nullptr if not found.
        /// \exception WorkspaceException
        ///     If an error occurs.
        auto        findAccount(
                            const Credentials & credentials,
                            const QString & login
                        ) const -> Account;

        /// \brief
        ///     Returns the set of all activity types in this workspace.
        /// \param credentials
        ///     The credentials of the service caller.
        /// \return
        ///     The set of all activity types in this workspace.
        /// \exception WorkspaceException
        ///     If an error occurs.
        auto        activityTypes(
                            const Credentials & credentials
                        ) const -> ActivityTypes;

        /// \brief
        ///     Returns the set of all public activities in this workspace.
        /// \param credentials
        ///     The credentials of the service caller.
        /// \return
        ///     The set of all public activities in this workspace.
        /// \exception WorkspaceException
        ///     If an error occurs.
        auto        publicActivities(
                            const Credentials & credentials
                        ) const -> PublicActivities;

        /// \brief
        ///     The set of all public activities in this workspace
        ///     including those which are also tasks.
        /// \param credentials
        ///     The credentials of the service caller.
        /// \return
        ///     The set of all public activities in this workspace
        ///     including those which are also tasks.
        /// \exception WorkspaceException
        ///     If an error occurs.
        auto        publicActivitiesAndTasks(
                            const Credentials & credentials
                        ) const -> PublicActivities;

        /// \brief
        ///     Returns the set of all public tasks in this workspace.
        /// \param credentials
        ///     The credentials of the service caller.
        /// \return
        ///     The set of all public tasks in this workspace.
        /// \exception WorkspaceException
        ///     If an error occurs.
        auto        publicTasks(
                            const Credentials & credentials
                        ) const -> PublicTasks;

        /// \brief
        ///     Returns the set of root public tasks in this workspace.
        /// \param credentials
        ///     The credentials of the service caller.
        /// \return
        ///     The set of root public tasks in this workspace.
        /// \exception WorkspaceException
        ///     If an error occurs.
        auto        rootPublicTasks(
                            const Credentials & credentials
                        ) const -> PublicTasks;

        /// \brief
        ///     Returns the set of all Projects in this workspace.
        /// \param credentials
        ///     The credentials of the service caller.
        /// \return
        ///     The set of all Projects in this workspace.
        /// \exception WorkspaceException
        ///     If an error occurs.
        auto        projects(
                            const Credentials & credentials
                        ) const -> Projects;

        /// \brief
        ///     Returns the set of root Projects in this workspace.
        /// \param credentials
        ///     The credentials of the service caller.
        /// \return
        ///     The set of root Projects in this workspace.
        /// \exception WorkspaceException
        ///     If an error occurs.
        auto        rootProjects(
                            const Credentials & credentials
                        ) const -> Projects;

        /// \brief
        ///     Returns the set of all WorkStreams in this workspace.
        /// \param credentials
        ///     The credentials of the service caller.
        /// \return
        ///     The set of all WorkStreams in this workspace.
        /// \exception WorkspaceException
        ///     If an error occurs.
        auto        workStreams(
                            const Credentials & credentials
                        ) const -> WorkStreams;

        /// \brief
        ///     The set of Beneficiaries in this database.
        /// \param credentials
        ///     The credentials of the service caller.
        /// \return
        ///     The set of Beneficiaries in this database.
        /// \exception WorkspaceException
        ///     If an error occurs.
        auto        beneficiaries(
                            const Credentials & credentials
                        ) const -> Beneficiaries;

        //////////
        //  Operations (access control)
    public:
        /// \brief
        ///     Checks whether the specified Credentials allow ANY kind of
        ///     access to this woirkspace.
        /// \param credentials
        ///     The credentials to validate.
        /// \return
        ///     True if the specified Credentials allow ANY kind of
        ///     access to this woirkspace, else false.
        /// \exception WorkspaceException
        ///     If an error occurs.
        bool        canAccess(
                            const Credentials & credentials
                        ) const;

        /// \brief
        ///     Returns the capabilities that the specified credentials grant
        ///     for this workspace.
        /// \param credentials
        ///     The credentials to validate.
        /// \return
        ///     The capabilities that the specified credentials grant
        ///     for this workspace. If none, returns Capabilities::None.
        /// \exception WorkspaceException
        ///     If an error occurs.
        auto        capabilities(
                            const Credentials & credentials
                        ) const -> Capabilities;

        /// \brief
        ///     Checks if the specified credentials grant all of
        ///     the specified capabilities within this workspace.
        /// \details
        ///     IMPORTANT: Only a single capability flag can be checked
        //      for, not a combination of them
        /// \param credentials
        ///     The credentials to check.
        /// \param requiredCapabilities
        ///     The required capabilities.
        /// \return
        ///     True if the specified credentials grant all of the
        ///     specified capabilities within this workspace, false if not.
        /// \exception WorkspaceException
        ///     If an error occurs.
        bool        grantsAll(
                            const Credentials & credentials,
                            Capabilities requiredCapabilities
                        ) const;

        /// \brief
        ///     Checks if the specified credentials grant any of
        ///     the specified capabilities within this workspace.
        /// \details
        ///     IMPORTANT: Only a single capability flag can be checked
        //      for, not a combination of them
        /// \param credentials
        ///     The credentials to check.
        /// \param requiredCapabilities
        ///     The required capabilities.
        /// \return
        ///     True if the specified credentials grant any of the
        ///     specified capabilityieswithin this workspace, false if not.
        /// \exception WorkspaceException
        ///     If an error occurs.
        bool        grantsAny(
                            const Credentials & credentials,
                            Capabilities requiredCapabilities
                        ) const;

        /// \brief
        ///     If there exists an a) enabled account b) of an
        ///     enabled user c) with the spcified credentials,
        ///     returns it; otherwise returns nullptr.
        /// \param credentials
        ///     The credentials to perform the check for.
        /// \return
        ///     The enabled Account of an enabled User which matches
        ///     the specified Credentials; nullptr if not found.
        /// \exception WorkspaceException
        ///     If an error occurs.
        auto        tryLogin(
                            const Credentials & credentials
                        ) const -> Account;

        /// \brief
        ///     If there exists an a) enabled account b) of an
        ///     enabled user c) with the spcified credentials,
        ///     returns it; otherwise an error occurs.
        /// \param credentials
        ///     The credentials to perform the check for.
        /// \return
        ///     The enabled Account of an enabled User which matches
        ///     the specified Credentials.
        /// \exception WorkspaceException
        ///     If an error occurs or login fails.
        auto        login(
                            const Credentials & credentials
                        ) const -> Account;

        //////////
        //  Operations (life cycle)
    public:
        /// \brief
        ///     Creates a new User in this workspace.
        /// \param credentials
        ///     The credentials of the service caller.
        /// \param enabled
        ///     True to create initially enabled User, false for disabled.
        /// \param emailAddresses
        ///     The list of e-mail addresses for the new User.
        /// \param realName
        ///     The "real name" for the new User.
        /// \param inactivityTimeout
        ///     The inactivity timeout for the new User; absent == default.
        /// \param uiLocale
        ///     The preferred UI locale for the new User; absent == default.
        /// \return
        ///     The newly created User.
        /// \exception WorkspaceException
        ///     If an error occurs.
        auto        createUser(
                            const Credentials & credentials,
                            bool enabled,
                            const QStringList & emailAddresses,
                            const QString & realName,
                            const InactivityTimeout & inactivityTimeout,
                            const UiLocale & uiLocale
                        ) -> User;

        /// \brief
        ///     Creates a new ActivityType in this workspace.
        /// \param credentials
        ///     The credentials of the service caller.
        /// \param displayName
        ///     The short (1 line) user-readable display name
        ///     for the new ActivityType.
        /// \param description
        ///     The multi-line user-readable description for the new
        ///     ActivityType; with lines separated by a newline '\n' character.
        /// \return
        ///     The newly created ActivityType.
        /// \exception WorkspaceException
        ///     If an error occurs.
        auto        createActivityType(
                        const Credentials & credentials,
                        const QString & displayName,
                        const QString & description
            ) -> ActivityType;

        /// \brief
        ///     Creates a new PublicActivity in this workspace.
        /// \param credentials
        ///     The credentials of the service caller.
        /// \param displayName
        ///     The short (1 line) user-readable display name
        ///     for the new PublicActivity.
        /// \param description
        ///     The multi-line user-readable description for the new
        ///     PublicActivity; with lines separated by a newline '\\n' character.
        /// \param timeout
        ///     The user-does-nothing timeout for the new PublicActivity;
        ///     absent == none.
        /// \param requireCommentOnStart
        ///     True if the newly created PublicActivity small require the
        ///     user to enter a comment when it is started.
        /// \param requireCommentOnStop
        ///     True if the newly created PublicActivifinishedty small require the
        ///     user to enter a comment when it is started.
        /// \param fullScreenReminder
        ///     True if a full-screen reminder shall be displayed while the
        ///     newly created PublicActivity is underway.
        /// \param activityType
        ///     The type for the new PublicActivity; nullptr == don't assign.
        /// \param workload
        ///     The Workload for the new PublicActivity; nullptr == don't assign.
        /// \return
        ///     The newly created PublicActivity.
        /// \exception WorkspaceException
        ///     If an error occurs.
        auto        createPublicActivity(
                            const Credentials & credentials,
                            const QString & displayName,
                            const QString & description,
                            const InactivityTimeout & timeout,
                            bool requireCommentOnStart,
                            bool requireCommentOnStop,
                            bool fullScreenReminder,
                            ActivityType activityType,
                            Workload workload
                        ) -> PublicActivity;

        /// \brief
        ///     Creates a new root PublicTask in this workspace
        ///     (that is, a PublicTask that has no parent).
        /// \param credentials
        ///     The credentials of the service caller.
        /// \param displayName
        ///     The short (1 line) user-readable display name
        ///     for the new PublicTask.
        /// \param description
        ///     The multi-line user-readable description for the new
        ///     PublicTask; with lines separated by a newline '\\n' character.
        /// \param timeout
        ///     The user-does-nothing timeout for the new PublicTask;
        ///     absent == none.
        /// \param requireCommentOnStart
        ///     True if the newly created PublicTask small require the
        ///     user to enter a comment when it is started.
        /// \param requireCommentOnStop
        ///     True if the newly created PublicTask small require the
        ///     user to enter a comment when it is started.
        /// \param fullScreenReminder
        ///     True if a full-screen reminder shall be displayed while the
        ///     newly created PublicTask is underway.
        /// \param activityType
        ///     The type for the new PublicTask; nullptr == don't assign.
        /// \param workload
        ///     The Workload for the new PublicTask; nullptr == don't assign.
        /// \param completed
        ///     True if the PublicTask shall initially be marked as
        ///     "completed", false if not.
        /// \param requireCommentOnCompletion
        ///     True if the newly created PublicTask small require the
        ///     user to enter a comment when marking it as "completed".
        /// \return
        ///     The newly created PublicTask.
        /// \exception WorkspaceException
        ///     If an error occurs.
        auto        createPublicTask(
                            const Credentials & credentials,
                            const QString & displayName,
                            const QString & description,
                            const InactivityTimeout & timeout,
                            bool requireCommentOnStart,
                            bool requireCommentOnStop,
                            bool fullScreenReminder,
                            ActivityType activityType,
                            Workload workload,
                            bool completed,
                            bool requireCommentOnCompletion
                        ) -> PublicTask;

        /// \brief
        ///     Creates a new root Project in this workspace
        ///     (that is, a Project that has no parent).
        /// \param credentials
        ///     The credentials of the service caller.
        /// \param displayName
        ///     The short (1 line) user-readable display name
        ///     for the new Project.
        /// \param description
        ///     The multi-line user-readable description for the new
        ///     Project; with lines separated by a newline '\\n' character.
        /// \param beneficiaries
        ///     The set of Beneficiaries to associate with the newly
        ///     created Project (can be empty).
        /// \param completed
        ///     True if the new Project shall be initially
        ///     marked as "completed", false if not.
        /// \return
        ///     The newly created Project.
        /// \exception WorkspaceException
        ///     If an error occurs.
        auto        createProject(
                            const Credentials & credentials,
                            const QString & displayName,
                            const QString & description,
                            const Beneficiaries & beneficiaries,
                            bool completed
                        ) -> Project;

        /// \brief
        ///     Creates a new work stream in this workspace.
        /// \param credentials
        ///     The credentials of the service caller.
        /// \param displayName
        ///     The short (1 line) user-readable display name
        ///     for the new WorkStream.
        /// \param description
        ///     The multi-line user-readable description for the new
        ///     WorkStream; with lines separated by a newline '\\n' character.
        /// \param beneficiaries
        ///     The set of Beneficiaries to associate with the newly
        ///     created WorkStream (can be empty).
        /// \return
        ///     The newly created WorkStream.
        /// \exception WorkspaceException
        ///     If an error occurs.
        auto        createWorkStream(
                            const Credentials & credentials,
                            const QString & displayName,
                            const QString & description,
                            const Beneficiaries & beneficiaries
                        ) -> WorkStream;

        /// \brief
        ///     Creates a new Beneficiary in this wprkspace.
        /// \param credentials
        ///     The credentials of the service caller.
        /// \param displayName
        ///     The short (1 line) user-readable display name
        ///     for the new Beneficiary.
        /// \param description
        ///     The multi-line user-readable description for the new
        ///     Beneficiary; with lines separated by a newline '\\n' character.
        /// \param workloads
        ///     The workloads to associate the new Beneficiary with.
        /// \return
        ///     The newly created Beneficiary.
        /// \exception DatabaseException
        ///     If an error occurs.
        auto        createBeneficiary(
                            const Credentials & credentials,
                            const QString & displayName,
                            const QString & description,
                            const Workloads & workloads
                        ) -> Beneficiary;

        //////////
        //  Signals
        //  Clients are encourated to use "queued" connections.
    signals:
        /// \brief
        ///     Emitted after the workspace is closed.
        /// \param notification
        ///     The object specifying the source and details
        ///     of the changes made to a Workspace.
        void        workspaceClosed(
                            WorkspaceClosedNotification notification
                        );

        /// \brief
        ///     Emitted after a new object is created
        /// \param notification
        ///     The object specifying the source and details
        ///     of the changes made to a Workspace.
        void        objectCreated(
                            ObjectCreatedNotification notification
                        );

        /// \brief
        ///     Emitted after an object is destroyed
        /// \param notification
        ///     The object specifying the source and details
        ///     of the changes made to a Workspace.
        void        objectDestroyed(
                            ObjectDestroyedNotification notification
                        );

        /// \brief
        ///     Emitted after an object is modified
        /// \param notification
        ///     The object specifying the source and details
        ///     of the changes made to a Workspace.
        void        objectModified(
                            ObjectModifiedNotification notification
                        );

        //////////
        //  Implementation
    private:
        mutable tt3::util::Mutex    _guard;     //  for synchronizing all accesses to workspace

        const WorkspaceAddress      _address;
        tt3::db::api::IDatabase *const _database;   //  never nullptr
        bool                        _isOpen = true; //  all Workspaces start off as "open"
        const bool                  _isReadOnly;

        //  Access control "cache"
        static inline const int _AccessCacheSizeCap = 16;
        mutable QMap<Credentials, Capabilities> _goodCredentialsCache;
        mutable QSet<Credentials>               _badCredentialsCache;

        //  Object proxy cache
        mutable QMap<Oid, Object>   _proxyCache;

        //  Helpers
        void        _ensureOpen() const;    //  throws WorkspaceException
        void        _markClosed();
        auto        _validateAccessRights(  //  throws WorkspaceException
                            const Credentials & credentials
                        ) const -> Capabilities;

        auto        _getProxy(  //  throws WorkspaceException
                            tt3::db::api::IUser * dataUser
                        ) const -> User;
        auto        _getProxy(  //  throws WorkspaceException
                            tt3::db::api::IAccount * dataAccount
                        ) const -> Account;
        auto        _getProxy(  //  throws WorkspaceException
                            tt3::db::api::IActivityType * dataActivityType
                        ) const -> ActivityType;
        auto        _getProxy(  //  throws WorkspaceException
                            tt3::db::api::IActivity * dataActivity
                        ) const -> Activity;
        auto        _getProxy(  //  throws WorkspaceException
                            tt3::db::api::IPublicActivity * dataPublicActivity
                        ) const -> PublicActivity;
        auto        _getProxy(  //  throws WorkspaceException
                            tt3::db::api::IPublicTask * dataPublicTask
                        ) const -> PublicTask;
        auto        _getProxy(  //  throws WorkspaceException
                            tt3::db::api::IPrivateActivity * dataPublicActivity
                        ) const -> PrivateActivity;
        auto        _getProxy(  //  throws WorkspaceException
                            tt3::db::api::IPrivateTask * dataPrivateTask
                        ) const -> PrivateTask;
        auto        _getProxy(  //  throws WorkspaceException
                            tt3::db::api::IWorkload * dataWorkload
                        ) const -> Workload;
        auto        _getProxy(  //  throws WorkspaceException
                            tt3::db::api::IProject * dataProject
                        ) const -> Project;
        auto        _getProxy(  //  throws WorkspaceException
                            tt3::db::api::IWorkStream * dataWorkStream
                        ) const -> WorkStream;
        auto        _getProxy(  //  throws WorkspaceException
                            tt3::db::api::IBeneficiary * dataBeneficiary
                        ) const -> Beneficiary;
        auto        _getProxy(  //  throws WorkspaceException
                            tt3::db::api::IWork * dataWork
                        ) const -> Work;
        auto        _getProxy(  //  throws WorkspaceException
                            tt3::db::api::IEvent * dataEvent
                        ) const -> Event;

        //////////
        //  Event handlers
    private slots:
        void        _onDatabaseClosed(
                            tt3::db::api::DatabaseClosedNotification notification
                        );
        void        _onObjectCreated(
                            tt3::db::api::ObjectCreatedNotification notification
                        );
        void        _onObjectDestroyed(
                            tt3::db::api::ObjectDestroyedNotification notification
                        );
        void        _onObjectModified(
                            tt3::db::api::ObjectModifiedNotification notification
                        );
    };
}

//  End of tt3-ws/Workspace.hpp
