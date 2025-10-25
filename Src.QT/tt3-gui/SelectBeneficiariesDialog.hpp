//
//  tt3-gui/SelectBeneficiariesDialog.hpp - The modal "Select beneficiaries" dialog
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
    namespace Ui { class SelectBeneficiariesDialog; }

    /// \class SelectBeneficiariesDialog tt3-gui/API.hpp
    /// \brief The modal "Select beneficiaries" dialog.
    class TT3_GUI_PUBLIC SelectBeneficiariesDialog
        :   private QDialog
    {
        Q_OBJECT
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(SelectBeneficiariesDialog)

        //////////
        //  Types
    public:
        /// \brief
        ///     The dialog result after a modal invocation.
        enum class Result
        {
            Ok,     ///< The user has selected a set of beneficiaries.
            Cancel  ///< The user has cancelled the dialog.
        };

        //////////
        //  Construction/destruction
    public:
        /// \brief
        ///     Constructs the dialog.
        /// \param parent
        ///     The parent for the dialog; nullptr == none.
        /// \param workspace
        ///     The workspace to select beneficiaries from.
        /// \param credentials
        ///     The credentials to use for data access.
        /// \param beneficiaries
        ///     The set of beneficiaries to start the selection with.
        /// \exception WorkspaceException
        ///     If an error occurs constructing the dialog.
    public:
        explicit SelectBeneficiariesDialog(
                QWidget * parent,
                tt3::ws::Workspace workspace,
                const tt3::ws::Credentials & credentials,
                const tt3::ws::Beneficiaries & beneficiaries
            );

        /// \brief
        ///     The class destructor.
        virtual ~SelectBeneficiariesDialog();

        //////////
        //  Operations
    public:
        /// \brief
        ///     Runs the dialog modally.
        /// \return
        ///     The user's choice.
        Result      doModal();

        /// \brief
        ///     Returns the set of beneficiaries selected by the user.
        /// \return
        ///     The set of beneficiaries selected by the user.
        auto        selectedBeneficiaries(
                        ) -> tt3::ws::Beneficiaries;

        //////////
        //  Implementation
    private:
        tt3::ws::Workspace      _workspace;
        tt3::ws::Credentials    _credentials;
        bool        _refreshUnderway = false;

        //  Helpers
        auto        _currentBeneficiary(
                        ) -> tt3::ws::Beneficiary;
        void        _setCurrentBeneficiary(
                            tt3::ws::Beneficiary beneficiary
                        );
        auto        _selectedBeneficiaries(
                        ) -> tt3::ws::Beneficiaries;
        void        _setSelectedBeneficiaries(
                            const tt3::ws::Beneficiaries & beneficiaries
                        );
        void        _refresh();
        void        _refreshBeneficiaryCheckStates();
        void        _refreshBeneficiaryCheckStates(
                            QTreeWidgetItem * beneficiaryItem,
                            const tt3::ws::Beneficiaries & selectedBeneficiaries
                        );

        //////////
        //  Controls
    private:
        Ui::SelectBeneficiariesDialog *const    _ui;
        //  Drawing resources
        TreeWidgetDecorations   _treeWidgetDecorations;
        ListWidgetDecorations   _listWidgetDecorations;

        //////////
        //  Event handlers
    private slots:
        void            _filterLineEditTextChanged(QString);
        void            _beneficiariesTreeWidgetItemChanged(QTreeWidgetItem * item, int);
        void            _beneficiariesListWidgetCurrentRowChanged(int);
        virtual void    accept() override;
        virtual void    reject() override;
    };
}

//  End of tt3-gui/SelectBeneficiariesDialog.hpp
