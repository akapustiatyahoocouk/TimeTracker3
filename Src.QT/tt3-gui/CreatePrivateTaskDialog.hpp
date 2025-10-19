//
//  tt3-gui/CreatePrivateTaskDialog.hpp - The modal "Create private task" dialog
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
    namespace Ui { class CreatePrivateTaskDialog; }

    /// \class CreatePrivateTaskDialog tt3-gui/API.hpp
    /// \brief The modal "Create private task" dialog
    class TT3_GUI_PUBLIC CreatePrivateTaskDialog final
        :   private QDialog
    {
        Q_OBJECT
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(CreatePrivateTaskDialog)

        //////////
        //  Types
    public:
        /// \brief
        ///     The dialog result after a modal invocation.
        enum class Result
        {
            Ok,     ///< Changes confirme; PublicTask has been created.
            Cancel  ///< The dialog has been cancelled.
        };

        //////////
        //  Construction/destruction
    public:
        /// \brief
        ///     Constructs the dialog.
        /// \param parent
        ///     The parent widget for the dialog; nullptr == none.
        /// \param owner
        ///     The User to create a new PrivateTask for.
        /// \param credentials
        ///     The credentials to use for data access.
        /// \param initialParentTask
        ///     The proposed parent PublicTask for the new
        ///     PublicTask; nullptr == none.
        /// \exception WorkspaceException
        ///     If a data access error occurs.
        CreatePrivateTaskDialog(
                QWidget * parent,
                tt3::ws::User owner,
                const tt3::ws::Credentials & credentials
            );

        /// \brief
        ///     Constructs the dialog.
        /// \param parent
        ///     The parent widget for the dialog; nullptr == none.
        /// \param parentTask
        ///     The proposed parent for the new PrivateTask.
        /// \param credentials
        ///     The credentials to use for data access.
        /// \param initialParentTask
        ///     The proposed parent PublicTask for the new
        ///     PublicTask; nullptr == none.
        /// \exception WorkspaceException
        ///     If a data access error occurs.
        CreatePrivateTaskDialog(
                QWidget * parent,
                tt3::ws::PrivateTask parentTask,
                const tt3::ws::Credentials & credentials
            );

        /// \brief
        ///     The class destructor.
        virtual ~CreatePrivateTaskDialog();

        //////////
        //  Operations
    public:
        /// \brief
        ///     Runs the dialog modally.
        /// \return
        ///     The user's choice; on OK a new PrivateTask has been created.
        Result          doModal();

        /// \brief
        ///     Returns the newly created PrivateTask.
        /// \return
        ///     The newly created PrivateActivity (on Ok) or
        ///     nullptr if the dialog was cancelled.
        auto            createdPrivateTask(
                            ) const -> tt3::ws::PrivateTask
        {
            return _createdPrivateTask;
        }

        //////////
        //  Implementation
        tt3::ws::Workspace  _workspace;
        const tt3::ws::Credentials  _credentials;
        tt3::ws::Validator::PrivateTask *const  _validator;

        tt3::ws::PrivateTask    _createdPrivateTask;

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
        auto            _selectedWorkload(
                            ) -> tt3::ws::Workload;
        void            _setSelectedWorkload(
                                tt3::ws::Workload workload
                            );
        auto            _selectedTimeout(
                            ) -> tt3::ws::InactivityTimeout;
        void            _refresh();

        //////////
        //  Controls
    private:
        Ui::CreatePrivateTaskDialog *const  _ui;

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

//  End of tt3-gui/CreatePrivateTaskDialog.hpp
