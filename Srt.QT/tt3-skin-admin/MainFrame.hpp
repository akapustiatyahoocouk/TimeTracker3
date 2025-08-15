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
    //  Main frame of the admin skin
    namespace Ui { class MainFrame; }

    class TT3_SKIN_ADMIN_PUBLIC MainFrame : public QMainWindow
    {
        Q_OBJECT
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(MainFrame)

        //////////
        //  Construction
    public:
        explicit MainFrame(QWidget * parent = nullptr);
        virtual ~MainFrame();

        //////////
        //  QWidget
    public:
        virtual void    moveEvent(QMoveEvent * event) override;
        virtual void    resizeEvent(QResizeEvent * event) override;
        virtual void	closeEvent(QCloseEvent * event) override;

        //////////
        //  Operations
    public:
        //  Refreshes this main frame and controls within
        void            refresh();

        //////////
        //  Implementation
    private:
        bool            _trackPosition = false;

        //  Helpers
        void            _loadPosition();
        void            _savePosition();

        void            _createWorkspace(const tt3::ws::WorkspaceAddress & workspaceAddress);
        void            _openWorkspace(const tt3::ws::WorkspaceAddress & workspaceAddress);

        //////////
        //  Controls
    private:
        Ui::MainFrame * _ui;
        QTimer          _savePositionTimer;

        //////////
        //  Signal handlers
    private slots:
        void            _savePositionTimerTimeout();
        void            _onActionNewWorkspace();
        void            _onActionOpenWorkspace();
        void            _onActionCloseWorkspace();
        void            _onActionExit();
        void            _onActionHelpContent();
        void            _onActionHelpIndex();
        void            _onActionHelpSearch();

        void            _onCurrentWorkspaceChanged();
        void            _onWorkspaceClosed(tt3::ws::WorkspaceClosedNotification);
    };
}

//  End of tt3-skin-admin/MainFrame.hpp
