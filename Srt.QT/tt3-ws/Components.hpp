//
//  tt3-ws/Components.hpp - tt3-ws Components
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
    //  The "TT3 Workspace" component
    class TT3_WS_PUBLIC Component final : public virtual tt3::util::IComponent
    {
        DECLARE_SINGLETON(Component)

        //////////
        //  Types
    public:
        //  The component's resources
        class TT3_WS_PUBLIC Resources final : public tt3::util::FileResourceFactory
        {
            DECLARE_SINGLETON(Resources)
        };

        //  The component's settings
        class TT3_WS_PUBLIC Settings final : public tt3::util::Settings
        {
            DECLARE_SINGLETON(Settings)

            //////////
            //  Constants
        public:
            //  The maximum size of the MRU workspaces list.
            static inline const int MaxRecentWorkspaces = 9;

            //////////
            //  Properties
        public:
            //  The addresses of the recently used workspaces, most recent first.
            tt3::util::Setting<WorkspaceAddressesList>  recentWorkspaces;

            //////////
            //  Operations
        public:
            //  Adds the specified entry to the MRU workspaces list.
            //  Idf the entry is already there, moves it to the top of the list.
            void                addRecentWorkspace(const WorkspaceAddress workspaceAddress);

            //  Removes the specified entry from the MRU workspaces list.
            //  If already not there, the call has no effect.
            void                removeRecentWorkspace(const WorkspaceAddress workspaceAddress);
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

//  End of tt3-qw/Components.hpp
