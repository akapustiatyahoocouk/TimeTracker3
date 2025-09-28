//
//  tt3-ws/PublicTask.hpp - A public task
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
    /// \class PublicTaskImpl tt3-ws/API.hpp
    /// \brief A public task.
    class TT3_WS_PUBLIC PublicTaskImpl final
        :   public PublicActivityImpl,
            public TaskImpl
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(PublicTaskImpl)

        friend class WorkspaceImpl;
        friend std::shared_ptr<PublicTaskImpl>;

        //////////
        //  Construction/destruction - from friends only
    private:
        PublicTaskImpl(
                Workspace workspace,
                tt3::db::api::IPublicTask * dataPublicTask
            );
        virtual ~PublicTaskImpl();

        //////////
        //  Operations (associations)
    public:
        /// \brief
        ///     Returns the immediate parent of this public task.
        /// \param credentials
        ///     The credentials of the service caller.
        /// \return
        ///     The immediate parent of this public task; nullptr == none.
        /// \exception WorkspaceException
        ///     If an error occurs.
        auto        parent(
                            const Credentials & credentials
                        ) const -> PublicTask;

        /// \brief
        ///     Sets the immediate parent of this public task.
        /// \details
        ///     Setting a parent to nullptr makes this a root private task.
        ///     Attempting to create parenthood loops is an error.
        /// \param credentials
        ///     The credentials of the service caller.
        /// \param parent
        ///     The new immediate parent for this public task; nullptr if none.
        /// \exception WorkspaceException
        ///     If an error occurs.
        void        setParent(
                            const Credentials & credentials,
                            PublicTask parent
                        );

        /// \brief
        ///     Returns the set of immediate children of this public task.
        /// \param credentials
        ///     The credentials of the service caller.
        /// \return
        ///     The set of immediate children of this public task.
        /// \exception WorkspaceException
        ///     If an error occurs.
        auto        children(
                            const Credentials & credentials
                        ) const -> PublicTasks;

        //////////
        //  Operations (life cycle)
    public:
        /// \brief
        ///     Creates a new child PublicTask under this one.
        /// \param credentials
        ///     The credentials of the service caller.
        /// \param displayName
        ///     The short (1 line) user-readable display name
        ///     for the new PublicTask.
        /// \param description
        ///     The multi-line user-readable description for the new
        ///     PublicTask; with lines separated by a newline '\\n' character.
        /// \param timeout
        ///     The user-does-nothing timeout for the new PublicTask;
        ///     absent == none.
        /// \param requireCommentOnStart
        ///     True if the newly created PublicTask small require the
        ///     user to enter a comment when it is started.
        /// \param requireCommentOnStop
        ///     True if the newly created PublicTask small require the
        ///     user to enter a comment when it is started.
        /// \param fullScreenReminder
        ///     True if a full-screen reminder shall be displayed while the
        ///     newly created PublicTask is underway.
        /// \param activityType
        ///     The type for the new PublicTask; nullptr == don't assign.
        /// \param workload
        ///     The Workload for the new PublicTask; nullptr == don't assign.
        /// \param completed
        ///     True if the PublicTask shall initially be marked as
        ///     "completed", false if not.
        /// \param requireCommentOnCompletion
        ///     True if the newly created PublicTask small require the
        ///     user to enter a comment when marking it as "completed".
        /// \return
        ///     The newly created PublicTask.
        /// \exception WorkspaceException
        ///     If an error occurs.
        auto        createChild(
                            const Credentials & credentials,
                            const QString & displayName,
                            const QString & description,
                            const InactivityTimeout & timeout,
                            bool requireCommentOnStart,
                            bool requireCommentOnStop,
                            bool fullScreenReminder,
                            ActivityType activityType,
                            Workload workload,
                            bool completed,
                            bool requireCommentOnCompletion
                        ) -> PublicTask;

        //////////
        //  Implementation
    private:
        tt3::db::api::IPublicTask *const    _dataPublicTask;  //  counts as "reference"

        //  Access control
        virtual bool    _canRead(       //  throws WorkspaceException
                                const Credentials & credentials
                            ) const override;
        virtual bool    _canModify(     //  throws WorkspaceException
                                const Credentials & credentials
                            ) const override;
        virtual bool    _canDestroy(    //  throws WorkspaceException
                                const Credentials & credentials
                            ) const override;
    };
}

//  Enable objects and object pointers for QVariant
Q_DECLARE_METATYPE(tt3::ws::PublicTaskImpl)
Q_DECLARE_METATYPE(tt3::ws::PublicTask)

//  End of tt3-ws/PublicTask.hpp
