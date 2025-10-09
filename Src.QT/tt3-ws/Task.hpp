//
//  tt3-ws/Task.hpp - A generic task
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
    /// \class TaskImpl tt3-ws/API.hpp
    /// \brief A generic task.
    class TT3_WS_PUBLIC TaskImpl
        :   public virtual ActivityImpl
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(TaskImpl)

        friend std::shared_ptr<TaskImpl>;
        friend class PublicTaskImpl;
        friend class PrivateTaskImpl;

        //////////
        //  Construction/destruction - from friends only
    private:
        TaskImpl(
                Workspace workspace,
                tt3::db::api::ITask * dataTask
            );
        virtual ~TaskImpl();

        //////////
        //  Operations (properties)
    public:
        /// \brief
        ///     Returns the completion status of this task.
        /// \param credentials
        ///     The credentials of the service caller.
        /// \return
        ///     The completion status of this task.
        /// \exception WorkspaceException
        ///     If an error occurs.
        bool        completed(
                            const Credentials & credentials
                        ) const;

        /// \brief
        ///     Returns the completion status of this task.
        /// \param credentials
        ///     The credentials of the service caller.
        /// \param completed
        ///     The completion status of this task.
        /// \exception WorkspaceException
        ///     If an error occurs.
        void        setCompleted(
                            const Credentials & credentials,
                            bool completed
                        );

        /// \brief
        ///     Checks if user must enter a comment when completing this task.
        /// \param credentials
        ///     The credentials of the service caller.
        /// \return
        ///     True if user must enter a comment when completing this
        ///     task (this comment will be recorded as an event), else false.
        /// \exception WorkspaceException
        ///     If an error occurs.
        bool        requireCommentOnCompletion(
                            const Credentials & credentials
                        ) const;

        /// \brief
        ///     Specifies if user must enter a comment when completing this task.
        /// \param credentials
        ///     The credentials of the service caller.
        /// \param requireCommentOnCompletion
        ///     True if user must enter a comment when completing this
        ///     task (this comment will be recorded as an event), else false.
        /// \exception WorkspaceException
        ///     If an error occurs.
        void        setRequireCommentOnCompletion(
                            const Credentials & credentials,
                            bool requireCommentOnCompletion
                        );

        //////////
        //  Implementation
    private:
        tt3::db::api::ITask *const  _dataTask;  //  counts as "reference"
    };
}

//  Enable objects and object pointers for QVariant
Q_DECLARE_METATYPE(tt3::ws::TaskImpl)
Q_DECLARE_METATYPE(tt3::ws::Task)

//  End of tt3-ws/Task.hpp
