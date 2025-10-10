//
//  tt3-gui/ModifyAccountDialog.hpp - The modal "Modify account" dialog
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
    namespace Ui { class ModifyAccountDialog; }

    /// \class ModifyAccountDialog "tt3-gui/API.hpp"
    /// \brief The modal "Modify account" dialog.
    class TT3_GUI_PUBLIC ModifyAccountDialog final
        :   private QDialog
    {
        Q_OBJECT
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(ModifyAccountDialog)

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
        /// @param account
        ///     The account to modify.
        /// @param credentials
        ///     The credentials to use for accessing Account's data.
        /// @exception WorkspaceException
        ///     If an error occurs retrieving Account details.
        ModifyAccountDialog(
                QWidget * parent,
                tt3::ws::Account account,
                const tt3::ws::Credentials & credentials
            );

        /// \brief
        ///     The class destructor.
        virtual ~ModifyAccountDialog();

        //////////
        //  Operations
    public:
        /// \brief
        ///     Runs the dialog modally.
        /// @return
        ///     The dialog result; Ok means "changes saved".
        Result              doModal();

        //////////
        //  Implementation
    private:
        tt3::ws::Account        _account;
        tt3::ws::Credentials    _credentials;
        tt3::ws::Validator::Account  *const _validator;
        const bool              _readOnly;

        QString             _oldPasswordHash;   //  used to determine if password was changed

        //  Helpers
        tt3::ws::User       _selectedUser();
        void                _setSelectedUser(tt3::ws::User user);
        QStringList         _selectedEmailAddresses();
        void                _setSelectedEmailAddresses(const QStringList & emailAddresses);
        QString             _selectedEmailAddress();
        void                _setSelectedEmailAddress(const QString & emailAddress);
        tt3::ws::Capabilities   _selectedCapabilities();
        void                _setSelectedCapabilities(tt3::ws::Capabilities capabilities);
        void                _refresh();

        //////////
        //  Controls
    private:
        Ui::ModifyAccountDialog *const  _ui;

        //////////
        //  Signal handlers
    private slots:
        void            _loginLineEditTextChanged(QString);
        void            _passwordLineEditTextChanged(QString);
        void            _emailAddressesListWidgetCurrentRowChanged(int);
        void            _addEmailAddressPushButtonClicked();
        void            _modifyEmailAddressPushButtonClicked();
        void            _removeEmailAddressPushButtonClicked();
        void            accept() override;
        void            reject() override;
    };
}

//  End of tt3-gui/ModifyAccountDialog.hpp
