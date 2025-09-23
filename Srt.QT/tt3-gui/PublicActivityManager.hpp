//
//  tt3-gui/PublicActivityManager.hpp - The Public Activity Manager widget
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
    namespace Ui { class PublicActivityManager; }

    /// \class PublicActivityManager tt3-gui/API.hpp
    /// \brief The Public Activity Manager widget
    class TT3_GUI_PUBLIC PublicActivityManager final
        :   public QWidget
    {
        Q_OBJECT
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(PublicActivityManager)

        //////////
        //  Construction/destruction
    public:
        /// \brief
        ///     Constructs the widget.
        /// \param parent
        ///     The parent for this widget; nullptr == none.
        explicit PublicActivityManager(
                QWidget * parent
            );

        /// \brief
        ///     The class destructor.
        virtual ~PublicActivityManager();

        //////////
        //  Operaions
    public:
        /// \brief
        ///     Returns the workspace currently viewed in this widget.
        /// \details
        ///     Starts off with the "current" workspace when the
        ///     widget is constructed.
        /// \return
        ///     The workspace currently viewed in this widget;
        ///     nullptr == none.
        auto            workspace(
                            ) const -> tt3::ws::Workspace;

        /// \brief
        ///     Sets the workspace currently viewed in this widget.
        /// \param workspace
        ///     The new workspace to be viewed in this widget;
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
        auto            credentials(
                            ) const -> tt3::ws::Credentials;

        /// \brief
        ///     Returns the credentials used by this widget to
        ///     display the workspace.
        /// \details
        ///     Starts off with the "current" credentials when
        ///     the widget is constructed.
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
        void            refreshRequested();

        //////////
        //  Implementation
    private:
        tt3::ws::Workspace      _workspace;
        tt3::ws::Credentials    _credentials;

        //  View model
        struct _WorkspaceModelImpl;
        struct _PublicActivityModelImpl;

        using _WorkspaceModel = std::shared_ptr<_WorkspaceModelImpl>;
        using _PublicActivityModel = std::shared_ptr<_PublicActivityModelImpl>;

        using _PublicActivityModels = QList<_PublicActivityModel>;

        struct _WorkspaceModelImpl
        {
            //  Aggregations
            _PublicActivityModels publicActivityModels;     //  ordered by text
        };

        struct _PublicActivityModelImpl
        {
            _PublicActivityModelImpl(tt3::ws::PublicActivity pa)
                :   publicActivity(pa) {}
            //  Properties
            tt3::ws::PublicActivity publicActivity;//  represented by this model
            QString     text;           //  for PublicActivity tree items
            QIcon       icon;           //  for PublicActivity tree items
            QFont       font;           //  for PublicActivity tree items
            QBrush      brush;          //  for PublicActivity tree items' text
            QString     tooltip;        //  for PublicActivity tree items' text
        };

        auto            _createWorkspaceModel(
                            ) -> _WorkspaceModel;
        auto            _createPublicActivityModel(
                                tt3::ws::PublicActivity publicActivity
                            ) -> _PublicActivityModel;
        void            _filterItems(_WorkspaceModel workspaceModel);

        //  Helpers
        void            _refreshPublicActivityItems(_WorkspaceModel workspaceModel);
        auto            _selectedPublicActivity(
                            ) -> tt3::ws::PublicActivity;
        void            _setSelectedPublicActivity(tt3::ws::PublicActivity PublicActivity);
        void            _startListeningToWorkspaceChanges();
        void            _stopListeningToWorkspaceChanges();
        void            _clearAndDisableAllControls();


        //////////
        //  Controls
    private:
        Ui::PublicActivityManager *const    _ui;
        std::unique_ptr<QMenu>  _publicActivitiesTreeContextMenu;
        QTimer                  _refreshTimer;

        //  Drawing resources
        TreeWidgetDecorations   _decorations;

        //////////
        //  Signal handlers
    private slots:
        void            _currentThemeChanged(ITheme *, ITheme *);
        void            _currentActivityChanged(tt3::ws::Activity, tt3::ws::Activity);
        void            _publicActivitiesTreeWidgetCurrentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*);
        void            _publicActivitiesTreeWidgetCustomContextMenuRequested(QPoint);
        void            _createPublicActivityPushButtonClicked();
        void            _modifyPublicActivityPushButtonClicked();
        void            _destroyPublicActivityPushButtonClicked();
        void            _startPublicActivityPushButtonClicked();
        void            _stopPublicActivityPushButtonClicked();
        void            _filterLineEditTextChanged(QString);
        void            _workspaceClosed(tt3::ws::WorkspaceClosedNotification notification);
        void            _objectCreated(tt3::ws::ObjectCreatedNotification notification);
        void            _objectDestroyed(tt3::ws::ObjectDestroyedNotification notification);
        void            _objectModified(tt3::ws::ObjectModifiedNotification notification);
        void            _refreshRequested();
        void            _refreshTimerTimeout();
    };
}

//  End of tt3-gui/PublicActivityManager.hpp

