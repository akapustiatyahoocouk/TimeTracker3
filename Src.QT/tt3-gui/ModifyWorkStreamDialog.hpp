//
//  tt3-gui/ModifyWorkStreamDialog.hpp - The modal "Modify work stream" dialog
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
    namespace Ui { class ModifyWorkStreamDialog; }

    /// \class ModifyWorkStreamDialog tt3-gui/API.hpp
    /// \brief The modal "Modify work stream" dialog
    class TT3_GUI_PUBLIC ModifyWorkStreamDialog final
        :   private QDialog
    {
        Q_OBJECT
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(ModifyWorkStreamDialog)

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
        /// @param workStream
        ///     The work stream to modify.
        /// @param credentials
        ///     The credentials to use for accessing User's data.
        /// @exception WorkspaceException
        ///     If an error occurs retrieving WorkStream details.
        ModifyWorkStreamDialog(
                QWidget * parent,
                tt3::ws::WorkStream workStream,
                const tt3::ws::Credentials & credentials
            );

        /// \brief
        ///     The class destructor.
        virtual ~ModifyWorkStreamDialog();

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
        tt3::ws::WorkStream _workStream;
        const tt3::ws::Credentials  _credentials;
        tt3::ws::Validator::WorkStream *const _validator;
        const bool      _readOnly;

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
        Ui::ModifyWorkStreamDialog *const   _ui;
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

//  End of tt3-gui/ModifyWorkStreamDialog.hpp

