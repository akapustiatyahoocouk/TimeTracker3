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

    /// \class Oid tt3-db-api/API.hpp
    /// \brief
    ///      The OID; unique per database.
    /// \details
    ///     OIDsre based on UUIDs, wich is required for e.g.
    ///     merging two databases
    class TT3_DB_API_PUBLIC Oid final
    {
        friend TT3_DB_API_PUBLIC QString tt3::util::toString<Oid>(const Oid & value);
        friend TT3_DB_API_PUBLIC Oid tt3::util::fromString<Oid>(const QString & s, qsizetype & scan);
        friend TT3_DB_API_PUBLIC size_t qHash(const Oid & key, size_t seed);

        //////////
        //  Constants
    public:
        /// \brief
        ///     The special "invalid" OID.
        static const Oid    Invalid;

        //////////
        //  Construction
    private:
        explicit Oid(const QUuid & impl) : _impl(impl) {}

    public:
        /// \brief
        ///     Constructs an "invalid" OID.
        Oid() = default;

        /// \brief
        ///     Parses a OID string in the same form as returned
        ///     by toString(); accepts both upper- and lower-case.
        /// \details
        ///     If the parsed string is not a valid OID, the result is
        ///     an "invalid" OID.
        /// \param oidString
        ///     The OID string to parse.
        explicit Oid(const QString & oidString) : _impl(oidString) {}

        //  The default copy constructor, destructor
        //  and assignment operator are all OK

        //////////
        //  Operators
    public:
        /// \brief
        ///     Compares two OIDs for equality.
        /// \param op2
        ///     The 2nd OID to compare this one to.
        /// \return
        ///     True if the two OIDs are equal, else false.
        bool            operator == (const Oid & op2) const { return _impl == op2._impl; }

        /// \brief
        ///     Compares two OIDs for inequality.
        /// \param op2
        ///     The 2nd OID to compare this one to.
        /// \return
        ///     False if the two OIDs are equal, else true.
        bool            operator != (const Oid & op2) const { return _impl != op2._impl; }

        /// \brief
        ///     Compares two OIDs for order.
        /// \param op2
        ///     The 2nd OID to compare this one to.
        /// \return
        ///     True if the first OID is numerically
        ///     "less than" the 2nd OID, else false.
        bool            operator <  (const Oid & op2) const { return _impl <  op2._impl; }

        /// \brief
        ///     Compares two OIDs for order.
        /// \param op2
        ///     The 2nd OID to compare this one to.
        /// \return
        ///     True if the first OID is numerically
        ///     "less than or equal to" the 2nd OID, else false.
        bool            operator <= (const Oid & op2) const { return _impl <= op2._impl; }

        /// \brief
        ///     Compares two OIDs for order.
        /// \param op2
        ///     The 2nd OID to compare this one to.
        /// \return
        ///     True if the first OID is numerically
        ///     "greater than" the 2nd OID, else false.
        bool            operator >  (const Oid & op2) const { return _impl >  op2._impl; }

        /// \brief
        ///     Compares two OIDs for order.
        /// \param op2
        ///     The 2nd OID to compare this one to.
        /// \return
        ///     True if the first OID is numerically
        ///     "greater than or equal to" the 2nd OID, else false.
        bool            operator >= (const Oid & op2) const { return _impl >= op2._impl; }

        //////////
        //  Operations
    public:
        /// \brief
        ///     Checks whether this is a "valid" OID.
        /// \return
        ///     True if this is a "valid" OID, else false.
        bool            isValid() const { return _impl != Invalid._impl; }

        /// \brief
        ///     Generates a highly likely random Oid.
        /// \details
        ///     Never generates an "invalid" Oid.
        /// \return
        ///     The newly generated random OID.
        static Oid      createRandom();

        //////////
        //  Implementation
    private:
        QUuid           _impl;
    };

    /// \class IObject tt3-db-api/API.hpp
    /// \brief A generic object that resides, or used to reside, in a database.
    /// \details
    ///     Database object instances are managed
    ///     by their corresponding database.
    ///     An instance of a IObject has an associated
    ///     reference count. Depending on it, that instance can be
    ///     in one of the following states:
    ///     -   New - the instance has just been created; its
    ///         reference count is 0. New instances are not
    ///         recycled.
    ///     -   Managed - a New instance becomes Managed when its
    ///         reference count goes from 0 to 1 for the first
    ///         time. Managed instances are not recycled.
    ///     -   Old - a Managed instance becodes Old when its
    ///         reference count changes from 1 to 0. Old instances
    ///         can be recycled by the database they belong
    ///         to. Changing their reference count from 0 to 1
    ///         again makes them Managed again.
    ///
    ///     Normallty, an IDatabase - implementing classwill act as
    ///     a cache for the real data kepy in the persistent storage,
    ///     At any time only the part of that data can be represented
    ///     by a corresponding IObject instance.
    ///     IMPORTANT: The implementation ensures that no two live
    ///     instanes of a derived class refer to the same database
    ///     object.
    class TT3_DB_API_PUBLIC IObject
    {
        //////////
        //  Types
    public:
        /// \brief
        ///     The possible object's states.
        enum class State
        {
            New,    ///< The instance is not YET reference-counted.
            Managed,///< The instance if reference-counted.
            Old     ///< The instance's reference count has dropped to zero.
        };

        //////////
        //  This is an interface
    protected:
        /// \brief
        ///     The default [interface] constructor.
        IObject() = default;

        /// \brief
        ///     The default [interface] destructor.
        virtual ~IObject() = default;

        //////////
        //  Operations (general)
    public:
        /// \brief
        ///     Returns the type of this database object.
        /// \details
        ///     Can be safely obtained for both live and dead objects.
        /// \return
        ///     The type of this database object.
        virtual auto    type(
                            ) const -> IObjectType * = 0;

        /// \brief
        ///     Returns he database where the corresponding data
        ///     object resides (if live) or used to reside (if dead).
        /// \details
        ///     Can be safely obtained for both live and dead objects.
        /// \return
        ///     The database where the corresponding data object
        ///     resides (if live) or used to reside (if dead).
        virtual auto    database() const
                            -> IDatabase * = 0;

        /// \brief
        ///     Returns the OID of the corresponding data object.
        /// \details
        ///     Can be safely obtained for both live and dead objects.
        /// \return
        ///     The OID of the corresponding data object.
        virtual Oid     oid() const = 0;

        /// \brief
        ///     Checks whether this instance represents an
        ///     existing data object residing in a database.
        /// \return
        ///     True if this instance represents an existing
        ///     data object residing in a database, else false.
        virtual bool    isLive() const = 0;

        //////////
        //  Operations (life cycle)
    public:
        /// \brief
        ///     Destroys the corresponding database object.
        /// \details
        ///     Delete-cascades as necessary. The instance of
        ///     the IObject - implementing class remains
        ///     in existence, but is marked as "representing
        ///     a dead object" and, therefore, unusable.
        /// \exception DatabaseException
        ///     If an error occurs.
        virtual void    destroy() = 0;

        //////////
        //  Operations (reference counting)
        //  All these operations are thread-safe.
    public:
        /// \brief
        ///     Returns the current "state" of this object.
        /// \return
        ///     The current "state" of this object.
        virtual State   state() const = 0;

        /// \brief
        ///     Returns the current "reference count" of this object.
        /// \return
        ///     The current "reference count" of this object.
        virtual int     referenceCount() const = 0;

        /// \brief
        ///     Increments the "reference count" of this object by 1.
        virtual void    addReference() = 0;

        /// \brief
        ///     Decrements the "reference count" of this object by 1.
        virtual void    removeReference() = 0;
    };
}

//  End of tt3-db-api/Object.hpp
