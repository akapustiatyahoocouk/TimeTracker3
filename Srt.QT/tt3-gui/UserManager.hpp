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
    //  The User Manager widget
    namespace Ui { class UserManager; }

    class TT3_GUI_PUBLIC UserManager : public QWidget
    {
        Q_OBJECT
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(UserManager)

        //////////
        //  Construction/destruction
    public:
        explicit UserManager(QWidget * parent);
        virtual ~UserManager();

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

        //////////
        //  Implementation
    private:
        tt3::ws::Workspace  _workspace;
        tt3::ws::Credentials    _credentials;

        //  Helpers
        void                _refreshUserItems();
        void                _refreshAccountItems(QTreeWidgetItem * userItem);
        QString             _userItemText(tt3::ws::User user) throws(tt3::ws::WorkspaceException);
        QString             _accountItemText(tt3::ws::Account account) throws(tt3::ws::WorkspaceException);
        tt3::ws::User       _selectedUser();
        void                _setSelectedUser(tt3::ws::User user);
        tt3::ws::Account    _selectedAccount();
        void                _setSelectedAccount(tt3::ws::Account account);

        //////////
        //  Controls
    private:
        Ui::UserManager *   _ui;
        std::unique_ptr<QMenu>  _usersTreeContextMenu;

        //////////
        //  Signal handlers
    private slots:
        void                _usersTreeWidgetCurrentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*);
        void                _usersTreeWidgetCustomContextMenuRequested(QPoint);
        void                _createUserPushButtonClicked();
        void                _modifyUserPushButtonClicked();
        void                _destroyUserPushButtonClicked();
        void                _createAccountPushButtonClicked();
        void                _modifyAccountPushButtonClicked();
        void                _destroyAccountPushButtonClicked();
    };
}

//  End of tt3-gui/UserManager.hpp

