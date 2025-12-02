//
//  tt3-gui/CreatePublicTaskDialog.hpp - The modal "Create public task" dialog
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
    namespace Ui { class CreatePublicTaskDialog; }

    /// \class CreatePublicTaskDialog tt3-gui/API.hpp
    /// \brief The modal "Create public task" dialog
    class TT3_GUI_PUBLIC CreatePublicTaskDialog final
        :   private QDialog
    {
        Q_OBJECT
        TT3_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(CreatePublicTaskDialog)

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
        /// \param workspace
        ///     The workspace to create a new PublicTask in.
        /// \param credentials
        ///     The credentials to use for data access.
        /// \param initialParentTask
        ///     The proposed parent PublicTask for the new
        ///     PublicTask; nullptr == none.
        /// \exception WorkspaceException
        ///     If a data access error occurs.
        CreatePublicTaskDialog(
                QWidget * parent,
                tt3::ws::Workspace workspace,
                const tt3::ws::Credentials & credentials,
                tt3::ws::PublicTask initialParentTask
            );

        /// \brief
        ///     The class destructor.
        virtual ~CreatePublicTaskDialog();

        //////////
        //  Operations
    public:
        /// \brief
        ///     Runs the dialog modally.
        /// \return
        ///     The user's choice; on OK a new PublicTask has been created.
        Result          doModal();

        /// \brief
        ///     Returns the newly created PublicTask.
        /// \return
        ///     The newly created PublicTask (on Ok) or
        ///     nullptr if the dialog was cancelled.
        auto            createdPublicTask(
                            ) const -> tt3::ws::PublicTask
        {
            return _createdPublicTask;
        }

        //////////
        //  Implementation
    private:
        tt3::ws::Workspace  _workspace;
        const tt3::ws::Credentials  _credentials;
        tt3::ws::Validator::PublicTask *const   _validator;

        tt3::ws::PublicTask _createdPublicTask;

        //  Helpers
        auto            _selectedParentTask(
                            ) -> tt3::ws::PublicTask;
        void            _setSelectedParentTask(
                                tt3::ws::PublicTask parentTask
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
        Ui::CreatePublicTaskDialog *const   _ui;

        //////////
        //  Signal handlers
    private slots:
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

//  End of tt3-gui/CreatePublicTaskDialog.hpp
