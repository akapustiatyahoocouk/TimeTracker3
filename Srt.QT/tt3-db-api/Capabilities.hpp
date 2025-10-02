//
//  tt3-db-api/Capabilities.hpp - login account capabilities
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
    /// \brief
    ///     Capabilities of a login account.
    enum class Capability
    {
        Administrator = 0x0001,
        ManageUsers = 0x0002,
        ManageActivityTypes = 0x0004,
        ManageBeheficiaries = 0x0008,
        ManageWorkloads = 0x0010,
        ManagePublicActivities = 0x0020,
        ManagePublicTasks = 0x0040,
        ManagePrivateActivities = 0x0080,
        ManagePrivateTasks = 0x0100,
        LogWork = 0x0200,
        LogEvents = 0x0400,
        GenerateReports = 0x0800,
        BackupAndRestore = 0x1000,
    /*  TODO kill off
        //  Flag combinatons
        None = 0x0000,
        All = 0x1FFF
    */
    };

    /// \class Capabilities tt3-db-api/API.hpp
    /// \brief A "set of Capability flags" ADT.
    /// \details
    ///     Implemented as a bit set for better performance.
    class TT3_DB_API_PUBLIC Capabilities final
    {
        //////////
        //  Construction/destruction/assignment
    private:
        explicit Capabilities(int mask) : _mask(mask) {}
    public:
        /// \brief
        ///     Constructs an empty set of Capability flags.
        constexpr Capabilities() : _mask(0) {}

        /// \brief
        ///     Constructs a set with a single Capability flag.
        /// \param c
        ///     The Capability flag to initialize the set with.
        constexpr Capabilities(Capability c) : _mask(int(c)) {}

        //  The default copy constructor, destructor anjd
        //  assignment are all OK

        //////////
        //  Operators
    public:
        /// \brief
        ///     Compares two Capability sets for equality.
        /// \param op2
        ///     The 2nd Capability set to compare this one with.
        /// \return
        ///     True if the two Capability sets are equal, else false.
        bool            operator == (const Capabilities & op2) const { return _mask == op2._mask; }

        /// \brief
        ///     Compares two Capability sets for inequality.
        /// \param op2
        ///     The 2nd Capability set to compare this one with.
        /// \return
        ///     False if the two Capability sets are equal, else true.
        bool            operator != (const Capabilities & op2) const { return _mask != op2._mask; }

        /// \brief
        ///     Calculates a union of two Capabilitty sets.
        /// \param op2
        ///     The 2nd Capability set.
        /// \return
        ///     The union of two Capabilitty sets.
        Capabilities    operator |(const Capabilities & op2) const
        {
            return Capabilities(_mask | op2._mask);
        }

        /// \brief
        ///     Calculates a union of two Capabilitty sets.
        /// \param op2
        ///     The 2nd Capability set.
        /// \return
        ///     The *this, whose value has been updated to
        ///     be the union of two Capabilitty sets.
        Capabilities &  operator |=(const Capabilities & op2)
        {
            _mask |= op2._mask;
            return *this;
        }

        /// \brief
        ///     Calculates an intersection of two Capabilitty sets.
        /// \param op2
        ///     The 2nd Capability set.
        /// \return
        ///     The intersection of two Capabilitty sets.
        Capabilities    operator &(const Capabilities & op2) const
        {
            return Capabilities(_mask & op2._mask);
        }

        /// \brief
        ///     Calculates an intersection of two Capabilitty sets.
        /// \param op2
        ///     The 2nd Capability set.
        /// \return
        ///     The *this, whose value has been updated to
        ///     be the intersection of two Capabilitty sets.
        Capabilities &  operator &=(const Capabilities & op2)
        {
            _mask &= op2._mask;
            return *this;
        }

        //////////
        //  Operations
    public:
        /// \brief
        ///     Checks if this capability set is empty.
        /// \return
        ///     True if this capability set is empty, false if not.
        constexpr bool  isEmpty() const
        {
            return _mask == 0;
        }

        /// \brief
        ///     Checks if this Capability set contains the
        ///     specified Capability.
        /// \param c
        ///     The Capability to check.
        /// \return
        ///     True if this Capability set contains the
        ///     specified Capability, else false.
        constexpr bool  contains(Capability c) const
        {
            return (_mask & int(c)) != 0;
        }

        //////////
        //  Implementation
    private:
        int             _mask;
    };

    /// \brief
    ///     Returns the Capability set that contains
    ///     all of the specified Capabilities.
    /// \param a
    ///     The Capability to include into the set.
    /// \param b
    ///     The Capability to include into the set.
    /// \return
    ///     The Capability set that contains all of
    ///     the specified Capabilities.
    TT3_DB_API_PUBLIC
    inline Capabilities operator |(Capability a, Capability b)
    {
        return Capabilities(a) | Capabilities(b);
    }
}

//  Formatting/parsing
namespace tt3::util
{
    template <> TT3_DB_API_PUBLIC
    QString toString<tt3::db::api::Capabilities>(
            const tt3::db::api::Capabilities & value
        );

    template <> TT3_DB_API_PUBLIC
    auto fromString<tt3::db::api::Capabilities>(
            const QString & s,
            qsizetype & scan
        ) -> tt3::db::api::Capabilities;
}

//  End of tt3-db-api/Capabilities.hpp
