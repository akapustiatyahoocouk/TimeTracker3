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
    /// \class LocaleManager tt3-util/API.hpp
    /// \brief The manager of known Locales.
    class TT3_UTIL_PUBLIC LocaleManager final
    {
        TT3_UTILITY_CLASS(LocaleManager)

        //////////
        //  Operations
    public:
        /// \brief
        ///     Returns the small (16x16) icon representing the specified locale.
        /// \param locale
        ///     The locale.
        /// \return
        ///     The small (16x16) icon representing the specified locale.
        static QIcon    smallIcon(const QLocale & locale);

        /// \brief
        ///     Returns the large (32x32) icon representing the specified locale.
        /// \param locale
        ///     The locale.
        /// \return
        ///     The large (32x32) icon representing the specified locale.
        static QIcon    largeIcon(const QLocale & locale);

        /// \brief
        ///     Returns user-readable display name of the specified locale.
        /// \param locale
        ///     The locale.
        /// \return
        ///     The user-readable display name of the specified locale.
        static QString  displayName(const QLocale & locale);

        //////////
        //  Implementation
    private:
        struct _Impl;

        //  Helpers
        static _Impl *  _impl();
    };

    /// \class CurrentLocale tt3-util/API.hpp
    /// \brief The accessor for a "current default" locale.
    /// \details
    ///     Only one global static instance of this class
    ///     exists, and other instances should NOT be
    ///     constructed.
    ///     Acts as a wrapper over QLocale::default().
    class TT3_UTIL_PUBLIC CurrentLocale final
        :   public QObject
    {
        Q_OBJECT
        TT3_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(CurrentLocale)

        //////////
        //  Construction/destruction
    public:
        /// \brief
        ///     The class constructor.
        CurrentLocale();

        /// \brief
        ///     The class destructor.
        virtual ~CurrentLocale();
        \
        //////////
        //  Operators
    public:
        /// \brief
        ///     Changes the "current" locale.
        /// \param locale
        ///     The locale to set as the "current" locale.
        void            operator = (const QLocale & locale);

        /// \brief
        ///     Returns the "current" locale.
        /// \return
        ///     The "current" locale; starts off as system locale.
                        operator QLocale () const;

        //////////
        //  Signals
    signals:
        /// \brief
        ///     Emitted after the "current" locale has changed.
        /// \param before
        ///     The "current" locale before the change.
        /// \param after
        ///     The "current" locale afters the change.
        void                changed(QLocale before, QLocale after);

        //////////
        //  Implementation
    private:
        struct _Impl;

        //  Helpers
        static _Impl *  _impl();
    };

#if defined(TT3_UTIL_LIBRARY)
    //  Building tt3-util
#else
    //  Building tt3-util client
    #ifdef Q_CC_MSVC
        Q_DECL_IMPORT CurrentLocale theCurrentLocale;
    #else
        extern CurrentLocale theCurrentLocale;
    #endif
#endif
}

//  End of tt3-util/Locale.hpp
