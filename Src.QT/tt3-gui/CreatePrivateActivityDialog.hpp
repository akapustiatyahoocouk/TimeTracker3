//
//  tt3-gui/CreatePrivateActivityDialog.hpp - The modal "Create private activity" dialog
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
    namespace Ui { class CreatePrivateActivityDialog; }

    /// \class CreatePrivateActivityDialog tt3-gui/API.hpp
    /// \brief The modal "Create private activity" dialog
    class TT3_GUI_PUBLIC CreatePrivateActivityDialog final
        :   private QDialog
    {
        Q_OBJECT
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(CreatePrivateActivityDialog)

        //////////
        //  Types
    public:
        /// \brief
        ///     The dialog result after a modal invocation.
        enum class Result
        {
            Ok,     ///< Changes confirme; PrivateActivity has been created.
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
        ///     The User to create a new PrivateActivity for.
        /// \param credentials
        ///     The credentials to use for data access.
        /// \exception WorkspaceException
        ///     If a data access error occurs.
        CreatePrivateActivityDialog(
                QWidget * parent,
                tt3::ws::User owner,
                const tt3::ws::Credentials & credentials
            );

        /// \brief
        ///     The class destructor.
        virtual ~CreatePrivateActivityDialog();

        //////////
        //  Operations
    public:
        /// \brief
        ///     Runs the dialog modally.
        /// \return
        ///     The user's choice; on OK a new PrivateActivity has been created.
        Result          doModal();

        /// \brief
        ///     Returns the newly created PrivateActivity.
        /// \return
        ///     The newly created PrivateActivity (on Ok) or
        ///     nullptr if the dialog was cancelled.
        auto            createdPrivateActivity(
                            ) const -> tt3::ws::PrivateActivity
        {
            return _createdPrivateActivity;
        }

        //////////
        //  Implementation
    private:
        tt3::ws::Workspace  _workspace;
        const tt3::ws::Credentials  _credentials;
        tt3::ws::Validator::PrivateActivity *const _validator;

        tt3::ws::PrivateActivity    _createdPrivateActivity;

        //  Helpers
        tt3::ws::User   _selectedUser();
        void            _setSelectedUser(
                                tt3::ws::User user
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
        Ui::CreatePrivateActivityDialog *const  _ui;

        //////////
        //  Signal handlers
    private slots:
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

//  End of tt3-gui/CreatePrivateActivityDialog.hpp
