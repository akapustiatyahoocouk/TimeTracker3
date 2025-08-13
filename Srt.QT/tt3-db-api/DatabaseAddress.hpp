//
//  tt3-db-api/DatabaseAddress.hpp - "database address" ADT
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
    //////////
    //  A "database address" represents a location of a speific
    //  database of a given type. Database address instances are
    //  managed by their corresponding database type.
    //  An instance of a IDatabaseAddress has an associated
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
    //      can be recycled by the database type they belong
    //      to. Changing their reference count from 0 to 1
    //      again makes them Managed again.
    //  IMPORTANT: The implementation ensures that no two
    //  instanes of a derived class refer to the same database;
    //  therefore, to compare two database addresses for equality
    //  it is sufficient to compare the pointers to the two
    //  instances.
    class TT3_DB_API_PUBLIC IDatabaseAddress
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
        IDatabaseAddress() = default;
        virtual ~IDatabaseAddress() = default;

        //////////
        //  Operations (general)
    public:
        //  The database type to which this database address belongs.
        virtual IDatabaseType * databaseType() const = 0;

        //  The user-readable form of this database address.
        virtual QString         displayForm() const = 0;

        //  The external (re-parsable) form of this database address.
        virtual QString         externalForm() const = 0;

        //////////
        //  Operations (reference counting)
        //  All these operations are thread-safe.
    public:
        //  TODO document
        virtual State           state() const = 0;
        virtual int             referenceCount() const = 0;
        virtual void            addReference() = 0;
        virtual void            removeReference() = 0;
    };
}

//  End of tt3-db-api/DatabaseAddress.hpp
