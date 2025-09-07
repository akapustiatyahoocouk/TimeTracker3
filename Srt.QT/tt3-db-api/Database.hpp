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
        //  Returns the type of this database.
        //  Can be safely obtained on both open and closed databases.
        virtual auto    type() const
                            -> IDatabaseType * = 0;

        //  Returns the address of this database.
        //  Can be safely obtained on both open and closed databases.
        virtual auto    address() const
                            -> IDatabaseAddress * = 0;

        //  Returns the validator used by this database.
        virtual auto    validator() const
                            -> IValidator * = 0;

        //  Checks whether this database is open or closed.
        //  All database instances start off as open.
        virtual bool    isOpen() const = 0;

        //  Closes this database; jas no effect if already closed.
        //  Throws DatabaseException if an error occurs.
        virtual void    close()
                            throws(DatabaseException) = 0;

        //////////
        //  Operations (associations)
    public:
        //  The set of all users in this database.
        //  Throws DatabaseException if an error occurs.
        virtual auto    users(
                            ) const
                            throws(DatabaseException)
                            -> Users = 0;

        //  The set of all accounts of all users in this database.
        //  Throws DatabaseException if an error occurs.
        virtual auto    accounts(
                            ) const
                            throws(DatabaseException)
                            -> Accounts = 0;

        //  The account with the specified login.
        //  Throws DatabaseException if an error occurs.
        virtual auto    findAccount(
                                const QString & login
                            ) const
                            throws(DatabaseException)
                            -> IAccount * = 0;

        //  The set of all activity types in this database.
        //  Throws DatabaseException if an error occurs.
        virtual auto    activityTypes(
                            ) const
                            throws(DatabaseException)
                            -> ActivityTypes = 0;

        //  The set of all public activities in this database
        //  which are not also tasks.
        //  Throws DatabaseException if an error occurs.
        virtual auto    publicActivities(
                            ) const
                            throws(DatabaseException)
                            -> PublicActivities = 0;

        //  The set of all public activities in this database
        //  which are also tasks.
        //  Throws DatabaseException if an error occurs.
        virtual auto    publicActivitiesAndTasks(
                            ) const
                            throws(DatabaseException)
                            -> PublicActivities = 0;

        //  The set of all/root public tasks in this database.
        //  Throws DatabaseException if an error occurs.
        virtual auto    publicTasks(
                            ) const
                            throws(DatabaseException)
                            -> PublicTasks = 0;
        virtual auto    rootPublicTasks(
                            ) const
                            throws(DatabaseException)
                            -> PublicTasks = 0;

        //  The set of all/root Projects in this database.
        //  Throws DatabaseException if an error occurs.
        virtual auto    projects(
                            ) const
                            throws(DatabaseException)
                            -> Projects = 0;
        virtual auto    rootProjects(
                            ) const
                            throws(DatabaseException)
                            -> Projects = 0;

        //  The set of all WorkStreams in this database.
        //  Throws DatabaseException if an error occurs.
        virtual auto    workStreams(
                            ) const
                            throws(DatabaseException)
                            -> WorkStreams = 0;

        //  The set of Beneficiaries in this database.
        //  Throws DatabaseException if an error occurs.
        virtual auto    beneficiaries(
                            ) const
                            throws(DatabaseException)
                            -> Beneficiaries = 0;

        //////////
        //  Operations (access control)
    public:
        //  If there exists an a) enabled account b) of an
        //  enabled user c) with the spcified login and
        //  password, returns it; otherwise returns nullptr.
        virtual auto    tryLogin(
                                const QString & login,
                                const QString & password
                            ) const
                            throws(DatabaseException)
                            -> IAccount * = 0;

        //  If there exists an a) enabled account b) of an
        //  enabled user c) with the spcified login and
        //  password, returns it; otherwise throws an
        //  AccessDeniedException.
        virtual auto    login(
                                const QString & login,
                                const QString & password
                            ) const
                            throws(DatabaseException)
                        -> IAccount * = 0;

        //////////
        //  Operations (life cycle)
    public:
        //  Creates a new User in this database.
        //  Throws DatabaseException if an error occurs.
        virtual auto    createUser(
                                bool enabled,
                                const QStringList & emailAddresses,
                                const QString & realName,
                                const tt3::db::api::InactivityTimeout & inactivityTimeout,
                                const tt3::db::api::UiLocale & uiLocale
                            )
                            throws(DatabaseException)
                            -> IUser * = 0;

        //  Creates a new ActivityType in this database.
        //  Throws DatabaseException if an error occurs.
        virtual auto    createActivityType(
                                const QString & displayName,
                                const QString & description
                            )
                            throws(DatabaseException)
                            -> IActivityType * = 0;

        //  Creates a new PublicActivity in this database.
        //  The activity type can be nullptr.
        //  Throws DatabaseException if an error occurs.
        virtual auto    createPublicActivity(
                                const QString & displayName,
                                const QString & description,
                                const InactivityTimeout & timeout,
                                bool requireCommentOnStart,
                                bool requireCommentOnFinish,
                                bool fullScreenReminder,
                                IActivityType * activityType,
                                IWorkload * workload
                            )
                            throws(DatabaseException)
                            -> IPublicActivity * = 0;

        //  Creates a new root PublicTask in this database.
        //  The activity type can be nullptr.
        //  Throws DatabaseException if an error occurs.
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
                            )
                            throws(DatabaseException)
                            -> IPublicTask * = 0;

        //  Creates a new root project in this database.
        //  Throws DatabaseException if an error occurs.
        virtual auto    createProject(
                                const QString & displayName,
                                const QString & description,
                                const Beneficiaries & beneficiaries
                            )
                            throws(DatabaseException)
                            -> IProject * = 0;

        //  Creates a new work stream in this database.
        //  Throws DatabaseException if an error occurs.
        virtual auto    createWorkStream(
                                const QString & displayName,
                                const QString & description,
                                const Beneficiaries & beneficiaries
                            )
                            throws(DatabaseException)
                            -> IWorkStream * = 0;

        //  Creates a new Beneficiary in this database.
        //  Throws DatabaseException if an error occurs.
        virtual auto    createBeneficiary(
                                const QString & displayName,
                                const QString & description
                            )
                            throws(DatabaseException)
                            -> IBeneficiary * = 0;

        //////////
        //  Operations (change notification handling)
    public:
        //  The change notifier used by this database
        virtual auto    changeNotifier(
                            ) -> ChangeNotifier * = 0;
    };
}

//  End of tt3-db-api/Database.hpp
