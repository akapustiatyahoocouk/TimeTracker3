//
//  tt3-ws/Activity.hpp - A generic activity
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
    /// \class ActivityImpl tt3-ws/API.hpp
    /// \brief A generic activity.
    class TT3_WS_PUBLIC ActivityImpl
        :   public ObjectImpl
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(ActivityImpl)

        friend class PublicActivityImpl;
        friend class PrivateActivityImpl;
        friend class TaskImpl;
        friend class PublicTaskImpl;
        friend class PrivateTaskImpl;

        //////////
        //  Construction/destruction - from friends only
    private:
        ActivityImpl(Workspace workspace, tt3::db::api::IActivity * dataActivity);
        virtual ~ActivityImpl();

        //////////
        //  Operations (properties)
    public:
        /// \brief
        ///     Returns the user-readable display name of this activity.
        /// \param credentials
        ///     The credentials of the service caller.
        /// \return
        ///     The user-readable display name of this activity.
        /// \exception WorkspaceException
        ///     If an error occurs.
        QString     displayName(
                            const Credentials & credentials
                        ) const;

        /// \brief
        ///     Sets the user-readable display name of this activity.
        /// \param credentials
        ///     The credentials of the service caller.
        /// \param displayName
        ///     The new user-readable display name for this activity.
        /// \exception WorkspaceException
        ///     If an error occurs.
        void        setDisplayName(
                            const Credentials & credentials,
                            const QString & displayName
                        );

        /// \brief
        ///     Returns the multi-line description of this activity.
        /// \param credentials
        ///     The credentials of the service caller.
        /// \return
        ///     The multi-line description of this activity,
        ///     with lines separated by a newline character '\n'.
        /// \exception WorkspaceException
        ///     If an error occurs.
        QString     description(
                            const Credentials & credentials
                        ) const;

        /// \brief
        ///     Sets the multi-line description of this activity.
        /// \param credentials
        ///     The credentials of the service caller.
        /// \param description
        ///     The new multi-line description for this activity,
        ///     with lines separated by a newline character '\n'.
        /// \exception WorkspaceException
        ///     If an error occurs.
        void        setDescription(
                            const Credentials & credentials,
                            const QString & description
                        );

        /// \brief
        ///     Returns the timeout after which the activity
        ///     stops automatically if the user does nothing at all.
        /// \param credentials
        ///     The credentials of the service caller.
        /// \return
        ///     The timeout after which the activity stops
        ///     automatically if the user does nothing at
        ///     all; absent == none.
        /// \exception WorkspaceException
        ///     If an error occurs.
        auto        timeout(
                            const Credentials & credentials
                        ) const -> InactivityTimeout;

        /// \brief
        ///     Sets the timeout after which the activity
        ///     stops automatically if the user does nothing at all.
        /// \param credentials
        ///     The credentials of the service caller.
        /// \param timeout
        ///     The new timeout after which the activity stops
        ///     automatically if the user does nothing at
        ///     all; absent == none.
        /// \exception WorkspaceException
        ///     If an error occurs.
        void        setTimeout(
                            const Credentials & credentials,
                            const InactivityTimeout & timeout
                        );

        /// \brief
        ///     Checks if user must enter a comment when starting this
        ///     activity.
        /// \param credentials
        ///     The credentials of the service caller.
        /// \return
        ///     True if user must enter a comment when starting this
        ///     activity (this comment will be recorded as an event),
        ///     false if not.
        /// \exception WorkspaceException
        ///     If an error occurs.
        bool        requireCommentOnStart(
                            const Credentials & credentials
                        ) const;

        /// \brief
        ///     Specifies whether user must enter a comment when
        ///     starting this activity.
        /// \param credentials
        ///     The credentials of the service caller.
        /// \param requireCommentOnStart
        ///     True if user must enter a comment when starting this
        ///     activity (this comment will be recorded as an event),
        ///     false if not.
        /// \exception WorkspaceException
        ///     If an error occurs.
        void        setRequireCommentOnStart(
                            const Credentials & credentials,
                            bool requireCommentOnStart
                        );

        /// \brief
        ///     Checks if user must enter a comment when stopping
        ///     this activity.
        /// \param credentials
        ///     The credentials of the service caller.
        /// \return
        ///     True if user must enter a comment when stopping this
        ///     activity (this comment will be recorded as an event),
        ///     false if not.
        /// \exception WorkspaceException
        ///     If an error occurs.
        bool        requireCommentOnStop(
                            const Credentials & credentials
                        ) const;

        /// \brief
        ///     Specifies if user must enter a comment when stopping
        ///     this activity.
        /// \param credentials
        ///     The credentials of the service caller.
        /// \param requireCommentOnStop
        ///     True if user must enter a comment when stopping this
        ///     activity (this comment will be recorded as an event),
        ///     false if not.
        /// \exception WorkspaceException
        ///     If an error occurs.
        void        setRequireCommentOnStop(
                            const Credentials & credentials,
                            bool requireCommentOnStop
                        );

        /// \brief
        ///     Checks if a full-screen reminder shall be displayed while
        ///     this activity is being performed.
        /// \param credentials
        ///     The credentials of the service caller.
        /// \return
        ///     True if a full-screen reminder shall be displayed while
        ///     this activity is being performed, else false.
        /// \exception WorkspaceException
        ///     If an error occurs.
        bool        fullScreenReminder(
                            const Credentials & credentials
                            ) const;

        /// \brief
        ///     Specifies if a full-screen reminder shall be displayed while
        ///     this activity is being performed.
        /// \param credentials
        ///     The credentials of the service caller.
        /// \param fullScreenReminder
        ///     True if a full-screen reminder shall be displayed while
        ///     this activity is being performed, else false.
        /// \exception WorkspaceException
        ///     If an error occurs.
        void        setFullScreenReminder(
                            const Credentials & credentials,
                            bool fullScreenReminder
                        );

        //////////
        //  Operations (associations)
    public:
        /// \brief
        ///     Returns the type of this Activity.
        /// \param credentials
        ///     The credentials of the service caller.
        /// \return
        ///     The type of this Activity, null == not assigned.
        /// \exception WorkspaceException
        ///     If an error occurs.
        auto        activityType(
                            const Credentials & credentials
                        ) const -> ActivityType;

        /// \brief
        ///     Sets the type of this Activity.
        /// \param credentials
        ///     The credentials of the service caller.
        /// \param activityType
        ///     The new type for this Activity, null == not assigned.
        /// \exception WorkspaceException
        ///     If an error occurs.
        void        setActivityType(
                            const Credentials & credentials,
                            ActivityType activityType
                        );

        /// \brief
        ///     Returns the Workload with which this Activity
        ///     is associated.
        /// \param credentials
        ///     The credentials of the service caller.
        /// \return
        ///     The Workload with which this Activity
        ///     is associated; nullptr == none.
        /// \exception WorkspaceException
        ///     If an error occurs.
        auto        workload(
                            const Credentials & credentials
                        ) const -> Workload;

        /// \brief
        ///     Sets the Workload with which this Activity
        ///     is associated.
        /// \param credentials
        ///     The credentials of the service caller.
        /// \param workload
        ///     The Workload with which this Activity is to be
        ///     associated; nullptr == none.
        /// \exception WorkspaceException
        ///     If an error occurs.
        void        setWorkload(
                            const Credentials & credentials,
                            Workload workload
                        );

        /// \brief
        ///     Returns the set of all Works logged against this Activity.
        /// \param credentials
        ///     The credentials of the service caller.
        /// \return
        ///     The set of all Works logged against this Activity.
        /// \exception WorkspaceException
        ///     If an error occurs.
        auto        works(
                            const Credentials & credentials
                        ) const -> Works;

        /// \brief
        ///     Returns the set of all Events logged against this Activity.
        /// \param credentials
        ///     The credentials of the service caller.
        /// \return
        ///     The set of all Events logged against this Activity.
        /// \exception WorkspaceException
        ///     If an error occurs.
        auto        events(
                            const Credentials & credentials
                        ) const -> Events;

        //////////
        //  Operations (access control)
    public:
        /// \brief
        ///     Checks whether if the specified credentials will
        ///     allow the caller to "start" this activity.
        /// \details
        ///     The caller must be able to log Work items. In
        ///     addition, if this Activity requires user comment
        ///     whet it is started and/or stopped, the caller
        ///     must also be able to log Events.
        /// \param credentials
        ///     The credentials of the service caller.
        /// \return
        ///     True if the specified credentials will allow
        ///     the caller to "start" this activity, else false.
        /// \exception WorkspaceException
        ///     If an error occurs.
        bool        canStart(
                            const Credentials & credentials
                        ) const;

        /// \brief
        ///     Checks whether if the specified credentials will
        ///     allow the caller to "stop" this activity.
        /// \details
        ///     The caller must be able to log Work items. In
        ///     addition, if this Activity requires user comment
        ///     whet it is stopped, the caller must also be able
        ///     to log Events.
        /// \param credentials
        ///     The credentials of the service caller.
        /// \return
        ///     True if the specified credentials will allow
        ///     the caller to "stop" this activity, else false.
        /// \exception WorkspaceException
        ///     If an error occurs.
        bool        canStop(
                            const Credentials & credentials
                        ) const;

        //////////
        //  Implementation
    private:
        tt3::db::api::IActivity *const  _dataActivity;  //  counts as "reference"
    };
}

//  Enable objects and object pointers for QVariant
Q_DECLARE_METATYPE(tt3::ws::ActivityImpl)
Q_DECLARE_METATYPE(tt3::ws::Activity)

//  Macro needed for MOC-generated .cpp files
#define TT3_WS_ACTIVITY_DEFINED

//  End of tt3-ws/Activity.hpp
