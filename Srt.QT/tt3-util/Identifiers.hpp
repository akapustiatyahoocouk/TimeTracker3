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
    class TT3_UTIL_PUBLIC Mnemonic final : public QString
    {
        //////////
        //  Constants
    public:
        //  The special "empty string" mnemonic.
        static const Mnemonic Empty;

        //////////
        //  Construction/destruction/assignment
    public:
        Mnemonic() : QString() {}
        explicit Mnemonic(const QString & s) : QString(s) {}
        //  The default copy constructor, assignment operator
        //  and destructor are all OK

        //////////
        //  Operators
    public:
        bool        operator == (const Mnemonic & op2) const { return this->toString() == op2.toString(); }
        bool        operator != (const Mnemonic & op2) const { return this->toString() == op2.toString(); }
        bool        operator <  (const Mnemonic & op2) const { return this->toString() == op2.toString(); }
        bool        operator <= (const Mnemonic & op2) const { return this->toString() == op2.toString(); }
        bool        operator >  (const Mnemonic & op2) const { return this->toString() == op2.toString(); }
        bool        operator >= (const Mnemonic & op2) const { return this->toString() == op2.toString(); }

        //////////
        //  Operations
    public:
        //  Returns the value of this mnemonic as a string.
        QString     toString() const { return *this; }
    };

    //  An identifier of a section within a
    //  resource factory's set of provided resources
    class TT3_UTIL_PUBLIC ResourceSectionId final : public QString
    {
        //////////
        //  Constants
    public:
        //  The special "empty string" resource section ID.
        static const ResourceSectionId Empty;

        //////////
        //  Construction/destruction/assignment
    public:
        ResourceSectionId() : QString() {}
        explicit ResourceSectionId(const QString & s) : QString(s) {}
        //  The default copy constructor, assignment operator
        //  and destructor are all OK

        //////////
        //  Operators
    public:
        bool        operator == (const Mnemonic & op2) const { return this->toString() == op2.toString(); }
        bool        operator != (const Mnemonic & op2) const { return this->toString() == op2.toString(); }
        bool        operator <  (const Mnemonic & op2) const { return this->toString() == op2.toString(); }
        bool        operator <= (const Mnemonic & op2) const { return this->toString() == op2.toString(); }
        bool        operator >  (const Mnemonic & op2) const { return this->toString() == op2.toString(); }
        bool        operator >= (const Mnemonic & op2) const { return this->toString() == op2.toString(); }

        //////////
        //  Operations
    public:
        //  Returns the value of this mnemonic as a string.
        QString     toString() const { return *this; }
    };

    //  An identifier of a resource within a
    //  resource section's set of provided resources
    class TT3_UTIL_PUBLIC ResourceId final : public QString
    {
        //////////
        //  Constants
    public:
        //  The special "empty string" resource  ID.
        static const ResourceId Empty;

        //////////
        //  Construction/destruction/assignment
    public:
        ResourceId() : QString() {}
        explicit ResourceId(const QString & s) : QString(s) {}
        //  The default copy constructor, assignment operator
        //  and destructor are all OK

        //////////
        //  Operators
    public:
        bool        operator == (const Mnemonic & op2) const { return this->toString() == op2.toString(); }
        bool        operator != (const Mnemonic & op2) const { return this->toString() == op2.toString(); }
        bool        operator <  (const Mnemonic & op2) const { return this->toString() == op2.toString(); }
        bool        operator <= (const Mnemonic & op2) const { return this->toString() == op2.toString(); }
        bool        operator >  (const Mnemonic & op2) const { return this->toString() == op2.toString(); }
        bool        operator >= (const Mnemonic & op2) const { return this->toString() == op2.toString(); }

        //////////
        //  Operations
    public:
        //  Returns the value of this mnemonic as a string.
        QString     toString() const { return *this; }
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
