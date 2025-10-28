//
//  tt3-skin-admin/MainFrame.hpp - Main frame of the admin skin
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
#pragma once
#include "tt3-skin-admin/API.hpp"

namespace tt3::skin::admin
{
    namespace Ui { class MainFrame; }

    /// \class MainFrame tt3-skin-admin/API.hpp
    /// \brief Main frame of the admin skin.
    class TT3_SKIN_ADMIN_PUBLIC MainFrame
        :   public QMainWindow
    {
        Q_OBJECT
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(MainFrame)

        //////////
        //  Construction
    public:
        /// \brief
        ///     Constrctrs the frame.
        /// \param parent
        ///     The parent for the frame, nullptr == none.
        explicit MainFrame(
                QWidget * parent = nullptr
            );

        /// \brief
        ///     The class destructor.
        virtual ~MainFrame();

        //////////
        //  QWidget
    public:
        /// \brief
        ///     Called to handle a "window move" event
        /// \param event
        ///     The event detils.
        virtual void    moveEvent(QMoveEvent * event) override;

        /// \brief
        ///     Called to handle a "window resize" event
        /// \param event
        ///     The event detils.
        virtual void    resizeEvent(QResizeEvent * event) override;

        /// \brief
        ///     Called to handle a "window close" event
        /// \param event
        ///     The event detils.
        virtual void	closeEvent(QCloseEvent * event) override;

        //////////
        //  Operations
    public:
        /// \brief
        ///     Refreshes this main frame and controls within
        void            refresh();

        //////////
        //  Implementation
    private:
        bool            _trackPosition = false;
        tt3::gui::LabelDecorations  _labelDecorations;

        //  Helpers
        void            _loadPosition();
        void            _savePosition();

        bool            _createWorkspace(tt3::ws::WorkspaceAddress workspaceAddress,
                              const QString & adminUser,
                              const QString & adminLogin, const QString & adminPassword);
        bool            _openWorkspace(
                                tt3::ws::WorkspaceAddress workspaceAddress,
                                tt3::ws::OpenMode openMode);
        bool            _reconcileCurrntCredentials(const tt3::ws::Workspace & workspace);
        void            _destroyWorkspace(tt3::ws::WorkspaceAddress workspaceAddress);

        void            _updateMruWorkspaces();
        void            _refreshCurrentActivityControls();

        //////////
        //  Controls
    private:
        Ui::MainFrame *const    _ui;
        QTimer                  _savePositionTimer;
        QTimer                  _refreshTimer;

        //  Custom controls for the tabbed pane
        //  in the middle are created dynamically
        QStackedLayout *        _manageUsersTabLayout = nullptr;
        tt3::gui::UserManager * _userManager = nullptr;

        QStackedLayout *        _manageActivityTypesTabLayout = nullptr;
        tt3::gui::ActivityTypeManager * _activityTypeManager = nullptr;

        QStackedLayout *        _managePublicActivitiesTabLayout = nullptr;
        tt3::gui::PublicActivityManager *   _publicActivityManager = nullptr;

        QStackedLayout *        _managePublicTasksTabLayout = nullptr;
        tt3::gui::PublicTaskManager *   _publicTaskManager = nullptr;

        QStackedLayout *        _managePrivateActivitiesTabLayout = nullptr;
        tt3::gui::PrivateActivityManager *  _privateActivityManager = nullptr;

        QStackedLayout *        _managePrivateTasksTabLayout = nullptr;
        tt3::gui::PrivateTaskManager *  _privateTaskManager = nullptr;

        QStackedLayout *        _manageProjectsTabLayout = nullptr;
        tt3::gui::ProjectManager *  _projectManager = nullptr;

        QStackedLayout *        _manageWorkStreamsTabLayout = nullptr;
        tt3::gui::WorkStreamManager *   _workStreamManager = nullptr;

        QStackedLayout *        _manageBeneficiariesTabLayout = nullptr;
        tt3::gui::BeneficiaryManager *   _beneficiaryManager = nullptr;

        QStackedLayout *        _manageMyDayTabLayout = nullptr;
        tt3::gui::MyDayManager *_myDayManager = nullptr;

        QStackedLayout *        _quickReportsTabLayout = nullptr;
        tt3::gui::QuickReportBrowser *  _quickReportBrowser = nullptr;

        //////////
        //  Signal handlers
    private slots:
        void            _savePositionTimerTimeout();
        void            _onActionNewWorkspace();
        void            _onActionOpenWorkspace();
        void            _onActionCloseWorkspace();
        void            _onActionDestroyWorkspace();
        void            _onActionRestart();
        void            _onActionExit();
        void            _onActionManageUsers();
        void            _onActionManageActivityTypes();
        void            _onActionManagePublicActivities();
        void            _onActionManagePublicTasks();
        void            _onActionManagePrivateActivities();
        void            _onActionManagePrivateTasks();
        void            _onActionManageProjects();
        void            _onActionManageWorkStreams();
        void            _onActionManageBeneficiaries();
        void            _onActionManageMyDay();
        void            _onActionQuickReports();
        void            _onActionRefresh();
        void            _onActionPreferences();
        void            _onActionLoginAsDifferentUser();
        void            _onActionHelpContent();
        void            _onActionHelpIndex();
        void            _onActionHelpSearch();
        void            _onActionAbout();
        void            _stopActivityPushButtonClicked();

        void            _workspaceClosed(tt3::ws::WorkspaceClosedNotification);
        void            _currentWorkspaceChanged(tt3::ws::Workspace, tt3::ws::Workspace);
        void            _currentCredentialsChanged(tt3::ws::Credentials, tt3::ws::Credentials);
        void            _currentActivityChanged(tt3::ws::Activity, tt3::ws::Activity);
        void            _currentThemeChanged(tt3::gui::ITheme*, tt3::gui::ITheme*);
        void            _managersTabWidgetCurrentChanged(int);
        void            _refreshTimerTimeout();
    };
}

//  End of tt3-skin-admin/MainFrame.hpp
