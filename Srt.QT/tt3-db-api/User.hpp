//
//  tt3-db-api/User.hpp - a user
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
    //  A user
    class TT3_DB_API_PUBLIC IUser : public virtual IPrincipal
    {
        //////////
        //  This is an interface
    protected:
        IUser() = default;
        virtual ~IUser() = default;

        //////////
        //  IObject
    public:
        virtual auto    type() const
                            -> IObjectType * override
        {
            return ObjectTypes::User::instance();
        }

        //////////
        //  Operations (properties)
    public:
        //  Returns/sets the "real name" of this User.
        //  Throws DatabaseException if an error occurs.
        virtual QString realName() const
                            throws(DatabaseException) = 0;
        virtual void    setRealName(
                                const QString & realName
                            )
                            throws(DatabaseException) = 0;

        //  Returns/sets the "inactivity timeout" of this User.
        //  This is the timeout after which, if the user does nothing,
        //  the "current" Activity is automatically stopped.
        //  Throws DatabaseException if an error occurs.
        virtual auto    inactivityTimeout(
                            ) const
                            throws(DatabaseException)
                            -> InactivityTimeout = 0;
        virtual void    setInactivityTimeout(
                                const InactivityTimeout & inactivityTimeout
                            )
                            throws(DatabaseException) = 0;

        //  Returns/sets the preferred UI locale  of this User.
        //  Throws DatabaseException if an error occurs.
        virtual auto    uiLocale(
                            ) const
                            throws(DatabaseException)
                            -> UiLocale = 0;
        virtual void    setUiLocale(
                                const UiLocale & uiLocale
                            )
                            throws(DatabaseException) = 0;

        //////////
        //  Operations (associations)
    public:
        //  Returns the set of all Accounts of this User.
        //  Throws DatabaseException if an error occurs.
        virtual auto    accounts(
                            ) const
                            throws(DatabaseException)
                            -> Accounts = 0;

        //  The set of all private activities of this User
        //  which are not also tasks.
        //  Throws DatabaseException if an error occurs.
        virtual auto    privateActivities(
                            ) const
                            throws(DatabaseException)
                            -> PrivateActivities = 0;

        //  The set of all private activities of this User
        //  which are also tasks.
        //  Throws DatabaseException if an error occurs.
        virtual auto    privateActivitiesAndTasks(
                            ) const
                            throws(DatabaseException)
                            -> PrivateActivities = 0;

        //  The set of all/root private tasks of this User.
        //  Throws DatabaseException if an error occurs.
        virtual auto    privateTasks(
                            ) const
                            throws(DatabaseException)
                            -> PrivateTasks = 0;
        virtual auto    rootPrivateTasks(
                            ) const
                            throws(DatabaseException)
                            -> PrivateTasks = 0;

        //  Returns/sets the set of all Workload permitted for
        //  this User.
        //  Throws DatabaseException if an error occurs.
        virtual auto    permittedWorkloads(
                            ) const
                            throws(DatabaseException)
                            -> Workloads = 0;
        virtual void    setPermittedWorkloads(
                                const Workloads & workloads
                            )
                            throws(DatabaseException) = 0;

        //  Adds/removes the specified Workload to/from the
        //  set of Workloads permitted to this User;
        //  has no effect if already there/not there.
        //  Throws DatabaseException if an error occurs.
        virtual void    addPermittedWorkload(
                                IWorkload * workload
                            )
                            throws(DatabaseException) = 0;
        virtual void    removePermittedWorkload(
                                IWorkload * workload
                            )
                            throws(DatabaseException) = 0;

        //  Returns the set of all Works logged by this User.
        //  Throws DatabaseException if an error occurs.
        virtual auto    works(
                            ) const
                            throws(DatabaseException)
                            -> Works = 0;

        //  Returns the set of all Events logged by this User.
        //  Throws DatabaseException if an error occurs.
        virtual auto    events(
                            ) const
                            throws(DatabaseException)
                            -> Events = 0;

        //////////
        //  Operations (life cycle)
    public:
        //  Creates a new Account for this User.
        //  All Accounts in a database must have different lohins.
        //  Throws DatabaseException if an error occurs.
        virtual auto    createAccount(
                                bool enabled,
                                const QStringList & emailAddresses,
                                const QString & login,
                                const QString & password,
                                Capabilities capabilities
                            )
                            throws(DatabaseException)
                            -> IAccount * = 0;

        //  Creates a new PrivateActivity for this User.
        //  The activity type can be nullptr.
        //  Throws DatabaseException if an error occurs.
        virtual auto    createPrivateActivity(
                                const QString & displayName,
                                const QString & description,
                                const InactivityTimeout & timeout,
                                bool requireCommentOnStart,
                                bool requireCommentOnFinish,
                                bool fullScreenReminder,
                                IActivityType * type,
                                tt3::db::api::IWorkload * workload
                            )
                            throws(DatabaseException)
                            -> IPrivateActivity * = 0;

        //  Creates a new root PrivateTask for this User.
        //  The activity type can be nullptr.
        //  Throws DatabaseException if an error occurs.
        virtual auto    createPrivateTask(
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
                            -> IPrivateTask * = 0;

        //  Creates a new unit of Work for this User against
        //  the specified activity.
        //  Throws DatabaseException if an error occurs.
        virtual auto    createWork(
                                const QDateTime & startedAt,
                                const QDateTime & finishedAt,
                                IActivity * activity
                            )
                            throws(DatabaseException)
                            -> IWork * = 0;

        //  Creates a new Event for this User against
        //  the specified activity (which can be nullptr is none).
        //  Throws DatabaseException if an error occurs.
        virtual auto    createEvent(
                                const QDateTime & occurredAt,
                                const QString & simmary,
                                IActivity * activity
                            )
                            throws(DatabaseException)
                            -> IEvent * = 0;
    };
}

//  End of tt3-db-api/User.hpp
