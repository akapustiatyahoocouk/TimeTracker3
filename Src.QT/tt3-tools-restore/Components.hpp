//
//  tt3-tools-restore/Components.hpp - tt3-tools-restore Components
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

namespace tt3::tools::restore
{
    /// \class Component tt3-tools-restore/API.hpp
    /// \brief The "TT3 Restore" component.
    class TT3_TOOLS_RESTORE_PUBLIC Component final
        :   public virtual tt3::util::IComponent
    {
        DECLARE_SINGLETON(Component)

        //////////
        //  Types
    public:
        /// \class Resources tt3-tools-restore/API.hpp
        /// \brief The component's resources.
        class TT3_TOOLS_RESTORE_PUBLIC Resources final
            :   public tt3::util::FileResourceFactory
        {
            DECLARE_SINGLETON(Resources)
        };

        /// \class Settings tt3-tools-restore/API.hpp
        /// \brief The component's settings.
        class TT3_TOOLS_RESTORE_PUBLIC Settings final
            :   public tt3::util::Settings
        {
            DECLARE_SINGLETON(Settings)
        };

        //////////
        //  IComponent
    public:
        /// \brief A type alias to improve code readability.
        using Mnemonic = tt3::util::Mnemonic;
        /// \brief A type alias to improve code readability.
        using ISubsystem = tt3::util::ISubsystem;
        /// \brief A type alias to improve code readability.
        using IPlugin = tt3::util::IPlugin;

        virtual IPlugin *       plugin() const override;
        virtual Mnemonic        mnemonic() const override;
        virtual QString         displayName() const override;
        virtual QString         description() const override;
        virtual QString         copyright() const override;
        virtual QVersionNumber  version() const override;
        virtual QString         buildNumber() const override;
        virtual ISubsystem *    subsystem() const override;
        virtual Resources *     resources() const override;
        virtual Settings *      settings() override;
        virtual const Settings *settings() const override;
        virtual void            iniialize() override;
        virtual void            deiniialize() override;
    };
}

//  End of tt3-tools-restore/Components.hpp
