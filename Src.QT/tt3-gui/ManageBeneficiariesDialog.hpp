//
//  tt3-gui/ManageBeneficiariesDialog.hpp - The modal "Manage Beneficiaries" dialog
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
    class BeneficiaryManager;

    namespace Ui { class ManageBeneficiariesDialog; }

    /// \class ManageBeneficiariesDialog tt3-gui/API.hpp
    /// \brief The modal "Manage Beneficiaries" dialog.
    class TT3_GUI_PUBLIC ManageBeneficiariesDialog final
        :   private QDialog
    {
        Q_OBJECT
        TT3_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(ManageBeneficiariesDialog)

        //////////
        //  Construction/destruction
    public:
        /// \brief
        ///     Constructs the dialo.
        /// \param parent
        ///     The parent widget for the dialog; nullptr == none.
        /// \param workspace
        ///     The workspace to manage Beneficiaries/accounts in.
        /// \param credentials
        ///     The credentials to use for data access.
        ManageBeneficiariesDialog(
                QWidget * parent,
                tt3::ws::Workspace workspace,
                const tt3::ws::Credentials & credentials
            );

        /// \brief
        ///     The class destructor.
        virtual ~ManageBeneficiariesDialog();

        //////////
        //  Operations
    public:
        /// \brief
        ///     Runs the dialog modally.
        void            doModal();

        //////////
        //  Controls
    private:
        Ui::ManageBeneficiariesDialog *const  _ui;
        //  Custom controls are created dynamically
        QStackedLayout *    _managerPanelLayout = nullptr;
        BeneficiaryManager *_beneficiaryManager = nullptr;
    };
}

//  End of tt3-gui/ManageBeneficiariesDialog.hpp
