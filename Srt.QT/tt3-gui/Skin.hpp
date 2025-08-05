//
//  tt3-gui/Skin.hpp - TT3-gui Skins framework
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

namespace gui
{
    //  A generic "skin" is a way to preent a GUI
    class TT3_GUI_PUBLIC ISkin
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(ISkin)

        //////////
        //  This is an interface
    public:
        ISkin() = default;
        virtual ~ISkin() = default;

        //////////
        //  Operations
    public:
        //  TODO document
        virtual QString     mnemonic() const = 0;
        virtual QString     displayName() const = 0;
        virtual QString     desciption() const = 0;

        //  True if this skin can be chosed by default
        //  if no skin or a nonexistent skin is requested
        //  (e.g. on first TT3 start).
        virtual bool        isDefault() const = 0;
    };

    //////////
    //  The registry of known skins
    class TT3_GUI_PUBLIC SkinRegistry final
    {
        UTILITY_CLASS(SkinRegistry)

        //////////
        //  Operationds
    public:
        //  TODO document
        static QSet<ISkin*> allSkins();
        static bool         registerSkin(ISkin * skin);
        static ISkin *      findSkin(const QString & mnemonic);

        //////////
        //  Implementation
    private:
        static QMap<QString, ISkin*>    _registry;  //  key = mnemonic
    };
}

//  End of tt3-gui/Skin.hpp
