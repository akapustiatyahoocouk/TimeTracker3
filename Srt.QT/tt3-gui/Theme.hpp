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
    //  A generic "theme" adjusts the appearance (e.g.
    //  colors, etc.) of TT3
    class TT3_GUI_PUBLIC ITheme
    {
        //////////
        //  This is an interface
    protected:
        ITheme() = default;
        virtual ~ITheme() = default;

        //////////
        //  Operations (properties)
    public:
        using Mnemonic = tt3::util::Mnemonic;

        //  The mnemonic identifier of this theme.
        virtual Mnemonic    mnemonic() const = 0;

        //  The user-readable display name of this theme
        //  for the current default locale.
        virtual QString     displayName() const = 0;

        //  The short (1 line) description of this theme
        //  for the current default locale.
        virtual QString     description() const = 0;

        //  The small (16x16) icon representing this theme.
        virtual QIcon       smallIcon() const = 0;

        //  The large (32x32) icon representing this theme.
        virtual QIcon       largeIcon() const = 0;

        //  The css used by this theme
        virtual QString     css() const = 0;
    };

    //  The standard (predefined) themes
    class TT3_GUI_PUBLIC StandardThemes final
    {
        UTILITY_CLASS(StandardThemes)

        //////////
        //  Themes
    public:
        //  The system theme
        class TT3_GUI_PUBLIC System final : public virtual ITheme
        {
            DECLARE_SINGLETON(System)

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

        //  The "light" theme
        class TT3_GUI_PUBLIC Light final : public virtual ITheme
        {
            DECLARE_SINGLETON(Light)

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

        //  The "dark" theme
        class TT3_GUI_PUBLIC Dark final : public virtual ITheme
        {
            DECLARE_SINGLETON(Dark)

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

    //  The manager of known themes
    class TT3_GUI_PUBLIC ThemeManager final
    {
        UTILITY_CLASS(ThemeManager)

        //////////
        //  Operations
    public:
        //  Returns the set of all registered themes.
        static QSet<ITheme*>allThemes();

        //  Registers the specified Theme; returns true on success,
        //  false on failure.
        static bool         registerTheme(ITheme * theme);

        //  Finds a registered theme by its mnemonic; returns
        //  nullptr if not found.
        static ITheme *     findTheme(const QString & mnemonic);

        //////////
        //  Implementation
    private:
        static tt3::util::Mutex _guard;
        static QMap<QString, ITheme*>   _registry;  //  key = mnemonic

        //  Helpers
        static void         _registerStandardThemes();
    };

    //  The accessor for a "currently active" theme.
    //  Only one global static instance of this class
    //  exists, and other instances should NOT be
    //  constructed. Cannot be nullptr,
    class TT3_GUI_PUBLIC CurrentTheme final : public QObject
    {
        Q_OBJECT
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(CurrentTheme)

        //////////
        //  Construction/destruction
    public:
        CurrentTheme();
        ~CurrentTheme();
        \
        //////////
        //  Operators
    public:
        void            operator = (ITheme * theme);
        ITheme *        operator -> () const;

        bool            operator == (nullptr_t null) const;
        bool            operator != (nullptr_t null) const;

        //////////
        //  Signals
    signals:
        //  Emitted after the current theme has changed.
        void                changed(ITheme * before, ITheme * after);

        //////////
        //  Implementation
    private:
        static std::atomic<int>     _instanceCount; //  ...to disallow a second instance
        mutable tt3::util::Mutex    _currentThemeGuard;
        ITheme *                    _currentTheme;
    };

#if defined(TT3_GUI_LIBRARY)
    //  Building tt3-gui
#else
    //  Building tt3-gui client
    Q_DECL_IMPORT CurrentTheme  theCurrentTheme;
#endif
}

//  End of tt3-gui/Theme.hpp
