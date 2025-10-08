//
//  tt3-gui/ModifyBeneficiaryDialog.hpp - The modal "Modify beneficiary" dialog
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
    namespace Ui { class ModifyBeneficiaryDialog; }

    /// \class ModifyBeneficiaryDialog tt3-gui/API.hpp
    /// \brief The modal "Modify beneficiary" dialog.
    class TT3_GUI_PUBLIC ModifyBeneficiaryDialog final
        :   private QDialog
    {
        Q_OBJECT
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(ModifyBeneficiaryDialog)

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
        /// @param beneficiary
        ///     The beneficiary to modify.
        /// @param credentials
        ///     The credentials to use for accessing User's data.
        /// @exception WorkspaceException
        ///     If an error occurs retrieving Beneficiary details.
        ModifyBeneficiaryDialog(
                QWidget * parent,
                tt3::ws::Beneficiary beneficiary,
                const tt3::ws::Credentials & credentials
            );
        virtual ~ModifyBeneficiaryDialog();

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
        tt3::ws::Beneficiary    _beneficiary;
        const tt3::ws::Credentials  _credentials;
        tt3::ws::Validator::Beneficiary *const _validator;
        const bool      _readOnly;

        //  Helpers
        auto            _selectedWorkloads(
                            ) -> tt3::ws::Workloads;
        void            _setSelectedWorkloads(
                                const tt3::ws::Workloads & workloads
                            );
        void            _refresh();

        //////////
        //  Controls
    private:
        Ui::ModifyBeneficiaryDialog *const _ui;

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

//  End of tt3-gui/ModifyActivityTypeDialog.hpp
