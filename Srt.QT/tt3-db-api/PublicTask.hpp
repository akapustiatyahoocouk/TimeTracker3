//
//  tt3-db-api/PublicTask.hpp - a public task
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
    //  A public task
    class TT3_DB_API_PUBLIC IPublicTask :
        public virtual IPublicActivity,
        public virtual ITask
    {
        //////////
        //  This is an interface
    protected:
        IPublicTask() = default;
        virtual ~IPublicTask() = default;

        //////////
        //  IObject
    public:
        virtual auto    type() const
                            -> IObjectType * override
        {
            return ObjectTypes::PublicTask::instance();
        }

        //////////
        //  Operations (associations)
    public:
        //  Returns/sets the immediate parent of this public task; nullptr if none.
        //  Setting a parent to nullptr makes this a root private task.
        //  Attempting to create parenthood loops is an error.
        //  Throws DatabaseException if an error occurs.
        virtual auto    parent() const
                            throws(DatabaseException)
                            -> IPublicTask * = 0;
        virtual void    setParent(
                                IPublicTask * parent
                            ) throws(DatabaseException) = 0;

        //  Returns the set of immediate children of this public task.
        //  Throws DatabaseException if an error occurs.
        virtual auto    children() const
                            throws(DatabaseException)
                            -> PublicTasks = 0;

        //////////
        //  Operations (life cycle)
    public:
        //  Creates a new child PublicTask under this one.
        //  The activity type can be nullptr.
        //  Throws DatabaseException if an error occurs.
        virtual auto    createChild(
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
                            ) throws(DatabaseException)
                            -> IPublicTask * = 0;
    };
}

//  End of tt3-db-api/PublicTask.hpp
