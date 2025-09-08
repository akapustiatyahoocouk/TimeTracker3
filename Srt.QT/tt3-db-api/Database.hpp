//
//  tt3-db-api/Database.hpp - "database" ADT
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
    //////////
    //  A "database" represents an open connection
    //  to a persistent data container.
    class TT3_DB_API_PUBLIC IDatabase
    {
        //////////
        //  This is an interface
    public:
        IDatabase() = default;
        virtual ~IDatabase() = default;

        //////////
        //  Operations (general)
    public:
        /// \brief
        ///     Returns the type of this database.
        /// \details
        ///     Can be safely obtained on both open and closed databases.
        /// \return
        ///     The type of this database.
        virtual auto    type() const
                            -> IDatabaseType * = 0;

        /// \brief
        ///     Returns the address of this database.
        /// \details
        ///     Can be safely obtained on both open and closed databases.
        /// \return
        ///     The address of this database.
        virtual auto    address() const
                            -> IDatabaseAddress * = 0;

        /// \brief
        ///     Returns the validator used by this database.
        /// \details
        ///     Can be safely obtained on both open and closed databases.
        /// \return
        ///     The validator used by this database.
        virtual auto    validator() const
                            -> IValidator * = 0;

        /// \brief
        ///     Checks whether this database is open or closed.
        /// \details
        ///     All database instances start off as open.
        /// \return
        ///     True if this database is open, false if closed.
        virtual bool    isOpen() const = 0;

        /// \brief
        ///     Closes this database; has no effect if already closed.
        /// \exception DatabaseException
        ///     If an error occurs.
        virtual void    close() = 0;

        //////////
        //  Operations (associations)
    public:
        /// \brief
        ///     Returns the set of all users in this database.
        /// \return
        ///     The set of all users in this database.
        /// \exception DatabaseException
        ///     If an error occurs.
        virtual auto    users(
                            ) const -> Users = 0;

        /// \brief
        ///     Returns the set of all accounts of all users in this database.
        /// \return
        ///     The set of all accounts of all users in this database.
        /// \exception DatabaseException
        ///     If an error occurs.
        virtual auto    accounts(
                            ) const -> Accounts = 0;

        /// \brief
        ///     Finds the account with the specified login.
        /// \return
        ///     The account with the specified login; nullptr
        ///     if not found.
        /// \exception DatabaseException
        ///     If an error occurs.
        virtual auto    findAccount(
                                const QString & login
                            ) const -> IAccount * = 0;

        /// \brief
        ///     Returns the set of all activity types in this database.
        /// \return
        ///     The set of all activity types in this database.
        /// \exception DatabaseException
        ///     If an error occurs.
        virtual auto    activityTypes(
                            ) const -> ActivityTypes = 0;

        /// \brief
        ///     Returns the set of all public activities in this
        ///     database which are not also tasks.
        /// \return
        ///     The set of all public activities in this
        ///     database which are not also tasks.
        /// \exception DatabaseException
        ///     If an error occurs.
        virtual auto    publicActivities(
                            ) const -> PublicActivities = 0;

        /// \brief
        ///     The set of all public activities in this database
        ///     including those which are also tasks.
        /// \return
        ///     The set of all public activities in this database
        ///     including those which are also tasks.
        /// \exception DatabaseException
        ///     If an error occurs.
        virtual auto    publicActivitiesAndTasks(
                            ) const
                            -> PublicActivities = 0;

        /// \brief
        ///     Returns the set of all public tasks in this database.
        /// \return
        ///     The set of all public tasks in this database.
        /// \exception DatabaseException
        ///     If an error occurs.
        virtual auto    publicTasks(
                            ) const -> PublicTasks = 0;

        /// \brief
        ///     Returns the set of root public tasks in this database.
        /// \return
        ///     The set of root public tasks in this database.
        /// \exception DatabaseException
        ///     If an error occurs.
        virtual auto    rootPublicTasks(
                            ) const -> PublicTasks = 0;

        /// \brief
        ///     Returns the set of all Projects in this database.
        /// \return
        ///     The set of all Projects in this database.
        /// \exception DatabaseException
        ///     If an error occurs.
        virtual auto    projects(
                            ) const -> Projects = 0;

        /// \brief
        ///     Returns the set of root Projects in this database.
        /// \return
        ///     The set of root Projects in this database.
        /// \exception DatabaseException
        ///     If an error occurs.
        virtual auto    rootProjects(
                            ) const -> Projects = 0;

        /// \brief
        ///     Returns the set of all WorkStreams in this database.
        /// \return
        ///     The set of all WorkStreams in this database.
        /// \exception DatabaseException
        ///     If an error occurs.
        virtual auto    workStreams(
                            ) const -> WorkStreams = 0;

        /// \brief
        ///     The set of Beneficiaries in this database.
        /// \return
        ///     The set of Beneficiaries in this database.
        /// \exception DatabaseException
        ///     If an error occurs.
        virtual auto    beneficiaries(
                            ) const -> Beneficiaries = 0;

        //////////
        //  Operations (access control)
    public:
        /// \brief
        ///     Finds an active account with the specified login/password.
        /// \details
        ///     If there exists an a) enabled account b) of an
        ///     enabled user c) with the spcified login and
        ///     password, returns it; otherwise returns nullptr.
        /// \param login
        ///     The account's login identifier.
        /// \param password
        ///     The account's password.
        /// \return
        ///     The enabled account of the enabled user, with login and
        ///     password matching the login parameters or nullptr if not
        ///     found or disabled.
        /// \exception DatabaseException
        ///     If an error occurs.
        virtual auto    tryLogin(
                                const QString & login,
                                const QString & password
                            ) const -> IAccount * = 0;

        /// \brief
        ///     Finds an active account with the specified login/password.
        /// \details
        ///     If there exists an a) enabled account b) of an
        ///     enabled user c) with the spcified login and
        ///     password, returns it; otherwise returns nullptr.
        /// \param login
        ///     The account's login identifier.
        /// \param password
        ///     The account's password.
        /// \return
        ///     The enabled account of the enabled user, with login and
        ///     password matching the login parameters; never nullptr.
        /// \exception AccessDeniedException
        ///     If the specified account is absent or disabled.
        /// \exception DatabaseException
        ///     If an error occurs.
        virtual auto    login(
                                const QString & login,
                                const QString & password
                            ) const -> IAccount * = 0;

        //////////
        //  Operations (life cycle)
    public:
        /// \brief
        ///     Creates a new User in this database.
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
        /// \exception DatabaseException
        ///     If an error occurs.
        virtual auto    createUser(
                                bool enabled,
                                const QStringList & emailAddresses,
                                const QString & realName,
                                const tt3::db::api::InactivityTimeout & inactivityTimeout,
                                const tt3::db::api::UiLocale & uiLocale
                            ) -> IUser * = 0;

        /// \brief
        ///     Creates a new ActivityType in this database.
        /// \param displayName
        ///     The short (1 line) user-readable display name
        ///     for the new ActivityType.
        /// \param description
        ///     The multi-line user-readable description for the new
        ///     ActivityType; with lines separated by a newline '\n' character.
        /// \return
        ///     The newly created ActivityType.
        /// \exception DatabaseException
        ///     If an error occurs.
        virtual auto    createActivityType(
                                const QString & displayName,
                                const QString & description
                            ) -> IActivityType * = 0;

        /// \brief
        ///     Creates a new PublicActivity in this database.
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
        /// \param requireCommentOnFinish
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
        /// \exception DatabaseException
        ///     If an error occurs.
        virtual auto    createPublicActivity(
                                const QString & displayName,
                                const QString & description,
                                const InactivityTimeout & timeout,
                                bool requireCommentOnStart,
                                bool requireCommentOnFinish,
                                bool fullScreenReminder,
                                IActivityType * activityType,
                                IWorkload * workload
                            ) -> IPublicActivity * = 0;

        /// \brief
        ///     Creates a new root PublicTask in this database
        ///     (that is, a PublicTask that has no parent).
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
        /// \param requireCommentOnFinish
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
        /// \exception DatabaseException
        ///     If an error occurs.
        virtual auto    createPublicTask(
                                const QString & displayName,
                                const QString & description,
                                const InactivityTimeout & timeout,
                                bool requireCommentOnStart,
                                bool requireCommentOnFinish,
                                bool fullScreenReminder,
                                IActivityType * activityType,
                                IWorkload * workload,
                                bool completed,
                                bool requireCommentOnCompletion
                            ) -> IPublicTask * = 0;

        /// \brief
        ///     Creates a new root Project in this database
        ///     (that is, a Project that has no parent).
        /// \param displayName
        ///     The short (1 line) user-readable display name
        ///     for the new Project.
        /// \param description
        ///     The multi-line user-readable description for the new
        ///     Project; with lines separated by a newline '\\n' character.
        /// \param beneficiaries
        ///     The set of Beneficiaries to associate with the newly
        ///     created Project (can be empty).
        /// \return
        ///     The newly created Project.
        /// \exception DatabaseException
        ///     If an error occurs.
        virtual auto    createProject(
                                const QString & displayName,
                                const QString & description,
                                const Beneficiaries & beneficiaries
                            ) -> IProject * = 0;

        /// \brief
        ///     Creates a new work stream in this database.
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
        /// \exception DatabaseException
        ///     If an error occurs.
        virtual auto    createWorkStream(
                                const QString & displayName,
                                const QString & description,
                                const Beneficiaries & beneficiaries
                            ) -> IWorkStream * = 0;

        /// \brief
        ///     Creates a new Beneficiary in this database.
        /// \param displayName
        ///     The short (1 line) user-readable display name
        ///     for the new Beneficiary.
        /// \param description
        ///     The multi-line user-readable description for the new
        ///     Beneficiary; with lines separated by a newline '\\n' character.
        /// \return
        ///     The newly created Beneficiary.
        /// \exception DatabaseException
        ///     If an error occurs.
        virtual auto    createBeneficiary(
                                const QString & displayName,
                                const QString & description
                            ) -> IBeneficiary * = 0;

        //////////
        //  Operations (change notification handling)
    public:
        /// \brief
        ///     Returns the change notifier used by this database.
        /// \details
        ///     Note, that an implementation may choose to use a dedicated
        ///     change notifier for every Database, or a shared change
        ///     notifier instance shared by all Databases - a change notification
        ///     always carries with it an indication of which Database the change
        ///     occurred in.
        /// \return
        ///     The change notifier used by this database.
        virtual auto    changeNotifier(
                            ) -> ChangeNotifier * = 0;
    };
}

//  End of tt3-db-api/Database.hpp
