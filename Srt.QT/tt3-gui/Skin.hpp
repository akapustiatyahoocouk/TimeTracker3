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
    //  A generic "skin" is a way to present a GUI
    class TT3_GUI_PUBLIC ISkin
    {
        //////////
        //  This is an interface
    protected:
        ISkin() = default;
        virtual ~ISkin() = default;

        //////////
        //  Operations (properties)
    public:
        using Mnemonic = tt3::util::Mnemonic;

        //  The mnemonic identifier of this skin
        virtual Mnemonic    mnemonic() const = 0;

        //  The user-readable display name of this skin
        //  for the current default locale.
        virtual QString     displayName() const = 0;

        //  The short (1 line) description of this skin
        //  for the current default locale.
        virtual QString     description() const = 0;

        //  The small (16x16) icon representing this skin.
        virtual QIcon       smallIcon() const = 0;

        //  The large (32x32) icon representing this skin.
        virtual QIcon       largeIcon() const = 0;

        //  True if this skin can be chosed by default
        //  if no skin or a nonexistent skin is requested
        //  (e.g. on first TT3 start).
        virtual bool        isDefault() const = 0;

        //////////
        //  Operations (state)
    public:
        //  Checks whether this skin is "active" (i.e. its UI is visible).
        virtual bool        isActive() const = 0;

        //  "Activates" this skin, showing its UI.
        //  Gas no effect if this skin is already "active".
        virtual void        activate() = 0;

        //  "Deactivates" this skin, hiding its UI.
        //  Gas no effect if this skin is already "inactive".
        virtual void        deactivate() = 0;

        //  Returns the main frame" of this skin's UI.
        //  Returns nullptr is this skin is "inactive" or
        //  its UI has no "main frame".
        virtual QMainWindow*mainWindow() = 0;
    };

    //  The manager of known skins
    class TT3_GUI_PUBLIC SkinManager final
    {
        UTILITY_CLASS(SkinManager)

        //////////
        //  Operations
    public:
        //  Returns the set of all registered Skins.
        static QSet<ISkin*> allSkins();

        //  Registers the specified Skin; returns true on success,
        //  false on failure.
        static bool         registerSkin(ISkin * skin);

        //  Finds a registered Skin by its mnemonic; returns
        //  nullptr if not found.
        static ISkin *      findSkin(const tt3::util::Mnemonic & mnemonic);

        //////////
        //  Implementation
    private:
        struct _Impl;

        //  Helpers
        static _Impl *      _impl();
    };

    //  The accessor for a "currently active" skin.
    //  Only one global static instance of this class
    //  exists, and other instances should NOT be
    //  constructed.
    //  Can be nullptr if there is no "current" skin.
    class TT3_GUI_PUBLIC CurrentSkin final : public QObject
    {
        Q_OBJECT
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(CurrentSkin)

        //////////
        //  Construction/destruction
    public:
        CurrentSkin();
        virtual ~CurrentSkin();
\
        //////////
        //  Operators
        //  IMPORTANT: Changing the "current" skin does
        //  NOT change its "active" status.
    public:
        void                operator = (ISkin * skin);
        ISkin *             operator -> () const;

        bool                operator == (nullptr_t null) const;
        bool                operator != (nullptr_t null) const;

        //////////
        //  Signals
    signals:
        //  Emitted after the current skin has changed.
        void                changed(ISkin * before, ISkin * after);

        //////////
        //  Implementation
    private:
        static std::atomic<int>     _instanceCount; //  ...to disallow a second instance
        mutable tt3::util::Mutex    _currentSkinGuard;
        ISkin *                     _currentSkin;
    };

#if defined(TT3_GUI_LIBRARY)
    //  Building tt3-gui
#else
    //  Building tt3-gui client
    Q_DECL_IMPORT CurrentSkin theCurrentSkin;
#endif
}

//  End of tt3-gui/Skin.hpp
