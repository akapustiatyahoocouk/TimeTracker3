//
//  tt3-report-worksummary/SelectUsersDialog.hpp - The modal "Select users" dialog.
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
#include "tt3-report-worksummary/API.hpp"

namespace tt3::report::worksummary
{
    namespace Ui { class SelectUsersDialog; }

    class TT3_REPORT_WORKSUMMARY_PUBLIC SelectUsersDialog final
        :   private QDialog
    {
        Q_OBJECT

        //////////
        //  Types
    public:
        /// \brief
        ///     The selection mode.
        enum class SelectionMode
        {
            SingleUser,     ///< Can select 1 user only.
            MultipleUsers   ///< Can select any number of users (not 0).
        };

        /// \brief
        ///     The dialog result after a modal invocation.
        enum class Result
        {
            Ok,     ///< Users selection confirmed.
            Cancel  ///< The dialog has been cancelled.
        };

        //////////
        //  Construction/destruction
    public:
        /// \brief
        ///     Constructs the dialog.
        /// \param parent
        ///     The parent for the dialog; nullptr == none.
        /// \param workspace
        ///     The workspace to select Users from.
        /// \param credentials
        ///     The credentials to use for data access.
        /// \param selectionMode
        ///     The selection mode.
        /// \param selectedUsers
        ///     The set of Users to start selection process with.
        SelectUsersDialog(
                QWidget * parent,
                tt3::ws::Workspace workspace,
                tt3::ws::ReportCredentials & credentials,
                SelectionMode selectionMode,
                const tt3::ws::Users & selectedUsers
            );

        /// \brief
        ///     The class destructor.
        virtual ~SelectUsersDialog();

        //////////
        //  Operations
    public:
        /// \brief
        ///     Runs the dialog modally.
        /// \return
        ///     The user's choice; on OK a new PublicTask has been created.
        Result          doModal();

        /// \brief
        ///     Returns the set of selected Users.
        /// \return
        ///     The set of selected Users.
        tt3::ws::Users  selectedUsers() const { return _selectedUsers; }

        //////////
        //  Implementation
    private:
        tt3::ws::Workspace  _workspace;
        tt3::ws::ReportCredentials  _credentials;
        SelectionMode       _selectionMode;

        tt3::ws::Users      _selectedUsers; //  currently chosen

        //////////
        //  Controls
    private:
        Ui::SelectUsersDialog *const    _ui;
    };
}

//  End of tt3-report-worksummary/SelectUsersDialog.hpp

