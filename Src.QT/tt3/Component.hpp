//
//  tt3/Component.hpp - TT3 Component
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

namespace tt3
{
    /// \class Component tt3/API.hpp
    /// \brief The "TT3 application" component.
    class Component final
        :   public virtual tt3::util::IComponent
    {
        TT3_DECLARE_COMPONENT(Component)

        //////////
        //  Types
    public:
        /// \class Resources tt3/API.hpp
        /// \brief The component's resources.
        class Resources final
            :   public tt3::util::FileResourceFactory
        {
            TT3_DECLARE_SINGLETON(Resources)
        };

        /// \class Settings tt3/API.hpp
        /// \brief The component's settings.
        class Settings final
            :   public tt3::util::Settings
        {
            TT3_DECLARE_SINGLETON(Settings)
        };

        //////////
        //  IComponent
    public:
        virtual auto    mnemonic() const -> Mnemonic override;
        virtual QString displayName() const override;
        virtual QString description() const override;
        virtual QString copyright() const override;
        virtual auto    version() const -> QVersionNumber override;
        virtual QString stage() const override;
        virtual QString buildNumber() const override;
        virtual auto    license() const -> tt3::util::ILicense * override;
        virtual auto    subsystem() const -> ISubsystem * override;
        virtual auto    resources() const -> Resources * override;
        virtual auto    settings() -> Settings * override;
        virtual auto    settings() const -> const Settings * override;
        virtual void    initialize() override;
        virtual void    deinitialize() override;
    };
}

//  End of tt3/Component.hpp
