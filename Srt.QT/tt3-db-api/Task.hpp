//
//  tt3-db-api/Task.hpp - a generic task
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
    //  A generic task
    class TT3_DB_API_PUBLIC ITask : public virtual IActivity
    {
        //////////
        //  This is an interface
    protected:
        ITask() = default;
        virtual ~ITask() = default;

        //////////
        //  Operations (properties)
    public:
        //  The cmpletion status of this task.
        //  Throws DatabaseException if an error occurs.
        virtual bool        completed() const throws(DatabaseException) = 0;
        virtual void        setCompleted(bool completed) throws(DatabaseException) = 0;

        //  True if user must enter a comment when completing this
        //  task; this comment will be recorded as an event.
        //  Throws DatabaseException if an error occurs.
        virtual bool            requireCommentOnCompletion() const throws(DatabaseException) = 0;
        virtual void            setRequireCommentOnCompletion(bool requireCommentOnCompletion) throws(DatabaseException) = 0;
    };
}

//  End of tt3-db-api/Task.hpp
