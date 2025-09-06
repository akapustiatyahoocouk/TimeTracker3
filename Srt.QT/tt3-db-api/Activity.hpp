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
    //  A generic activity
    class TT3_DB_API_PUBLIC IActivity : public virtual IObject
    {
        //////////
        //  This is an interface
    protected:
        IActivity() = default;
        virtual ~IActivity() = default;

        //////////
        //  Operations (properties)
    public:
        //  Returns/sets the user-readable display name of
        //  this activity.
        //  Throws DatabaseException if an error occurs.
        virtual auto    displayName(
                            ) const throws(DatabaseException)
                            -> QString = 0;
        virtual void    setDisplayName(
                                const QString & displayName
                            ) throws(DatabaseException) = 0;

        //  Returns/sets the multi-line description of this activity,
        //  with lines separated by a newline character '\n'.
        //  Throws DatabaseException if an error occurs.
        virtual auto    description(
                            ) const throws(DatabaseException)
                            -> QString = 0;
        virtual void    setDescription(
                                const QString & description
                            ) throws(DatabaseException) = 0;

        //  The optional timeout after which the activity stops
        //  automatically if the user does nothing at all
        //  Throws DatabaseException if an error occurs.
        virtual auto    timeout(
                            ) const throws(DatabaseException)
                            -> InactivityTimeout = 0;
        virtual void    setTimeout(
                                const InactivityTimeout & timeout
                            ) throws(DatabaseException) = 0;

        //  True if user must enter a comment when starting this
        //  activity; this comment will be recorded as an event.
        //  Throws DatabaseException if an error occurs.
        virtual bool    requireCommentOnStart(
                            ) const throws(DatabaseException) = 0;
        virtual void    setRequireCommentOnStart(
                                bool requireCommentOnStart
                            ) throws(DatabaseException) = 0;

        //  True if user must enter a comment when finishing this
        //  activity; this comment will be recorded as an event.
        //  Throws DatabaseException if an error occurs.
        virtual bool    requireCommentOnFinish(
                            ) const throws(DatabaseException) = 0;
        virtual void    setRequireCommentOnFinish(
                                bool requireCommentOnFinish
                            ) throws(DatabaseException) = 0;

        //  True if a full-screen reminder shall be displayed while
        //  this activity is being performed.
        //  Throws DatabaseException if an error occurs.
        virtual bool    fullScreenReminder(
                            ) const throws(DatabaseException) = 0;
        virtual void    setFullScreenReminder(
                                bool fullScreenReminder
                            ) throws(DatabaseException) = 0;

        //////////
        //  Operations (associations)
    public:
        //  The type of this Activity, null if not assigned.
        //  Throws DatabaseException if an error occurs.
        virtual auto    activityType(
                            ) const throws(DatabaseException)
                            -> IActivityType * = 0;
        virtual void    setActivityType(
                                IActivityType * activityType
                            ) throws(DatabaseException) = 0;

        //  Returns/sets the Workload with which this Activity
        //  is associated; nullptr == none.
        //  Throws DatabaseException if an error occurs.
        virtual auto    workload(
                            ) const throws(DatabaseException)
                            -> IWorkload * = 0;
        virtual void    setWorkload(
                                IWorkload * workload
                            ) throws(DatabaseException) = 0;

        //  Returns the set of all Works logged against this Activity.
        //  Throws DatabaseException if an error occurs.
        virtual auto    works(
                            ) const throws(DatabaseException)
                            -> Works = 0;

        //  Returns the set of all Events logged against this Activity.
        //  Throws DatabaseException if an error occurs.
        virtual auto    events(
                            ) const throws(DatabaseException)
                            -> Events = 0;
    };
}

//  End of tt3-db-api/Activity.hpp
