//
//  tt3-report/KnownCustomReportTemplate.cpp - tt3::report::KnownCustomReportTemplate strucy implementation
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
//  Operators
bool KnownCustomReportTemplate::operator == (const KnownCustomReportTemplate & op2) const
{
    return mnemonic == op2.mnemonic &&
           displayName == op2.displayName &&
           location == op2.location;
}

bool KnownCustomReportTemplate::operator != (const KnownCustomReportTemplate & op2) const
{
    return !(*this == op2);
}

//////////
//  Formatting/parsing
namespace
{   //  We'll need to escape commas, backslashes.
    //  angle brackets and special characters
    QString escape(const QString & s)
    {
        char escape[8];

        QString result;
        for (QChar c : s)
        {
            switch (c.unicode())
            {
                //  special characters
                case '\a':
                    result += "\\a";
                    break;
                case '\b':
                    result += "\\b";
                    break;
                case '\f':
                    result += "\\f";
                    break;
                case '\n':
                    result += "\\n";
                    break;
                case '\r':
                    result += "\\r";
                    break;
                case '\t':
                    result += "\\t";
                    break;
                case '\v':
                    result += "\\v";
                    break;
                //  literal escapes
                case '<':   case '>':
                case '[':   case ']':
                case ',':   case '\\':
                    result += '\\';
                    result += c;
                    break;
                //  default case
                default:
                    if (c.unicode() > 255)
                    {   //  0uXXXX
                        sprintf(escape, "\\u%04X", c.unicode());
                        result += escape;
                    }
                    else if (c < '\x20' || c >= '\x7F')
                    {   //  \xXX
                        sprintf(escape, "\\x%02X", c.unicode());
                        result += escape;
                    }
                    else
                    {
                        result += c;
                    }
                    break;
            }
        }
        return result;
    }

    int xdigit(const QChar & c)
    {
        if (c >= '0' && c <= '9')
        {
            return c.unicode() - '0';
        }
        else if (c >= 'a' && c <= 'f')
        {
            return c.unicode() - 'a' + 10;
        }
        else if (c >= 'A' && c <= 'F')
        {
            return c.unicode() - 'A' + 10;
        }
        else
        {
            return -1;
        }
    }

    QString unescape(const QString & s, qsizetype & i)
    {   //   throws tt3::util::ParseException
        Q_ASSERT(i >= 0);
        QString result;
        while (i < s.length())
        {
            if (s[i] == ',' || s[i] == '>')
            {   //  End of item
                return result;
            }
            if (s[i] != '\\')
            {   //  A literal character
                result += s[i];
                i++;
                continue;
            }
            //  We have an escape sequence. Skip '\'
            i++;
            if (i >= s.length())
            {   //  OOPS! Missing - end of item
                return result;
            }
            //  Special character ?
            if (s[i] == 'a')
            {   //  \a
                result += '\a';
                i++;
            }
            else if (s[i] == 'b')
            {   //  \b
                result += '\b';
                i++;
            }
            else if (s[i] == 'f')
            {   //  \f
                result += '\f';
                i++;
            }
            else if (s[i] == 'n')
            {   //  \n
                result += '\n';
                i++;
            }
            else if (s[i] == 'r')
            {   //  \r
                result += '\r';
                i++;
            }
            else if (s[i] == 't')
            {   //  \t
                result += '\t';
                i++;
            }
            else if (s[i] == 'v')
            {   //  \a
                result += '\v';
                i++;
            }
            //  Numeric escape ?
            else if (s[i] == 'x')
            {   //  \xXX
                i++;
                if (i + 1 < s.length() &&
                    xdigit(s[i]) != -1 && xdigit(s[i + 1]) != -1)
                {
                    result += QChar(xdigit(s[i]) * 16 +
                                    xdigit(s[i + 1]));
                    i += 2;
                }
                else
                {   //  OOPS! Invalid hex escape!
                    return result;
                }
            }
            else if (s[i] == 'u')
            {   //  \uXXXX
                i++;
                if (i + 3 < s.length() &&
                    xdigit(s[i]) != -1 && xdigit(s[i + 1]) != -1 &&
                    xdigit(s[i + 2]) != -1 && xdigit(s[i + 3]) != -1)
                {
                    result += QChar(xdigit(s[i]) * 4096 +
                                    xdigit(s[i + 1]) * 256 +
                                    xdigit(s[i + 2]) * 16 +
                                    xdigit(s[i + 3]));
                    i += 4;
                }
                else
                {   //  OOPS! Invalid hex escape!
                    return result;
                }
            }
            //  Literal escape
            else
            {
                result += s[i++];
            }
        }
        return result;
    }
}

namespace tt3::util
{
    template <> TT3_REPORT_PUBLIC
    QString toString<KnownCustomReportTemplate>(
            const KnownCustomReportTemplate & value
        )
    {
        return '<' +
               escape(value.mnemonic.toString()) +
               ',' +
               escape(value.displayName) +
               ',' +
               escape(value.location) +
               '>';
    }

    template <> TT3_REPORT_PUBLIC
    QString toString<KnownCustomReportTemplates>(
            const KnownCustomReportTemplates & value
        )
    {
        QString result;
        result += '[';
        for (int i = 0; i < value.size(); i++)
        {
            result += toString(value[i]);
            if (i + 1 < value.size())
            {
                result += ',';
            }
        }
        result += ']';
        return result;
    }

    template <> TT3_REPORT_PUBLIC
    auto fromString<KnownCustomReportTemplate>(
            const QString & s,
            qsizetype & scan
        ) -> KnownCustomReportTemplate
    {
        //  Skip '<'
        if (scan >= s.length() || s[scan] != '<')
        {
            throw tt3::util::ParseException(s, scan);
        }
        qsizetype prescan = scan + 1;
        //  Parse mnemonic
        QString mnemonic = unescape(s, prescan);
        //  Skip ','
        if (prescan >= s.length() || s[prescan] != ',')
        {
            throw tt3::util::ParseException(s, prescan);
        }
        prescan++;
        //  Parse display name
        QString displayName = unescape(s, prescan);
        //  Skip ','
        if (prescan >= s.length() || s[prescan] != ',')
        {
            throw tt3::util::ParseException(s, prescan);
        }
        prescan++;
        //  Parse location
        QString location = unescape(s, prescan);
        //  Skip '>'
        if (prescan >= s.length() || s[prescan] != '>')
        {
            throw tt3::util::ParseException(s, prescan);
        }
        //  We're done!
        scan = prescan + 1;
        return KnownCustomReportTemplate(tt3::util::Mnemonic(mnemonic), displayName, location);
    }

    template <> TT3_REPORT_PUBLIC
    auto fromString<KnownCustomReportTemplates>(
            const QString & s,
            qsizetype & scan
        ) -> KnownCustomReportTemplates
    {
        //  Skip '['
        if (scan >= s.length() || s[scan] != '[')
        {
            throw tt3::util::ParseException(s, scan);
        }
        qsizetype prescan = scan + 1;
        //  Parse list items
        KnownCustomReportTemplates result;
        if (prescan < s.length() && s[prescan] != ']')
        {   //  At least 1 item exists
            for (; ; )
            {
                KnownCustomReportTemplate element = fromString<KnownCustomReportTemplate>(s, prescan);   //  may throw
                if (!result.contains(element))
                {   //  ...but mnemonics can still conflict!
                    result.append(element);
                }
                //  More ?
                if (prescan < s.length() && s[prescan] == ',')
                {   //  yes
                    prescan++;
                }
                else
                {   //  no
                    break;
                }
            }
        }
        //  Skip ']'
        if (prescan >= s.length() || s[prescan] != ']')
        {
            throw tt3::util::ParseException(s, scan);
        }
        //  Done
        scan = prescan + 1;
        return result;
    }
}

//  End of tt3-report/KnownCustomReportTemplate.cpp
