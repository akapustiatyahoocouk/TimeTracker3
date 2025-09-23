//
//  tt3-gui/ModifyPublicActivityDialog.hpp - The modal "Modify public activity" dialog
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
    namespace Ui { class ModifyPublicActivityDialog; }

    /// \class ModifyPublicActivityDialog "tt3-gui/API.hpp"
    /// \brief The modal "Modify public activity" dialog.
    class TT3_GUI_PUBLIC ModifyPublicActivityDialog final
        :   private QDialog
    {
        Q_OBJECT
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(ModifyPublicActivityDialog)

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
        /// @param publicActivity
        ///     The public activity to modify.
        /// @param credentials
        ///     The credentials to use for accessing User's data.
        /// @exception WorkspaceException
        ///     If an error occurs retrieving ActivityType details.
        ModifyPublicActivityDialog(
                QWidget * parent,
                tt3::ws::PublicActivity publicActivity,
                const tt3::ws::Credentials & credentials
            );

        /// \brief
        ///     The class destructor.
        virtual ~ModifyPublicActivityDialog();

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
        tt3::ws::PublicActivity _publicActivity;
        const tt3::ws::Credentials  _credentials;
        tt3::ws::Validator::PublicActivity *const _validator;
        const bool      _readOnly;

        tt3::ws::Workload   _selectedWorkload;  //  currenty selected

        //  Helpers
        auto            _selectedActivityType(
                            ) -> tt3::ws::ActivityType;
        void            _setSelectedActivityType(
                                tt3::ws::ActivityType activityType
                            );
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
        Ui::ModifyPublicActivityDialog *const   _ui;

        //////////
        //  Signal handlers
    private slots:
        void            _displayNameLineEditTextChanged(QString);
        void            _descriptionTextEditTextChanged();
        void            _selectWorkloadPushButtonClicked();
        void            _timeoutCheckBoxStateChanged(int);
        void            _hoursComboBoxCurrentIndexChanged(int);
        void            _minutesComboBoxCurrentIndexChanged(int);
        virtual void    accept() override;
        virtual void    reject() override;
    };
}

//  End of tt3-gui/ModifyPublicActivityDialog.hpp

