//
//  tt3-ws/Component.hpp - tt3-ws Component
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

namespace tt3::ws
{
    /// \class Component tt3-ws/API.hpp
    /// \brief The "TT3 Workspace" component.
    class TT3_WS_PUBLIC Component final
        :   public virtual tt3::util::IComponent
    {
        TT3_DECLARE_COMPONENT(Component)

        //////////
        //  Types
    public:
        /// \class Resources tt3-ws/API.hpp
        /// \brief The component's resources.
        class TT3_WS_PUBLIC Resources final
            :   public tt3::util::FileResourceFactory
        {
            TT3_DECLARE_SINGLETON(Resources)
        };

        /// \class Settings tt3-ws/API.hpp
        /// \brief The component's settings.
        class TT3_WS_PUBLIC Settings final
            :   public tt3::util::Settings
        {
            TT3_DECLARE_SINGLETON(Settings)

            //////////
            //  Constants
        public:
            /// \brief
            ///     The maximum size of the MRU workspaces list.
            static inline const int MaxRecentWorkspaces = 9;

            //////////
            //  Properties
        public:
            /// \brief
            ///     The addresses of the recently used workspaces, most
            ///     recent first.
            tt3::util::Setting<WorkspaceAddressesList>  recentWorkspaces;

            //////////
            //  Operations
        public:
            /// \brief
            ///     Adds the specified entry to the MRU workspaces list.
            /// \details
            ///     If the entry is already there, moves it to the top
            ///     of the list.
            /// \param workspaceAddress
            ///     The workspace address to add to the MRU list
            void            addRecentWorkspace(
                                    WorkspaceAddress workspaceAddress
                                );

            /// \brief
            ///     Removes the specified entry from the MRU
            ///     workspaces list.
            /// \details
            ///     If already not there, the call has no effect.
            /// \param workspaceAddress
            ///     The workspace address to remove from the MRU list
            void            removeRecentWorkspace(
                                    WorkspaceAddress workspaceAddress
                                );
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

//  End of tt3-ws/Component.hpp
