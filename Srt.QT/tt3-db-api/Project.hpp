//
//  tt3-db-api/Project.hpp - a project
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
    /// \class IProject tt3-db-api/API.hpp
    /// \brief A workload that has a completion status.
    class TT3_DB_API_PUBLIC IProject
        :   public virtual IWorkload
    {
        //////////
        //  This is an interface
    protected:
        IProject() = default;
        virtual ~IProject() = default;

        //////////
        //  IObject
    public:
        virtual auto    type(
                            ) const -> IObjectType * override
        {
            return ObjectTypes::Project::instance();
        }

        //////////
        //  Operations (properties)
    public:
        /// \brief
        ///     Returns the completion status of this Project.
        /// \return
        ///     The completion status of this Project.
        /// \exception DatabaseException
        ///     If an error occurs.
        virtual bool    completed(
                            ) const = 0;

        /// \brief
        ///     Sets the completion status of this Project.
        /// \param completed
        ///     The new completion status for this Project.
        /// \exception DatabaseException
        ///     If an error occurs.
        virtual void    setCompleted(
                                bool completed
                            ) = 0;

        //////////
        //  Operations (associations)
    public:
        /// \brief
        ///     Returns the immediate parent of this Project.
        /// \return
        ///     The immediate parent of this Project, nullptr == none.
        /// \exception DatabaseException
        ///     If an error occurs.
        virtual auto    parent(
                            ) const -> IProject * = 0;
        /// \brief
        ///     Sets the immediate parent of this Project.
        /// \details
        ///     Attempting to create parenthood loops is an error.
        /// \param parent
        ///     The new immediate parent of this Project, nullptr == none.
        /// \exception DatabaseException
        ///     If an error occurs.
        virtual void    setParent(
                                IProject * parent
                            ) = 0;

        /// \brief
        ///     Returns the set of immediate children of this Project.
        /// \return
        ///     The set of immediate children of this Project.
        /// \exception DatabaseException
        ///     If an error occurs.
        virtual auto    children(
                            ) const -> Projects = 0;

        //////////
        //  Operations (life cycle)
    public:
        /// \brief
        ///     Creates a new child project under this project.
        /// \param displayName
        ///     The short (1 line) user-readable display name
        ///     for the new Project.
        /// \param description
        ///     The multi-line user-readable description for the new
        ///     Project; with lines separated by a newline '\\n' character.
        /// \param beneficiaries
        ///     The set of Beneficiaries to associate with the newly
        ///     created Project (can be empty).
        /// \return
        ///     The newly created Project.
        /// \exception DatabaseException
        ///     If an error occurs.
        virtual auto    createChild(
                                const QString & displayName,
                                const QString & description,
                                const Beneficiaries & beneficiaries
                        ) -> IProject * = 0;
    };
}

//  End of tt3-db-api/Project.hpp
