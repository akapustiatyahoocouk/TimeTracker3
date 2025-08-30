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
#pragma once
#include "tt3-util/API.hpp"

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

    //  The accessor for a "current default" locale.
    //  Only one global static instance of this class
    //  exists, and other instances should NOT be
    //  constructed.
    //  Acts as a wrapper over QLocale::default().
    class TT3_UTIL_PUBLIC CurrentLocale final : public QObject
    {
        Q_OBJECT
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(CurrentLocale)

        //////////
        //  Construction/destruction
    public:
        CurrentLocale();
        virtual ~CurrentLocale();
        \
        //////////
        //  Operators
    public:
        void            operator = (const QLocale & locale);
                        operator QLocale () const;

        //////////
        //  Signals
    signals:
        //  Emitted after the current locale has changed.
        void                changed(QLocale before, QLocale after);

        //////////
        //  Implementation
    private:
        static std::atomic<int>     _instanceCount; //  ...to disallow a second instance
        mutable tt3::util::Mutex    _currentLocaleGuard;
    };

#if defined(TT3_UTIL_LIBRARY)
    //  Building tt3-util
#else
    //  Building tt3-util client
    Q_DECL_IMPORT CurrentLocale theCurrentLocale;
#endif
}

//  End of tt3-util/Locale.hpp
