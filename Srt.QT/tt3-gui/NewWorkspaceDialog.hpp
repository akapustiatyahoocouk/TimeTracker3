//
//  tt3-gui/NewWorkspaceDialog.hpp - The modal "New workspace" dialog
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
    //  The modal "New workspace" dialog
    namespace Ui { class NewWorkspaceDialog; }

    class TT3_GUI_PUBLIC NewWorkspaceDialog final
        :   private QDialog
    {
        Q_OBJECT
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(NewWorkspaceDialog)

        //////////
        //  Types
    public:
        //  The dialog result after a modal invocation
        enum class Result { Ok, Cancel };

        //////////
        //  Construction/destruction
    public:
        explicit NewWorkspaceDialog(QWidget * parent);
        virtual ~NewWorkspaceDialog();

        //////////
        //  Operations
    public:
        //  Runs the dialog modally, returning user's choice
        Result          doModal();

        //  The workspace address selected by the user or
        //  nullptr if the user has cancelled the dialog.
        tt3::ws::WorkspaceAddress   selectedWorkspaceAddress() { return _workspaceAddress; }

        //  The chosen administrator user name.
        QString         adminUser() const { return _adminUser; }

        //  The chosen administrator login.
        QString         adminLogin() const { return _adminLogin; }

        //  The chosen administrator password.
        QString         adminPassword() const { return _adminPassword; }

        //////////
        //  Implementation
    private:
        tt3::ws::WorkspaceAddress   _workspaceAddress;  //  invalid == not selected
        QString         _adminUser;
        QString         _adminLogin;
        QString         _adminPassword;

        //  Helpers
        void            _refresh();

        //////////
        //  Controlss
    private:
        Ui::NewWorkspaceDialog *    _ui;

        //////////
        //  Event handlers
    private slots:
        void            _workspaceTypeComboBoxCurrentIndexChanged(int);
        void            _browsePushButtonClicked();
        void            _userNameLineEditTextChanged(QString);
        void            _loginLineEditTextChanged(QString);
        void            _passwordLineEditTextChanged(QString);
        void            _confirmPasswordLineEditTextChanged(QString);
        void            accept() override;
        void            reject() override;
    };
}

//  End of tt3-gui/NewWorkspaceDialog.hpp

