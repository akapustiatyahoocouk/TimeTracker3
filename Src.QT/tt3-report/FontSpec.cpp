//
//  tt3-report/FontSpec.cpp - tt3::report::FontSpec class implementation
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
const FontSpec FontSpec::Default("");
const FontSpec FontSpec::Serif("serif");
const FontSpec FontSpec::SansSerif("sans-serif");
const FontSpec FontSpec::Monospaced("monospace");

//////////
//  Construction/destruction/assignment
FontSpec::FontSpec()
    :   _name()
{
}

FontSpec::FontSpec(const QString & name)
    :   _name(name)
{
}

//////////
//  Operators
bool FontSpec::operator == (const FontSpec & op2) const
{
    return QString::compare(_name, op2._name, Qt::CaseInsensitive) == 0;
}

bool FontSpec::operator != (const FontSpec & op2) const
{
    return QString::compare(_name, op2._name, Qt::CaseInsensitive) != 0;
}

bool FontSpec::operator <  (const FontSpec & op2) const
{
    return QString::compare(_name, op2._name, Qt::CaseInsensitive) < 0;
}

bool FontSpec::operator <= (const FontSpec & op2) const
{
    return QString::compare(_name, op2._name, Qt::CaseInsensitive) <= 0;
}

bool FontSpec::operator >  (const FontSpec & op2) const
{
    return QString::compare(_name, op2._name, Qt::CaseInsensitive) > 0;
}

bool FontSpec::operator >= (const FontSpec & op2) const
{
    return QString::compare(_name, op2._name, Qt::CaseInsensitive) >= 0;
}

//////////
//  Operations
bool FontSpec::isDefault() const
{
    return _name.isEmpty();
}

bool FontSpec::isFontClassName() const
{
    return _name == Serif._name ||
           _name == SansSerif._name ||
           _name == Monospaced._name;
}

bool FontSpec::isFontName() const
{
    return _name != Default._name &&
           _name != Serif._name &&
           _name != SansSerif._name &&
           _name != Monospaced._name;
}

//  Formatting/parsing
namespace tt3::util
{
    template <> TT3_REPORT_PUBLIC
    QString toString<FontSpec>(
            const FontSpec & value
        )
    {
        return toString(value.name());
    }

    template <> TT3_REPORT_PUBLIC
    auto fromString<FontSpec>(
            const QString & s,
            qsizetype & scan
        ) -> FontSpec
    {
        QString name = fromString<QString>(s, scan);
        return FontSpec(name);
    }
}

//  End of tt3-report/FontSpec.cpp
