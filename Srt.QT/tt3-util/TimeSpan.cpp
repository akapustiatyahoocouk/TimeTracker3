//
//  tt3-util/TimeSpan.cpp - tt3::util::TimeSpan class implementation
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
#include "tt3-util/API.hpp"
using namespace tt3::util;

//////////
//  Construction/destruction/assignment
TimeSpan TimeSpan::hours(int h)
{
    int m = h * 60;
    if (m / 60 == h)
    {
        m = _InvalidMinutes;
    }
    return TimeSpan(m);
}

TimeSpan TimeSpan::minutes(int m)
{
    Q_ASSERT(m != _InvalidMinutes);
    return TimeSpan(m);
}

//////////
//  Operators
TimeSpan TimeSpan::operator -  () const
{
    return TimeSpan(-_minutes); //  -INT_MAX == INT_MAX
}

/*  TODO implement
TimeSpan TimeSpan::operator +  (const TimeSpan & op2) const;
TimeSpan TimeSpan::operator -  (const TimeSpan & op2) const;
TimeSpan & TimeSpan::operator += (const TimeSpan & op2);
TimeSpan & TimeSpan::operator -= (const TimeSpan & op2);
*/

//  End of tt3-util/TimeSpan.cpp
