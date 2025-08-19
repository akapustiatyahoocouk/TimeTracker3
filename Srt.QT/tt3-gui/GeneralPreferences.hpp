//
//  tt3-gui/GeneralPreferences.hpp - The "/General" preferences
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

namespace tt3::gui
{
    //  The "/General" preferences
    class TT3_GUI_PUBLIC GeneralPreferences final : public Preferences
    {
        DECLARE_SINGLETON(GeneralPreferences)

        //////////
        //  Preferences
    public:
        virtual QString         mnemonic() const override;
        virtual QString         displayName() const override;
        virtual Preferences *   parent() const override;
        virtual PreferencesEditor * createEditor() override;
    };
}

//  End of tt3-gui/GeneralPreferences.hpp

