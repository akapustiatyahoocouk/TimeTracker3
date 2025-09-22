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
#pragma once
#include "tt3-ws/API.hpp"

namespace tt3::ws
{
    /// \class ActivityImpl tt3-ws/API.hpp
    /// \brief A generic activity.
    class TT3_WS_PUBLIC ActivityImpl
        :   public ObjectImpl
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(ActivityImpl)

        friend class PublicActivityImpl;

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
        auto        displayName(
                            const Credentials & credentials
                        ) const -> QString;

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
        auto        description(
                            const Credentials & credentials
                        ) const -> QString;

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
        ///     Checks if user must enter a comment when finishing
        ///     this activity.
        /// \param credentials
        ///     The credentials of the service caller.
        /// \return
        ///     True if user must enter a comment when finishing this
        ///     activity (this comment will be recorded as an event),
        ///     false if not.
        /// \exception WorkspaceException
        ///     If an error occurs.
        bool        requireCommentOnFinish(
                            const Credentials & credentials
                        ) const;

        /// \brief
        ///     Specifies if user must enter a comment when finishing
        ///     this activity.
        /// \param credentials
        ///     The credentials of the service caller.
        /// \param requireCommentOnFinish
        ///     True if user must enter a comment when finishing this
        ///     activity (this comment will be recorded as an event),
        ///     false if not.
        /// \exception WorkspaceException
        ///     If an error occurs.
        void        setRequireCommentOnFinish(
                            const Credentials & credentials,
                            bool requireCommentOnFinish
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
        //  Implementation
    private:
        tt3::db::api::IActivity *const  _dataActivity;  //  counts as "reference"
    };

    /// \class CurrentActivity tt3-ws/API.hpp
    /// \brief The accessor for a "current" activity pseudo-variable.
    /// \details
    ///     Only one global static instance of this class exists,
    ///     and other instances should NOT be constructed.
    ///     Can be nullptr if there is no "current" Activity.
    class TT3_WS_PUBLIC CurrentActivity final
        :   public QObject
    {
        Q_OBJECT
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(CurrentActivity)

        //////////
        //  Construction/destruction
    public:
        /// \brief
        ///     The class constructor.
        CurrentActivity();

        /// \brief
        ///     The class destructor.
        virtual ~CurrentActivity();
        \
        //////////
        //  Operators
    public:
        /// \brief
        ///     Replaces the "current" Activity.
        /// \details
        ///     Technically this is a shortcut form for a
        ///     "replace(activity, theCurrentCredentials);
        ///     any errors are logged but ignored.
        /// \param activity
        ///     The activity to make "current"; nullptr == none.
        void        operator = (const Activity & activity);

        /// \brief
        ///     Returns the pointer to the "current" Activity.
        /// \return
        ///     The pointer to the "current" Activity; nullptr == none.
        Activity    operator -> () const;

        /// \brief
        ///     Returns the pointer to the "current" Activity.
        /// \return
        ///     The pointer to the "current" Activity; nullptr == none.
                    operator Activity() const;

        /// \brief
        ///     Checks whether the "current" Activity is set (is not nullptr).
        /// \return
        ///     True if the "current" Activityis set, else false.
        bool        operator == (nullptr_t /*null*/) const;

        /// \brief
        ///     Checks whether the "current" Activity is set (is not nullptr).
        /// \return
        ///     False if the "current" Activityis set, else true.
        bool        operator != (nullptr_t /*null*/) const;

        /// \brief
        ///     Checks whether the sppecified activity is "current".
        /// \param activity
        ///     The activity to check.
        /// \return
        ///     True if the sppecified activity is "current", else false.
        bool        operator == (Activity activity) const;

        /// \brief
        ///     Checks whether the sppecified activity is "current".
        /// \param activity
        ///     The activity to check.
        /// \return
        ///     False if the sppecified activity is "current", else true.
        bool        operator != (Activity activity) const;

        //////////
        //  Operations
    public:
        /// \brief
        ///     Returns the last date+time (UTC) when the
        ///     "current" activity was assigned to.
        /// \return
        ///     The last date+time (UTC) when the
        ///     "current" activity was assigned to.
        QDateTime   lastChangedAt() const;

        /// \brief
        ///     Replaces the "current" activity with
        ///     the specified activity.
        /// \details
        ///     If the "current" activity alredy exists and
        ///     requires a comment to be entered (as Event)
        ///     when it's fnished, this is handled internally
        ///     by asking the user to supply that comment.
        ///     Similaely, if the new activity is not nullptr
        ///     and requires a comment when started, user will
        ///     be prompted to enter this comment internally.
        ///     If tge comment entry dialog is cancelled, the
        ///     "current" activity remains unchanged.
        /// \param with
        ///     The activity t set as "current"; ullptr == none.
        /// \param credentials
        ///     The credentials to use for data access.
        /// \return
        ///     True if the "current" activity was actually
        ///     replaced, false if not,
        /// \exception WorkspaceException
        ///     If a data access error occurs.
        bool        replaceWoth(
                            Activity with,
                            const Credentials & credentials
                        );

        //////////
        //  Signals
        //  Clients are encourated to use "queued" connections.
    signals:
        /// \brief
        ///     Emitted after the current Activity has changed.
        /// \param before
        ///     The Activity "current" before the change, nullptr == none.
        /// \param after
        ///     The Activity "current" after the change, nullptr == none.
        void        changed(Activity before, Activity after);

        //////////
        //  Implementation
    private:
        struct _Impl;

        //  Helpers
        static _Impl *  _impl();
    };

#if defined(TT3_WS_LIBRARY)
    //  Building tt3-ws
#else
    //  Building tt3-ws client
    Q_DECL_IMPORT CurrentActivity theCurrentActivity;
#endif
}

//  Enable objects and object pointers for QVariant
Q_DECLARE_METATYPE(tt3::ws::ActivityImpl)
Q_DECLARE_METATYPE(tt3::ws::Activity)

//  Macro needed for MOC-generated .cpp files
#define TT3_WS_ACTIVITY_DEFINED

//  End of tt3-ws/Activity.hpp
