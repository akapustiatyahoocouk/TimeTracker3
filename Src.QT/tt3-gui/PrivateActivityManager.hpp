//
//  tt3-gui/PrivateActivityManager.hpp - The Public Activity Manager widget
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
    class TT3_GUI_PUBLIC ManageQuickPicksListDialog;

    namespace Ui { class PrivateActivityManager; }

    /// \class PrivateActivityManager tt3-gui/API.hpp
    /// \brief The Private Activity Manager widget
    class TT3_GUI_PUBLIC PrivateActivityManager final
        :   public QWidget
    {
        Q_OBJECT
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(PrivateActivityManager)

        friend class TT3_GUI_PUBLIC ManageQuickPicksListDialog;

        //////////
        //  Construction/destruction
    public:
        /// \brief
        ///     Constructs the widget.
        /// \param parent
        ///     The parent for this widget; nullptr == none.
        explicit PrivateActivityManager(
                QWidget * parent
            );

        /// \brief
        ///     The class destructor.
        virtual ~PrivateActivityManager();

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
        //  on by e.g. "manage quick picks" dialog.
        struct _WorkspaceModelImpl;
        struct _UserModelImpl;
        struct _PrivateActivityModelImpl;

        using _WorkspaceModel = std::shared_ptr<_WorkspaceModelImpl>;
        using _UserModel = std::shared_ptr<_UserModelImpl>;
        using _PrivateActivityModel = std::shared_ptr<_PrivateActivityModelImpl>;

        using _UserModels = QList<_UserModel>;
        using _PrivateActivityModels = QList<_PrivateActivityModel>;

        struct _WorkspaceModelImpl
        {
            //  Aggregations
            _UserModels userModels;     //  ordered by text
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
            _PrivateActivityModels privateActivityModels;     //  ordered by text
        };

        struct _PrivateActivityModelImpl
        {
            _PrivateActivityModelImpl(tt3::ws::PrivateActivity pa)
                :   privateActivity(pa) {}
            //  Properties
            tt3::ws::PrivateActivity privateActivity;//  represented by this model
            QString     text;           //  for PrivateActivity tree items
            QIcon       icon;           //  for PrivateActivity tree items
            QFont       font;           //  for PrivateActivity tree items
            QBrush      brush;          //  for PrivateActivity tree items' text
            QString     tooltip;        //  for PrivateActivity tree items' text
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
        static auto     _createPrivateActivityModel(
                                tt3::ws::PrivateActivity privateActivity,
                                const tt3::ws::Credentials & credentials,
                                const TreeWidgetDecorations & decorations
                            ) -> _PrivateActivityModel;
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
        static void     _refreshWorkspaceTree(
                                QTreeWidget * privateActivitiesTreeWidget,
                                _WorkspaceModel workspaceModel
                            );
        static void     _refreshUserItem(
                                QTreeWidgetItem * userItem,
                                _UserModel userModel
                            );
        static void     _refreshPrivateActivityItem(
                                QTreeWidgetItem * privateActivityItem,
                                _PrivateActivityModel privateActivityModel
                            );

        //  Helpers
        tt3::ws::User   _selectedUser();
        void            _setSelectedUser(
                                tt3::ws::User user
                            );
        auto            _selectedPrivateActivity(
                            ) -> tt3::ws::PrivateActivity;
        void            _setSelectedPrivateActivity(
                                tt3::ws::PrivateActivity publicActivity
                            );
        void            _startListeningToWorkspaceChanges();
        void            _stopListeningToWorkspaceChanges();
        void            _clearAndDisableAllControls();
        void            _applyCurrentLocale();


        //////////
        //  Controls
    private:
        Ui::PrivateActivityManager *const   _ui;
        std::unique_ptr<QMenu>  _privateActivitiesTreeContextMenu;
        QTimer                  _refreshTimer;

        //  Drawing resources
        TreeWidgetDecorations   _decorations;

        //////////
        //  Signal handlers
    private slots:
        void            _currentThemeChanged(ITheme *, ITheme *);
        void            _currentLocaleChanged(QLocale, QLocale);
        void            _currentActivityChanged(tt3::ws::Activity, tt3::ws::Activity);
        void            _privateActivitiesTreeWidgetCurrentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*);
        void            _privateActivitiesTreeWidgetCustomContextMenuRequested(QPoint);
        void            _createPrivateActivityPushButtonClicked();
        void            _modifyPrivateActivityPushButtonClicked();
        void            _destroyPrivateActivityPushButtonClicked();
        void            _startPrivateActivityPushButtonClicked();
        void            _stopPrivateActivityPushButtonClicked();
        void            _filterLineEditTextChanged(QString);
        void            _workspaceClosed(tt3::ws::WorkspaceClosedNotification notification);
        void            _objectCreated(tt3::ws::ObjectCreatedNotification notification);
        void            _objectDestroyed(tt3::ws::ObjectDestroyedNotification notification);
        void            _objectModified(tt3::ws::ObjectModifiedNotification notification);
        void            _refreshRequested();
        void            _refreshTimerTimeout();
    };
}

//  End of tt3-gui/PrivateActivityManager.hpp
