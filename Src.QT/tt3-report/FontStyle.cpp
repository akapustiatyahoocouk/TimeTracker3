//
//  tt3-report/FontStyle.cpp - tt3::report::FontStyle enum implementation
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
const FontStyle FontStyle::Plain(0);
const FontStyle FontStyle::Bold(FontFlag::Bold);
const FontStyle FontStyle::Italic(FontFlag::Italic);
const FontStyle FontStyle::Underline(FontFlag::Underline);
const FontStyle FontStyle::StrikeThrough(FontFlag::StrikeThrough);

namespace
{
    const FontFlag AllFontStyle[] =
    {
        FontFlag::Bold,
        FontFlag::Italic,
        FontFlag::Underline,
        FontFlag::StrikeThrough
    };

    const char Separator = '+';
}

//////////
//  Formatting/parsing
namespace tt3::util
{
    template <> TT3_REPORT_PUBLIC
    QString toString<FontStyle>(
            const FontStyle & value
        )
    {
        //  Special cases
        if (value == FontStyle::Plain)
        {
            return "plain";
        }
        //  General case
        QString result;
        for (size_t i = 0; i < sizeof(AllFontStyle) / sizeof(AllFontStyle[0]); i++)
        {
            if (value.contains(AllFontStyle[i]))
            {
                if (!result.isEmpty())
                {
                    result += Separator;
                }
                result += toString(AllFontStyle[i]);
            }
        }
        return result;
    }

    template <> TT3_REPORT_PUBLIC
    auto fromString<FontStyle>(
            const QString & s,
            qsizetype & scan
           ) -> FontStyle
    {
        if (scan < 0 || scan > s.length())
        {   //  OOPS!
            throw ParseException(s, scan);
        }
        //  Handle special cases
        if (s.mid(scan, 8).startsWith("plain", Qt::CaseInsensitive))
        {
            scan += 5;
            return FontStyle::Plain;
        }
        //  General case
        FontStyle result;
        if (scan >= s.length())
        {
            return result;
        }
        qsizetype prescan = scan;
        bool separatorConsumed = false;
        for (; ; )
        {
            //  Does a font flag start at s[scan] |
            FontStyle addend;
            for (size_t i = 0; i < sizeof(AllFontStyle) / sizeof(AllFontStyle[0]); i++)
            {
                QString name = toString(AllFontStyle[i]);
                if (s.mid(prescan, 16).startsWith(name, Qt::CaseInsensitive))
                {   //  Yes
                    addend = AllFontStyle[i];
                    prescan += name.length();
                    break;
                }
            }
            if (addend.isEmpty())
            {   //  No more!
                separatorConsumed = false;
                break;
            }
            result |= addend;
            //  More ?
            if (prescan < s.length() && s[prescan] == Separator)
            {
                prescan++;
                separatorConsumed = true;
            }
            else
            {
                separatorConsumed = false;
                break;
            }
        }
        if (separatorConsumed)
        {
            prescan--;
        }
        scan = prescan;
        return result;
    }
}

//  End of tt3-report/FontStyle.cpp
