//
//  tt3-ws/User.hpp - A named user
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
    /// \class UserImpl tt3-ws/API.hpp
    /// \brief A named user.
    class TT3_WS_PUBLIC UserImpl final :
        public PrincipalImpl
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(UserImpl)

        friend class WorkspaceImpl;
        friend std::shared_ptr<UserImpl>;
        friend class WorkloadImpl;

        //////////
        //  Construction/destruction - from friends only
    private:
        UserImpl(Workspace workspace, tt3::db::api::IUser * dataUser);
        virtual ~UserImpl();

        //////////
        //  Operations (properties)
    public:
        /// \brief
        ///     Returns the "real name" of this User.
        /// \param credentials
        ///     The credentials of the service caller.
        /// \return
        ///     The "real name" of this User.
        /// \exception WorkspaceException
        ///     If an error occurs.
        QString     realName(
                            const Credentials & credentials
                        ) const;

        /// \brief
        ///     Sets the "real name" of this User.
        /// \param credentials
        ///     The credentials of the service caller.
        /// \param realName
        ///     The new "real name" for this User.
        /// \exception WorkspaceException
        ///     If an error occurs.
        void        setRealName(
                            const Credentials & credentials,
                            const QString & realName
                        );

        /// \brief
        ///     Returns the "inactivity timeout" of this User.
        /// \details
        ///     This is the timeout after which, if the user does nothing,
        ///     the "current" Activity is automatically stopped.
        /// \param credentials
        ///     The credentials of the service caller.
        /// \return
        ///     The "inactivity timeout" of this User; absent == none.
        /// \exception WorkspaceException
        ///     If an error occurs.
        auto        inactivityTimeout(
                            const Credentials & credentials
                        ) const -> InactivityTimeout;

        /// \brief
        ///     Sets the "inactivity timeout" of this User.
        /// \details
        ///     This is the timeout after which, if the user does nothing,
        ///     the "current" Activity is automatically stopped.
        /// \param credentials
        ///     The credentials of the service caller.
        /// \param inactivityTimeout
        ///     The new "inactivity timeout" for this User; absent == none.
        /// \exception WorkspaceException
        ///     If an error occurs.
        void        setInactivityTimeout(
                            const Credentials & credentials,
                            const InactivityTimeout & inactivityTimeout
                        );

        /// \brief
        ///     Returns the preferred UI locale  of this User.
        /// \param credentials
        ///     The credentials of the service caller.
        /// \return
        ///     The preferred UI locale  of this User; absent == use system.
        /// \exception WorkspaceException
        ///     If an error occurs.
        auto        uiLocale(
                            const Credentials & credentials
                        ) const -> UiLocale;

        /// \brief
        ///     Sets the preferred UI locale  of this User.
        /// \param credentials
        ///     The credentials of the service caller.
        /// \param uiLocale
        ///     The new preferred UI locale  of this User; absent == use system.
        /// \exception WorkspaceException
        ///     If an error occurs.
        void        setUiLocale(
                            const Credentials & credentials,
                            const UiLocale & uiLocale
                        );

        //////////
        //  Operations (associations)
    public:
        /// \brief
        ///     Returns the set of all Accounts of this User.
        /// \param credentials
        ///     The credentials of the service caller.
        /// \return
        ///     The set of all Accounts of this User.
        /// \exception WorkspaceException
        ///     If an error occurs.
        auto        accounts(
                            const Credentials & credentials
                        ) const -> Accounts;

        /// \brief
        ///     Returns the set of all private activities of this User
        ///     which are NOT also tasks.
        /// \param credentials
        ///     The credentials of the service caller.
        /// \return
        ///     The set of all private activities of this User
        ///     which are NOT also tasks.
        /// \exception WorkspaceException
        ///     If an error occurs.
        auto        privateActivities(
                            const Credentials & credentials
                        ) const -> PrivateActivities;

        /// \brief
        ///     Returns the set of all private activities of this User,
        ///     including those which are also tasks.
        /// \param credentials
        ///     The credentials of the service caller.
        /// \return
        ///     The set of all private activities of this User,
        ///     including those which are also tasks.
        /// \exception WorkspaceException
        ///     If an error occurs.
        auto        privateActivitiesAndTasks(
                            const Credentials & credentials
                        ) const -> PrivateActivities;

        /// \brief
        ///     Returns the set of all private tasks of this User.
        /// \param credentials
        ///     The credentials of the service caller.
        /// \return
        ///     The set of all private tasks of this User.
        /// \exception WorkspaceException
        ///     If an error occurs.
        auto        privateTasks(
                            const Credentials & credentials
                        ) const -> PrivateTasks;

        /// \brief
        ///     Returns the set of root private tasks of this User.
        /// \param credentials
        ///     The credentials of the service caller.
        /// \return
        ///     The set of root private tasks of this User.
        /// \exception WorkspaceException
        ///     If an error occurs.
        auto        rootPrivateTasks(
                            const Credentials & credentials
                        ) const -> PrivateTasks;

        /// \brief
        ///     Returns the set of all Workload permitted for
        ///     this User.
        /// \param credentials
        ///     The credentials of the service caller.
        /// \return
        ///     The set of all Workload permitted for
        ///     this User.
        /// \exception WorkspaceException
        ///     If an error occurs.
        auto        permittedWorkloads(
                            const Credentials & credentials
                        ) const -> Workloads;

        /// \brief
        ///     Sets the set of all Workload permitted for
        ///     this User.
        /// \param credentials
        ///     The credentials of the service caller.
        /// \param workloads
        ///     The new set of all Workload permitted for
        ///     this User.
        /// \exception WorkspaceException
        ///     If an error occurs.
        void        setPermittedWorkloads(
                            const Credentials & credentials,
                            const Workloads & workloads
                        );

        /// \brief
        ///     Adds the specified Workload to the
        ///     set of Workloads permitted to this User.
        /// \details
        ///     The call has no effect if already there.
        /// \param credentials
        ///     The credentials of the service caller.
        /// \param workload
        ///     The workload to add.
        /// \exception WorkspaceException
        ///     If an error occurs.
        void        addPermittedWorkload(
                            const Credentials & credentials,
                            Workload workload
                        );

        /// \brief
        ///     Removes the specified Workload from the
        ///     set of Workloads permitted to this User.
        /// \details
        ///     The call has no effect if already not there.
        /// \param credentials
        ///     The credentials of the service caller.
        /// \param workload
        ///     The workload to remove.
        /// \exception WorkspaceException
        ///     If an error occurs.
        void        removePermittedWorkload(
                            const Credentials & credentials,
                            Workload workload
                        );

        //////////
        //  Operations (life cycle)
    public:
        /// \brief
        ///     Creates a new Account for this User.
        /// \param credentials
        ///     The credentials of the service caller.
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
        /// \return
        ///     The newly created Account.
        /// \exception WorkspaceException
        ///     If an error occurs.
        auto        createAccount(
                            const Credentials & credentials,
                            bool enabled,
                            const QStringList & emailAddresses,
                            const QString & login,
                            const QString & password,
                            Capabilities capabilities
                        ) -> Account;

        /// \brief
        ///     Creates a new PrivateActivity for this User.
        /// \param credentials
        ///     The credentials of the service caller.
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
        /// \param requireCommentOnStop
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
        /// \exception WorkspaceException
        ///     If an error occurs.
        auto        createPrivateActivity(
                            const Credentials & credentials,
                            const QString & displayName,
                            const QString & description,
                            const InactivityTimeout & timeout,
                            bool requireCommentOnStart,
                            bool requireCommentOnStop,
                            bool fullScreenReminder,
                            ActivityType activityType,
                            Workload workload
                        ) -> PrivateActivity;

        /// \brief
        ///     Creates a new root PrivateTask in this database
        ///     (that is, a PrivateTask that has no parent).
        /// \param credentials
        ///     The credentials of the service caller.
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
        /// \param requireCommentOnStop
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
        /// \exception WorkspaceException
        ///     If an error occurs.
        auto        createPrivateTask(
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
                        ) -> PrivateTask;

        //////////
        //  Implementation
    private:
        tt3::db::api::IUser *const  _dataUser;    //  counts as "reference"

        //  Access control
        virtual bool    _canRead(       //  throws WorkspaceException
                                const Credentials & credentials
                            ) const override;
        virtual bool    _canModify(     //  throws WorkspaceException
                                const Credentials & credentials
                            ) const override;
        virtual bool    _canDestroy(    //  throws WorkspaceException
                                const Credentials & credentials
                            ) const override;
        virtual bool    _destroyingLosesAccess(
                            ) const override;
    };
}

//  Enable objects and object pointers for QVariant
Q_DECLARE_METATYPE(tt3::ws::UserImpl)
Q_DECLARE_METATYPE(tt3::ws::User)

//  End of tt3-ws/User.hpp
