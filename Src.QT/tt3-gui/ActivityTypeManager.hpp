//
//  tt3-gui/ActivityTypeManager.hpp - The Activity Type Manager widget
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
    namespace Ui { class ActivityTypeManager; }

    /// \class ActivityTypeManager tt3-gui/API.hpp
    /// \brief The Activity Type Manager widget.
    class TT3_GUI_PUBLIC ActivityTypeManager final
        :   public QWidget
    {
        Q_OBJECT
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(ActivityTypeManager)

        //////////
        //  Construction/destruction
    public:
        /// \brief
        ///     Constructs the widget.
        /// \param parent
        ///     The parent for the widget; nullptr == none.
        explicit ActivityTypeManager(
                QWidget * parent
            );

        /// \brief
        ///     The class destructor.
        virtual ~ActivityTypeManager();

        //////////
        //  Operaions
    public:
        /// \brief
        ///     Returns the workspace currently viewed in this widget.
        /// \details
        ///     Starts off with the "current" workspace
        ///     when the widget is constructed.
        /// \return
        ///     The workspace currently viewed in this widget; nullptr == none.
        auto        workspace(
                        ) const -> tt3::ws::Workspace;

        /// \brief
        ///     Sets the workspace currently viewed in this widget.
        /// \param workspace
        ///     The workspace to be viewed in this widget; nullptr == none.
        void        setWorkspace(
                            tt3::ws::Workspace workspace
                        );

        /// \brief
        ///     Returns the credentials used by this widget to
        ///     display the workspace.
        /// \details
        ///     Starts off with the "current" credentials
        ///     when the widget is constructed.
        /// \return
        ///     The credentials used by this widget to display
        ///     the workspace.
        auto        credentials(
                        ) const -> tt3::ws::Credentials;

        /// \brief
        ///     Sets the credentials used by this widget to
        ///     display the workspace.
        /// \param credentials
        ///     The credentials to be used by this widget to
        ///     display the workspace.
        void        setCredentials(
                            const tt3::ws::Credentials & credentials
                        );

        /// \brief
        ///     Refreshes the content of this widget.
        void        refresh();

        /// \brief
        ///     Requests that refresh() br called as soon as
        ///     practicable on this widget's event loop thread.
        void        requestRefresh();

        //////////
        //  Signals
    signals:
        /// \brief
        ///     Emitted when this widget needs to refresh().
        void        refreshRequested();

        //////////
        //  Implementation
    private:
        tt3::ws::Workspace      _workspace;
        tt3::ws::Credentials    _credentials;

        //  View model
        struct _WorkspaceModelImpl;
        struct _ActivityTypeModelImpl;

        using _WorkspaceModel = std::shared_ptr<_WorkspaceModelImpl>;
        using _ActivityTypeModel = std::shared_ptr<_ActivityTypeModelImpl>;

        using _ActivityTypeModels = QList<_ActivityTypeModel>;

        struct _WorkspaceModelImpl
        {
            //  Aggregations
            _ActivityTypeModels activityTypeModels;     //  ordered by text
        };

        struct _ActivityTypeModelImpl
        {
            _ActivityTypeModelImpl(tt3::ws::ActivityType at) : activityType(at) {}
            //  Properties
            tt3::ws::ActivityType   activityType;//  represented by this model
            QString             text;           //  for ActivityType tree items
            QIcon               icon;           //  for ActivityType tree items
            QFont               font;           //  for ActivityType tree items
            QBrush              brush;          //  for ActivityType tree items' text
            QString             tooltip;        //  for ActivityType tree items' text
        };

        auto        _createWorkspaceModel(
                        ) -> _WorkspaceModel;
        auto        _createActivityTypeModel(
                            tt3::ws::ActivityType activityType
                        ) -> _ActivityTypeModel;
        void        _filterItems(_WorkspaceModel workspaceModel);
        void        _refreshWorkspaceTree(
                            _WorkspaceModel workspaceModel
                        );
        void        _refreshActivityTypeItem(
                            QTreeWidgetItem * activityTypeItem,
                            _ActivityTypeModel activityTypeModel
                        );

        //  Helpers
        auto        _selectedActivityType(
                        ) -> tt3::ws::ActivityType;
        void        _setSelectedActivityType(tt3::ws::ActivityType activityType);
        void        _startListeningToWorkspaceChanges();
        void        _stopListeningToWorkspaceChanges();
        void        _clearAndDisableAllControls();
        void        _applyCurrentLocale();

        //////////
        //  Controls
    private:
        Ui::ActivityTypeManager *const  _ui;
        std::unique_ptr<QMenu>  _activityTypesTreeContextMenu;

        //  Drawing resources
        TreeWidgetDecorations   _decorations;

        //////////
        //  Signal handlers
    private slots:
        void        _currentThemeChanged(ITheme *, ITheme *);
        void        _currentLocaleChanged(QLocale, QLocale);
        void        _activityTypesTreeWidgetCurrentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*);
        void        _activityTypesTreeWidgetCustomContextMenuRequested(QPoint);
        void        _createActivityTypePushButtonClicked();
        void        _modifyActivityTypePushButtonClicked();
        void        _destroyActivityTypePushButtonClicked();
        void        _filterLineEditTextChanged(QString);
        void        _workspaceClosed(tt3::ws::WorkspaceClosedNotification notification);
        void        _objectCreated(tt3::ws::ObjectCreatedNotification notification);
        void        _objectDestroyed(tt3::ws::ObjectDestroyedNotification notification);
        void        _objectModified(tt3::ws::ObjectModifiedNotification notification);
        void        _refreshRequested();
    };
}

//  End of tt3-gui/ActivityTypeManager.hpp

