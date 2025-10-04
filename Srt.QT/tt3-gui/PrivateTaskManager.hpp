//
//  tt3-gui/PrivateTaskManager.hpp - The Private Task Manager widget
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
    namespace Ui { class PrivateTaskManager; }

    /// \class PrivateTaskManager tt3-gui/API.hpp
    /// \brief The Private Task Manager widget
    class TT3_GUI_PUBLIC PrivateTaskManager final
        :   public QWidget
    {
        Q_OBJECT
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(PrivateTaskManager)

        friend class SelectPrivateTaskParentDialog;

        //////////
        //  Construction/destruction
    public:
        /// \brief
        ///     Constructs the widget.
        /// \param parent
        ///     The parent for this widget; nullptr == none.
        explicit PrivateTaskManager(
                QWidget * parent
            );

        /// \brief
        ///     The class destructor.
        virtual ~PrivateTaskManager();

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
        struct _WorkspaceModelImpl;
        struct _UserModelImpl;
        struct _PrivateTaskModelImpl;

        using _WorkspaceModel = std::shared_ptr<_WorkspaceModelImpl>;
        using _UserModel = std::shared_ptr<_UserModelImpl>;
        using _PrivateTaskModel = std::shared_ptr<_PrivateTaskModelImpl>;

        using _UserModels = QList<_UserModel>;
        using _PrivateTaskModels = QList<_PrivateTaskModel>;

        struct _WorkspaceModelImpl
        {
            //  Aggregations
            _UserModels     userModels; //  ordered by text
        };

        struct _UserModelImpl
        {
            _UserModelImpl(tt3::ws::User u)
                :   user(u) {}
            //  Properties
            tt3::ws::User   user;       //  represented by this model
            QString     text;           //  for User tree items
            QIcon       icon;           //  for User tree items
            QFont       font;           //  for User tree items
            QBrush      brush;          //  for User tree items' text
            QString     tooltip;        //  for User tree items' text
            //  Aggregations
            _PrivateTaskModels  privateTaskModels; //  ordered by text
        };

        struct _PrivateTaskModelImpl
        {
            _PrivateTaskModelImpl(tt3::ws::PrivateTask pt)
                :   privateTask(pt) {}
            //  Properties
            tt3::ws::PrivateTask privateTask;   //  represented by this model
            QString     text;           //  for PrivateTask tree items
            QIcon       icon;           //  for PrivateTask tree items
            QFont       font;           //  for PrivateTask tree items
            QBrush      brush;          //  for PrivateTask tree items' text
            QString     tooltip;        //  for PrivateTask tree items' text
            //  Aggregations
            _PrivateTaskModels  childModels; // ordered by text
        };

        static auto     _createWorkspaceModel(
                                tt3::ws::Workspace workspace,
                                const tt3::ws::Credentials & credentials,
                                const TreeWidgetDecorations & decorations
                            ) -> _WorkspaceModel;
        static auto     _createUserModel(
                                tt3::ws::User user,
                                const tt3::ws::Credentials & credentials,
                                const TreeWidgetDecorations & decorations
                            ) -> _UserModel;
        static auto     _createPrivateTaskModel(
                                tt3::ws::PrivateTask privateTask,
                                const tt3::ws::Credentials & credentials,
                                const TreeWidgetDecorations & decorations
                            ) -> _PrivateTaskModel;
        static void     _removeCompletedItems(
                                _WorkspaceModel workspaceModel,
                                const tt3::ws::Credentials & credentials
                            );
        static void     _removeCompletedItems(
                                _UserModel userModel,
                                const tt3::ws::Credentials & credentials
                            );
        static void     _removeCompletedItems(
                                _PrivateTaskModel privateTaskModel,
                                const tt3::ws::Credentials & credentials
                            );
        static void     _filterItems(
                                _WorkspaceModel workspaceModel,
                                const QString & filter,
                                const TreeWidgetDecorations & decorations
                            );
        static void     _filterItems(
                                _UserModel userModel,
                                const QString & filter,
                                const TreeWidgetDecorations & decorations
                            );
        static void     _filterItems(
                                _PrivateTaskModel privateTaskModel,
                                const QString & filter,
                                const TreeWidgetDecorations & decorations
                            );
        static void     _refreshWorkspaceTree(
                                QTreeWidget * privateTasksTreeWidget,
                                _WorkspaceModel workspaceModel
                            );
        static void     _refreshUserItem(
                                QTreeWidgetItem * userItem,
                                _UserModel userModel
                            );
        static void     _refreshPrivateTaskItem(
                                QTreeWidgetItem * privateTaskItem,
                                _PrivateTaskModel privateTaskModel
                            );

        //  Helpers
        auto            _selectedUser(
                            ) -> tt3::ws::User;
        void            _setSelectedUser(
                                tt3::ws::User user
                            );
        auto            _selectedPrivateTask(
                            ) -> tt3::ws::PrivateTask;
        bool            _setSelectedPrivateTask(
                                tt3::ws::PrivateTask publicTask
                            );
        bool            _setSelectedPrivateTask(
                                QTreeWidgetItem * parentItem,
                                tt3::ws::PrivateTask publicTask
                            );
        void            _startListeningToWorkspaceChanges();
        void            _stopListeningToWorkspaceChanges();
        void            _clearAndDisableAllControls();

        //////////
        //  Controls
    private:
        Ui::PrivateTaskManager *const   _ui;
        std::unique_ptr<QMenu>  _privateTasksTreeContextMenu;
        QTimer                  _refreshTimer;

        //  Drawing resources
        TreeWidgetDecorations   _decorations;

        //////////
        //  Signal handlers
    private slots:
        void            _currentThemeChanged(ITheme *, ITheme *);
        void            _currentActivityChanged(tt3::ws::Activity, tt3::ws::Activity);
        void            _privateTasksTreeWidgetCurrentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*);
        void            _privateTasksTreeWidgetCustomContextMenuRequested(QPoint);
        void            _createPrivateTaskPushButtonClicked();
        void            _modifyPrivateTaskPushButtonClicked();
        void            _destroyPrivateTaskPushButtonClicked();
        void            _startPrivateTaskPushButtonClicked();
        void            _stopPrivateTaskPushButtonClicked();
        void            _completePrivateTaskPushButtonClicked();
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

//  End of tt3-gui/PrivateTaskManager.hpp
