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
    /// \class Component tt3-gui/API.hpp
    /// \brief The "TT3 Gui" component.
    class TT3_GUI_PUBLIC Component final
        :   public virtual tt3::util::IComponent
    {
        DECLARE_SINGLETON(Component)

        //////////
        //  Types
    public:
        /// \class Resources tt3-gui/API.hpp
        /// \brief The component's resources.
        class TT3_GUI_PUBLIC Resources final
            :   public tt3::util::FileResourceFactory
        {
            DECLARE_SINGLETON(Resources)
        };

        /// \class Settings tt3-gui/API.hpp
        /// \brief The component's settings.
        class TT3_GUI_PUBLIC Settings final
            :   public tt3::util::Settings
        {
            DECLARE_SINGLETON(Settings)

            //////////
            //  Properties
        public:
            /// \brief
            ///     The mnemonic of the active skin.
            tt3::util::Setting<tt3::util::Mnemonic> activeSkin;

            /// \brief
            ///     The mnemonic of the active theme.
            tt3::util::Setting<tt3::util::Mnemonic> activeTheme;

            /// \brief
            ///     The login last used.
            tt3::util::Setting<QString> lastLogin;

            /// \brief
            ///     The preferred UI locale.
            tt3::util::Setting<QLocale> uiLocale;

            /// \brief
            ///     True if last used workspace should be loaded on startup.
            tt3::util::Setting<bool>    loadLastWorkspaceAtStartup;

            /// \brief
            ///     The mnemonic identifier of the last created or open workspace type.
            tt3::util::Setting<tt3::util::Mnemonic> lastUsedWorkspaceType;

            /// \brief
            ///     True if the last used login shall be remembered on startup.
            tt3::util::Setting<bool>    rememberLastLogin;

            /// \brief
            ///     The mnemonic identifier of the Preferences node that was
            ///     "current" during last activation of the "Preferences" dialog.
            tt3::util::Setting<tt3::util::Mnemonic> currentPreferences;

            //////////
            //  Properties (user confirmations)
        public:
            /// \brief
            ///     True if user needs to confirm closing the current workspace.
            tt3::util::Setting<bool>    confirmCloseWorkspace;

            /// \brief
            ///     True if the user needs to confirm restarting TT3.
            tt3::util::Setting<bool>    confirmRestart;

            /// \brief
            ///     True if the user needs to confirm exiting TT3.
            tt3::util::Setting<bool>    confirmExit;

            //////////
            //  Properties (view configurations)
        public:
            /// \brief
            ///     True if the User Manager widget shall display disabled
            ///     Users and Accounts (false to hide them).
            tt3::util::Setting<bool>    showDisabledUsersAndAccounts;

            /// \brief
            ///     True if the Public Task Manager widget shall display
            ///     completed Public Tasks (false to hide them).
            tt3::util::Setting<bool>    showCompletedPublicTasks;

            /// \brief
            ///     True if the Private Task Manager widget shall display
            ///     completed Private Tasks (false to hide them).
            tt3::util::Setting<bool>    showCompletedPrivateTasks;

            /// \brief
            ///     True if the Project Manager widget shall display
            ///     completed Projects (false to hide them).
            tt3::util::Setting<bool>    showCompletedProjects;

            /// \brief
            ///     The number of days to limit the activities log
            ///     in the "My Day" manager to.
            tt3::util::Setting<int>     myDayLogDepth;

            //////////
            //  Properties (dialogs)
        public:
            /// \brief
            ///     The "current" tab in the ManageQuickPicksListDialog.
            tt3::util::Setting<int>     manageQuickPicksListDialogTab;

            /// \brief
            ///     The "current" tab in the SelectWorkload(s)Dialog.
            tt3::util::Setting<int>     selectWorkloadsDialogTab;

            //////////
            //  Properties (quick reports)
        public:
            /// \brief
            ///     The mnemonic identifier of the "current" quick report.
            tt3::util::Setting<tt3::util::Mnemonic> quickReport;

            /// \brief
            ///     The refresh interval of the quick report, in seconds; 0 == never.
            tt3::util::Setting<int>     quickReportRefreshInterval;
        };

        //////////
        //  IComponent
    public:
        /// \brief A type alias to improve code readability.
        using Mnemonic = tt3::util::Mnemonic;
        /// \brief A type alias to improve code readability.
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
