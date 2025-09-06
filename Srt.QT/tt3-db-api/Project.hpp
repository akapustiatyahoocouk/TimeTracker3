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
    //  A project
    class TT3_DB_API_PUBLIC IProject : public virtual IWorkload
    {
        //////////
        //  This is an interface
    protected:
        IProject() = default;
        virtual ~IProject() = default;

        //////////
        //  IObject
    public:
        virtual IObjectType *   type() const override
        {
            return ObjectTypes::Project::instance();
        }

        //////////
        //  Operations (properties)
    public:
        //  Returns/sets the completion status of this Project.
        //  Throws DatabaseException if an error occurs.
        virtual bool        completed() const throws(DatabaseException) = 0;
        virtual void        setCompleted(bool completed) throws(DatabaseException) = 0;

        //////////
        //  Operations (associations)
    public:
        //  Returns/sets the immediate parent of this
        //  Project; nullptr == none.
        //  Attempting to create parenthood loops is an error.
        //  Throws DatabaseException if an error occurs.
        virtual IProject *  parent() const throws(DatabaseException) = 0;
        virtual void        setParent(IProject * parent) throws(DatabaseException) = 0;

        //  Returns the set of immediate children of this Project.
        //  Throws DatabaseException if an error occurs.
        virtual Projects    children() const throws(DatabaseException) = 0;

        //////////
        //  Operations (life cycle)
    public:
        //  Creates a new child project under this project.
        //  Throws DatabaseException if an error occurs.
        virtual IProject *  createChild(
            const QString & displayName,
            const QString & description,
            const Beneficiaries & beneficiaries) throws(DatabaseException) = 0;
    };
}

//  End of tt3-db-api/Project.hpp
