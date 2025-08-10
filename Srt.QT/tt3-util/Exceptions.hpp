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
    {
        //////////
        //  Construction/destruction/assignment
    public:
        Exception()
            :   _errorMessage("Unknowqn error") {}
        explicit Exception(const QString & errorMessage)
            :   _errorMessage(errorMessage) {}

        //////////
        //  QException
    public:
        virtual Exception *     clone() const { return new Exception(*this); }
        virtual void            raise() const { throw *this; }

        //////////
        //  Operations
    public:
        //  TODO document
        QString         errorMessage() const { return _errorMessage; }

        //////////
        //  Implementation
    private:
        QString         _errorMessage;
    };
}


//  End of tt3-util/Exceptions.cpp
