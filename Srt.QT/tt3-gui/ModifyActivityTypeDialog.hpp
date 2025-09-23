//
//  tt3-gui/ModifyActivityTypeDialog.hpp - The modal "Modify activity type" dialog
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
    namespace Ui { class ModifyActivityTypeDialog; }

    /// \class ModifyActivityTypeDialog "tt3-gui/API.hpp"
    /// \brief The modal "Modify activity type" dialog.
    class TT3_GUI_PUBLIC ModifyActivityTypeDialog final
        :   private QDialog
    {
        Q_OBJECT
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(ModifyActivityTypeDialog)

        //////////
        //  Types
    public:
        /// The dialog result after a modal invocation
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
        /// @param activityType
        ///     The activity type to modify.
        /// @param credentials
        ///     The credentials to use for accessing User's data.
        /// @exception WorkspaceException
        ///     If an error occurs retrieving ActivityType details.
        ModifyActivityTypeDialog(
                QWidget * parent,
                tt3::ws::ActivityType activityType,
                const tt3::ws::Credentials & credentials
            );
        virtual ~ModifyActivityTypeDialog();

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
        tt3::ws::ActivityType   _activityType;
        const tt3::ws::Credentials  _credentials;
        tt3::ws::Validator::ActivityType *const _validator;
        const bool      _readOnly;

        //  Helpers
        void            _refresh();

        //////////
        //  Controls
    private:
        Ui::ModifyActivityTypeDialog *const _ui;

        //////////
        //  Signal handlers
    private slots:
        void            _displayNameLineEditTextChanged(QString);
        void            _descriptionTextEditTextChanged();
        void            accept() override;
        void            reject() override;
    };
}

//  End of tt3-gui/ModifyActivityTypeDialog.hpp

