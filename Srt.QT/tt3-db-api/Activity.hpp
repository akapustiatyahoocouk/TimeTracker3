//
//  tt3-db-api/Activity.hpp - a generic activity
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
    /// \class IActivity tt3-db-api/API.hpp
    /// \brief A generic activity.
    class TT3_DB_API_PUBLIC IActivity :
        public virtual IObject
    {
        //////////
        //  This is an interface
    protected:
        IActivity() = default;
        virtual ~IActivity() = default;

        //////////
        //  Operations (properties)
    public:
        /// \brief
        ///     Returns the user-readable display name of this activity.
        /// \return
        ///     The user-readable display name of this activity.
        /// \exception DatabaseException
        ///     If an error occurs.
        virtual QString displayName() const = 0;

        /// \brief
        ///     Sets the user-readable display name of this activity.
        /// \param displayName
        ///     The new user-readable display name for this activity.
        /// \exception DatabaseException
        ///     If an error occurs.
        virtual void    setDisplayName(
                                const QString & displayName
                            ) = 0;

        /// \brief
        ///     Returns the multi-line description of this activity.
        /// \return
        ///     The multi-line description of this activity,
        ///     with lines separated by a newline character '\n'.
        /// \exception DatabaseException
        ///     If an error occurs.
        virtual QString description() const = 0;

        /// \brief
        ///     Sets the multi-line description of this activity.
        /// \param description
        ///     The new multi-line description for this activity,
        ///     with lines separated by a newline character '\n'.
        /// \exception DatabaseException
        ///     If an error occurs.
        virtual void    setDescription(
                                const QString & description
                            ) = 0;

        /// \brief
        ///     Returns the timeout after which the activity
        ///     stops automatically if the user does nothing at all.
        /// \return
        ///     The timeout after which the activity stops
        ///     automatically if the user does nothing at
        ///     all; absent == none.
        /// \exception DatabaseException
        ///     If an error occurs.
        virtual auto    timeout(
                            ) const -> InactivityTimeout = 0;

        /// \brief
        ///     Sets the timeout after which the activity
        ///     stops automatically if the user does nothing at all.
        /// \param timeout
        ///     The new timeout after which the activity stops
        ///     automatically if the user does nothing at
        ///     all; absent == none.
        /// \exception DatabaseException
        ///     If an error occurs.
        virtual void    setTimeout(
                                const InactivityTimeout & timeout
                            ) = 0;

        /// \brief
        ///     Checks if user must enter a comment when starting this
        ///     activity.
        /// \return
        ///     True if user must enter a comment when starting this
        ///     activity (this comment will be recorded as an event),
        ///     false if not.
        /// \exception DatabaseException
        ///     If an error occurs.
        virtual bool    requireCommentOnStart(
                            ) const = 0;

        /// \brief
        ///     Specifies whether user must enter a comment when
        ///     starting this activity.
        /// \param requireCommentOnStart
        ///     True if user must enter a comment when starting this
        ///     activity (this comment will be recorded as an event),
        ///     false if not.
        /// \exception DatabaseException
        ///     If an error occurs.
        virtual void    setRequireCommentOnStart(
                                bool requireCommentOnStart
                            ) = 0;

        /// \brief
        ///     Checks if user must enter a comment when stopping
        ///     this activity.
        /// \return
        ///     True if user must enter a comment when stopping this
        ///     activity (this comment will be recorded as an event),
        ///     false if not.
        /// \exception DatabaseException
        ///     If an error occurs.
        virtual bool    requireCommentOnStop(
                            ) const = 0;

        /// \brief
        ///     Specifies if user must enter a comment when stopping
        ///     this activity.
        /// \param requireCommentOnStop
        ///     True if user must enter a comment when stopping this
        ///     activity (this comment will be recorded as an event),
        ///     false if not.
        /// \exception DatabaseException
        ///     If an error occurs.
        virtual void    setRequireCommentOnStop(
                                bool requireCommentOnStop
                            ) = 0;

        /// \brief
        ///     Checks if a full-screen reminder shall be displayed while
        ///     this activity is being performed.
        /// \return
        ///     True if a full-screen reminder shall be displayed while
        ///     this activity is being performed, else false.
        /// \exception DatabaseException
        ///     If an error occurs.
        virtual bool    fullScreenReminder(
                            ) const = 0;
        /// \brief
        ///     Specifies if a full-screen reminder shall be displayed while
        ///     this activity is being performed.
        /// \param fullScreenReminder
        ///     True if a full-screen reminder shall be displayed while
        ///     this activity is being performed, else false.
        /// \exception DatabaseException
        ///     If an error occurs.
        virtual void    setFullScreenReminder(
                                bool fullScreenReminder
                            ) = 0;

        //////////
        //  Operations (associations)
    public:
        /// \brief
        ///     Returns the type of this Activity.
        /// \return
        ///     The type of this Activity, null == not assigned.
        /// \exception DatabaseException
        ///     If an error occurs.
        virtual auto    activityType(
                            ) const -> IActivityType * = 0;

        /// \brief
        ///     Sets the type of this Activity.
        /// \param activityType
        ///     The new type for this Activity, null == not assigned.
        /// \exception DatabaseException
        ///     If an error occurs.
        virtual void    setActivityType(
                                IActivityType * activityType
                            ) = 0;

        /// \brief
        ///     Returns the Workload with which this Activity
        ///     is associated.
        /// \return
        ///     The Workload with which this Activity
        ///     is associated; nullptr == none.
        /// \exception DatabaseException
        ///     If an error occurs.
        virtual auto    workload(
                            ) const -> IWorkload * = 0;

        /// \brief
        ///     Sets the Workload with which this Activity
        ///     is associated.
        /// \param workload
        ///     The Workload with which this Activity is to be
        ///     associated; nullptr == none.
        /// \exception DatabaseException
        ///     If an error occurs.
        virtual void    setWorkload(
                                IWorkload * workload
                            ) = 0;

        /// \brief
        ///     Returns the set of all Works logged against this Activity.
        /// \return
        ///     The set of all Works logged against this Activity.
        /// \exception DatabaseException
        ///     If an error occurs.
        virtual auto    works(
                            ) const -> Works = 0;

        /// \brief
        ///     Returns the set of all Events logged against this Activity.
        /// \return
        ///     The set of all Events logged against this Activity.
        /// \exception DatabaseException
        ///     If an error occurs.
        virtual auto    events(
                            ) const -> Events = 0;
    };
}

//  End of tt3-db-api/Activity.hpp
