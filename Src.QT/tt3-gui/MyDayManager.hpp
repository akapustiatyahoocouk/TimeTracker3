//
//  tt3-gui/MyDayManager.hpp - The "My day" manager widget
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
#include "tt3-gui/API.hpp"

namespace tt3::gui
{
    namespace Ui { class MyDayManager; }

    /// \class UserManager tt3-gui/API.hpp
    /// \brief The User Manager widget.
    class TT3_GUI_PUBLIC MyDayManager final
        :   public QWidget
    {
        Q_OBJECT
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(MyDayManager)

        //////////
        //  Construction/destruction
    public:
        /// \brief
        ///     Constructs the dialog
        /// \param parent
        ///     The parent for the widget; nullptr == none.
        explicit MyDayManager(QWidget * parent);
        virtual ~MyDayManager();

        //////////
        //  Operaions
    public:
        /// \brief
        ///     Returns the workspace currently viewed in
        ///     this widget.
        /// \details
        ///     Starts off with the "current" workspace when the
        ///     widget is constructed.
        /// \return
        ///     The workspace currently viewed in this
        ///     widget; nullptr == none.
        tt3::ws::Workspace  workspace() const;

        /// \brief
        ///     Sets the workspace currently viewed in
        ///     this widget.
        /// \param workspace
        ///     The workspace to be viewed in this widget;
        ///     nullptr == none.
        void            setWorkspace(
                                tt3::ws::Workspace workspace
                            );

        /// \brief
        ///     Returns the credentials used by this widget to
        ///     display the workspace.
        /// \details
        ///     Starts off with the "current" credentials when
        ///     the widget is constructed.
        /// \return
        ///     The credentials used by this widget to display
        ///     the workspace.
        tt3::ws::Credentials credentials() const;

        /// \brief
        ///     Sets the credentials used by this widget to
        ///     display the workspace.
        /// \param credentials
        ///     The credentials to be used by this widget to
        ///     display the workspace.
        void            setCredentials(
                                const tt3::ws::Credentials & credentials
                            );

        /// \brief
        ///     Refreshes the content of this widget.
        void            refresh();

        /// \brief
        ///     Requests that refresh() br called as soon as
        ///     practicable on this widget's event loop thread.
        void            requestRefresh();

        //////////
        //  Signals
    signals:
        /// \brief
        ///     Emitted when this widget needs to refresh().
        /// \details
        ///     The widget itself always subscribes to this
        ///     signal to handle internal refresh requests; however,
        ///     the signal allows external listeners to react to
        ///     the widget's refresh requests as well.
        void            refreshRequested();

        //////////
        //  Implementation
    private:
        tt3::ws::Workspace      _workspace;
        tt3::ws::Credentials    _credentials;

        //  Helpers
        void            _startListeningToWorkspaceChanges();
        void            _stopListeningToWorkspaceChanges();
        void            _clearAndDisableAllControls();
        void            _recreateQuickPickButtons();
        void            _recreateDynamicControls();

        //////////
        //  Controls
    private:
        Ui::MyDayManager *const _ui;
        QTimer          _refreshTimer;
        //  Dynamic controls
        QList<tt3::ws::Activity>    _quickPicksList;
        QList<QPushButton*> _quickPicksButtons; //  parallel to _quickPicksButtons

        //  Drawing resources
        PushButtonDecorations   _pushButtonDecorations;

        //////////
        //  Signal handlers
    private slots:
        void            _currentThemeChanged(ITheme *, ITheme *);
        void            _workspaceClosed(tt3::ws::WorkspaceClosedNotification notification);
        void            _objectCreated(tt3::ws::ObjectCreatedNotification notification);
        void            _objectDestroyed(tt3::ws::ObjectDestroyedNotification notification);
        void            _objectModified(tt3::ws::ObjectModifiedNotification notification);
        void            _refreshRequested();
        void            _quickPicksPushButtonClicked();
        void            _logEventPushButtonClicked();
        void            _quickPickPushButtonClicked();
        void            _refreshTimerTimeout();
        void            _currentActivityChanged(tt3::ws::Activity, tt3::ws::Activity);
    };
}

//  End of tt3-gui/MyDayManager.hpp
