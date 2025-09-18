//
//  tt3-gui/CreatePublicActivityDialog.hpp - The modal "Create public activity" dialog
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
    namespace Ui { class CreatePublicActivityDialog; }

    /// \class CreatePublicActivityDialog tt3-gui/API.hpp
    /// \brief The modal "Create public activity" dialog
    class TT3_GUI_PUBLIC CreatePublicActivityDialog final
        :   private QDialog
    {
        Q_OBJECT
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(CreatePublicActivityDialog)

        //////////
        //  Types
    public:
        /// \brief
        ///     The dialog result after a modal invocation.
        enum class Result
        {
            Ok,     ///< Changes confirme; PublicActivity has been created.
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
        ///     The workspace to create a new PublicActivity in.
        /// \param credentials
        ///     The credentials to use for data access.
        /// \exception WorkspaceException
        ///     If a data access error occurs.
        CreatePublicActivityDialog(
                QWidget * parent,
                tt3::ws::Workspace workspace,
                const tt3::ws::Credentials & credentials
            );

        /// \brief
        ///     The class destructor.
        virtual ~CreatePublicActivityDialog();

        //////////
        //  Operations
    public:
        /// \brief
        ///     Runs the dialog modally.
        /// \return
        ///     The user's choice; on OK a new PublicActivity has been created.
        Result          doModal();

        /// \brief
        ///     Returns the newly created PublicActivity.
        /// \return
        ///     The newly created PublicActivity (on Ok) or
        ///     nullptr if the dialog was cancelled.
        auto            createdPublicActivity(
                            ) const -> tt3::ws::PublicActivity
        {
            return _createdPublicActivity;
        }

        //////////
        //  Implementation
    private:
        tt3::ws::Workspace  _workspace;
        const tt3::ws::Credentials  _credentials;
        tt3::ws::Validator::PublicActivity *const _validator;

        tt3::ws::Workload       _selectedWorkload = nullptr;        //  currenty selected

        tt3::ws::PublicActivity _createdPublicActivity;

        //  Helpers
        auto            _selectedActivityType(
                            ) -> tt3::ws::ActivityType;
        auto            _selectedTimeout(
                            ) -> tt3::ws::InactivityTimeout;
        void            _refresh();

        //////////
        //  Controls
    private:
        Ui::CreatePublicActivityDialog *    _ui;

        //////////
        //  Signal handlers
    private slots:
        void            _displayNameLineEditTextChanged(QString);
        void            _descriptionTextEditTextChanged();
        void            _timeoutCheckBoxStateChanged(int);
        void            _hoursComboBoxCurrentIndexChanged(int);
        void            _minutesComboBoxCurrentIndexChanged(int);
        void            _selectActivityTypePushButtonClicked();
        void            _selectWorkloadPushButtonClicked();
        virtual void    accept() override;
        virtual void    reject() override;
    };
}

//  End of tt3-gui/CreatePublicActivityDialog.hpp

