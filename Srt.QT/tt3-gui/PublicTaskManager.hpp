//
//  tt3-gui/PublicTaskManager.hpp - The Public Task Manager widget
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
    class TT3_GUI_PUBLIC SelectPublicTaskParentDialog;

    namespace Ui { class PublicTaskManager; }

    /// \class PublicTaskManager tt3-gui/API.hpp
    /// \brief The Public Task Manager widget
    class TT3_GUI_PUBLIC PublicTaskManager final : public QWidget
    {
        Q_OBJECT
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(PublicTaskManager)

        friend class SelectPublicTaskParentDialog;

        //////////
        //  Construction/destruction
    public:
        /// \brief
        ///     Constructs the widget.
        /// \param parent
        ///     The parent for this widget; nullptr == none.
        explicit PublicTaskManager(
                QWidget * parentr
            );

        /// \brief
        ///     The class destructor.
        virtual ~PublicTaskManager();

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
        ///     Sets the credentials used by this widget to
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
        //  Model services are "static" because they are oiggybacked
        //  on by e.g. "select new parent for a public task" dialog.
        //  TODO similar "static" treatment in other XXXManager classes
        struct _WorkspaceModelImpl;
        struct _PublicTaskModelImpl;

        using _WorkspaceModel = std::shared_ptr<_WorkspaceModelImpl>;
        using _PublicTaskModel = std::shared_ptr<_PublicTaskModelImpl>;

        using _PublicTaskModels = QList<_PublicTaskModel>;

        struct _WorkspaceModelImpl
        {
            //  Aggregations
            _PublicTaskModels publicTaskModels; //  ordered by text
        };

        struct _PublicTaskModelImpl
        {
            _PublicTaskModelImpl(tt3::ws::PublicTask pt)
                :   publicTask(pt) {}
            //  Properties
            tt3::ws::PublicTask publicTask;//  represented by this model
            QString     text;           //  for PublicTask tree items
            QIcon       icon;           //  for PublicTask tree items
            QFont       font;           //  for PublicTask tree items
            QBrush      brush;          //  for PublicTask tree items' text
            QString     tooltip;        //  for PublicTask tree items' text
            //  Aggregations
            _PublicTaskModels childModels;  //  ordered by text
        };

        static auto     _createWorkspaceModel(
                                tt3::ws::Workspace workspace,
                                const tt3::ws::Credentials & credentials,
                                const TreeWidgetDecorations & decorations
                            ) -> _WorkspaceModel;
        static auto     _createPublicTaskModel(
                                tt3::ws::PublicTask publicTask,
                                const tt3::ws::Credentials & credentials,
                                const TreeWidgetDecorations & decorations
                            ) -> _PublicTaskModel;
        static void     _removeCompletedItems(
                                _WorkspaceModel workspaceModel,
                                const tt3::ws::Credentials & credentials
                            );
        static void     _removeCompletedItems(
                                _PublicTaskModel publicTaskModel,
                                const tt3::ws::Credentials & credentials
                            );
        static void     _filterItems(
                                _WorkspaceModel workspaceModel,
                                const QString & filter,
                                const TreeWidgetDecorations & decorations
                            );
        static void     _filterItems(
                                _PublicTaskModel publicTaskModel,
                                const QString & filter,
                                const TreeWidgetDecorations & decorations
                            );
        static void     _refreshWorkspaceItem(
                                QTreeWidget * publicTasksTreeWidget,
                                _WorkspaceModel workspaceModel
                            );
        static void     _refreshPublicTaskItem(
                                QTreeWidgetItem * publicTaskItem,
                                _PublicTaskModel publicTaskModel
                            );

        //  Helpers
        auto            _selectedPublicTask(
                            ) -> tt3::ws::PublicTask;
        bool            _setSelectedPublicTask(
                                tt3::ws::PublicTask publicTask
                            );
        bool            _setSelectedPublicTask(
                                QTreeWidgetItem * parentItem,
                                tt3::ws::PublicTask publicTask
                            );
        void            _startListeningToWorkspaceChanges();
        void            _stopListeningToWorkspaceChanges();
        void            _clearAndDisableAllControls();

        //////////
        //  Controls
    private:
        Ui::PublicTaskManager *const    _ui;
        std::unique_ptr<QMenu>  _publicTasksTreeContextMenu;
        QTimer                  _refreshTimer;

        //  Drawing resources
        TreeWidgetDecorations   _decorations;

        //////////
        //  Signal handlers
    private slots:
        void            _currentThemeChanged(ITheme *, ITheme *);
        void            _currentActivityChanged(tt3::ws::Activity, tt3::ws::Activity);
        void            _publicTasksTreeWidgetCurrentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*);
        void            _publicTasksTreeWidgetCustomContextMenuRequested(QPoint);
        void            _createPublicTaskPushButtonClicked();
        void            _modifyPublicTaskPushButtonClicked();
        void            _destroyPublicTaskPushButtonClicked();
        void            _startPublicTaskPushButtonClicked();
        void            _stopPublicTaskPushButtonClicked();
        void            _completePublicTaskPushButtonClicked();
        void            _showCompletedCheckBoxToggled(bool);
        void            _viewOptionSettingValueChanged();
        void            _filterLineEditTextChanged(QString);
        void            _workspaceClosed(tt3::ws::WorkspaceClosedNotification notification);
        void            _objectCreated(tt3::ws::ObjectCreatedNotification notification);
        void            _objectDestroyed(tt3::ws::ObjectDestroyedNotification notification);
        void            _objectModified(tt3::ws::ObjectModifiedNotification notification);
        void            _refreshRequested();
        void            _refreshTimerTimeout();
    };
}

//  Macro needed for MOC-generated .cpp files
#define TT3_GUI_PUBLIC_TASK_MANAGER_DEFINED

//  End of tt3-gui/PublicTaskManager.hpp
