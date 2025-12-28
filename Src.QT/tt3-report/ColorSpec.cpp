//
//  tt3-report/ColorSpec.cpp - tt3::report::ColorSpec class implementation
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
//  Construction/destruction/assignment
ColorSpec::ColorSpec()
    :   _colorClass(ColorClass::Default),
        _customColor(0, 0, 0, 0)
{
}

ColorSpec::ColorSpec(ColorClass colorClass)
    :   _colorClass(colorClass),
        _customColor(0, 0, 0, 0)
{
}

ColorSpec::ColorSpec(const QColor & customColor)
    :   _colorClass(ColorClass::Custom),
        _customColor(customColor)
{
}

ColorSpec::ColorSpec(int r, int g, int b, int a)
    :   _colorClass(ColorClass::Custom),
        _customColor(r, g, b, a)
{
}

//////////
//  Operators
bool ColorSpec::operator == (const ColorSpec & op2) const
{
    if (_colorClass != op2._colorClass)
    {   //  Color classes differ
        return false;
    }
    else if (_colorClass == ColorClass::Custom)
    {   //  Custom colors must match
        return _customColor == op2._customColor;
    }
    else
    {   //  All OK
        return true;
    }
}

bool ColorSpec::operator != (const ColorSpec & op2) const
{
    return !(*this == op2);
}

//  Formatting/parsing
namespace tt3::util
{
    template <> TT3_REPORT_PUBLIC
    QString toString<ColorSpec>(
            const ColorSpec & value
        )
    {
        switch (value.colorClass())
        {
            case ColorSpec::ColorClass::Default:
                return "default";
            case ColorSpec::ColorClass::Transparent:
                return "transparent";
            case ColorSpec::ColorClass::Custom:
            default:    //  be defensive in release mode
                return toString(value.customColor());
        }
    }

    template <> TT3_REPORT_PUBLIC
    auto fromString<ColorSpec>(
            const QString & s,
            qsizetype & scan
        ) -> ColorSpec
    {
        if (scan < 0 || scan > s.length())
        {   //  OOPS!
            throw ParseException(s, scan);
        }
        if (s.mid(scan, 7).compare("default", Qt::CaseInsensitive) == 0)
        {
            scan += 7;
            return ColorSpec::ColorClass::Default;
        }
        if (s.mid(scan, 11).compare("transparent", Qt::CaseInsensitive) == 0)
        {
            scan += 11;
            return ColorSpec::ColorClass::Transparent;
        }
        return fromString<QColor>(s, scan); //  may throw
    }
}

//  End of tt3-report/ColorSpec.cpp
