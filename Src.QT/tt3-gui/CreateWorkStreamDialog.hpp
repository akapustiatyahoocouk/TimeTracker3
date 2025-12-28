//
//  tt3-gui/CreateWorkStreamDialog.hpp - The modal "Create work stream" dialog
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
    namespace Ui { class CreateWorkStreamDialog; }

    /// \class CreateWorkStreamDialog tt3-gui/API.hpp
    /// \brief The modal "Create work stream" dialog
    class TT3_GUI_PUBLIC CreateWorkStreamDialog final
        :   private QDialog
    {
        Q_OBJECT
        TT3_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(CreateWorkStreamDialog)

        //////////
        //  Types
    public:
        /// \brief
        ///     The dialog result after a modal invocation.
        enum class Result
        {
            Ok,     ///< Changes confirme; WorkStream has been created.
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
        ///     The workspace to create a new WorkStream in.
        /// \param credentials
        ///     The credentials to use for data access.
        /// \exception WorkspaceException
        ///     If a data access error occurs.
        CreateWorkStreamDialog(
                QWidget * parent,
                tt3::ws::Workspace workspace,
                const tt3::ws::Credentials & credentials
            );

        /// \brief
        ///     The class destructor.
        virtual ~CreateWorkStreamDialog();

        //////////
        //  Operations
    public:
        /// \brief
        ///     Runs the dialog modally.
        /// \return
        ///     The user's choice; on OK a new WorkStream has been created.
        Result          doModal();

        /// \brief
        ///     Returns the newly created WorkStream.
        /// \return
        ///     The newly created WorkStream (on Ok) or
        ///     nullptr if the dialog was cancelled.
        auto            createdWorkStream(
                            ) const -> tt3::ws::WorkStream
        {
            return _createdWorkStream;
        }

        //////////
        //  Implementation
    private:
        tt3::ws::Workspace  _workspace;
        const tt3::ws::Credentials  _credentials;
        tt3::ws::Validator::WorkStream *const   _validator;

        tt3::ws::WorkStream _createdWorkStream;

        //  Helpers
        auto            _selectedBeneficiaries(
                            ) -> tt3::ws::Beneficiaries;
        void            _setSelectedBeneficiaries(
                                tt3::ws::Beneficiaries beneficiaries
                            );
        void            _refresh();

        //////////
        //  Controls
    private:
        Ui::CreateWorkStreamDialog *const  _ui;
        //  Drawing resources
        ListWidgetDecorations   _listWidgetDecorations;

        //////////
        //  Signal handlers
    private slots:
        void            _displayNameLineEditTextChanged(QString);
        void            _descriptionPlainTextEditTextChanged();
        void            _selectBeneficiariesPushButtonClicked();
        virtual void    accept() override;
        virtual void    reject() override;
    };
}

//  End of tt3-gui/CreateWorkStreamDialog.hpp
