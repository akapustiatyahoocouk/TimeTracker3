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

//////////
//  Formatting and parsing
namespace tt3::util
{   //  Uses {00000000-0000-0000-0000-000000000000} braced format.
    //  Formats as uppercase, parses upper- or lowercase.
    template <> TT3_DB_API_PUBLIC
    auto toString<tt3::db::api::Oid>(
            const tt3::db::api::Oid & value
        ) -> QString;

    template <> TT3_DB_API_PUBLIC
    auto fromString<tt3::db::api::Oid>(
            const QString & s,
            qsizetype & scan
        ) -> tt3::db::api::Oid;
}

namespace tt3::db::api
{
    TT3_DB_API_PUBLIC size_t qHash(const Oid & key, size_t seed = 0);

    //  The OID; unique per database.
    //  Are based on UUIDs, wich is required for e.g.
    //  merging two databases
    class TT3_DB_API_PUBLIC Oid final
    {
        //////////
        //  Constants
    public:
        //  The special "invalid" OID
        static const Oid    Invalid;

        friend TT3_DB_API_PUBLIC QString tt3::util::toString<Oid>(const Oid & value);
        friend TT3_DB_API_PUBLIC Oid tt3::util::fromString<Oid>(const QString & s, qsizetype & scan);
        friend TT3_DB_API_PUBLIC size_t qHash(const Oid & key, size_t seed);

        //////////
        //  Construction
    private:
        explicit Oid(const QUuid & impl) : _impl(impl) {}

    public:
        //  Constructs an "invalid" OID
        Oid() = default;

        //  Parses a OID string in the same form as returned
        //  by toString(); accepts both upper- and lower-case
        explicit Oid(const QString & oidString) : _impl(oidString) {}

        //  The default copy constructor, destructor
        //  and assignment operator are all OK

        //////////
        //  Operators
    public:
        bool                operator == (const Oid & op2) const { return _impl == op2._impl; }
        bool                operator != (const Oid & op2) const { return _impl != op2._impl; }
        bool                operator <  (const Oid & op2) const { return _impl <  op2._impl; }
        bool                operator <= (const Oid & op2) const { return _impl <= op2._impl; }
        bool                operator >  (const Oid & op2) const { return _impl >  op2._impl; }
        bool                operator >= (const Oid & op2) const { return _impl >= op2._impl; }

        //////////
        //  Operations
    public:
        //  Checks whether this is a "valid" OID
        bool                isValid() const { return _impl != Invalid._impl; }

        //  Generates a highly likely random Oid.
        //  Never generates an "invalid" Oid.
        static Oid          createRandom();

        //////////
        //  Implementation
    private:
        QUuid               _impl;
    };

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
        virtual auto    type() const
                            -> IObjectType * = 0;

        //  The database where the corresponding data object
        //  resides (if live) or used to reside (if dead).
        virtual auto    database() const
                            -> IDatabase * = 0;

        //  The OID of the corresponding data object; can be
        //  safely obtained for both live and dead objects.
        virtual Oid     oid() const = 0;

        //  True if this instance represents an existing data
        //  object residing in a database, else false.
        virtual bool    isLive() const = 0;

        //////////
        //  Operations (life cycle)
    public:
        /// @brief
        ///     Destroys the corresponding database object.
        /// @details
        ///     Delete-cascades as necessary. The instance of
        ///     the IObject - implementing class remains
        ///     in existence, but is marked as "representing
        ///     a dead object" and, therefore, unusable.
        /// @exception DatabaseException
        ///     If an error occurs.
        virtual void    destroy() = 0;

        //////////
        //  Operations (reference counting)
        //  All these operations are thread-safe.
    public:
        //  The current "state" of this object.
        virtual State   state() const = 0;

        //  The current "reference count" of this object.
        virtual int     referenceCount() const = 0;

        //  Increments the "reference count" of this object by 1.
        virtual void    addReference() = 0;

        //  Decrements the "reference count" of this object by 1.
        virtual void    removeReference() = 0;
    };
}

//  End of tt3-db-api/Object.hpp
