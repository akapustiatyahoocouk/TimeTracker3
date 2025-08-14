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
    class TT3_UTIL_PUBLIC ParseException : public QException
    {
        //////////
        //  Construction/destruction/assignment
    public:
        ParseException(const QString & string, int position)
            :   _string(string), _position(position) {}

        //////////
        //  QException
    public:
        void                raise() const override { throw *this; }
        ParseException *    clone() const override { return new ParseException(*this); }

        //////////
        //  Operations
    public:
        //  The string which has failed to parse
        QString             string() const { return _string; }

        //  The position within the parsed string where a parse error occurred
        int                 position() const { return _position; }

        //////////
        //  Implementation
    private:
        QString             _string;
        int                 _position;
    };

    //  Parses a string representction of a value of type T that
    //  starts at position "scan"in "s". Upon success advances "scan" to
    //  just past the parsed portion of "s" and returns the parsed value.
    //  Upon faulure throws "ParseException" without advancing "scan",
    template <class T>
    T fromString(const QString & s, int & scan) throws(ParseException) = delete;

    //  C++ types
    template <> TT3_UTIL_PUBLIC bool fromString<bool>(const QString & s, int & scan) throws(ParseException);
    template <> TT3_UTIL_PUBLIC char fromString<char>(const QString & s, int & scan) throws(ParseException);
    template <> TT3_UTIL_PUBLIC signed char fromString<signed char>(const QString & s, int & scan) throws(ParseException);
    template <> TT3_UTIL_PUBLIC unsigned char fromString<unsigned char>(const QString & s, int & scan) throws(ParseException);
    template <> TT3_UTIL_PUBLIC signed int fromString<signed int>(const QString & s, int & scan) throws(ParseException);
    template <> TT3_UTIL_PUBLIC unsigned int fromString<unsigned int>(const QString & s, int & scan) throws(ParseException);
    template <> TT3_UTIL_PUBLIC signed long fromString<signed long>(const QString & s, int & scan) throws(ParseException);
    template <> TT3_UTIL_PUBLIC unsigned long fromString<unsigned long>(const QString & s, int & scan) throws(ParseException);
    template <> TT3_UTIL_PUBLIC signed long long fromString<signed long long>(const QString & s, int & scan) throws(ParseException);
    template <> TT3_UTIL_PUBLIC unsigned long long fromString<unsigned long long>(const QString & s, int & scan) throws(ParseException);

    //  QT types
    template <> TT3_UTIL_PUBLIC QChar fromString<QChar>(const QString & s, int & scan) throws(ParseException);
    template <> TT3_UTIL_PUBLIC QString fromString<QString>(const QString & s, int & scan) throws(ParseException);
    template <> TT3_UTIL_PUBLIC QRect fromString<QRect>(const QString & s, int & scan) throws(ParseException);
    template <> TT3_UTIL_PUBLIC QVersionNumber fromString<QVersionNumber>(const QString & s, int & scan) throws(ParseException);
    template <> TT3_UTIL_PUBLIC QLocale fromString<QLocale>(const QString & s, int & scan) throws(ParseException);

    //  tt3::util types
    template <> TT3_UTIL_PUBLIC TimeSpan fromString<TimeSpan>(const QString & s, int & scan) throws(ParseException);

    //////////
    //  Parsing helpers

    //  Yields "defaultValue" if the parsing fails.
    template <class T>
    T fromString(const QString & s, int & scan, const T & defaultValue)
    {
        try
        {
            return fromString<T>(s, scan);
        }
        catch (const ParseException &)
        {
            return defaultValue;
        }
    }

    //  Parses the entire string, yields "defaultValue" if the parsing fails.
    template <class T>
    T fromString(const QString & s, const T & defaultValue = T())
    {
        try
        {
            int scan = 0;
            T result = fromString<T>(s, scan);
            return (scan == s.length()) ? result : defaultValue;
        }
        catch (const ParseException &)
        {
            return defaultValue;
        }
    }
}

//  End of tt3-util/FromString.hpp
