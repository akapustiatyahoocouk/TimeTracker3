//
//  tt3-gui/ModifyPrivateTaskDialog.hpp - The modal "Modify private task" dialog
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
    namespace Ui { class ModifyPrivateTaskDialog; }

    /// \class ModifyPrivateTaskDialog tt3-gui/API.hpp
    /// \brief The modal "Modify private task" dialog.
    class TT3_GUI_PUBLIC ModifyPrivateTaskDialog final
        :   private QDialog
    {
        Q_OBJECT
        TT3_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(ModifyPrivateTaskDialog)

        //////////
        //  Types
    public:
        /// \brief
        ///     The dialog result after a modal invocation.
        enum class Result
        {
            Ok,     ///< The user has confirmed and saved the changes.
            Cancel  ///< The user has cancelled the dialog.
        };

        //////////
        //  Construction/destruction
    public:
        /// \brief
        ///     Constructs the dialog.
        /// @param parent
        ///     The parent widget for the dialog; nullptr == none.
        /// @param privateTask
        ///     The private task to modify.
        /// @param credentials
        ///     The credentials to use for accessing User's data.
        /// @exception WorkspaceException
        ///     If an error occurs retrieving PrivateTask details.
        ModifyPrivateTaskDialog(
                QWidget * parent,
                tt3::ws::PrivateTask privateTask,
                const tt3::ws::Credentials & credentials
            );

        /// \brief
        ///     The class destructor.
        virtual ~ModifyPrivateTaskDialog();

        //////////
        //  Operations
    public:
        /// \brief
        ///     Runs the dialog modally.
        /// @return
        ///     The dialog result; Ok means "changes saved".
        Result          doModal();

        //////////
        //  Implementation
    private:
        tt3::ws::PrivateTask    _privateTask;
        const tt3::ws::Credentials  _credentials;
        tt3::ws::Validator::PrivateTask *const  _validator;
        const bool      _readOnly;

        //  Helpers
        tt3::ws::User   _selectedUser();
        void            _setSelectedUser(tt3::ws::User user);
        auto            _selectedParentTask(
                            ) -> tt3::ws::PrivateTask;
        void            _setSelectedParentTask(
                                tt3::ws::PrivateTask parentTask
                            );
        auto            _selectedActivityType(
                            ) -> tt3::ws::ActivityType;
        void            _setSelectedActivityType(
                                tt3::ws::ActivityType activityType
                            );
        auto            _selectedWorkload(
                            ) -> tt3::ws::Workload;
        void            _setSelectedWorkload(
                                tt3::ws::Workload workload
                            );
        auto            _selectedTimeout(
                            ) -> tt3::ws::InactivityTimeout;
        void            _setSelectedTimeout(
                                const tt3::ws::InactivityTimeout & timeout
                            );
        void            _refresh();

        //////////
        //  Controls
    private:
        Ui::ModifyPrivateTaskDialog *const  _ui;

        //////////
        //  Signal handlers
    private slots:
        void            _userComboBoxCurrentIndexChanged(int);
        void            _selectParentTaskPushButtonClicked();
        void            _displayNameLineEditTextChanged(QString);
        void            _descriptionPlainTextEditTextChanged();
        void            _selectWorkloadPushButtonClicked();
        void            _timeoutCheckBoxStateChanged(int);
        void            _hoursComboBoxCurrentIndexChanged(int);
        void            _minutesComboBoxCurrentIndexChanged(int);
        virtual void    accept() override;
        virtual void    reject() override;
    };
}

//  End of tt3-gui/ModifyPrivateTaskDialog.hpp

