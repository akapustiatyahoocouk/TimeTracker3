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
        /// \brief
        ///     Returns the completion status of this task.
        /// \return
        ///     The completion status of this task.
        /// \exception DatabaseException
        ///     If an error occurs.
        virtual bool    completed(
                            ) const = 0;

        /// \brief
        ///     Returns the completion status of this task.
        /// \param completed
        ///     The completion status of this task.
        /// \exception DatabaseException
        ///     If an error occurs.
        virtual void    setCompleted(
                                bool completed
                            ) = 0;

        /// \brief
        ///     Checks if user must enter a comment when completing this task.
        /// \return
        ///     True if user must enter a comment when completing this
        ///     task (this comment will be recorded as an event), else false.
        /// \exception DatabaseException
        ///     If an error occurs.
        virtual bool    requireCommentOnCompletion(
                            ) const = 0;

        /// \brief
        ///     Specifies if user must enter a comment when completing this task.
        /// \param requireCommentOnCompletion
        ///     True if user must enter a comment when completing this
        ///     task (this comment will be recorded as an event), else false.
        /// \exception DatabaseException
        ///     If an error occurs.
        virtual void    setRequireCommentOnCompletion(
                                bool requireCommentOnCompletion
                            ) = 0;
    };
}

//  End of tt3-db-api/Task.hpp
