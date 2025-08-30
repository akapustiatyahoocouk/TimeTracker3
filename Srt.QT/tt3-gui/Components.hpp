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

namespace tt3::gui
{
    //  The "TT3 Gui" component
    class TT3_GUI_PUBLIC Component final : public virtual tt3::util::IComponent
    {
        DECLARE_SINGLETON(Component)

        //////////
        //  Types
    public:
        //  The component's resources
        class TT3_GUI_PUBLIC Resources final : public tt3::util::FileResourceFactory
        {
            DECLARE_SINGLETON(Resources)
        };

        //  The component's settings
        class TT3_GUI_PUBLIC Settings final : public tt3::util::Settings
        {
            DECLARE_SINGLETON(Settings)

            //////////
            //  Properties
        public:
            //  The mnemonic of the active skin
            tt3::util::Setting<tt3::util::Mnemonic> activeSkin;

            //  The mnemonic of the active theme
            tt3::util::Setting<tt3::util::Mnemonic> activeTheme;

            //  The login last used
            tt3::util::Setting<QString> lastLogin;

            //  The preferred UI locale
            tt3::util::Setting<QLocale> uiLocale;

            //  True if last used workspace should be loaded on startup.
            tt3::util::Setting<bool>    loadLastWorkspaceAtStartup;

            //  True if the last used login shall be remembered on startup.
            tt3::util::Setting<bool>    rememberLastLogin;

            //  The mnemonic identifier of the Preferences node that  was
            //  "current" during last activation og yje "Preferences" dialog.
            tt3::util::Setting<tt3::util::Mnemonic> currentPreferences;
        };

        //////////
        //  IComponent
    public:
        using Mnemonic = tt3::util::Mnemonic;
        using Subsystem = tt3::util::ISubsystem;

        virtual Mnemonic        mnemonic() const override;
        virtual QString         displayName() const override;
        virtual QString         description() const override;
        virtual QString         copyright() const override;
        virtual QVersionNumber  version() const override;
        virtual QString         buildNumber() const override;
        virtual Subsystem *     subsystem() const override;
        virtual Resources *     resources() const override;
        virtual Settings *      settings() override;
        virtual const Settings *settings() const override;
    };
}

//  End of tt3-gui/Components.hpp
