//
//  tt3-db-api/Components.hpp - tt3-db-api Components
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

namespace tt3::db::api
{
    /// \class Component tt3-db-api/API.hpp
    /// \brief The "TT3 DB API" component.
    class TT3_DB_API_PUBLIC Component final :
        public virtual tt3::util::IComponent
    {
        DECLARE_SINGLETON(Component)

        //////////
        //  Types
    public:
        /// \class Resources tt3-db-api/API.hpp
        /// \brief The component's resources.
        class TT3_DB_API_PUBLIC Resources final
            :   public tt3::util::FileResourceFactory
        {
            DECLARE_SINGLETON(Resources)
        };

        /// \class Settings tt3-db-api/API.hpp
        /// \brief The component's settings.
        class TT3_DB_API_PUBLIC Settings final
            :   public tt3::util::Settings
        {
            DECLARE_SINGLETON(Settings)
        };

        //////////
        //  IComponent
    public:
        virtual auto    mnemonic(
                            ) const -> tt3::util::Mnemonic override;
        virtual auto    displayName(
                            ) const -> QString override;
        virtual auto    description(
                            ) const -> QString override;
        virtual auto    copyright(
                            ) const -> QString override;
        virtual auto    version(
                            ) const -> QVersionNumber override;
        virtual auto    buildNumber(
                            ) const -> QString override;
        virtual auto    subsystem(
                            ) const -> tt3::util::ISubsystem * override;
        virtual auto    resources(
                            ) const -> Resources * override;
        virtual auto    settings(
                            ) -> Settings * override;
        virtual auto    settings(
                            ) const -> const Settings * override;
    };
}

//  End of tt3-db-api/Components.hpp
