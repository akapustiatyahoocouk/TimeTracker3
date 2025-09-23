//
//  tt3-gui/CreateActivityTypeDialog.hpp - The modal "Create activity type" dialog
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
    namespace Ui { class CreateActivityTypeDialog; }

    /// \class CreateActivityTypeDialog tt3-gui/API.hpp "tt3-gui/API.hpp"
    /// \brief The modal "Create activity type" dialog
    class TT3_GUI_PUBLIC CreateActivityTypeDialog final
        :   private QDialog
    {
        Q_OBJECT
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(CreateActivityTypeDialog)

        //////////
        //  Types
    public:
        /// The dialog result after a modal invocation
        enum class Result
        {
            Ok,     ///< The user has confirmed the choice, activity type created.
            Cancel  ///< The user has cancelled the dialog.
        };

        //////////
        //  Construction/destruction
    public:
        /// \brief
        ///     Constructs the dialog.
        /// \param parent
        ///     The parent widget for the dialog.
        /// \param workspace
        ///     The workspace to create a new activity type in.
        /// \param credentials
        ///     The credentials to use for workspace access.
        /// \exception WorkspaceException
        ///     If a data access error occurs.
        CreateActivityTypeDialog(
                QWidget * parent,
                tt3::ws::Workspace workspace,
                const tt3::ws::Credentials & credentials
            );
        virtual ~CreateActivityTypeDialog();

        //////////
        //  Operations
    public:
        /// \brief
        ///     Runs the dialog modally.
        /// \return
        ///     The user's choice
        Result      doModal();

        /// \brief
        ///     Returns the newly created user.
        /// \return
        ///     The newly created user (on Ok) or nullptr
        ///     (if the dialog was cancelled).
        auto        createdActivityType(
                        ) const -> tt3::ws::ActivityType { return _createdActivityType; }

        //////////
        //  Implementation
    private:
        tt3::ws::Workspace  _workspace;
        const tt3::ws::Credentials  _credentials;
        tt3::ws::Validator::ActivityType *const _validator;

        tt3::ws::ActivityType   _createdActivityType;

        //  Helpers
        void        _refresh();

        //////////
        //  Controls
    private:
        Ui::CreateActivityTypeDialog *const _ui;

        //////////
        //  Signal handlers
    private slots:
        void            _displayNameLineEditTextChanged(QString);
        void            _descriptionTextEditTextChanged();
        void            accept() override;
        void            reject() override;
    };
}

//  End of tt3-gui/CreateActivityTypeDialog.hpp

