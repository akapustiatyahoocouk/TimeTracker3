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
    /// \class IUser tt3-db-api/API.hpp
    /// \brief A user.
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
        /// \brief
        ///     Returns the "real name" of this User.
        /// \return
        ///     The "real name" of this User.
        /// \exception DatabaseException
        ///     If an error occurs.
        virtual QString realName(
                            ) const = 0;

        /// \brief
        ///     sETS the "real name" of this User.
        /// \param realName
        ///     The new "real name" for this User.
        /// \exception DatabaseException
        ///     If an error occurs.
        virtual void    setRealName(
                                const QString & realName
                            ) = 0;

        /// \brief
        ///     Returns the "inactivity timeout" of this User.
        /// \details
        //      This is the timeout after which, if the user does nothing,
        //      the "current" Activity is automatically stopped.
        /// \return
        ///     The "inactivity timeout" of this User; absent == none.
        /// \exception DatabaseException
        ///     If an error occurs.
        virtual auto    inactivityTimeout(
                            ) const -> InactivityTimeout = 0;

        /// \brief
        ///     Sets the "inactivity timeout" of this User.
        /// \details
        //      This is the timeout after which, if the user does nothing,
        //      the "current" Activity is automatically stopped.
        /// \param inactivityTimeout
        ///     The new "inactivity timeout" for this User; absent == none.
        /// \exception DatabaseException
        ///     If an error occurs.
        virtual void    setInactivityTimeout(
                                const InactivityTimeout & inactivityTimeout
                            ) = 0;

        /// \brief
        ///     Returns the preferred UI locale  of this User.
        /// \return
        ///     The preferred UI locale  of this User; absent == default.
        /// \exception DatabaseException
        ///     If an error occurs.
        virtual auto    uiLocale(
                            ) const -> UiLocale = 0;

        /// \brief
        ///     Sets the preferred UI locale  of this User.
        /// \param uiLocale
        ///     The new preferred UI locale  for this User; absent == default.
        /// \exception DatabaseException
        ///     If an error occurs.
        virtual void    setUiLocale(
                                const UiLocale & uiLocale
                            ) = 0;

        //////////
        //  Operations (associations)
    public:
        /// \brief
        ///     Returns the set of all Accounts of this User.
        /// \return
        ///     The set of all Accounts of this User.
        /// \exception DatabaseException
        ///     If an error occurs.
        virtual auto    accounts(
                            ) const -> Accounts = 0;

        /// \brief
        ///     Returns the set of all private activities of this User
        ///     which are NOT also tasks.
        /// \return
        ///     The set of all private activities of this User
        ///     which are NOT also tasks.
        /// \exception DatabaseException
        ///     If an error occurs.
        virtual auto    privateActivities(
                            ) const -> PrivateActivities = 0;

        /// \brief
        ///     Returns the set of all private activities of this User,
        ///     including those which are also tasks.
        /// \return
        ///     The set of all private activities of this User,
        ///     including those which are also tasks.
        /// \exception DatabaseException
        ///     If an error occurs.
        virtual auto    privateActivitiesAndTasks(
                            ) const -> PrivateActivities = 0;

        /// \brief
        ///     Returns the set of all private tasks of this User.
        /// \return
        ///     The set of all private tasks of this User.
        /// \exception DatabaseException
        ///     If an error occurs.
        virtual auto    privateTasks(
                            ) const -> PrivateTasks = 0;

        /// \brief
        ///     Returns the set of root private tasks of this User.
        /// \return
        ///     The set of root private tasks of this User.
        /// \exception DatabaseException
        ///     If an error occurs.
        virtual auto    rootPrivateTasks(
                            ) const -> PrivateTasks = 0;

        /// \brief
        ///     Returns the set of all Workload permitted for
        ///     this User.
        /// \return
        ///     The set of all Workload permitted for
        ///     this User.
        /// \exception DatabaseException
        ///     If an error occurs.
        virtual auto    permittedWorkloads(
                            ) const -> Workloads = 0;

        /// \brief
        ///     Sets the set of all Workload permitted for
        ///     this User.
        /// \param workloads
        ///     The new set of all Workload permitted for
        ///     this User.
        /// \exception DatabaseException
        ///     If an error occurs.
        virtual void    setPermittedWorkloads(
                                const Workloads & workloads
                            ) = 0;

        /// \brief
        ///     Adds the specified Workload to the
        ///     set of Workloads permitted to this User.
        /// \details
        ///     The call has no effect if already there.
        /// \param workload
        ///     The workload to add.
        /// \exception DatabaseException
        ///     If an error occurs.
        virtual void    addPermittedWorkload(
                                IWorkload * workload
                            ) = 0;

        /// \brief
        ///     Removes the specified Workload from the
        ///     set of Workloads permitted to this User.
        /// \details
        ///     The call has no effect if already not there.
        /// \param workload
        ///     The workload to remove.
        /// \exception DatabaseException
        ///     If an error occurs.
        virtual void    removePermittedWorkload(
                                IWorkload * workload
                            ) = 0;

        /// \brief
        ///     Returns the set of all Works logged by this User.
        /// \return
        ///     The set of all Works logged by this User.
        /// \exception DatabaseException
        ///     If an error occurs.
        virtual auto    works(
                            ) const -> Works = 0;

        /// \brief
        ///     Returns the set of all Events logged by this User.
        /// \return
        ///     The set of all Events logged by this User.
        /// \exception DatabaseException
        ///     If an error occurs.
        virtual auto    events(
                            ) const -> Events = 0;

        //////////
        //  Operations (life cycle)
    public:
        /// \brief
        ///     Creates a new Account for this User.
        /// \param enabled
        ///     True if the new Account shall be created as "enabled",
        ///     false for "disabled".
        /// \param emailAddresses
        ///     The list of e-mail addresses for the new Account.
        /// \param login
        ///     The login identifier for the new Account.
        ///     All Accounts in a database must have different lohins.
        /// \param password
        ///     The password for the new Account.
        /// \param capabilities
        ///     The capabilities for the new Account.
        /// \exception DatabaseException
        ///     If an error occurs.
        virtual auto    createAccount(
                                bool enabled,
                                const QStringList & emailAddresses,
                                const QString & login,
                                const QString & password,
                                Capabilities capabilities
                            ) -> IAccount * = 0;

        /// \brief
        ///     Creates a new PrivateActivity for this User.
        /// \param displayName
        ///     The short (1 line) user-readable display name
        ///     for the new PrivateActivity.
        /// \param description
        ///     The multi-line user-readable description for the new
        ///     PrivateActivity; with lines separated by a newline '\\n' character.
        /// \param timeout
        ///     The user-does-nothing timeout for the new PrivateActivity;
        ///     absent == none.
        /// \param requireCommentOnStart
        ///     True if the newly created PrivateActivity small require the
        ///     user to enter a comment when it is started.
        /// \param requireCommentOnFinish
        ///     True if the newly created PrivateActivity small require the
        ///     user to enter a comment when it is started.
        /// \param fullScreenReminder
        ///     True if a full-screen reminder shall be displayed while the
        ///     newly created PrivateActivity is underway.
        /// \param activityType
        ///     The type for the new PrivateActivity; nullptr == don't assign.
        /// \param workload
        ///     The Workload for the new PrivateActivity; nullptr == don't assign.
        /// \return
        ///     The newly created PrivateActivity.
        /// \exception DatabaseException
        ///     If an error occurs.
        virtual auto    createPrivateActivity(
                                const QString & displayName,
                                const QString & description,
                                const InactivityTimeout & timeout,
                                bool requireCommentOnStart,
                                bool requireCommentOnFinish,
                                bool fullScreenReminder,
                                IActivityType * activityType,
                                tt3::db::api::IWorkload * workload
                            ) -> IPrivateActivity * = 0;

        /// \brief
        ///     Creates a new root PrivateTask in this database
        ///     (that is, a PrivateTask that has no parent).
        /// \param displayName
        ///     The short (1 line) user-readable display name
        ///     for the new PrivateTask.
        /// \param description
        ///     The multi-line user-readable description for the new
        ///     PrivateTask; with lines separated by a newline '\\n' character.
        /// \param timeout
        ///     The user-does-nothing timeout for the new PrivateTask;
        ///     absent == none.
        /// \param requireCommentOnStart
        ///     True if the newly created PrivateTask small require the
        ///     user to enter a comment when it is started.
        /// \param requireCommentOnFinish
        ///     True if the newly created PrivateTask small require the
        ///     user to enter a comment when it is started.
        /// \param fullScreenReminder
        ///     True if a full-screen reminder shall be displayed while the
        ///     newly created PrivateTask is underway.
        /// \param activityType
        ///     The type for the new PrivateTask; nullptr == don't assign.
        /// \param workload
        ///     The Workload for the new PrivateTask; nullptr == don't assign.
        /// \param completed
        ///     True if the PrivateTask shall initially be marked as
        ///     "completed", false if not.
        /// \param requireCommentOnCompletion
        ///     True if the newly created PrivateTask small require the
        ///     user to enter a comment when marking it as "completed".
        /// \return
        ///     The newly created PrivateTask.
        /// \exception DatabaseException
        ///     If an error occurs.
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
                            ) -> IPrivateTask * = 0;

        /// \brief
        ///     Creates a new unit of Work for this User against
        ///     the specified activity.
        /// \param startedAt
        ///     The UTC date+time when this unit of work has started.
        /// \param finishedAt
        ///     The UTC date+time when this unit of work has finished.
        /// \param activity
        ///     The activity associated with this unit of work;
        ///     cann0t be nullptr.
        /// \exception DatabaseException
        ///     If an error occurs.
        virtual auto    createWork(
                                const QDateTime & startedAt,
                                const QDateTime & finishedAt,
                                IActivity * activity
                            ) -> IWork * = 0;

        /// \brief
        ///     Creates a new Event for this User against
        ///     the specified activity.
        /// \param occurredAt
        ///     TheUTC date+time when the event has occurred.
        /// \param summary
        ///     The brief 1-line summary for the new Event.
        /// \param activity
        ///     The activity associated with the new Event; nullptr == none.
        /// \exception DatabaseException
        ///     If an error occurs.
        virtual auto    createEvent(
                                const QDateTime & occurredAt,
                                const QString & summary,
                                IActivity * activity
                            ) -> IEvent * = 0;
    };
}

//  End of tt3-db-api/User.hpp
