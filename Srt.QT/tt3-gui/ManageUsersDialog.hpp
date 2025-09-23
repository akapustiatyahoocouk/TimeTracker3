//
//  tt3-gui/ManageUsersDialog.hpp - The modal "Manage users" dialog
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
    class TT3_GUI_PUBLIC UserManager;

    namespace Ui { class ManageUsersDialog; }

    /// \class ManageUsersDialog tt3-gui/API.hpp
    /// \brief The modal "Manage users/accounts" dialog.
    class TT3_GUI_PUBLIC ManageUsersDialog final
        :   private QDialog
    {
        Q_OBJECT
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(ManageUsersDialog)

        //////////
        //  Construction/destruction
    public:
        /// \brief
        ///     Constructs the dialo.
        /// \param parent
        ///     The parent widget for the dialog; nullptr == none.
        /// \param workspace
        ///     The workspace to manage users/accounts in.
        /// \param credentials
        ///     The credentials to use for data access.
        ManageUsersDialog(
                QWidget * parent,
                tt3::ws::Workspace workspace,
                const tt3::ws::Credentials & credentials
            );

        /// \brief
        ///     The class destructor.
        virtual ~ManageUsersDialog();

        //////////
        //  Operations
    public:
        /// \brief
        ///     Runs the dialog modally.
        void            doModal();

        //////////
        //  Controls
    private:
        Ui::ManageUsersDialog *const    _ui;
        //  Custom controls are created dynamically
        QStackedLayout *_managerPanelLayout = nullptr;
        UserManager *   _userManager = nullptr;
    };
}

//  End of tt3-gui/ManageUsersDialog.hpp

