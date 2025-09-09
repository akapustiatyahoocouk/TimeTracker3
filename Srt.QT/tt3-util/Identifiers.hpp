//
//  tt3-util/Identifiers.cpp - Various identifier ADTs
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

namespace tt3::util
{
    /// \class Mnemonic tt3-util/API.hpp "tt3-util/API.hpp"
    /// \brief
    ///     A "mnemonic" is a read-only string that uniquely
    //      identifies a stock object within a set of similar objects
    class TT3_UTIL_PUBLIC Mnemonic final
    {
        //////////
        //  Constants
    public:
        /// \brief
        ///     The special "empty string" mnemonic.
        static const Mnemonic Empty;

        //////////
        //  Construction/destruction/assignment
    public:
        /// \brief
        ///     Constructs an "empty" mnemonic.
        Mnemonic() : _impl() {}

        /// \brief
        ///     Constructs a Mnemonic from the specified string.
        /// \param s
        ///     The string.
        explicit Mnemonic(const QString & s) : _impl(s) {}

        //  The default copy constructor, assignment operator
        //  and destructor are all OK

        //////////
        //  Operators
    public:
        /// \brief
        ///     Compares two Mnemonics for equality.
        /// \param op2
        ///     The 2nd Mnemonic to compare this one with.
        /// \return
        ///     True if the two mnemonics are equal, else false.
        bool        operator == (const Mnemonic & op2) const { return _impl == op2._impl; }

        /// \brief
        ///     Compares two Mnemonics for nequality.
        /// \param op2
        ///     The 2nd Mnemonic to compare this one with.
        /// \return
        ///     False if the two mnemonics are equal, else true.
        bool        operator != (const Mnemonic & op2) const { return _impl != op2._impl; }

        /// \brief
        ///     Compares two Mnemonics for order.
        /// \param op2
        ///     The 2nd Mnemonic to compare this one with.
        /// \return
        ///     True if this Mnemonic is lexicographically
        ///     "less than" the 2nd one, else false.
        bool        operator <  (const Mnemonic & op2) const { return _impl <  op2._impl; }

        /// \brief
        ///     Compares two Mnemonics for order.
        /// \param op2
        ///     The 2nd Mnemonic to compare this one with.
        /// \return
        ///     True if this Mnemonic is lexicographically
        ///     "less than or equal to" the 2nd one, else false.
        bool        operator <= (const Mnemonic & op2) const { return _impl <= op2._impl; }

        /// \brief
        ///     Compares two Mnemonics for order.
        /// \param op2
        ///     The 2nd Mnemonic to compare this one with.
        /// \return
        ///     True if this Mnemonic is lexicographically
        ///     "greater than" the 2nd one, else false.
        bool        operator >  (const Mnemonic & op2) const { return _impl >  op2._impl; }

        /// \brief
        ///     Compares two Mnemonics for order.
        /// \param op2
        ///     The 2nd Mnemonic to compare this one with.
        /// \return
        ///     True if this Mnemonic is lexicographically
        ///     "greater than or equal to" the 2nd one, else false.
        bool        operator >= (const Mnemonic & op2) const { return _impl >= op2._impl; }

        /// \brief
        ///     Extends this Mnemonic with a suffix and returns
        ///     the resultibg extended mnemonic
        /// \param suffix
        ///     The suffix to add to this Mnemonic to form a result.
        Mnemonic    operator + (const QString & suffix) const { return Mnemonic(_impl + suffix); }

        //////////
        //  Operations
    public:
        /// \brief
        ///     Returns the value of this mnemonic as a string.
        /// \return
        ///     The value of this mnemonic as a string.
        QString     toString() const { return _impl; }

        //////////
        //  Implementation
    private:
        QString     _impl;
    };

    /// \class ResourceSectionId tt3-util/API.hpp "tt3-util/API.hpp"
    /// \brief
    ///     An identifier of a section within a
    ///     resource factory's set of provided resources
    class TT3_UTIL_PUBLIC ResourceSectionId final
    {
        //////////
        //  Constants
    public:
        /// \brief
        ///     The special "empty" resource section ID,
        ///     guaranteed not to refer to any valid resource section.
        static const ResourceSectionId Empty;

        //////////
        //  Construction/destruction/assignment
    public:
        /// \brief
        ///     Constructs an "empty" resource section ID.
        ResourceSectionId() : _impl() {}

        /// \brief
        ///     Constructs a resource section ID from
        ///     the specified string.
        /// \param s
        ///     The string.
        explicit ResourceSectionId(const QString & s) : _impl(s) {}

        //  The default copy constructor, assignment operator
        //  and destructor are all OK

        //////////
        //  Operators
    public:
        /// \brief
        ///     Compares two ResourceSectionIds for equality.
        /// \param op2
        ///     The 2nd ResourceSectionId to compare this one with.
        /// \return
        ///     True if the two ResourceSectionIds are equal, else false.
        bool        operator == (const ResourceSectionId & op2) const { return _impl == op2._impl; }

        /// \brief
        ///     Compares two ResourceSectionIds for inequality.
        /// \param op2
        ///     The 2nd ResourceSectionId to compare this one with.
        /// \return
        ///     False if the two ResourceSectionIds are equal, else true.
        bool        operator != (const ResourceSectionId & op2) const { return _impl != op2._impl; }

        /// \brief
        ///     Compares two ResourceSectionIds for order.
        /// \param op2
        ///     The 2nd ResourceSectionId to compare this one with.
        /// \return
        ///     True if this ResourceSectionId is lexicographically
        ///     "less than" the 2nd one, else false.
        bool        operator <  (const ResourceSectionId & op2) const { return _impl <  op2._impl; }

        /// \brief
        ///     Compares two ResourceSectionIds for order.
        /// \param op2
        ///     The 2nd ResourceSectionId to compare this one with.
        /// \return
        ///     True if this ResourceSectionId is lexicographically
        ///     "less than or equal to" the 2nd one, else false.
        bool        operator <= (const ResourceSectionId & op2) const { return _impl <= op2._impl; }

        /// \brief
        ///     Compares two ResourceSectionIds for order.
        /// \param op2
        ///     The 2nd ResourceSectionId to compare this one with.
        /// \return
        ///     True if this ResourceSectionId is lexicographically
        ///     "greater than" the 2nd one, else false.
        bool        operator >  (const ResourceSectionId & op2) const { return _impl >  op2._impl; }

        /// \brief
        ///     Compares two ResourceSectionIds for order.
        /// \param op2
        ///     The 2nd ResourceSectionId to compare this one with.
        /// \return
        ///     True if this ResourceSectionId is lexicographically
        ///     "greater than or equal to" the 2nd one, else false.
        bool        operator >= (const ResourceSectionId & op2) const { return _impl >= op2._impl; }

        //////////
        //  Operations
    public:
        /// \brief
        ///     Returns the value of this resource section ID as a string.
        /// \return
        ///     The value of this resource section ID as a string.
        QString     toString() const { return _impl; }

        //////////
        //  Implementation
    private:
        QString     _impl;
    };

    /// \class ResourceId tt3-util/API.hpp "tt3-util/API.hpp"
    /// \brief
    ///     An identifier of a resource within a
    ///     resource section's set of provided resources
    class TT3_UTIL_PUBLIC ResourceId final
    {
        //////////
        //  Constants
    public:
        /// \brief
        ///     The special "empty string" resource  ID,
        ///     guaranteed to not refer to any resource.
        static const ResourceId Empty;

        //////////
        //  Construction/destruction/assignment
    public:
        /// \brief
        ///     Constructs an "empty" resource ID.
        ResourceId() : _impl() {}

        /// \brief
        ///     Constructs the resource ID from the specified string.
        /// \param s
        ///     The string.
        explicit ResourceId(const QString & s) : _impl(s) {}

        //  The default copy constructor, assignment operator
        //  and destructor are all OK

        //////////
        //  Operators
    public:
        /// \brief
        ///     Compares two ResourceIds for equality.
        /// \param op2
        ///     The 2nd ResourceId to compare this one with.
        /// \return
        ///     True if the two ResourceIds are equal, else false.
        bool        operator == (const ResourceId & op2) const { return _impl == op2._impl; }

        /// \brief
        ///     Compares two ResourceIds for inequality.
        /// \param op2
        ///     The 2nd ResourceId to compare this one with.
        /// \return
        ///     False if the two ResourceIds are equal, else true.
        bool        operator != (const ResourceId & op2) const { return _impl != op2._impl; }

        /// \brief
        ///     Compares two ResourceIds for order.
        /// \param op2
        ///     The 2nd ResourceId to compare this one with.
        /// \return
        ///     True if this ResourceId is lexicographically
        ///     "less than" the 2nd one, else false.
        bool        operator <  (const ResourceId & op2) const { return _impl <  op2._impl; }

        /// \brief
        ///     Compares two ResourceIds for order.
        /// \param op2
        ///     The 2nd ResourceId to compare this one with.
        /// \return
        ///     True if this ResourceId is lexicographically
        ///     "less than or equal to" the 2nd one, else false.
        bool        operator <= (const ResourceId & op2) const { return _impl <= op2._impl; }

        /// \brief
        ///     Compares two ResourceIds for order.
        /// \param op2
        ///     The 2nd ResourceId to compare this one with.
        /// \return
        ///     True if this ResourceId is lexicographically
        ///     "greater than" the 2nd one, else false.
        bool        operator >  (const ResourceId & op2) const { return _impl >  op2._impl; }

        /// \brief
        ///     Compares two ResourceIds for order.
        /// \param op2
        ///     The 2nd ResourceId to compare this one with.
        /// \return
        ///     True if this ResourceId is lexicographically
        ///     "greater than or eqial to" the 2nd one, else false.
        bool        operator >= (const ResourceId & op2) const { return _impl >= op2._impl; }

        //////////
        //  Operations
    public:
        /// \brief
        ///     Returns the value of this resource ID as a string.
        /// \return
        ///     The value of this resource ID as a string.
        QString     toString() const { return _impl; }

        //////////
        //  Implementation
    private:
        QString     _impl;
    };
}

//  Helper macros for creation of identifiers
#define M(value)    tt3::util::Mnemonic(#value)
#define RSID(value) tt3::util::ResourceSectionId(#value)
#define RID(value)  tt3::util::ResourceId(#value)

//////////
//  Helper algorithms
inline size_t qHash(const tt3::util::Mnemonic & key, size_t seed = 0)
{
    return qHash(key.toString(), seed);
}

inline size_t qHash(const tt3::util::ResourceSectionId & key, size_t seed = 0)
{
    return qHash(key.toString(), seed);
}

inline size_t qHash(const tt3::util::ResourceId & key, size_t seed = 0)
{
    return qHash(key.toString(), seed);
}

//  End of tt3-util/Identifiers.cpp
