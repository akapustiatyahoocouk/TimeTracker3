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
    //  The Activity Type Manager widget
    namespace Ui { class ActivityTypeManager; }

    class TT3_GUI_PUBLIC ActivityTypeManager final
        :   public QWidget
    {
        Q_OBJECT
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(ActivityTypeManager)

        //////////
        //  Construction/destruction
    public:
        explicit ActivityTypeManager(QWidget * parent);
        virtual ~ActivityTypeManager();

        //////////
        //  Operaions
    public:
        //  Returns/sets the workspace currently viewed
        //  in this widget; nullptr == none.
        //  Starts off with the "current" workspace
        //  when the widget is constructed.
        tt3::ws::Workspace  workspace() const;
        void                setWorkspace(tt3::ws::Workspace workspace);

        //  Returns/sets the credentials used by this
        //  widget to display the workspace.
        //  Starts off with the "current" credentials
        //  when the widget is constructed.
        tt3::ws::Credentials credentials() const;
        void                setCredentials(const tt3::ws::Credentials & credentials);

        //  Refreshes the content of this widget
        void                refresh();

        //  Requests that refresh() br called as soon as
        //  practicable on this widget's event loop thread
        void                requestRefresh();

        //////////
        //  Signals
    signals:
        //  Emitted when this widget needs to refresh()
        void                refreshRequested();

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

        _WorkspaceModel     _createWorkspaceModel();
        _ActivityTypeModel  _createActivityTypeModel(tt3::ws::ActivityType activityType);
        void                _filterItems(_WorkspaceModel workspaceModel);

        //  Helpers
        void                _refreshActivityTypeItems(_WorkspaceModel workspaceModel);
        tt3::ws::ActivityType   _selectedActivityType();
        void                _setSelectedActivityType(tt3::ws::ActivityType activityType);
        void                _startListeningToWorkspaceChanges();
        void                _stopListeningToWorkspaceChanges();

        //////////
        //  Controls
    private:
        Ui::ActivityTypeManager *   _ui;
        std::unique_ptr<QMenu>  _activityTypesTreeContextMenu;

        //  Drawing resources
        TreeWidgetDecorations   _decorations;

        //////////
        //  Signal handlers
    private slots:
        void                _currentThemeChanged(ITheme *, ITheme *);
        void                _activityTypesTreeWidgetCurrentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*);
        void                _activityTypesTreeWidgetCustomContextMenuRequested(QPoint);
        void                _createActivityTypePushButtonClicked();
        void                _modifyActivityTypePushButtonClicked();
        void                _destroyActivityTypePushButtonClicked();
        void                _filterLineEditTextChanged(QString);
        void                _workspaceClosed(tt3::ws::WorkspaceClosedNotification notification);
        void                _objectCreated(tt3::ws::ObjectCreatedNotification notification);
        void                _objectDestroyed(tt3::ws::ObjectDestroyedNotification notification);
        void                _objectModified(tt3::ws::ObjectModifiedNotification notification);
        void                _refreshRequested();
    };
}

//  End of tt3-gui/ActivityTypeManager.hpp

