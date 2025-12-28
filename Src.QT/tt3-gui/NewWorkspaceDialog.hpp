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
    namespace Ui { class NewWorkspaceDialog; }

    /// \class NewWorkspaceDialog tt3-gui/API.hpp
    /// \brief The modal "New workspace" dialog.
    class TT3_GUI_PUBLIC NewWorkspaceDialog final
        :   private QDialog
    {
        Q_OBJECT
        TT3_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(NewWorkspaceDialog)

        //////////
        //  Types
    public:
        /// \brief
        ///     The dialog result after a modal invocation.
        enum class Result
        {
            Ok,     ///< The user has provided required details for the new workspace.
            Cancel  ///< The user has cancelled the dialog.
        };

        //////////
        //  Construction/destruction
    public:
        /// \brief
        ///     Constructs the dialog.
        /// \param parent
        ///     The parent widget for the dialog.
        explicit NewWorkspaceDialog(
                QWidget * parent
            );

        /// \brief
        ///     The class destructor.
        virtual ~NewWorkspaceDialog();

        //////////
        //  Operations
    public:
        /// \brief
        ///     Runs the dialog modally.
        /// \return
        ///     The user's choice.
        Result          doModal();

        /// \brief
        ///     Returns the workspace address selected by the user or.
        /// \return
        ///     The workspace address selected by the user or
        ///     nullptr if the user has cancelled the dialog.
        tt3::ws::WorkspaceAddress   selectedWorkspaceAddress() { return _workspaceAddress; }

        /// \brief
        ///     Returns the chosen administrator user name.
        /// \return
        ///     The chosen administrator user name.
        QString         adminUser() const { return _adminUser; }

        /// \brief
        ///     Returns the chosen administrator login.
        /// \return
        ///     The chosen administrator login.
        QString         adminLogin() const { return _adminLogin; }

        /// \brief
        ///     Returns the chosen administrator password.
        /// \return
        ///     The chosen administrator password.
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
        auto            _selectedWorkspaceType(
                            ) -> tt3::ws::WorkspaceType;
        void            _setSelectedWorkspaceType(
                                tt3::ws::WorkspaceType workspaceType
                            );

        //////////
        //  Controlss
    private:
        Ui::NewWorkspaceDialog *const   _ui;

        //////////
        //  Signal handlers
    private slots:
        void            _workspaceTypeComboBoxCurrentIndexChanged(int);
        void            _browsePushButtonClicked();
        void            _userNameLineEditTextChanged(QString);
        void            _loginLineEditTextChanged(QString);
        void            _passwordLineEditTextChanged(QString);
        void            _confirmPasswordLineEditTextChanged(QString);
        virtual void    accept() override;
        virtual void    reject() override;
    };
}

//  End of tt3-gui/NewWorkspaceDialog.hpp

