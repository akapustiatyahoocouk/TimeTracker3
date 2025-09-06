//
//  tt3-db-api/PrivateTask.hpp - a private task
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
    //  ApPrivate task
    class TT3_DB_API_PUBLIC IPrivateTask :
        public virtual IPrivateActivity,
        public virtual ITask
    {
        //////////
        //  This is an interface
    protected:
        IPrivateTask() = default;
        virtual ~IPrivateTask() = default;

        //////////
        //  IObject
    public:
        virtual IObjectType *   type() const override
        {
            return ObjectTypes::PrivateTask::instance();
        }

        //////////
        //  Operations (associations)
    public:
        //  Returns/sets the immediate parent of this private task; nullptr if none.
        //  Setting a parent to nullptr makes this a root private task.
        //  Attempting to create parenthood loops is an error.
        //  Throws DatabaseException if an error occurs.
        virtual IPrivateTask *  parent() const throws(DatabaseException) = 0;
        virtual void            setParent(IPrivateTask * parent) throws(DatabaseException) = 0;

        //  Returns the set of immediate children of this private task.
        //  Throws DatabaseException if an error occurs.
        virtual PrivateTasks    children() const throws(DatabaseException) = 0;

        //////////
        //  Operations (life cycle)
    public:
        //  Creates a new child PrivateTask under this one.
        //  The activity type can be nullptr.
        //  Throws DatabaseException if an error occurs.
        virtual IPrivateTask *  createChild(
                                    const QString & displayName,
                                    const QString & description,
                                    const InactivityTimeout & timeout,
                                    bool requireCommentOnStart,
                                    bool requireCommentOnFinish,
                                    bool fullScreenReminder,
                                    IActivityType * activityType,
                                    IWorkload * workload,
                                    bool completed,
                                    bool requireCommentOnCompletion) throws(DatabaseException) = 0;
    };
}

//  End of tt3-db-api/PublicTask.hpp
