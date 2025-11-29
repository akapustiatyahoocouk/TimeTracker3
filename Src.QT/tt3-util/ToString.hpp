//
//  tt3-util/ToString.hpp - Value formatting framework
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
    template <class T>
    QString toString(const T & value) = delete;

    //  C++ types
    template <> TT3_UTIL_PUBLIC QString toString<nullptr_t>(const nullptr_t & value);
    template <> TT3_UTIL_PUBLIC QString toString<bool>(const bool & value);
    template <> TT3_UTIL_PUBLIC QString toString<char>(const char & value);
    template <> TT3_UTIL_PUBLIC QString toString<signed char>(const signed char & value);
    template <> TT3_UTIL_PUBLIC QString toString<unsigned char>(const unsigned char & value);
    template <> TT3_UTIL_PUBLIC QString toString<signed short>(const signed short & value);
    template <> TT3_UTIL_PUBLIC QString toString<unsigned short>(const unsigned short & value);
    template <> TT3_UTIL_PUBLIC QString toString<signed int>(const signed int & value);
    template <> TT3_UTIL_PUBLIC QString toString<unsigned int>(const unsigned int & value);
    template <> TT3_UTIL_PUBLIC QString toString<signed long>(const signed long & value);
    template <> TT3_UTIL_PUBLIC QString toString<unsigned long>(const unsigned long & value);
    template <> TT3_UTIL_PUBLIC QString toString<signed long long>(const signed long long & value);
    template <> TT3_UTIL_PUBLIC QString toString<unsigned long long>(const unsigned long long & value);
    template <> TT3_UTIL_PUBLIC QString toString<float>(const float & value);
    template <> TT3_UTIL_PUBLIC QString toString<double>(const double & value);
    TT3_UTIL_PUBLIC inline      QString toString(const char * value) { return value; }

    //  QT types
    template <> TT3_UTIL_PUBLIC QString toString<QChar>(const QChar & value);
    template <> TT3_UTIL_PUBLIC QString toString<QString>(const QString & value);
    template <> TT3_UTIL_PUBLIC QString toString<QRect>(const QRect & value);
    template <> TT3_UTIL_PUBLIC QString toString<QVersionNumber>(const QVersionNumber & value);
    template <> TT3_UTIL_PUBLIC QString toString<QLocale>(const QLocale & value);
    template <> TT3_UTIL_PUBLIC QString toString<QColor>(const QColor & value);
    template <> TT3_UTIL_PUBLIC QString toString<QDateTime>(const QDateTime & value);
    template <> TT3_UTIL_PUBLIC QString toString<QDate>(const QDate & value);

    //  tt3::util types
    template <> TT3_UTIL_PUBLIC QString toString<TimeSpan>(const TimeSpan & value);
    template <> TT3_UTIL_PUBLIC QString toString<Mnemonic>(const Mnemonic & value);
    template <> TT3_UTIL_PUBLIC QString toString<ResourceSectionId>(const ResourceSectionId & value);
    template <> TT3_UTIL_PUBLIC QString toString<ResourceId>(const ResourceId & value);
}

//  End of tt3-util/ToString.hpp
