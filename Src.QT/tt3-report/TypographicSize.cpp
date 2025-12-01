//
//  tt3-report/TypographicSize.cpp - tt3::report::TypographicSize class implementation
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
TypographicSize::TypographicSize()
    :   _amount(12.0),
        _unit(TypographicUnit::Point)
{
}

TypographicSize::TypographicSize(float amount, const TypographicUnit & unit)
    :   _amount(amount),
        _unit(unit)
{
}

auto TypographicSize::cm(float amount) -> TypographicSize
{
    return TypographicSize(amount, TypographicUnit::Centimeter);
}

auto TypographicSize::mm(float amount) -> TypographicSize
{
    return TypographicSize(amount, TypographicUnit::Millimeter);
}

auto TypographicSize::in(float amount) -> TypographicSize
{
    return TypographicSize(amount, TypographicUnit::Inch);
}

auto TypographicSize::pc(float amount) -> TypographicSize
{
    return TypographicSize(amount, TypographicUnit::Pica);
}

auto TypographicSize::pt(float amount) -> TypographicSize
{
    return TypographicSize(amount, TypographicUnit::Point);
}

//////////
//  Operators
bool TypographicSize::operator == (const TypographicSize & op2) const
{
    return pointSize() == op2.pointSize();
}

bool TypographicSize::operator != (const TypographicSize & op2) const
{
    return pointSize() != op2.pointSize();
}

bool TypographicSize::operator <  (const TypographicSize & op2) const
{
    return pointSize() < op2.pointSize();
}

bool TypographicSize::operator <= (const TypographicSize & op2) const
{
    return pointSize() <= op2.pointSize();
}

bool TypographicSize::operator >  (const TypographicSize & op2) const
{
    return pointSize() > op2.pointSize();
}

bool TypographicSize::operator >= (const TypographicSize & op2) const
{
    return pointSize() >= op2.pointSize();
}

//  Formatting/parsing
namespace tt3::util
{
    template <> TT3_REPORT_PUBLIC
    QString toString<TypographicSize>(
            const TypographicSize & value
        )
    {
        return toString(value.amount()) + toString(value.unit());
    }

    template <> TT3_REPORT_PUBLIC
    auto fromString<TypographicSize>(
            const QString & s,
            qsizetype & scan
        ) -> TypographicSize
    {
        qsizetype prescan = scan;
        auto amount = fromString<float>(s, prescan);    //  may throw
        auto unit = fromString<TypographicUnit>(s, prescan);    //  may throw
        scan = prescan;
        return TypographicSize(amount, unit);
    }
}

//  End of tt3-report/TypographicSize.cpp
