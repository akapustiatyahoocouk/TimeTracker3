//
//  tt3-skin-slim/MainFrame.hpp - Main frame of the slim skin
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
#include "tt3-skin-slim/API.hpp"

namespace tt3::skin::slim
{
    namespace Ui { class MainFrame; }

    /// \class MainFrame tt3-skin-slim/API.hpp
    /// \brief Main frame of the slim skin.
    class TT3_SKIN_SLIM_PUBLIC MainFrame final
        :   public QMainWindow
    {
        Q_OBJECT
        TT3_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(MainFrame)

        friend class Skin;

        //////////
        //  Constants
    public:
        /// \brief
        ///     The minimum size of the main frame.
        inline static const QSize MinimumSize {96, 64};

        /// \brief
        ///     The minimum allowed opacity (in %).
        inline static const int MinOpacity = 10;

        /// \brief
        ///     The maximum allowed opacity (in %).
        inline static const int MaxOpacity = 100;

        //////////
        //  Construction/destruction
    public:
        /// \brief
        ///     Constructs the main frame.
        MainFrame();

        /// \brief
        ///     The class destructor.
        ~MainFrame();

        //////////
        //  QWidget
    protected:
        virtual bool    event(QEvent * event) override;

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

        virtual void    mousePressEvent(QMouseEvent * event) override;
        virtual void    mouseReleaseEvent(QMouseEvent * event) override;
        virtual void    mouseMoveEvent(QMouseEvent * event) override;
        virtual void    contextMenuEvent(QContextMenuEvent * event) override;
        virtual void    keyPressEvent(QKeyEvent * event) override;
        virtual void    paintEvent(QPaintEvent * event) override;

        //////////
        //  Operations
    public:
        /// \brief
        ///     Shows this frame; has no effect if already visible.
        void            show();

        /// \brief
        ///     Hides this frame; has no effect if already invisible.
        void            hide();

        /// \brief
        ///     Refreshes this main frame and controls within
        void            refresh();

        /// \brief
        ///     Specifies whether this frame shall be a topmost
        ///     frame even when it is inactive.
        /// \param alwaysOnTop
        ///     True to make this frame topmost, false to return to normal.
        void            setAlwaysOnTop(bool alwaysOnTop);

        /// \brief
        ///     Sets the opacity of this frame.
        /// \param opacity
        ///     The new opacity (in %) for this frame;
        ///     100 == fully opaque.
        void            setOpacity(int opacity);

        //////////
        //  Implementation
    private:
        bool            _trackPosition = false;

        bool            _dragging = false;
        QPoint          _dragAnchor;

        struct _ControlAreaImpl
        {
            _ControlAreaImpl(
                    const QRect & r,
                    const QColor & b
                ) : rect(r), bias(b) {}
            virtual ~_ControlAreaImpl() = default;

            QRect       rect;
            QColor      bias;
            bool        pressed = false;
            QString     text;   //  populated when painted
        };

        struct _ActivityAreaImpl : public _ControlAreaImpl
        {
            _ActivityAreaImpl(
                    const QRect & r,
                    tt3::ws::Activity a
                ) : _ControlAreaImpl(r, QColor(0x008000)),
                    activity(a) {}

            tt3::ws::Activity   activity;
        };

        struct _ApplicationAreaImpl : public _ControlAreaImpl
        {
            _ApplicationAreaImpl(
                    const QRect & r
                ) : _ControlAreaImpl(r, QColor(0x800080)) {}
        };

        using _ControlArea = std::shared_ptr<_ControlAreaImpl>;
        using _ControlAreas = QList<_ControlArea>;

        _ControlAreas   _controlAreas;

        //  Helpers
        void            _loadPosition();
        void            _savePosition();

        void            _recalculateControlAreas();
        void            _draw(QPainter & p, const _ControlArea & controlArea);
        void            _drawRect3D(QPainter & p, const QRect & rc, const QColor & tl, const QColor & br);

        bool            _createWorkspace(
                                tt3::ws::WorkspaceAddress workspaceAddress,
                                const QString & adminUser,
                                const QString & adminLogin,
                                const QString & adminPassword
                            );
        bool            _openWorkspace(
                                tt3::ws::WorkspaceAddress workspaceAddress,
                                tt3::ws::OpenMode openMode
                            );
        bool            _reconcileCurrntCredentials(const tt3::ws::Workspace & workspace);
        void            _destroyWorkspace(tt3::ws::WorkspaceAddress workspaceAddress);

        void            _generateReport(
                                tt3::report::IReportType * reportType
                            );

        QMenu *         _createContextMenu();
        QAction *       _createActionMinimize(QObject * parent);
        QAction *       _createActionRestore(QObject * parent);
        QAction *       _createActionStopCurrentActivity(QObject * parent);
        QAction *       _createActionRefresh(QObject * parent);

        QAction *       _createActionNewWorkspace(QObject * parent);
        QAction *       _createActionOpenWorkspace(QObject * parent);
        QAction *       _createActionCloseWorkspace(QObject * parent);
        QAction *       _createActionDestroyWorkspace(QObject * parent);
        QAction *       _createActionRecentWorkspaces(QObject * parent);
        QAction *       _createActionOpenRecentWorkspace(
                                const tt3::ws::WorkspaceAddress & workspaceAddress,
                                QObject * parent
                            );
        QAction *       _createActionRestart(QObject * parent);
        QAction *       _createActionExit(QObject * parent);
        QAction *       _createActionManageUsers(QObject * parent);
        QAction *       _createActionManageActivityTypes(QObject * parent);
        QAction *       _createActionManagePublicActivities(QObject * parent);
        QAction *       _createActionManagePublicTasks(QObject * parent);
        QAction *       _createActionManagePrivateActivities(QObject * parent);
        QAction *       _createActionManagePrivateTasks(QObject * parent);
        QAction *       _createActionManageProjects(QObject * parent);
        QAction *       _createActionManageWorkStreams(QObject * parent);
        QAction *       _createActionManageBeneficiaries(QObject * parent);
        QAction *       _createActionManageMyDay(QObject * parent);
        QAction *       _createActionManageQuickPicks(QObject * parent);
        QAction *       _createActionQuickReports(QObject * parent);
        QAction *       _createActionInvokeTool(
                                tt3::util::ITool * tool,
                                QObject * parent
                            );
        QAction *       _createActionInvokeReport(
                                tt3::report::IReportType * reportType,
                                QObject * parent
                            );

        QAction *       _createActionLoginAsDifferentUser(QObject * parent);
        QAction *       _createActionPreferences(QObject * parent);
        QAction *       _createActionHelpContent(QObject * parent);
        QAction *       _createActionHelpIndex(QObject * parent);
        QAction *       _createActionHelpSearch(QObject * parent);
        QAction *       _createActionAbout(QObject * parent);

        //////////
        //  Controls
    private:
        Ui::MainFrame *const    _ui;
        QTimer          _trackPositionTimer;
        QTimer          _savePositionTimer;
        QTimer          _refreshTimer;
        QSystemTrayIcon*_trayIcon = nullptr;
        std::unique_ptr<QMenu>  _contextMenu;

        //  Drawing resources
        tt3::gui::LabelDecorations  _labelDecorations;

        //////////
        //  Signal handlers
    private slots:
        void            _trackPositionTimerTimeout();
        void            _savePositionTimerTimeout();
        void            _workspaceClosed(tt3::ws::WorkspaceClosedNotification);
        void            _currentWorkspaceChanged(tt3::ws::Workspace, tt3::ws::Workspace);
        void            _currentCredentialsChanged(tt3::ws::Credentials, tt3::ws::Credentials);
        void            _currentActivityChanged(tt3::ws::Activity, tt3::ws::Activity);
        void            _currentThemeChanged(tt3::gui::ITheme*, tt3::gui::ITheme*);
        void            _currentLocaleChanged(QLocale, QLocale);
        void            _refreshTimerTimeout();
        void            _onTrayIconActivated(QSystemTrayIcon::ActivationReason reason);

        void            _onActionMinimize();
        void            _onActionRestore();
        void            _onActionStopCurrentActivity();
        void            _onActionRefresh();

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
        void            _onActionManageQuickPicks();
        void            _onActionQuickReports();
        void            _onActionLoginAsDifferentUser();
        void            _onActionPreferences();
        void            _onActionHelpContent();
        void            _onActionHelpIndex();
        void            _onActionHelpSearch();
        void            _onActionAbout();
    };
}

//  End of tt3-skin-slim/MainFrame.hpp

