//
//  tt3-util/Exceptions.cpp - Exceptions framework
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
    //  The common base class for all tt3-specific throwable objects.
    using Throwable = QException;

    //  The common base class for all tt3-specific exceptions
    //  (i.e. throwabled intended to be caught).
    class TT3_UTIL_PUBLIC Exception : public Throwable
    {
        //////////
        //  Construction/destruction/assignment
    public:
        Exception() = default;

        //////////
        //  Operations
    public:
        //  Returns the error message describing the
        //  exception for the current default locale
        virtual QString     errorMessage() const = 0;
    };

    //  Thrown when a requested resource is not available
    class TT3_UTIL_PUBLIC MissingResourceException : public Exception
    {
        using Self = MissingResourceException;

        //////////
        //  Construction/destruction/assignment
    public:
        MissingResourceException(
            const QString & resourceFactoryName,
            const ResourceSectionId & sectionId, const ResourceId & resourceId);
        //  The default copy constructor, assignment operator
        //  and destructor are all OK.

        //////////
        //  QException
    public:
        void                raise() const override { throw *this; }
        Self *              clone() const override { return new Self(*this); }

        //////////
        //  Exception
    public:
        virtual QString     errorMessage() const override;

        //////////
        //  Properties
    public:
        //  The name of the resource factory where a resource is missing.
        QString             resourceFactoryName() const { return _resourceFactoryName; }

        //  The ID of the resource section where the resource is missing.
        ResourceSectionId   sectionId() const { return _sectionId; }

        //  The ID of the resource that is missing.
        ResourceId          resourceId() const { return _resourceId; }

        //////////
        //  Implementation
    private:
        QString             _resourceFactoryName;
        ResourceSectionId   _sectionId;
        ResourceId          _resourceId;
    };

    //  Thrown when an attempt to parse a string fails
    class TT3_UTIL_PUBLIC ParseException : public Exception
    {
        using Self = ParseException;

        //////////
        //  Construction/destruction/assignment
    public:
        ParseException(
                const QString & string,
                qsizetype position
            );
        //  The default copy constructor, assignment operator
        //  and destructor are all OK.

        //////////
        //  QException
    public:
        void                raise() const override { throw *this; }
        Self *              clone() const override { return new Self(*this); }

        //////////
        //  Exception
    public:
        virtual QString errorMessage() const override;

        //////////
        //  Operations
    public:
        //  The string which has failed to parse
        QString             string() const { return _string; }

        //  The position within the parsed string where a parse error occurred
        qsizetype           position() const { return _position; }

        //////////
        //  Implementation
    private:
        QString             _string;
        qsizetype           _position;
    };
}

//  End of tt3-util/Exceptions.cpp
