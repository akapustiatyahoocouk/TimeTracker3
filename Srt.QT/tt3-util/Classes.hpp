//
//  tt3-util/Classes.hpp - forward declarations and typedefs
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
    class TT3_UTIL_PUBLIC Settings;
    class TT3_UTIL_PUBLIC ISubsystem;
    class TT3_UTIL_PUBLIC IComponent;
    class TT3_UTIL_PUBLIC IMessageDigest;
    class TT3_UTIL_PUBLIC ILicense;
    class TT3_UTIL_PUBLIC IPlugin;

    //  Collections
    using Subsystems = QSet<ISubsystem*>;
    using Components = QSet<IComponent*>;
    using MessageDigests = QSet<IMessageDigest*>;
    using Licenses = QSet<ILicense*>;
    using Plugins = QSet<IPlugin*>;

    //  Exceptions
    class TT3_UTIL_PUBLIC ParseException;
    class TT3_UTIL_PUBLIC MissingResourceException;
}

//  TODO move these to Locale???.hpp
using Locales = QSet<QLocale>;

inline bool operator < (const QLocale & a, const QLocale & b)
{
    return a.name() < b.name();
}

//  End of tt3-util/Classes.hpp
