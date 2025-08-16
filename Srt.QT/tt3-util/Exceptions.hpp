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
    //////////
    //  The common base class for all tt3-specific exceptions
    class TT3_UTIL_PUBLIC Exception : public QException
    {   //  TODO make abstract & define subclasses
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
}


//  End of tt3-util/Exceptions.cpp
