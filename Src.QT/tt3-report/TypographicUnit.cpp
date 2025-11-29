//
//  tt3-report/TypographicUnit.cpp - tt3::report::TypographicUnit class implementation
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
#include "tt3-report/API.hpp"
using namespace tt3::report;

//////////
//  Constants
const TypographicUnit TypographicUnit::Centimeter(28.3464567f);
const TypographicUnit TypographicUnit::Millimeter(2.83464567f);
const TypographicUnit TypographicUnit::Inch(72.0);
const TypographicUnit TypographicUnit::Pica(12.0);
const TypographicUnit TypographicUnit::Point(1.0);

//////////
//  Construction/destruction/assignment
TypographicUnit::TypographicUnit(float points)
    :   _points(points)
{
}

//////////
//  Operators
bool TypographicUnit::operator == (const TypographicUnit & op2) const
{
    return _points == op2._points;
}

bool TypographicUnit::operator != (const TypographicUnit & op2) const
{
    return _points != op2._points;
}

bool TypographicUnit::operator <  (const TypographicUnit & op2) const
{
    return _points < op2._points;
}

bool TypographicUnit::operator <= (const TypographicUnit & op2) const
{
    return _points <= op2._points;
}

bool TypographicUnit::operator >  (const TypographicUnit & op2) const
{
    return _points > op2._points;
}

bool TypographicUnit::operator >= (const TypographicUnit & op2) const
{
    return _points >= op2._points;
}

//  Formatting/parsing
namespace tt3::util
{
    template <> TT3_REPORT_PUBLIC
    QString toString<tt3::report::TypographicUnit>(
            const tt3::report::TypographicUnit & value
        )
    {
        if (value == TypographicUnit::Point)
        {   //  1st, as expected to be the most common
            return "pt";
        }
        if (value == TypographicUnit::Pica)
        {
            return "pc";
        }
        if (value == TypographicUnit::Inch)
        {
            return "in";
        }
        if (value == TypographicUnit::Millimeter)
        {
            return "mm";
        }
        if (value == TypographicUnit::Centimeter)
        {
            return "cm";
        }
        //  Give up
        return "pt";
    }

    template <> TT3_REPORT_PUBLIC
    auto fromString<tt3::report::TypographicUnit>(
            const QString & s,
            qsizetype & scan
        ) -> tt3::report::TypographicUnit
    {
        //  All unit codes are 2 characters long
        if (scan < 0 || scan + 2 > s.length())
        {   //  OOPS!
            throw ParseException(s, scan);
        }
        //  General case
        if (s[scan] == 'c' && s[scan + 1] == 'm')
        {
            scan += 2;
            return TypographicUnit::Centimeter;
        }
        if (s[scan] == 'm' && s[scan + 1] == 'm')
        {
            scan += 2;
            return TypographicUnit::Millimeter;
        }
        if (s[scan] == 'i' && s[scan + 1] == 'n')
        {
            scan += 2;
            return TypographicUnit::Inch;
        }
        if (s[scan] == 'p' && s[scan + 1] == 'c')
        {
            scan += 2;
            return TypographicUnit::Pica;
        }
        if (s[scan] == 'p' && s[scan + 1] == 't')
        {
            scan += 2;
            return TypographicUnit::Point;
        }
        //  Give up
        throw ParseException(s, scan);
    }
}

//  End of tt3-report/TypographicUnit.cpp
