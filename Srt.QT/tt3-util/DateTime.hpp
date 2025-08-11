//
//  tt3-util/DateTime.cpp - Date/time helpers
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
    //  A time interval
    class TT3_UTIL_PUBLIC TimeSpan
    {
        //////////
        //  Construction/destruction/assignment
    private:
        explicit TimeSpan(int minutes) : _minutes(minutes) {}
    public:
        TimeSpan() : _minutes(_InvalidMinutes) {}

        static TimeSpan hours(int h);
        static TimeSpan minutes(int m);

        //////////
        //  Operators
    public:
        //  An invalid time span compares "less than"
        //  any valid time span
        bool        operator == (const TimeSpan & op2) const { return _minutes == op2._minutes; }
        bool        operator != (const TimeSpan & op2) const { return _minutes != op2._minutes; }
        bool        operator <  (const TimeSpan & op2) const { return _minutes <  op2._minutes; }
        bool        operator <= (const TimeSpan & op2) const { return _minutes <= op2._minutes; }
        bool        operator >  (const TimeSpan & op2) const { return _minutes >  op2._minutes; }
        bool        operator >= (const TimeSpan & op2) const { return _minutes >= op2._minutes; }

        //  TODO document
        TimeSpan    operator -  () const;
        TimeSpan    operator +  (const TimeSpan & op2) const;
        TimeSpan    operator -  (const TimeSpan & op2) const;
        TimeSpan &  operator += (const TimeSpan & op2);
        TimeSpan &  operator -= (const TimeSpan & op2);

        //////////
        //  Operations
    public:
        //  TODO document
        bool        isValid() const { return _minutes != _InvalidMinutes; }
        int         asHours() const { Q_ASSERT(isValid()); return _minutes / 60; }
        int         asMinutes() const { Q_ASSERT(isValid()); return _minutes; }

        //////////
        //  Implementation
    private:
        static const int _InvalidMinutes = INT_MIN;
        int         _minutes;
    };
}

//  End of tt3-util/Plugin.cpp
