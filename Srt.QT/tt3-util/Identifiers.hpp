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
    //  A "mnemonic" is a read-only string that uniquely
    //  identifies a stock object within a set
    //  of similar objects
    class TT3_UTIL_PUBLIC Mnemonic final
    {
        //////////
        //  Constants
    public:
        //  The special "empty string" mnemonic.
        static const Mnemonic Empty;

        //////////
        //  Construction/destruction/assignment
    public:
        Mnemonic() : _impl() {}
        explicit Mnemonic(const QString & s) : _impl(s) {}
        //  The default copy constructor, assignment operator
        //  and destructor are all OK

        //////////
        //  Operators
    public:
        bool        operator == (const Mnemonic & op2) const { return _impl == op2._impl; }
        bool        operator != (const Mnemonic & op2) const { return _impl != op2._impl; }
        bool        operator <  (const Mnemonic & op2) const { return _impl <  op2._impl; }
        bool        operator <= (const Mnemonic & op2) const { return _impl <= op2._impl; }
        bool        operator >  (const Mnemonic & op2) const { return _impl >  op2._impl; }
        bool        operator >= (const Mnemonic & op2) const { return _impl >= op2._impl; }

        //  Extends this Mnemonic with a suffix and returns
        //  the resultibg extended mnemonic
        Mnemonic    operator + (const QString & suffix) const { return Mnemonic(_impl + suffix); }

        //////////
        //  Operations
    public:
        //  Returns the value of this mnemonic as a string.
        QString     toString() const { return _impl; }

        //////////
        //  Implementation
    private:
        QString     _impl;
    };

    //  An identifier of a section within a
    //  resource factory's set of provided resources
    class TT3_UTIL_PUBLIC ResourceSectionId final
    {
        //////////
        //  Constants
    public:
        //  The special "empty string" resource section ID.
        static const ResourceSectionId Empty;

        //////////
        //  Construction/destruction/assignment
    public:
        ResourceSectionId() : _impl() {}
        explicit ResourceSectionId(const QString & s) : _impl(s) {}
        //  The default copy constructor, assignment operator
        //  and destructor are all OK

        //////////
        //  Operators
    public:
        bool        operator == (const ResourceSectionId & op2) const { return _impl == op2._impl; }
        bool        operator != (const ResourceSectionId & op2) const { return _impl != op2._impl; }
        bool        operator <  (const ResourceSectionId & op2) const { return _impl <  op2._impl; }
        bool        operator <= (const ResourceSectionId & op2) const { return _impl <= op2._impl; }
        bool        operator >  (const ResourceSectionId & op2) const { return _impl >  op2._impl; }
        bool        operator >= (const ResourceSectionId & op2) const { return _impl >= op2._impl; }

        //////////
        //  Operations
    public:
        //  Returns the value of this resource section ID as a string.
        QString     toString() const { return _impl; }

        //////////
        //  Implementation
    private:
        QString     _impl;
    };

    //  An identifier of a resource within a
    //  resource section's set of provided resources
    class TT3_UTIL_PUBLIC ResourceId final
    {
        //////////
        //  Constants
    public:
        //  The special "empty string" resource  ID.
        static const ResourceId Empty;

        //////////
        //  Construction/destruction/assignment
    public:
        ResourceId() : _impl() {}
        explicit ResourceId(const QString & s) : _impl(s) {}
        //  The default copy constructor, assignment operator
        //  and destructor are all OK

        //////////
        //  Operators
    public:
        bool        operator == (const ResourceId & op2) const { return _impl == op2._impl; }
        bool        operator != (const ResourceId & op2) const { return _impl != op2._impl; }
        bool        operator <  (const ResourceId & op2) const { return _impl <  op2._impl; }
        bool        operator <= (const ResourceId & op2) const { return _impl <= op2._impl; }
        bool        operator >  (const ResourceId & op2) const { return _impl >  op2._impl; }
        bool        operator >= (const ResourceId & op2) const { return _impl >= op2._impl; }

        //////////
        //  Operations
    public:
        //  Returns the value of this resource ID as a string.
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
