//
//  tt3-gui/Style.hpp - tt3 Theming framework
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
    /// \class IStyle tt3-gui/API.hpp
    /// \brief
    ///     A generic "Style" adjusts the appearance (that is
    ///     clook-and-feel) of TT3.
    class TT3_GUI_PUBLIC IStyle
    {
        //////////
        //  This is an interface
    protected:
        /// \brief
        ///     The empty [interface] constructor.
        IStyle() = default;

        /// \brief
        ///     The empty [interface] destructor.
        virtual ~IStyle() = default;

        //////////
        //  Operations (properties)
    public:
        /// \brief A type alias to improve code readability.
        using Mnemonic = tt3::util::Mnemonic;

        /// \brief
        ///     Returns the mnemonic identifier of this Style.
        /// \return
        ///     The mnemonic identifier of this Style.
        virtual auto    mnemonic() const -> Mnemonic = 0;

        /// \brief
        ///     Returns the user-readable display name of this Style.
        /// \return
        ///     The user-readable display name of this Style
        ///     for the current default locale.
        virtual QString displayName() const = 0;

        /// \brief
        ///     Returns the short (1 line) description of this Style.
        /// \return
        ///     The short (1 line) description of this Style
        ///     for the current default locale.
        virtual QString description() const = 0;

        /// \brief
        ///     Returns the small (16x16) icon representing this Style.
        /// \return
        ///     The small (16x16) icon representing this Style.
        virtual QIcon   smallIcon() const = 0;

        /// \brief
        ///     Returns the large (32x32) icon representing this Style.
        /// \return
        ///     The large (32x32) icon representing this Style.
        virtual QIcon   largeIcon() const = 0;

        /// \brief
        ///     Applies this Style to the currently running QApplication.
        /// TODO use the same apply() pattern to ITheme instead
        /// of exposing the theme's underlying CSS
        virtual void    apply() = 0;
    };

    using Styles = QSet<IStyle*>;

    /// \class BuiltinStyle tt3-gui/API.hpp
    /// \brief
    ///     A "Style" implemented by Qt's QStyle.
    class TT3_GUI_PUBLIC BuiltinStyle final
        :   public virtual IStyle
    {
        TT3_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(BuiltinStyle)

        friend class StyleManager;

        //////////
        //  Construction/destroucion - internal only
    private:
        explicit BuiltinStyle(const QString & styleName);
        virtual ~BuiltinStyle() = default;

        //////////
        //  IStyle
    public:
        virtual auto    mnemonic() const -> Mnemonic override;
        virtual QString displayName() const override;
        virtual QString description() const override;
        virtual QIcon   smallIcon() const override;
        virtual QIcon   largeIcon() const override;
        virtual void    apply() override;

        //////////
        //  Implementation
    private:
        const QString   _styleName;     //  as known to Qt, LOWERCASE!
        mutable QString _displayName;   //  evaluated lazily as needed
    };

    /// \class StyleManager tt3-gui/API.hpp
    /// \brief The manager of known Styles.
    class TT3_GUI_PUBLIC StyleManager final
    {
        TT3_UTILITY_CLASS(StyleManager)

        //////////
        //  Operations
    public:
        /// \brief
        ///     Returns the set of all registered Styles.
        /// \return
        ///     The set of all registered Styles.
        static Styles   all();

        /// \brief
        ///     Registers the specified Style.
        /// \param style
        ///     The Style to register
        /// \return
        ///     True on success, false on failure. Repeated
        ///     registration of the same Style is treated
        ///     as a "success".
        static bool     register(IStyle * style);

        /// \brief
        ///     Finds a registered Style by its mnemonic.
        /// \param mnemonic
        ///     The mnemonic to look for.
        /// \return
        ///     The registered Style with the required mnemonic;
        ///     nullptr if not found.
        static IStyle * find(const tt3::util::Mnemonic & mnemonic);

        //////////
        //  Implementation
    private:
        struct _Impl;

        //  Helpers
        static _Impl *  _impl();
    };

    /// \class CurrentStyle tt3-gui/API.hpp
    /// \brief The accessor for a "currently active" Style.
    /// \details
    ///     Only one global static instance of this class
    ///     exists, and other instances should NOT be
    ///     constructed. Cannot be nullptr,
    class TT3_GUI_PUBLIC CurrentStyle final
        :   public QObject
    {
        Q_OBJECT
        TT3_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(CurrentStyle)

        //////////
        //  Construction/destruction
    public:
        /// \brief
        ///     The class constructor.
        CurrentStyle();

        /// \brief
        ///     The class destructor.
        virtual ~CurrentStyle();
    \
        //////////
        //  Operators
    public:
        /// \brief
        ///     Assigns the specified Style to the "current" Style.
        /// \param style
        ///     The Style to assign to the "current one.
        void            operator = (IStyle * style);

        /// \brief
        ///     Returns the raw poibnter to the "current" Style.
        /// \return
        ///     The raw poibnter to the "current" Style; nullptr
        ///     if there is no "current" Style.
        IStyle *        operator -> () const;

        /// \brief
        ///     Checks if the "current" Style is non-existent.
        /// \return
        ///     True if the "current" Style is non-existent (nullptr),
        ///     else false.
        bool            operator == (nullptr_t /*null*/) const;

        /// \brief
        ///     Checks if the "current" Style is non-existent.
        /// \return
        ///     False if the "current" Style is non-existent (nullptr),
        ///     else true.
        bool            operator != (nullptr_t /*null*/) const;

        //////////
        //  Signals
    signals:
        /// \brief
        ///     Emitted after the current Style has changed.
        /// \param before
        ///     The "current" Style before the change; nullptr == none.
        /// \param after
        ///     The "current" Style after the change; nullptr == none.
        void                changed(IStyle * before, IStyle * after);

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
        Q_DECL_IMPORT CurrentStyle  theCurrentStyle;
    #else
        extern CurrentStyle  theCurrentStyle;
    #endif
#endif
}

//  End of tt3-gui/Style.hpp
