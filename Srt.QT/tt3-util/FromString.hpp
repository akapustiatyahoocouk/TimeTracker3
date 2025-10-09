//
//  tt3-util/FromString.hpp - Value parsing framework
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
    //  Parses a string representction of a value of type T that
    //  starts at position "scan"in "s". Upon success advances "scan" to
    //  just past the parsed portion of "s" and returns the parsed value.
    //  Throws ParseException if a parse error occurs (without advancing "scan"),

    /// Parses a value from the portion of a string.
    /// Upon success, consumes as much of the string as possible.
    ///
    /// \param s
    ///     The string to parse a portion of.
    /// \param scan
    ///     The current scan position; advanced past the parsed
    ///     portion of the string upon success, left intact
    ///     upon failure.
    /// \return
    ///     The parsed value.
    /// \exception ParseException
    ///     If the parsing fails.
    ///
    template <class T>
    T fromString(
            const QString & s,
            qsizetype & scan
        ) = delete;

    //  C++ types
    template <> TT3_UTIL_PUBLIC bool fromString<bool>(const QString & s, qsizetype & scan);
    template <> TT3_UTIL_PUBLIC char fromString<char>(const QString & s, qsizetype & scan);
    template <> TT3_UTIL_PUBLIC signed char fromString<signed char>(const QString & s, qsizetype & scan);
    template <> TT3_UTIL_PUBLIC unsigned char fromString<unsigned char>(const QString & s, qsizetype & scan);
    template <> TT3_UTIL_PUBLIC signed int fromString<signed int>(const QString & s, qsizetype & scan);
    template <> TT3_UTIL_PUBLIC unsigned int fromString<unsigned int>(const QString & s, qsizetype & scan);
    template <> TT3_UTIL_PUBLIC signed long fromString<signed long>(const QString & s, qsizetype & scan);
    template <> TT3_UTIL_PUBLIC unsigned long fromString<unsigned long>(const QString & s, qsizetype & scan);
    template <> TT3_UTIL_PUBLIC signed long long fromString<signed long long>(const QString & s, qsizetype & scan);
    template <> TT3_UTIL_PUBLIC unsigned long long fromString<unsigned long long>(const QString & s, qsizetype & scan);

    //  QT types
    template <> TT3_UTIL_PUBLIC QChar fromString<QChar>(const QString & s, qsizetype & scan);
    template <> TT3_UTIL_PUBLIC QString fromString<QString>(const QString & s, qsizetype & scan);
    template <> TT3_UTIL_PUBLIC QRect fromString<QRect>(const QString & s, qsizetype & scan);
    template <> TT3_UTIL_PUBLIC QVersionNumber fromString<QVersionNumber>(const QString & s, qsizetype & scan);
    template <> TT3_UTIL_PUBLIC QLocale fromString<QLocale>(const QString & s, qsizetype & scan);
    template <> TT3_UTIL_PUBLIC QColor fromString<QColor>(const QString & s, qsizetype & scan);
    template <> TT3_UTIL_PUBLIC QDateTime fromString<QDateTime>(const QString & s, qsizetype & scan);

    //  tt3::util types
    template <> TT3_UTIL_PUBLIC TimeSpan fromString<TimeSpan>(const QString & s, qsizetype & scan);
    template <> TT3_UTIL_PUBLIC Mnemonic fromString<Mnemonic>(const QString & s, qsizetype & scan);
    template <> TT3_UTIL_PUBLIC ResourceSectionId fromString<ResourceSectionId>(const QString & s, qsizetype & scan);
    template <> TT3_UTIL_PUBLIC ResourceId fromString<ResourceId>(const QString & s, qsizetype & scan);

    //////////
    //  Parsing helpers

    //  Yields "defaultValue" if the parsing fails.
    template <class T>
    T fromString(const QString & s, qsizetype & scan, const T & defaultValue)
    {
        try
        {
            return fromString<T>(s, scan);
        }
        catch (const ParseException & ex)
        {
            //  TODO qCritical() << ex.errorMessage();
            return defaultValue;
        }
    }

    //  Parses the entire string, yields "defaultValue" if the parsing fails.
    template <class T>
    T fromString(const QString & s, const T & defaultValue = T())
    {
        try
        {
            qsizetype scan = 0;
            T result = fromString<T>(s, scan);
            return (scan == s.length()) ? result : defaultValue;
        }
        catch (const ParseException & /*ex*/)
        {
            //  TODO qCritical() << ex.errorMessage();
            return defaultValue;
        }
    }
}

//  End of tt3-util/FromString.hpp
