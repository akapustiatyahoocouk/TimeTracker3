//
//  tt3-report/UnderlineMode.cpp - tt3::report::UnderlineMode enum implementation
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

namespace
{
    struct Traits
    {
        UnderlineMode   value;
        const char *const   name;
    };

    const Traits theTraits[] =
    {
        { UnderlineMode::Default, "default" },
        { UnderlineMode::None   , "none"    },
        { UnderlineMode::Single , "single"  },
        { UnderlineMode::Double , "double"  }
    };
}

//  Formatting/parsing
namespace tt3::util
{
    template <> TT3_REPORT_PUBLIC
    QString toString<UnderlineMode>(
            const UnderlineMode & value
        )
    {
        for (const auto & traits : theTraits)
        {
            if (traits.value == value)
            {
                return traits.name;
            }
        }
        //  OOPS! Be defensive in release mode
        Q_ASSERT(false);
        return toString(UnderlineMode::Default);
    }

    template <> TT3_REPORT_PUBLIC
    auto fromString<UnderlineMode>(
            const QString & s,
            qsizetype & scan
        ) -> UnderlineMode
    {
        if (scan < 0 || scan > s.length())
        {   //  OOPS!
            throw ParseException(s, scan);
        }
        //  Try parsing
        QString temp = s.mid(scan, 16); //  16 chars max
        for (const auto & traits : theTraits)
        {
            if (temp.startsWith(traits.name, Qt::CaseInsensitive))
            {   //  This one!
                scan += strlen(traits.name);
                return traits.value;
            }
        }
        //  Give up
        throw ParseException(s, scan);
    }
}

//  End of tt3-report/UnderlineMode.cpp
