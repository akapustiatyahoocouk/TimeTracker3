//
//  tt3-util/ToString.cpp - Value formatting framework
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

//  C++ types
template <> TT3_UTIL_PUBLIC QString tt3::util::toString<nullptr_t>(const nullptr_t & /*value*/)
{
    return "(null)";
}

template <> TT3_UTIL_PUBLIC QString tt3::util::toString<bool>(const bool & value)
{
    return value ? "true" : "false";
}

template <> TT3_UTIL_PUBLIC QString tt3::util::toString<char>(const char & value)
{
    return QString(QChar(value));
}

template <> TT3_UTIL_PUBLIC QString tt3::util::toString<signed char>(const signed char & value)
{
    return toString<signed long long>(value);
}

template <> TT3_UTIL_PUBLIC QString tt3::util::toString<unsigned char>(const unsigned char & value)
{
    return toString<unsigned long long>(value);
}

template <> TT3_UTIL_PUBLIC QString tt3::util::toString<signed short>(const signed short & value)
{
    return toString<signed long long>(value);
}

template <> TT3_UTIL_PUBLIC QString tt3::util::toString<unsigned short>(const unsigned short & value)
{
    return toString<unsigned long long>(value);
}

template <> TT3_UTIL_PUBLIC QString tt3::util::toString<signed int>(const signed int & value)
{
    return toString<signed long long>(value);
}

template <> TT3_UTIL_PUBLIC QString tt3::util::toString<unsigned int>(const unsigned int & value)
{
    return toString<unsigned long long>(value);
}

template <> TT3_UTIL_PUBLIC QString tt3::util::toString<signed long>(const signed long & value)
{
    return toString<signed long long>(value);
}

template <> TT3_UTIL_PUBLIC QString tt3::util::toString<unsigned long>(const unsigned long & value)
{
    return toString<unsigned long long>(value);
}

template <> TT3_UTIL_PUBLIC QString tt3::util::toString<signed long long>(const signed long long & value)
{
    char s[64];
    sprintf(s, "%lld", value);
    return s;
}

template <> TT3_UTIL_PUBLIC QString tt3::util::toString<unsigned long long>(const unsigned long long & value)
{
    char s[64];
    sprintf(s, "%llu", value);
    return s;
}

template <> TT3_UTIL_PUBLIC QString tt3::util::toString<float>(const float & value)
{
    return toString<double>(value);
}

template <> TT3_UTIL_PUBLIC QString tt3::util::toString<double>(const double & value)
{
    switch (std::fpclassify(value))
    {
        case FP_INFINITE:
            return (value > 0.0) ? "#+INF" : "#-INF";
        case FP_NAN:
            return "#QNAN";
        case FP_NORMAL:
        case FP_SUBNORMAL:
        case FP_ZERO:
            default:
                char s[128];
                sprintf(s, "%g", value);
                return s;
    }
}

//  QT types
template <> TT3_UTIL_PUBLIC QString tt3::util::toString<QChar>(const QChar & value)
{
    return QString(value);
}

template <> TT3_UTIL_PUBLIC QString tt3::util::toString<QString>(const QString & value)
{
    return value;
}

template <> TT3_UTIL_PUBLIC QString tt3::util::toString<QRect>(const QRect & value)
{
    char s[128];

    sprintf(s, "(%d,%d)-(%d,%d)",
            value.left(), value.top(),
            value.left() + value.width(), value.top() + value.height());
    return s;
}

template <> TT3_UTIL_PUBLIC QString tt3::util::toString<QVersionNumber>(const QVersionNumber & value)
{
    return toString(value.majorVersion()) + "." +
           toString(value.minorVersion()) + "." +
           toString(value.microVersion());
}

template <> TT3_UTIL_PUBLIC QString tt3::util::toString<QLocale>(const QLocale & value)
{
    return (value == QLocale::c()) ? "C" : value.name();
}

template <> TT3_UTIL_PUBLIC QString tt3::util::toString<QColor>(const QColor & value)
{
    if (value.isValid())
    {
        if (value.alpha() == 0)
        {   //  transparent
            return "transparent";
        }
        else if (value.alpha() == 255)
        {   //  opaque
            return value.name(QColor::NameFormat::HexRgb);
        }
        else
        {   //  semi-transparent
            return value.name(QColor::NameFormat::HexArgb);
        }
    }
    else
    {
        return "invalid";
    }
}

template <> TT3_UTIL_PUBLIC QString tt3::util::toString<QDateTime>(const QDateTime & value)
{
    if (value.isValid())
    {
        char s[32];
        sprintf(s, "%04d%02d%02dT%02d%02d%02d.%03d",
                value.date().year(),
                value.date().month(),
                value.date().day(),
                value.time().hour(),
                value.time().minute(),
                value.time().second(),
                value.time().msec());
        return s;
    }
    return "-";
}

template <> TT3_UTIL_PUBLIC QString tt3::util::toString<QDate>(const QDate & value)
{
    if (value.isValid())
    {
        char s[16];
        sprintf(s, "%04d%02d%02d",
                value.year(),
                value.month(),
                value.day());
        return s;
    }
    return "-";
}

//  tt3::util types
template <> TT3_UTIL_PUBLIC QString tt3::util::toString<tt3::util::TimeSpan>(const TimeSpan & value)
{
    return value.isValid() ? toString(value.asMinutes()) : "?";
}

template <> TT3_UTIL_PUBLIC QString tt3::util::toString<tt3::util::Mnemonic>(const Mnemonic & value)
{
    return value.toString();
}

template <> TT3_UTIL_PUBLIC QString tt3::util::toString<tt3::util::ResourceSectionId>(const ResourceSectionId & value)
{
    return value.toString();
}

template <> TT3_UTIL_PUBLIC QString tt3::util::toString<tt3::util::ResourceId>(const ResourceId & value)
{
    return value.toString();
}

//  End of tt3-util/ToString.cpp

