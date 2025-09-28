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
    /// \class IPrivateTask tt3-db-api/API.hpp
    /// \brief A private task.
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
        virtual auto    type(
                            ) const -> IObjectType * override
        {
            return ObjectTypes::PrivateTask::instance();
        }

        //////////
        //  Operations (associations)
    public:
        /// \brief
        ///     Returns the immediate parent of this private task.
        /// \return
        ///     The immediate parent of this private task; nullptr == none.
        /// \exception DatabaseException
        ///     If an error occurs.
        virtual auto    parent(
                            ) const -> IPrivateTask * = 0;

        /// \brief
        ///     Sets the immediate parent of this private task; nullptr if none.
        /// \details
        ///     Setting a parent to nullptr makes this a root private task.
        ///     Attempting to create parenthood loops is an error.
        /// \param parent
        ///     The new immediate parent of this private task; nullptr == none.
        /// \exception DatabaseException
        ///     If an error occurs.
        virtual void    setParent(
                                IPrivateTask * parent
                            ) = 0;

        /// \brief
        ///     Returns the set of immediate children of this private task.
        /// \return
        ///     The set of immediate children of this private task.
        /// \exception DatabaseException
        ///     If an error occurs.
        virtual auto    children(
                            ) const -> PrivateTasks = 0;

        //////////
        //  Operations (life cycle)
    public:
        /// \brief
        ///     Creates a new child PrivateTask under this one.
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
        ///     The Workload for the new v; nullptr == don't assign.
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
                            ) -> IPrivateTask * = 0;
    };
}

//  End of tt3-db-api/PublicTask.hpp
