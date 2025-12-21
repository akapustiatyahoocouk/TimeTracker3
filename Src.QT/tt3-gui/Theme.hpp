//
//  tt3-gui/Theme.hpp - tt3 Theming framework
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
#include "tt3-gui/API.hpp"

namespace tt3::gui
{
    /// \class ITheme tt3-gui/API.hpp
    /// \brief
    ///     A generic "theme" adjusts the appearance (that is,
    ///     colors, etc.) of TT3.
    class TT3_GUI_PUBLIC ITheme
    {
        //////////
        //  This is an interface
    protected:
        /// \brief
        ///     The empty [interface] constructor.
        ITheme() = default;

        /// \brief
        ///     The empty [interface] destructor.
        virtual ~ITheme() = default;

        //////////
        //  Operations (properties)
    public:
        /// \brief A type alias to improve code readability.
        using Mnemonic = tt3::util::Mnemonic;

        /// \brief
        ///     Returns the mnemonic identifier of this theme.
        /// \return
        ///     The mnemonic identifier of this theme.
        virtual Mnemonic    mnemonic() const = 0;

        /// \brief
        ///     Returns the user-readable display name of this theme.
        /// \return
        ///     The user-readable display name of this theme
        ///     for the current default locale.
        virtual QString     displayName() const = 0;

        /// \brief
        ///     Returns the short (1 line) description of this theme.
        /// \return
        ///     The short (1 line) description of this theme
        ///     for the current default locale.
        virtual QString     description() const = 0;

        /// \brief
        ///     Returns the small (16x16) icon representing this theme.
        /// \return
        ///     The small (16x16) icon representing this theme.
        virtual QIcon       smallIcon() const = 0;

        /// \brief
        ///     Returns the large (32x32) icon representing this theme.
        /// \return
        ///     The large (32x32) icon representing this theme.
        virtual QIcon       largeIcon() const = 0;

        /// \brief
        ///     Returns the CSS used by this theme
        /// \return
        ///     The CSS used by this theme
        virtual QString     css() const = 0;
    };

    using Themes = QSet<ITheme*>;

    /// \class StandardThemes tt3-gui/API.hpp
    /// \brief The standard (predefined) themes.
    class TT3_GUI_PUBLIC StandardThemes final
    {
        TT3_UTILITY_CLASS(StandardThemes)

        //////////
        //  Operations
    public:
        /// \brief
        ///     Returns the set of all standard Themes.
        /// \return
        ///     The set of all standard Themes.
        static Themes       all();

        //////////
        //  Themes
    public:
        /// \class System tt3-gui/API.hpp
        /// \brief The system theme.
        class TT3_GUI_PUBLIC System final
            :   public virtual ITheme
        {
            TT3_DECLARE_SINGLETON(System)

            //////////
            //  ITheme
        public:
            virtual Mnemonic    mnemonic() const override;
            virtual QString     displayName() const override;
            virtual QString     description() const override;
            virtual QIcon       smallIcon() const override;
            virtual QIcon       largeIcon() const override;
            virtual QString     css() const override;

            //////////
            //  Implementation
        private:
            QString             _css;
        };

        /// \class Light tt3-gui/API.hpp
        /// \brief The "light" theme.
        class TT3_GUI_PUBLIC Light final
            :   public virtual ITheme
        {
            TT3_DECLARE_SINGLETON(Light)

            //////////
            //  ITheme
        public:
            virtual Mnemonic    mnemonic() const override;
            virtual QString     displayName() const override;
            virtual QString     description() const override;
            virtual QIcon       smallIcon() const override;
            virtual QIcon       largeIcon() const override;
            virtual QString     css() const override;

            //////////
            //  Implementation
        private:
            QString             _css;
        };

        /// \class Dark tt3-gui/API.hpp
        /// \brief The "dark" theme.
        class TT3_GUI_PUBLIC Dark final
            :   public virtual ITheme
        {
            TT3_DECLARE_SINGLETON(Dark)

            //////////
            //  ITheme
        public:
            virtual Mnemonic    mnemonic() const override;
            virtual QString     displayName() const override;
            virtual QString     description() const override;
            virtual QIcon       smallIcon() const override;
            virtual QIcon       largeIcon() const override;
            virtual QString     css() const override;

            //////////
            //  Implementation
        private:
            QString             _css;
        };
    };

    /// \class ThemeManager tt3-gui/API.hpp
    /// \brief The manager of known themes.
    class TT3_GUI_PUBLIC ThemeManager final
    {
        TT3_UTILITY_CLASS(ThemeManager)

        //////////
        //  Operations
    public:
        /// \brief
        ///     Returns the set of all registered themes.
        /// \return
        ///     The set of all registered themes.
        static Themes       all();

        /// \brief
        ///     Registers the specified Theme.
        /// \param theme
        ///     The theme to register
        /// \return
        ///     True on success, false on failure. Repeated
        ///     registration of the same theme is treated
        ///     as a "success".
        static bool     register(ITheme * theme);

        /// \brief
        ///     Finds a registered theme by its mnemonic.
        /// \param mnemonic
        ///     The mnemonic to look for.
        /// \return
        ///     The registered theme with the required mnemonic;
        ///     nullptr if not found.
        static ITheme * find(const tt3::util::Mnemonic & mnemonic);

        //////////
        //  Implementation
    private:
        struct _Impl;

        //  Helpers
        static _Impl *  _impl();
    };

    /// \class CurrentTheme tt3-gui/API.hpp
    /// \brief The accessor for a "currently active" theme.
    /// \details
    ///     Only one global static instance of this class
    ///     exists, and other instances should NOT be
    ///     constructed. Cannot be nullptr,
    class TT3_GUI_PUBLIC CurrentTheme final
        :   public QObject
    {
        Q_OBJECT
        TT3_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(CurrentTheme)

        //////////
        //  Construction/destruction
    public:
        /// \brief
        ///     The class constructor.
        CurrentTheme();

        /// \brief
        ///     The class destructor.
        virtual ~CurrentTheme();
        \
        //////////
        //  Operators
    public:
        /// \brief
        ///     Assigns the specified theme to the "current" theme.
        /// \param theme
        ///     The theme to assign to the "current one.
        void            operator = (ITheme * theme);

        /// \brief
        ///     Returns the raw poibnter to the "current" theme.
        /// \return
        ///     The raw poibnter to the "current" theme; nullptr
        ///     if there is no "current" theme.
        ITheme *        operator -> () const;

        /// \brief
        ///     Checks if the "current" theme is non-existent.
        /// \return
        ///     True if the "current" theme is non-existent (nullptr),
        ///     else false.
        bool            operator == (nullptr_t /*null*/) const;

        /// \brief
        ///     Checks if the "current" theme is non-existent.
        /// \return
        ///     False if the "current" theme is non-existent (nullptr),
        ///     else true.
        bool            operator != (nullptr_t /*null*/) const;

        //////////
        //  Signals
    signals:
        /// \brief
        ///     Emitted after the current theme has changed.
        /// \param before
        ///     The "current" theme before the change; nullptr == none.
        /// \param after
        ///     The "current" theme after the change; nullptr == none.
        void                changed(ITheme * before, ITheme * after);

        //////////
        //  Implementation
    private:
        struct _Impl;

        //  Helpers
        static _Impl *  _impl();
    };

#if defined(TT3_GUI_LIBRARY)
    //  Building tt3-gui
#else
    //  Building tt3-gui client
    #ifdef Q_OS_WINDOWS
        Q_DECL_IMPORT CurrentTheme  theCurrentTheme;
    #else
        extern CurrentTheme  theCurrentTheme;
    #endif
#endif
}

//  End of tt3-gui/Theme.hpp
