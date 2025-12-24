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

        //////////
        //  Constants
    public:
        /// \brief
        ///     The minimum size of the main frame.
        inline static const QSize MinimumSize {96, 64};

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

        virtual void    mousePressEvent(QMouseEvent * event) override;
        virtual void    mouseReleaseEvent(QMouseEvent * event) override;
        virtual void    mouseMoveEvent(QMouseEvent * event) override;

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

        //////////
        //  Implementation
    private:
        bool            _trackPosition = false;

        bool            _dragging = false;
        QPoint          _dragAnchor;

        //  Helpers
        void            _loadPosition();
        void            _savePosition();
        void            _ensureWithinScreenBounds();
        void            _setFrameGeometry(const QRect & bounds);

        //////////
        //  Controls
    private:
        Ui::MainFrame *const    _ui;
        QTimer          _trackPositionTimer;
        QTimer          _savePositionTimer;
        QTimer          _refreshTimer;

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

        void            _onActionExit();
    };
}

//  End of tt3-skin-slim/MainFrame.hpp

