//
//  tt3-db-api/Object.hpp - a generic database objectT
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
    //  The OID; unique per database
    typedef uint64_t Oid;
    static inline const Oid InvalidOid = 0;

    //  A generic object that resides, or used to reside,
    //  in a database. Database object instances are managed
    //  by their corresponding database.
    //  An instance of a IObject has an associated
    //  reference count. Depending on it, that instance can be
    //  in one of the following states:
    //  *   New - the instance has just been created; its
    //      reference count is 0. New instances are not
    //      recycled.
    //  *   Managed - a New instance becomes Managed when its
    //      reference count goes from 0 to 1 for the first
    //      time. Managed instances are not recycled.
    //  *   Old - a Managed instance becodes Old when its
    //      reference count changes from 1 to 0. Old instances
    //      can be recycled by the database they belong
    //      to. Changing their reference count from 0 to 1
    //      again makes them Managed again.
    //  Normallty, an IDatabase - implementing classwill act as
    //  a cache for the real data kepy in the persistent storage,
    //  At any time only the part of that data can be represented
    //  by a corresponding IObject instance.
    //  IMPORTANT: The implementation ensures that no two live
    //  instanes of a derived class refer to the same database
    //  object.
    class TT3_DB_API_PUBLIC IObject
    {
        //////////
        //  Types
    public:
        enum class State
        {
            New,
            Managed,
            Old
        };

        //////////
        //  This is an interface
    protected:
        IObject() = default;
        virtual ~IObject() = default;

        //////////
        //  Operations (general)
    public:
        //  The type of this database object; can be
        //  safely obtained for both live and dead objects
        virtual IObjectType *   type() const = 0;

        //  The database where the corresponding data object
        //  resides (if live) or used to reside (if dead).
        virtual IDatabase * database() const = 0;

        //  The OID of the corresponding data object; can be
        //  safely obtained for both live and dead objects.
        virtual Oid         oid() const = 0;

        //  True if this instance represents an existing data
        //  object residing in a database, else false.
        virtual bool        isLive() const = 0;

        //////////
        //  Operations (life cycle)
    public:
        //  Destroys the corresponding database object,
        //  delete-cascading as necessary. The instance of
        //  the IObject - implementing class remains
        //  in existence, but is marked as "reoresenting
        //  a dead object" and, therefore, unusable.
        virtual void        destroy() throws(DatabaseException) = 0;

        //////////
        //  Operations (reference counting)
        //  All these operations are thread-safe.
    public:
        //  The current "state" of this object.
        virtual State           state() const = 0;

        //  The current "reference count" of this object.
        virtual int             referenceCount() const = 0;

        //  Increments the "reference count" of this object by 1.
        virtual void            addReference() = 0;

        //  Decrements the "reference count" of this object by 1.
        virtual void            removeReference() = 0;
    };
}

//  End of tt3-db-api/Object.hpp
