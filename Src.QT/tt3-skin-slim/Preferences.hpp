//
//  tt3-skin-slim/Preferences.hpp - Slim skin preferences
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

namespace tt3::skin::slim
{
    /// \class SlimSkinPreferences tt3-skin-slim/API.hpp
    /// \brief The "/Interface/SlimSkin" preferences.
    class TT3_SKIN_SLIM_PUBLIC SlimSkinPreferences final
    :   public tt3::gui::Preferences
    {
        TT3_DECLARE_SINGLETON(SlimSkinPreferences)

        //////////
        //  Preferences
    public:
        virtual Mnemonic        mnemonic() const override;
        virtual QString         displayName() const override;
        virtual Preferences *   parent() const override;
        virtual int             order() const { return 0; }
        virtual auto            createEditor(
                                    ) -> tt3::gui::PreferencesEditor * override;
    };

}

//  End of tt3-skin-slim/Preferences.hpp
