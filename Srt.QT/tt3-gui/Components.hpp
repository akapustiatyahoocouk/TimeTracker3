//
//  tt3-gui/Components.hpp - TT3-gui Components
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
    //  The "TT3 GUO" component settings
    class TT3_GUI_PUBLIC GuiSettings final : public util::Settings
    {
        DECLARE_SINGLETON(GuiSettings)

        //////////
        //  Properties
    public:
        mutable util::Setting<QString>  activeSkin; //  The mnemonic of the active skin
    };

    //  The "TT3 Gui" component
    class TT3_GUI_PUBLIC GuiComponent final : public util::Component
    {
        DECLARE_SINGLETON(GuiComponent)

        //////////
        //  Component
    public:
        virtual GuiSettings &   settings() override { return *GuiSettings::instance(); }

        //////////
        //  Implementation
    private:
    };
}

//  End of tt3-gui/Components.hpp
