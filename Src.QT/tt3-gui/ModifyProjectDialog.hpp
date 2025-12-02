//
//  tt3-gui/ModifyProjectDialog.hpp - The modal "Modify project" dialog
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
    namespace Ui { class ModifyProjectDialog; }

    /// \class ModifyProjectDialog tt3-gui/API.hpp
    /// \brief The modal "Modify project" dialog
    class TT3_GUI_PUBLIC ModifyProjectDialog final
        :   private QDialog
    {
        Q_OBJECT
        TT3_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(ModifyProjectDialog)

        //////////
        //  Types
    public:
        /// \brief
        ///     The dialog result after a modal invocation.
        enum class Result
        {
            Ok,     ///< Changes confirme; changes have been saved.
            Cancel  ///< The dialog has been cancelled.
        };

        //////////
        //  Construction/destruction
    public:
        /// \brief
        ///     Constructs the dialog.
        /// \param parent
        ///     The parent widget for the dialog; nullptr == none.
        /// \param project
        ///     The Project to modify.
        /// \param credentials
        ///     The credentials to use for data access.
        /// \exception WorkspaceException
        ///     If a data access error occurs.
        ModifyProjectDialog(
                QWidget * parent,
                tt3::ws::Project project,
                const tt3::ws::Credentials & credentials
            );

        /// \brief
        ///     The class destructor.
        virtual ~ModifyProjectDialog();

        //////////
        //  Operations
    public:
        /// \brief
        ///     Runs the dialog modally.
        /// \return
        ///     The user's choice; on OK all changes have been saved.
        Result          doModal();

        //////////
        //  Implementation
    private:
        tt3::ws::Project    _project;
        const tt3::ws::Credentials  _credentials;
        tt3::ws::Validator::Project *const   _validator;
        const bool      _readOnly;

        //  Helpers
        auto            _selectedParentProject(
                            ) -> tt3::ws::Project;
        void            _setSelectedParentProject(
                                tt3::ws::Project parentProject
                            );
        auto            _selectedBeneficiaries(
                            ) -> tt3::ws::Beneficiaries;
        void            _setSelectedBeneficiaries(
                                tt3::ws::Beneficiaries beneficiaries
                            );
        void            _refresh();

        //////////
        //  Controls
    private:
        Ui::ModifyProjectDialog *const  _ui;
        //  Drawing resources
        ListWidgetDecorations   _listWidgetDecorations;

        //////////
        //  Signal handlers
    private slots:
        void            _selectParentProjectPushButtonClicked();
        void            _displayNameLineEditTextChanged(QString);
        void            _descriptionPlainTextEditTextChanged();
        void            _selectBeneficiariesPushButtonClicked();
        virtual void    accept() override;
        virtual void    reject() override;
    };
}

//  End of tt3-gui/ModifyProjectDialog.hpp
