//
//  tt3-gui/UserManager.hpp - The User Manager widget
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
    namespace Ui { class UserManager; }

    /// \class UserManager tt3-gui/API.hpp
    /// \brief The User Manager widget.
    class TT3_GUI_PUBLIC UserManager
        :   public QWidget
    {
        Q_OBJECT
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(UserManager)

        //////////
        //  Construction/destruction
    public:
        /// \brief
        ///     Constructs the dialog
        /// \param parent
        ///     The parent for the widget; nullptr == none.
        explicit UserManager(
                QWidget * parent
            );

        /// \brief
        ///     The class destructor.
        virtual ~UserManager();

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
        void                setWorkspace(
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
        void                setCredentials(
                                    const tt3::ws::Credentials & credentials
                                );

        /// \brief
        ///     Refreshes the content of this widget.
        void                refresh();

        /// \brief
        ///     Requests that refresh() br called as soon as
        ///     practicable on this widget's event loop thread.
        void                requestRefresh();

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
        void                refreshRequested();

        //////////
        //  Implementation
    private:
        tt3::ws::Workspace      _workspace;
        tt3::ws::Credentials    _credentials;

        //  View model
        struct _WorkspaceModelImpl;
        struct _UserModelImpl;
        struct _AccountModelImpl;

        using _WorkspaceModel = std::shared_ptr<_WorkspaceModelImpl>;
        using _UserModel = std::shared_ptr<_UserModelImpl>;
        using _AccountModel = std::shared_ptr<_AccountModelImpl>;

        using _UserModels = QList<_UserModel>;
        using _AccountModels = QList<_AccountModel>;

        struct _WorkspaceModelImpl
        {
            //  Aggregations
            _UserModels         userModels;     //  ordered by text
        };

        struct _UserModelImpl
        {
            _UserModelImpl(tt3::ws::User u) : user(u) {}
            //  Properties
            tt3::ws::User       user;           //  represented by this model
            QString             text;           //  for User tree items
            QIcon               icon;           //  for User tree items
            QFont               font;           //  for User tree items
            QBrush              brush;          //  for User tree items' text
            //  Aggregations
            _AccountModels      accountModels;  //  ordered by text
        };

        struct _AccountModelImpl
        {
            _AccountModelImpl(tt3::ws::Account a) : account(a) {}
            //  Properties
            tt3::ws::Account    account;        //  represented by this model
            QString             text;           //  for Account tree items
            QIcon               icon;           //  for Account tree items
            QFont               font;           //  for Account tree items
            QBrush              brush;          //  for User tree items' text
        };

        _WorkspaceModel     _createWorkspaceModel();
        _UserModel          _createUserModel(tt3::ws::User user);
        _AccountModel       _createAccountModel(tt3::ws::Account account);
        void                _removeDisabledItems(_WorkspaceModel workspaceModel);
        void                _removeDisabledItems(_UserModel userModel);
        void                _filterItems(_WorkspaceModel workspaceModel);
        void                _filterItems(_UserModel userModel);

        //  Helpers
        void                _refreshUserItems(_WorkspaceModel workspaceModel);
        void                _refreshAccountItems(QTreeWidgetItem * userItem, _UserModel userModel);
        tt3::ws::User       _selectedUser();
        void                _setSelectedUser(tt3::ws::User user);
        tt3::ws::Account    _selectedAccount();
        void                _setSelectedAccount(tt3::ws::Account account);
        void                _startListeningToWorkspaceChanges();
        void                _stopListeningToWorkspaceChanges();

        //////////
        //  Controls
    private:
        Ui::UserManager *   _ui;
        std::unique_ptr<QMenu>  _usersTreeContextMenu;

        //  Drawing resources
        TreeWidgetDecorations   _decorations;

        //////////
        //  Signal handlers
    private slots:
        void                _currentThemeChanged(ITheme *, ITheme *);
        void                _usersTreeWidgetCurrentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*);
        void                _usersTreeWidgetCustomContextMenuRequested(QPoint);
        void                _createUserPushButtonClicked();
        void                _modifyUserPushButtonClicked();
        void                _destroyUserPushButtonClicked();
        void                _createAccountPushButtonClicked();
        void                _modifyAccountPushButtonClicked();
        void                _destroyAccountPushButtonClicked();
        void                _showDisabledCheckBoxToggled(bool);
        void                _viewOptionSettingValueChanged();
        void                _filterLineEditTextChanged(QString);
        void                _workspaceClosed(tt3::ws::WorkspaceClosedNotification notification);
        void                _objectCreated(tt3::ws::ObjectCreatedNotification notification);
        void                _objectDestroyed(tt3::ws::ObjectDestroyedNotification notification);
        void                _objectModified(tt3::ws::ObjectModifiedNotification notification);
        void                _refreshRequested();
    };
}

//  End of tt3-gui/UserManager.hpp

