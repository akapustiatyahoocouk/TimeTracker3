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
        explicit UserManager(QWidget * parent,
                             tt3::ws::IWorkspaceProvider * workspaceProvider,
                             tt3::ws::ICredentialsProvider * credentialsProvider);
        virtual ~UserManager();

        //////////
        //  Operaions
    public:
        void                refresh();

        //////////
        //  Implementation
    private:
        tt3::ws::IWorkspaceProvider *const      _workspaceProvider;
        tt3::ws::ICredentialsProvider *const    _credentialsProvider;

        //  Helpers
        void                _refreshUserItems(const tt3::ws::Workspace & workspace, const tt3::ws::Credentials & credentials);
        void                _refreshAccountItems(QTreeWidgetItem * userItem, const tt3::ws::Credentials & credentials);
        static QString      _userItemText(tt3::ws::User user, const tt3::ws::Credentials & credentials) throws(tt3::ws::WorkspaceException);
        static QString      _accountItemText(tt3::ws::Account account, const tt3::ws::Credentials & credentials) throws(tt3::ws::WorkspaceException);
        tt3::ws::User       _selectedUser();
        tt3::ws::Account    _selectedAccount();

        //////////
        //  Controls
    private:
        Ui::UserManager *   _ui;

        //////////
        //  Signal handlers
    private slots:
        void                _onProvidedWorkspaceChanged(tt3::ws::Workspace before, tt3::ws::Workspace after);
        void                _onProvidedCredentialsChanged(tt3::ws::Credentials before, tt3::ws::Credentials after);
        void                _usersTreeWidgetCurrentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*);
    };
}

//  End of tt3-gui/UserManager.hpp

