//
//  tt3-ws/Object.hpp - A generic workspace object
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
    //  A generic workspace object
    class TT3_WS_PUBLIC ObjectImpl
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(ObjectImpl)

        //////////
        //  Construction/destruction
    private:
        ObjectImpl(Workspace workspace, tt3::db::api::IObject * dataObject);
        virtual ~ObjectImpl();

        //////////
        //  Operations (general)
    public:
        //  The type of this workspace object; can be
        //  safely obtained for both live and dead objects
        ObjectType *    type() const;

        //  The workspace where the corresponding object
        //  resides (if live) or used to reside (if dead).
        Workspace       workspace() const;

        //  The OID of the corresponding object; can be
        //  safely obtained for both live and dead objects.
        Oid             oid() const;

        //  True if this instance represents an existing data
        //  object residing in a database, else false.
        bool            isLive() const;

        //////////
        //  Opertions (life cycle)
    public:
        //  Destroys this object, delete-cascading
        //  as necessary. The instance (and all other
        //  delete-cascadedinstances) is marked as
        //  "reoresenting a dead object" and,
        //  therefore, unusable.
        //  Throws WorkspaceException if an error occurs.
        void            destroy(const Credentials & credentials) throws(WorkspaceException);

        //////////
        //  Implementation
    private:
        Workspace       _workspace;
        tt3::db::api::IObject *const    _dataObject;    //  counts as "refrence"

        //  Helpers
        void            _ensureLive() const throws(WorkspaceException);
    };
}

//  End of tt3-ws/Object.hpp
