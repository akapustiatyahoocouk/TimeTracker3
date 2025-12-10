//
//  tt3-report/FontSpecs.cpp - tt3::report::FontSpecs class implementation
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


//  Formatting/parsing
namespace tt3::util
{
    template <> TT3_REPORT_PUBLIC
    QString toString<FontSpecs>(
            const FontSpecs & value
        )
    {
        QString result;
        for (int i = 0; i < value.size(); i++)
        {
            if (i > 0)
            {
                result += ',';
            }
            result += tt3::util::toString(value[i]);
        }
        return '[' + result + ']';
    }

    template <> TT3_REPORT_PUBLIC
    auto fromString<FontSpecs>(
            const QString & s,
            qsizetype & scan
        ) -> FontSpecs
    {
        //  Skip '['
        if (scan >= s.length() || s[scan] != '[')
        {
            throw tt3::util::ParseException(s, scan);
        }
        qsizetype prescan = scan + 1;
        //  Parse list items
        FontSpecs result;
        if (prescan < s.length() && s[prescan] != ']')
        {   //  At least 1 item exists
            for (; ; )
            {
                FontSpec workspaceAddress = fromString<FontSpec>(s, prescan);   //  may throw
                if (!result.contains(workspaceAddress))
                {
                    result.append(workspaceAddress);
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

//  End of tt3-report/FontSpecs.cpp
