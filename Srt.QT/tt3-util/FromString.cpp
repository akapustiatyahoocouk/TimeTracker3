//
//  tt3-util/FromString.cpp - Value parsing framework
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

namespace
{
    void skipCharacter(const QString & s, int & scan, QChar c) throws(tt3::util::ParseException)
    {
        if (scan < s.length() && s[scan] == c)
        {
            scan++;
        }
        else
        {
            throw tt3::util::ParseException(s, scan);
        }
    }
}

//  C++ types
template <> TT3_UTIL_PUBLIC bool tt3::util::fromString<bool>(const QString & s, int & scan) throws(ParseException)
{
    if (scan < 0)
    {
        throw ParseException(s, scan);
    }
    //  true/false
    if (scan + 4 <= s.length() &&
        s[scan + 0].toLower() == 't' &&
        s[scan + 1].toLower() == 'r' &&
        s[scan + 2].toLower() == 'u' &&
        s[scan + 3].toLower() == 'e')
    {
        scan += 4;
        return true;
    }
    if (scan + 5 <= s.length() &&
        s[scan + 0].toLower() == 'f' &&
        s[scan + 1].toLower() == 'a' &&
        s[scan + 2].toLower() == 'l' &&
        s[scan + 3].toLower() == 's' &&
        s[scan + 4].toLower() == 'e')
    {
        scan += 5;
        return false;
    }
    //  yes/no
    if (scan + 3 <= s.length() &&
        s[scan + 0].toLower() == 'y' &&
        s[scan + 1].toLower() == 'e' &&
        s[scan + 2].toLower() == 's')
    {
        scan += 3;
        return true;
    }
    if (scan + 2 <= s.length() &&
        s[scan + 0].toLower() == 'n' &&
        s[scan + 1].toLower() == 'o')
    {
        scan += 2;
        return false;
    }
    //  t/f
    if (scan < s.length() && s[scan].toLower() == 't')
    {
        scan++;
        return true;
    }
    if (scan < s.length() && s[scan].toLower() == 'f')
    {
        scan++;
        return false;
    }
    //  y/n
    if (scan < s.length() && s[scan].toLower() == 'y')
    {
        scan++;
        return true;
    }
    if (scan < s.length() && s[scan].toLower() == 'n')
    {
        scan++;
        return false;
    }
    //  1/0
    if (scan < s.length() && s[scan].toLower() == '1')
    {
        scan++;
        return true;
    }
    if (scan < s.length() && s[scan].toLower() == '0')
    {
        scan++;
        return false;
    }
    //  Give up
    throw ParseException(s, scan);
}

template <> TT3_UTIL_PUBLIC char tt3::util::fromString<char>(const QString & s, int & scan) throws(ParseException)
{
    if (scan >= 0 && scan < s.length() && static_cast<char>(s[scan].unicode()) == s[scan])
    {
        return static_cast<char>(s[scan++].unicode());
    }
    throw ParseException(s, scan);
}

template <> TT3_UTIL_PUBLIC signed char tt3::util::fromString<signed char>(const QString & s, int & scan) throws(ParseException)
{
    int prescan = scan;
    signed long long temp = fromString<signed long long>(s, prescan);
    if (static_cast<signed char>(temp) == temp)
    {
        scan = prescan;
        return static_cast<signed char>(temp);
    }
    throw ParseException(s, scan);
}

template <> TT3_UTIL_PUBLIC unsigned char tt3::util::fromString<unsigned char>(const QString & s, int & scan) throws(ParseException)
{
    int prescan = scan;
    unsigned long long temp = fromString<unsigned long long>(s, prescan);
    if (static_cast<unsigned char>(temp) == temp)
    {
        scan = prescan;
        return static_cast<unsigned char>(temp);
    }
    throw ParseException(s, scan);
}

template <> TT3_UTIL_PUBLIC signed short tt3::util::fromString<signed short>(const QString & s, int & scan) throws(ParseException)
{
    int prescan = scan;
    signed long long temp = fromString<signed long long>(s, prescan);
    if (static_cast<signed short>(temp) == temp)
    {
        scan = prescan;
        return static_cast<signed short>(temp);
    }
    throw ParseException(s, scan);
}

template <> TT3_UTIL_PUBLIC unsigned short tt3::util::fromString<unsigned short>(const QString & s, int & scan) throws(ParseException)
{
    int prescan = scan;
    unsigned long long temp = fromString<unsigned long long>(s, prescan);
    if (static_cast<unsigned short>(temp) == temp)
    {
        scan = prescan;
        return static_cast<unsigned short>(temp);
    }
    throw ParseException(s, scan);
}

template <> TT3_UTIL_PUBLIC signed int tt3::util::fromString<signed int>(const QString & s, int & scan) throws(ParseException)
{
    int prescan = scan;
    signed long long temp = fromString<signed long long>(s, prescan);
    if (static_cast<signed int>(temp) == temp)
    {
        scan = prescan;
        return static_cast<signed int>(temp);
    }
    throw ParseException(s, scan);
}

template <> TT3_UTIL_PUBLIC unsigned int tt3::util::fromString<unsigned int>(const QString & s, int & scan) throws(ParseException)
{
    int prescan = scan;
    unsigned long long temp = fromString<unsigned long long>(s, prescan);
    if (static_cast<unsigned int>(temp) == temp)
    {
        scan = prescan;
        return static_cast<unsigned int>(temp);
    }
    throw ParseException(s, scan);
}

template <> TT3_UTIL_PUBLIC signed long tt3::util::fromString<signed long>(const QString & s, int & scan) throws(ParseException)
{
    int prescan = scan;
    signed long long temp = fromString<signed long long>(s, prescan);
    if (static_cast<signed long>(temp) == temp)
    {
        scan = prescan;
        return static_cast<signed long>(temp);
    }
    throw ParseException(s, scan);
}

template <> TT3_UTIL_PUBLIC unsigned long tt3::util::fromString<unsigned long>(const QString & s, int & scan) throws(ParseException)
{
    int prescan = scan;
    unsigned long long temp = fromString<unsigned long long>(s, prescan);
    if (static_cast<unsigned long>(temp) == temp)
    {
        scan = prescan;
        return static_cast<unsigned long>(temp);
    }
    throw ParseException(s, scan);
}

template <> TT3_UTIL_PUBLIC signed long long tt3::util::fromString<signed long long>(const QString & s, int & scan) throws(ParseException)
{
    if (scan < 0)
    {
        throw ParseException(s, scan);
    }
    int prescan = scan;
    bool negative = false;
    if (prescan < s.length() && s[prescan] == '-')
    {
        negative = true;
        prescan++;
    }
    else if (prescan < s.length() && s[prescan] == '+')
    {
        prescan++;
    }
    unsigned long long magnitude = fromString<unsigned long long>(s, prescan);
    signed long long result = static_cast<unsigned long long>(magnitude);
    if (negative)
    {
        result = -result;
    }
    if ((negative && result >= 0) || (!negative && result < 0))
    {   //  OOPS! Overflow!
        throw ParseException(s, scan);
    }
    scan = prescan;
    return result;
}

template <> TT3_UTIL_PUBLIC unsigned long long tt3::util::fromString<unsigned long long>(const QString & s, int & scan) throws(ParseException)
{
    if (scan < 0)
    {
        throw ParseException(s, scan);
    }
    unsigned long long temp = 0;
    int numDigits = 0;
    int prescan = scan;
    while (prescan < s.length() && s[prescan] >= '0' && s[prescan] <= '9')
    {
        int digit = s[prescan].unicode() - '0';
        unsigned long long newTemp = temp * 10 + digit;
        if (newTemp / 10 != temp)
        {   //  OOPS! Overflow!
            throw ParseException(s, scan);
        }
        temp = newTemp;
        prescan++;
        numDigits++;
    }
    if (numDigits == 0)
    {
        throw ParseException(s, scan);
    }
    scan = prescan;
    return temp;
}

//  QT types
template <> TT3_UTIL_PUBLIC QChar tt3::util::fromString<QChar>(const QString & s, int & scan) throws(ParseException)
{
    if (scan >= 0 && scan < s.length())
    {
        return s[scan++];
    }
    throw ParseException(s, scan);
}

template <> TT3_UTIL_PUBLIC QString tt3::util::fromString<QString>(const QString & s, int & scan) throws(ParseException)
{
    if (scan >= 0 && scan <= s.length())
    {
        QString result = s.mid(scan);
        scan = int(s.length());
        return result;
    }
    throw ParseException(s, scan);
}

template <> TT3_UTIL_PUBLIC QRect tt3::util::fromString<QRect>(const QString & s, int & scan) throws(ParseException)
{
    int prescan = scan;
    int x1, x2, y1, y2;

    skipCharacter(s, prescan, '(');
    x1 = fromString<int>(s,prescan);
    skipCharacter(s, prescan, ',');
    y1 = fromString<int>(s,prescan);
    skipCharacter(s, prescan, ')');
    skipCharacter(s, prescan, '-');
    skipCharacter(s, prescan, '(');
    x2 = fromString<int>(s,prescan);
    skipCharacter(s, prescan, ',');
    y2 = fromString<int>(s,prescan);
    skipCharacter(s, prescan, ')');

    scan = prescan;
    return QRect(x1, y1, x2 - x1, y2 - y1);
}

template <> TT3_UTIL_PUBLIC QVersionNumber tt3::util::fromString<QVersionNumber>(const QString & s, int & scan) throws(ParseException)
{
    int prescan = scan;

    int major = fromString<int>(s,prescan);
    skipCharacter(s, prescan, '.');
    int minor = fromString<int>(s,prescan);
    skipCharacter(s, prescan, '.');
    int micro = fromString<int>(s,prescan);
    if (major < 0 || minor < 0 || micro < 0)
    {
        throw ParseException(s, scan);
    }

    scan = prescan;
    return QVersionNumber(major, minor, micro);
}

template <> TT3_UTIL_PUBLIC QLocale tt3::util::fromString<QLocale>(const QString & s, int & scan) throws(ParseException)
{
    if (scan > s.length())
    {
        throw ParseException(s, scan);
    }

    QLocale result;
    if (scan + 4  < s.length() &&
        s[scan].isLower() &&
        s[scan + 1].isLower() &&
        s[scan + 2] == '_' &&
        s[scan + 3].isUpper() &&
        s[scan + 4].isUpper())
    {   //  ll-CC
        result = QLocale(s.mid(scan, 5));
        scan += 5;
    }
    else if (scan + 1  < s.length() &&
             s[scan].isLower() &&
             s[scan + 1].isLower())
    {   //  ll
        result = QLocale(s.mid(scan, 5));
        scan += 2;
    }
    else if (scan < s.length() && s[scan] == 'C')
    {   //  C
        scan++;
        return QLocale::c();
    }
    else
    {   //  Invariant
        result = QLocale(QLocale::Language::AnyLanguage, QLocale::Territory::AnyCountry);
    }
    return result;
}

//  tt3::util types
template <> TT3_UTIL_PUBLIC tt3::util::TimeSpan tt3::util::fromString<tt3::util::TimeSpan>(const QString & s, int & scan) throws(ParseException)
{
    if (scan < s.length() && s[scan] == '?')
    {
        scan++;
        return tt3::util::TimeSpan::Invalid;
    }
    int minutes = fromString<int>(s, scan);
    return tt3::util::TimeSpan::minutes(minutes);
}

template <> TT3_UTIL_PUBLIC tt3::util::Mnemonic tt3::util::fromString<tt3::util::Mnemonic>(const QString & s, int & scan) throws(ParseException)
{
    return tt3::util::Mnemonic(fromString<QString>(s, scan));
}

template <> TT3_UTIL_PUBLIC tt3::util::ResourceSectionId tt3::util::fromString<tt3::util::ResourceSectionId>(const QString & s, int & scan) throws(ParseException)
{
    return tt3::util::ResourceSectionId(fromString<QString>(s, scan));
}

template <> TT3_UTIL_PUBLIC tt3::util::ResourceId tt3::util::fromString<tt3::util::ResourceId>(const QString & s, int & scan) throws(ParseException)
{
    return tt3::util::ResourceId(fromString<QString>(s, scan));
}

//  End of tt3-util/FromString.cpp
