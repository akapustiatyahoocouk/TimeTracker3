//
//  tt3-gui/CreateBeneficiaryDialog.hpp - The modal "Create beneficiary" dialog
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
    namespace Ui { class CreateBeneficiaryDialog; }

    /// \class CreateBeneficiaryDialog tt3-gui/API.hpp
    /// \brief The modal "Create beneficiary" dialog
    class TT3_GUI_PUBLIC CreateBeneficiaryDialog final
        :   private QDialog
    {
        Q_OBJECT
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(CreateBeneficiaryDialog)

        //////////
        //  Types
    public:
        /// The dialog result after a modal invocation
        enum class Result
        {
            Ok,     ///< The user has confirmed the choice, beneficiary created.
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
        ///     The workspace to create a new beneficiary in.
        /// \param credentials
        ///     The credentials to use for workspace access.
        /// \exception WorkspaceException
        ///     If a data access error occurs.
        CreateBeneficiaryDialog(
                QWidget * parent,
                tt3::ws::Workspace workspace,
                const tt3::ws::Credentials & credentials
            );

        /// \brief
        ///     The class destructor.
        virtual ~CreateBeneficiaryDialog();

        //////////
        //  Operations
    public:
        /// \brief
        ///     Runs the dialog modally.
        /// \return
        ///     The user's choice
        Result      doModal();

        /// \brief
        ///     Returns the newly created beneficiary.
        /// \return
        ///     The newly created beneficiary (on Ok) or nullptr
        ///     (if the dialog was cancelled).
        auto        createdBeneficiary(
                        ) const -> tt3::ws::Beneficiary { return _createdBeneficiary; }

        //////////
        //  Implementation
    private:
        tt3::ws::Workspace  _workspace;
        const tt3::ws::Credentials  _credentials;
        tt3::ws::Validator::Beneficiary *const _validator;

        tt3::ws::Beneficiary    _createdBeneficiary;

        //  Helpers
        auto        _selectedWorkloads(
                        ) -> tt3::ws::Workloads;
        void        _setSelectedWorkloads(
                            const tt3::ws::Workloads & workloads
                        );
        void        _refresh();

        //////////
        //  Controls
    private:
        Ui::CreateBeneficiaryDialog *const  _ui;
        //  Drawing resources
        ListWidgetDecorations   _listWidgetDecorations;

        //////////
        //  Signal handlers
    private slots:
        void            _displayNameLineEditTextChanged(QString);
        void            _descriptionPlainTextEditTextChanged();
        void            _selectWorkloadsPushButtonClicked();
        void            accept() override;
        void            reject() override;
    };
}

//  End of tt3-gui/CreateBeneficiaryDialog.hpp

