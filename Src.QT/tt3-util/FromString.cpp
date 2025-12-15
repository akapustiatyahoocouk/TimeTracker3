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
    void skipCharacter(const QString & s, qsizetype & scan, QChar c)
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
template <> TT3_UTIL_PUBLIC
bool tt3::util::fromString<bool>(const QString & s, qsizetype & scan)
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

template <> TT3_UTIL_PUBLIC
char tt3::util::fromString<char>(const QString & s, qsizetype & scan)
{
    if (scan >= 0 && scan < s.length() && static_cast<char>(s[scan].unicode()) == s[scan])
    {
        return static_cast<char>(s[scan++].unicode());
    }
    throw ParseException(s, scan);
}

template <> TT3_UTIL_PUBLIC
signed char tt3::util::fromString<signed char>(const QString & s, qsizetype & scan)
{
    qsizetype prescan = scan;
    signed long long temp = fromString<signed long long>(s, prescan);
    if (static_cast<signed char>(temp) == temp)
    {
        scan = prescan;
        return static_cast<signed char>(temp);
    }
    throw ParseException(s, scan);
}

template <> TT3_UTIL_PUBLIC
unsigned char tt3::util::fromString<unsigned char>(const QString & s, qsizetype & scan)
{
    qsizetype prescan = scan;
    unsigned long long temp = fromString<unsigned long long>(s, prescan);
    if (static_cast<unsigned char>(temp) == temp)
    {
        scan = prescan;
        return static_cast<unsigned char>(temp);
    }
    throw ParseException(s, scan);
}

template <> TT3_UTIL_PUBLIC
signed short tt3::util::fromString<signed short>(const QString & s, qsizetype & scan)
{
    qsizetype prescan = scan;
    signed long long temp = fromString<signed long long>(s, prescan);
    if (static_cast<signed short>(temp) == temp)
    {
        scan = prescan;
        return static_cast<signed short>(temp);
    }
    throw ParseException(s, scan);
}

template <> TT3_UTIL_PUBLIC
unsigned short tt3::util::fromString<unsigned short>(const QString & s, qsizetype & scan)
{
    qsizetype prescan = scan;
    unsigned long long temp = fromString<unsigned long long>(s, prescan);
    if (static_cast<unsigned short>(temp) == temp)
    {
        scan = prescan;
        return static_cast<unsigned short>(temp);
    }
    throw ParseException(s, scan);
}

template <> TT3_UTIL_PUBLIC
signed int tt3::util::fromString<signed int>(const QString & s, qsizetype & scan)
{
    qsizetype prescan = scan;
    signed long long temp = fromString<signed long long>(s, prescan);
    if (static_cast<signed int>(temp) == temp)
    {
        scan = prescan;
        return static_cast<signed int>(temp);
    }
    throw ParseException(s, scan);
}

template <> TT3_UTIL_PUBLIC
unsigned int tt3::util::fromString<unsigned int>(const QString & s, qsizetype & scan)
{
    qsizetype prescan = scan;
    unsigned long long temp = fromString<unsigned long long>(s, prescan);
    if (static_cast<unsigned int>(temp) == temp)
    {
        scan = prescan;
        return static_cast<unsigned int>(temp);
    }
    throw ParseException(s, scan);
}

template <> TT3_UTIL_PUBLIC
signed long tt3::util::fromString<signed long>(const QString & s, qsizetype & scan)
{
    qsizetype prescan = scan;
    signed long long temp = fromString<signed long long>(s, prescan);
    if (static_cast<signed long>(temp) == temp)
    {
        scan = prescan;
        return static_cast<signed long>(temp);
    }
    throw ParseException(s, scan);
}

template <> TT3_UTIL_PUBLIC
unsigned long tt3::util::fromString<unsigned long>(const QString & s, qsizetype & scan)
{
    qsizetype prescan = scan;
    unsigned long long temp = fromString<unsigned long long>(s, prescan);
    if (static_cast<unsigned long>(temp) == temp)
    {
        scan = prescan;
        return static_cast<unsigned long>(temp);
    }
    throw ParseException(s, scan);
}

template <> TT3_UTIL_PUBLIC
signed long long tt3::util::fromString<signed long long>(const QString & s, qsizetype & scan)
{
    if (scan < 0)
    {
        throw ParseException(s, scan);
    }
    qsizetype prescan = scan;
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

template <> TT3_UTIL_PUBLIC
unsigned long long tt3::util::fromString<unsigned long long>(const QString & s, qsizetype & scan)
{
    if (scan < 0 || scan >= s.length())
    {
        throw ParseException(s, scan);
    }
    unsigned long long temp = 0;
    qsizetype numDigits = 0;
    qsizetype prescan = scan;
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

template <> TT3_UTIL_PUBLIC
float tt3::util::fromString<float>(const QString & s, qsizetype & scan)
{
    return static_cast<float>(fromString<double>(s, scan));
}

template <> TT3_UTIL_PUBLIC
double tt3::util::fromString<double>(const QString & s, qsizetype & scan)
{
    if (scan < 0 || scan >= s.length())
    {
        throw ParseException(s, scan);
    }
    //  Handle special cases
    if (scan + 5 <= s.length() &&
        s[scan + 0] == '#' &&
        s[scan + 1] == '+' &&
        s[scan + 2] == 'I' &&
        s[scan + 3] == 'N' &&
        s[scan + 4] == 'F')
    {   //  "#+INF"
        scan += 5;
        return std::numeric_limits<double>::infinity();
    }
    if (scan + 5 <= s.length() &&
        s[scan + 0] == '#' &&
        s[scan + 1] == '-' &&
        s[scan + 2] == 'I' &&
        s[scan + 3] == 'N' &&
        s[scan + 4] == 'F')
    {   //  "#-INF"
        scan += 5;
        return -std::numeric_limits<double>::infinity();
    }
    if (scan + 5 <= s.length() &&
        s[scan + 0] == '#' &&
        s[scan + 1] == 'Q' &&
        s[scan + 2] == 'N' &&
        s[scan + 3] == 'A' &&
        s[scan + 4] == 'N')
    {   //  "#QNAN"
        scan += 5;
        return std::numeric_limits<double>::quiet_NaN();
    }
    //  Default case
    //  [+-]?d+[.d*]?[[eE][+-]?d+]?
    qsizetype prescan = scan;
    bool negative = false;
    double magnitude= 0.0;
    qsizetype numDigits = 0;
    //  Parse sign
    if (prescan < s.length() && s[prescan] == '+')
    {   //  +
        prescan++;
    }
    else if (prescan < s.length() && s[prescan] == '-')
    {   //  -
        negative = true;
        prescan++;
    }
    //  Parse integer digits
    while (prescan < s.length() &&
           s[prescan] >= '0' && s[prescan] <= '9')
    {
        int digit = s[prescan++].unicode() - '0';
        magnitude = magnitude * 10.0 + digit;
        numDigits++;
    }
    //  Parse fractional part
    if (prescan < s.length() && s[prescan] == '.')
    {
        double scale = 1.0;
        prescan++;
        while (prescan < s.length() &&
               s[prescan] >= '0' && s[prescan] <= '9')
        {
            int digit = s[prescan++].unicode() - '0';
            magnitude = magnitude * 10.0 + digit;
            scale *= 10.0;
            numDigits++;
        }
        magnitude /= scale;
    }
    //  Parse exponent
    qsizetype expscan = prescan;
    if (prescan < s.length() &&
        (s[prescan] == 'E' || s[prescan] == 'e'))
    {
        prescan++;  //  skip E
        //  Parse exponent sign
        bool exponentNegative = false;
        if (prescan < s.length() && s[prescan] == '+')
        {   //  +
            prescan++;
        }
        else if (prescan < s.length() && s[prescan] == '-')
        {   //  -
            exponentNegative = true;
            prescan++;
        }
        //  Parse exponent digits
        qsizetype numExponentDigits = 0;
        int exponentMagnitude = 0;
        while (prescan < s.length() &&
               s[prescan] >= '0' && s[prescan] <= '9')
        {
            int digit = s[prescan++].unicode() - '0';
            exponentMagnitude = exponentMagnitude * 10 + digit;
            numExponentDigits++;
            if (numExponentDigits > 3)
            {   //  OOPS! No more!
                break;
            }
        }
        if (numExponentDigits == 0)
        {   //  OOPS! Exponent is empty - ignore it
            prescan = expscan;
        }
        else
        {   //  Factor exponent into magnitude
            magnitude =
                exponentNegative ?
                    magnitude / std::pow(10.0, exponentMagnitude) :
                    magnitude * std::pow(10.0, exponentMagnitude);
        }
    }
    //  Form the value & we're done
    scan = prescan;
    return negative ? -magnitude : magnitude;
}

//  QT types
template <> TT3_UTIL_PUBLIC
QChar tt3::util::fromString<QChar>(const QString & s, qsizetype & scan)
{
    if (scan >= 0 && scan < s.length())
    {
        return s[scan++];
    }
    throw ParseException(s, scan);
}

template <> TT3_UTIL_PUBLIC
QString tt3::util::fromString<QString>(const QString & s, qsizetype & scan)
{
    if (scan >= 0 && scan <= s.length())
    {
        QString result = s.mid(scan);
        scan = s.length();
        return result;
    }
    throw ParseException(s, scan);
}

template <> TT3_UTIL_PUBLIC
QRect tt3::util::fromString<QRect>(const QString & s, qsizetype & scan)
{
    qsizetype prescan = scan;
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

template <> TT3_UTIL_PUBLIC
QVersionNumber tt3::util::fromString<QVersionNumber>(const QString & s, qsizetype & scan)
{
    qsizetype prescan = scan;

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

template <> TT3_UTIL_PUBLIC
QLocale tt3::util::fromString<QLocale>(const QString & s, qsizetype & scan)
{
    if (scan < 0 || scan > s.length())
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

template <> TT3_UTIL_PUBLIC
QColor tt3::util::fromString<QColor>(const QString & s, qsizetype & scan)
{
    static QRegularExpression hexArgbRegex("#[0-9a-fA-F]{8}");
    static QRegularExpression hexRgbRegex("#[0-9a-fA-F]{6}");
    static QRegularExpression rgbRegex("rgb\\s*\\(\\s*([0-9]{1,3})\\s*,\\s*([0-9]{1,3})\\s*,\\s*([0-9]{1,3})\\s*\\)");
    static QRegularExpression rgbaRegex("rgb\\s*\\(\\s*([0-9]{1,3})\\s*,\\s*([0-9]{1,3})\\s*,\\s*([0-9]{1,3})\\s*,\\s*([0-9]{1,3})\\s*\\)");

    if (scan < 0 || scan > s.length())
    {
        throw ParseException(s, scan);
    }

    if (s.mid(scan).startsWith("transparent"))
    {
        scan += 11;
        return QColor(0, 0, 0, 0);
    }
    if (s.mid(scan).startsWith("invalid"))
    {
        scan += 7;
        return QColor();
    }
    if (scan + 9 <= s.length() &&
        hexArgbRegex.match(s.mid(scan, 9)).hasMatch())
    {
        QColor c = QColor(s.mid(scan, 9));
        scan += 9;
        return c;
    }
    if (scan + 7 <= s.length() &&
        hexRgbRegex.match(s.mid(scan, 7)).hasMatch())
    {
        QColor c = QColor(s.mid(scan, 7));
        scan += 7;
        return c;
    }

    QRegularExpressionMatch rgbMatch = rgbRegex.match(s.mid(scan));
    if (rgbMatch.hasMatch())
    {
        int r = fromString<int>(rgbMatch.captured(1).trimmed(), 0);
        int g = fromString<int>(rgbMatch.captured(2).trimmed(), 0);
        int b = fromString<int>(rgbMatch.captured(3).trimmed(), 0);
        if (r <= 255 && g <= 255 && b <= 255)
        {
            scan += int(rgbMatch.capturedLength());
            return QColor(r, g, b);
        }
    }
    QRegularExpressionMatch rgbaMatch = rgbaRegex.match(s.mid(scan));
    if (rgbaMatch.hasMatch())
    {
        int r = fromString<int>(rgbaMatch.captured(1).trimmed(), 0);
        int g = fromString<int>(rgbaMatch.captured(2).trimmed(), 0);
        int b = fromString<int>(rgbaMatch.captured(3).trimmed(), 0);
        int a = fromString<int>(rgbaMatch.captured(4).trimmed(), 0);
        if (r <= 255 && g <= 255 && b <= 255 && a <= 255)
        {
            scan += int(rgbaMatch.capturedLength());
            return QColor(r, g, b, a);
        }
    }

    //  Done
    throw ParseException(s, scan);
}

template <> TT3_UTIL_PUBLIC
QDateTime tt3::util::fromString<QDateTime>(const QString & s, qsizetype & scan)
{
    static QRegularExpression regex("([0-9]{4})([0-9]{2})([0-9]{2})T([0-9]{2})([0-9]{2})([0-9]{2})\\.([0-9]{3})");

    if (scan < 0 || scan >= s.length())
    {
        throw ParseException(s, scan);
    }
    //  Special cases
    if (scan < s.length() && s[scan] == '-')
    {   //  Invalid QDateTime!
        scan++;
        return QDateTime();
    }
    //  General case
    if (scan + 19 > s.length())
    {
        throw ParseException(s, scan);
    }
    QRegularExpressionMatch regexMatch = regex.match(s.mid(scan));
    if (regexMatch.hasMatch())
    {
        int year = fromString<int>(regexMatch.captured(1), 0);
        int month = fromString<int>(regexMatch.captured(2), 0);
        int day = fromString<int>(regexMatch.captured(3), 0);
        int hour = fromString<int>(regexMatch.captured(4), 0);
        int minute = fromString<int>(regexMatch.captured(5), 0);
        int second = fromString<int>(regexMatch.captured(6), 0);
        int msec = fromString<int>(regexMatch.captured(7), 0);
        QDateTime result(QDate(year, month, day),
                         QTime(hour, minute, second, msec),
                         QTimeZone::UTC);
        if (!result.isValid())
        {
            throw ParseException(s, scan);
        }
        scan += 19;
        return result;
    }
    throw ParseException(s, scan);
}

template <> TT3_UTIL_PUBLIC
QDate tt3::util::fromString<QDate>(const QString & s, qsizetype & scan)
{
    static QRegularExpression regex("([0-9]{4})([0-9]{2})([0-9]{2})");

    if (scan < 0 || scan >= s.length())
    {
        throw ParseException(s, scan);
    }
    //  Special cases
    if (scan < s.length() && s[scan] == '-')
    {   //  Invalid QDate!
        scan++;
        return QDate();
    }
    //  General case
    if (scan + 8 > s.length())
    {
        throw ParseException(s, scan);
    }
    QRegularExpressionMatch regexMatch = regex.match(s.mid(scan));
    if (regexMatch.hasMatch())
    {
        int year = fromString<int>(regexMatch.captured(1), 0);
        int month = fromString<int>(regexMatch.captured(2), 0);
        int day = fromString<int>(regexMatch.captured(3), 0);
        QDate result(year, month, day);
        if (!result.isValid())
        {
            throw ParseException(s, scan);
        }
        scan += 8;
        return result;
    }
    throw ParseException(s, scan);
}

//  tt3::util types
template <> TT3_UTIL_PUBLIC
tt3::util::TimeSpan tt3::util::fromString<tt3::util::TimeSpan>(const QString & s, qsizetype & scan)
{
    if (scan < s.length() && s[scan] == '?')
    {
        scan++;
        return tt3::util::TimeSpan::Invalid;
    }
    int minutes = fromString<int>(s, scan);
    return tt3::util::TimeSpan::minutes(minutes);
}

template <> TT3_UTIL_PUBLIC
tt3::util::Mnemonic tt3::util::fromString<tt3::util::Mnemonic>(const QString & s, qsizetype & scan)
{
    return tt3::util::Mnemonic(fromString<QString>(s, scan));
}

template <> TT3_UTIL_PUBLIC
tt3::util::ResourceSectionId tt3::util::fromString<tt3::util::ResourceSectionId>(const QString & s, qsizetype & scan)
{
    return tt3::util::ResourceSectionId(fromString<QString>(s, scan));
}

template <> TT3_UTIL_PUBLIC
tt3::util::ResourceId tt3::util::fromString<tt3::util::ResourceId>(const QString & s, qsizetype & scan)
{
    return tt3::util::ResourceId(fromString<QString>(s, scan));
}

//  End of tt3-util/FromString.cpp
