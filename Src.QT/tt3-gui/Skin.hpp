//
//  tt3-gui/Skin.hpp - tt3 Skins framework
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
    /// \class ISkin tt3-gui/API.hpp
    /// \brief A generic "skin" is a way to present a GUI.
    class TT3_GUI_PUBLIC ISkin
    {
        //////////
        //  This is an interface
    protected:
        /// \brief
        ///     The empty [interface] constructor.
        ISkin() = default;

        /// \brief
        ///     The empty [interface] destructor.
        virtual ~ISkin() = default;

        //////////
        //  Operations (properties)
    public:
        /// \brief A type alias to improve code readability.
        using Mnemonic = tt3::util::Mnemonic;

        /// \brief
        ///     Returns the mnemonic identifier of this skin
        /// \return
        ///     The mnemonic identifier of this skin
        virtual Mnemonic    mnemonic() const = 0;

        /// \brief
        ///     Returns the user-readable display name of this skin.
        /// \return
        ///     The user-readable display name of this skin
        ///     for the current default locale.
        virtual QString     displayName() const = 0;

        /// \brief
        ///     Returns the short (1 line) description of this skin.
        /// \return
        ///     The short (1 line) description of this skin
        ///     for the current default locale.
        virtual QString     description() const = 0;

        /// \brief
        ///     Returns the small (16x16) icon representing this skin.
        /// \return
        ///     The small (16x16) icon representing this skin.
        virtual QIcon       smallIcon() const = 0;

        /// \brief
        ///     Returns the large (32x32) icon representing this skin.
        /// \return
        ///     The large (32x32) icon representing this skin.
        virtual QIcon       largeIcon() const = 0;

        /// \brief
        ///     Checks if this is a "defalt" skin.
        /// \return
        ///     True if this skin can be chosed by default
        ///     if no skin or a nonexistent skin is requested
        ///     (that is, on first TT3 start).
        virtual bool        isDefault() const = 0;

        //////////
        //  Operations (state)
    public:
        /// \brief
        ///     Checks whether this skin is "active" (its UI is visible).
        /// \return
        ///     True if this skin is "active", else false.
        virtual bool        isActive() const = 0;

        /// \brief
        ///     "Activates" this skin, showing its UI.
        /// \details
        ///     Has no effect if this skin is already "active".
        virtual void        activate() = 0;

        /// \brief
        ///     "Deactivates" this skin, hiding its UI.
        /// \details
        ///     Has no effect if this skin is already "inactive".
        virtual void        deactivate() = 0;

        /// \brief
        ///     Returns the main frame" of this skin's UI.
        /// \return
        ///     The main frame" of this skin's UI, or nullptr if
        ///     this skin is "inactive" or its UI has no "main frame".
        virtual QMainWindow*mainWindow() = 0;
    };

    /// \class SkinManager tt3-gui/API.hpp
    /// \brief The manager of known skins
    class TT3_GUI_PUBLIC SkinManager final
    {
        TT3_UTILITY_CLASS(SkinManager)

        //////////
        //  Operations
    public:
        /// \brief
        ///     Returns the set of all registered Skins.
        /// \return
        ///     The set of all registered Skins.
        static QSet<ISkin*> allSkins();

        /// \brief
        ///     Registers the specified Skin.
        /// \param skin
        ///     The Skin to register,
        /// \return
        ///     True on success, false on failure. A repeated
        ///     registration of the same skin is treated as a "success".
        static bool         registerSkin(ISkin * skin);

        /// \brief
        ///     Finds a registered Skin by its mnemonic.
        /// \param mnemonic
        ///     The mnemonic to look for.
        /// \return
        ///     The registered skin with the required mnemonic;
        ///     nullptr if not found.
        static ISkin *      findSkin(const tt3::util::Mnemonic & mnemonic);

        //////////
        //  Implementation
    private:
        struct _Impl;

        //  Helpers
        static _Impl *      _impl();
    };

    /// \class CurrentSkin tt3-gui/API.hpp
    /// \brief The accessor for a "currently active" skin.
    /// \details
    ///     Only one global static instance of this class
    ///     exists, and other instances should NOT be
    ///     constructed.
    //      Can be nullptr if there is no "current" skin.
    class TT3_GUI_PUBLIC CurrentSkin final
        :   public QObject
    {
        Q_OBJECT
        TT3_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(CurrentSkin)

        //////////
        //  Construction/destruction
    public:
        /// \brief
        ///     The class constructor.
        CurrentSkin();

        /// \brief
        ///     The class destructor.
        virtual ~CurrentSkin();
\
        //////////
        //  Operators
    public:
        /// \brief
        ///     Replaces the "current" skin with the secified skin.
        /// \details
        ///     Changing the "current" skin does NOT change its "active" status.
        /// \param skin
        ///     The skin to make "current"; nullptr == none.
        void                operator = (ISkin * skin);

        /// \brief
        ///     Returns a raw pointer to the "current" skin.
        /// \return
        ///     The raw pointer to the "current" skin; nullptr == there is none.
        ISkin *             operator -> () const;

        /// \brief
        ///     Checks if the "current" skin is non-existent.
        /// \return
        ///     True if the "current" skin is non-existent (nullptr),
        ///     else false.
        bool                operator == (nullptr_t /*null*/) const;

        /// \brief
        ///     Checks if the "current" skin is non-existent.
        /// \return
        ///     False if the "current" skin is non-existent (nullptr),
        ///     else true.
        bool                operator != (nullptr_t /*null*/) const;

        //////////
        //  Signals
    signals:
        /// \brief
        ///     Emitted after the current skin has changed.
        /// \param before
        ///     The "current" skin before the change; nullptr == none.
        /// \param after
        ///     The "current" skin after the change; nullptr == none.
        void                changed(ISkin * before, ISkin * after);

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
    Q_DECL_IMPORT CurrentSkin theCurrentSkin;
#endif
}

//  End of tt3-gui/Skin.hpp
