//
//  tt3-util/Locale.hpp - Locale extension services
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
    //////////
    //  The manager of known Locales
    class TT3_UTIL_PUBLIC LocaleManager final
    {
        UTILITY_CLASS(LocaleManager)

        //////////
        //  Operations
    public:
        //  Returns the small (16x16) icon representing the specified locale.
        static QIcon        smallIcon(const QLocale & locale);

        //  Returns the large (32x32) icon representing the specified locale.
        static QIcon        largeIcon(const QLocale & locale);

        //////////
        //  Implementation
    private:
        static tt3::util::Mutex     _guard;
        static QMap<QLocale, QIcon> _smallIcons;
        static QMap<QLocale, QIcon> _largeIcons;
    };
}

using Locales = QSet<QLocale>;

//  We need to use QLocale as QMap keys
inline bool operator < (const QLocale & a, const QLocale & b)
{
    return a.name() < b.name();
}

//  End of tt3-util/Locale.hpp
