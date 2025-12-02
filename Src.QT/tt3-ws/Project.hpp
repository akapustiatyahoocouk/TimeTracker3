//
//  tt3-ws/Project.hpp - A project
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
    /// \class ProjectImpl tt3-ws/API.hpp
    /// \brief A project.
    class TT3_WS_PUBLIC ProjectImpl final
        :   public WorkloadImpl
    {
        TT3_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(ProjectImpl)

        friend class WorkspaceImpl;
        friend std::shared_ptr<ProjectImpl>;

        //////////
        //  Construction/destruction - from friends only
    private:
        ProjectImpl(
                Workspace workspace,
                tt3::db::api::IProject * dataProject
            );
        virtual ~ProjectImpl();

        //////////
        //  Operations (properties)
    public:
        /// \brief
        ///     Returns the completion status of this Project.
        /// \param credentials
        ///     The credentials of the service caller.
        /// \return
        ///     The completion status of this Project.
        /// \exception WorkspaceException
        ///     If an error occurs.
        bool        completed(
                            const Credentials & credentials
                        ) const;

        /// \brief
        ///     Sets the completion status of this Project.
        /// \param credentials
        ///     The credentials of the service caller.
        /// \param completed
        ///     The new completion status for this Project.
        /// \exception WorkspaceException
        ///     If an error occurs.
        void        setCompleted(
                            const Credentials & credentials,
                            bool completed
                        );

        //////////
        //  Operations (associations)
    public:
        /// \brief
        ///     Returns the immediate parent of this project.
        /// \param credentials
        ///     The credentials of the service caller.
        /// \return
        ///     The immediate parent of this project; nullptr == none.
        /// \exception WorkspaceException
        ///     If an error occurs.
        auto        parent(
                            const Credentials & credentials
                        ) const -> Project;

        /// \brief
        ///     Sets the immediate parent of this project.
        /// \details
        ///     Setting a parent to nullptr makes this a root project.
        ///     Attempting to create parenthood loops is an error.
        /// \param credentials
        ///     The credentials of the service caller.
        /// \param parent
        ///     The new immediate parent for this project; nullptr if none.
        /// \exception WorkspaceException
        ///     If an error occurs.
        void        setParent(
                            const Credentials & credentials,
                            Project parent
                        );

        /// \brief
        ///     Returns the set of immediate children of this project.
        /// \param credentials
        ///     The credentials of the service caller.
        /// \return
        ///     The set of immediate children of this project.
        /// \exception WorkspaceException
        ///     If an error occurs.
        auto        children(
                                const Credentials & credentials
                            ) const -> Projects;

        //////////
        //  Operations (life cycle)
    public:
        /// \brief
        ///     Creates a new child project under this project.
        /// \param credentials
        ///     The credentials of the service caller.
        /// \param displayName
        ///     The short (1 line) user-readable display name
        ///     for the new Project.
        /// \param description
        ///     The multi-line user-readable description for the new
        ///     Project; with lines separated by a newline '\\n' character.
        /// \param beneficiaries
        ///     The set of Beneficiaries to associate with the newly
        ///     created Project (can be empty).
        /// \param completed
        ///     True if the new Project shall be initially
        ///     marked as "completed", false if not.
        /// \return
        ///     The newly created Project.
        /// \exception WorkspaceException
        ///     If an error occurs.
        auto        createChild(
                            const Credentials & credentials,
                            const QString & displayName,
                            const QString & description,
                            const Beneficiaries & beneficiaries,
                            bool completed
                        ) -> Project;

        //////////
        //  Implementation
    private:
        tt3::db::api::IProject *const   _dataProject;  //  counts as "reference"

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
Q_DECLARE_METATYPE(tt3::ws::ProjectImpl)
Q_DECLARE_METATYPE(tt3::ws::Project)

//  End of tt3-ws/Project.hpp
