//
//  tt3-gui/CreateAccountDialog.hpp - The modal "Create account" dialog
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
    namespace Ui { class CreateAccountDialog; }

    /// \class CreateAccountDialog tt3-gui/API.hpp
    /// \brief The modal "Create account" dialog.
    class TT3_GUI_PUBLIC CreateAccountDialog final
        :   private QDialog
    {
        Q_OBJECT
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(CreateAccountDialog)

        //////////
        //  Types
    public:
        /// \brief
        ///     The dialog result after a modal invocation
        enum class Result
        {
            Ok,     ///< Changes confirmed; the Account has been created.
            Cancel  ///< The user has cancelled the dialog.
        };

        //////////
        //  Construction/destruction
    public:
        /// \brief
        ///     Constructs the dialog.
        /// \param parent
        ///     The parent widget for the dialog; nullptr == none.
        /// \param user
        ///     The User to create a new Account for.
        /// \param credentials
        ///     The credentials to use for data access.
        /// \exception WorkspaceException
        ///     If a data access error occurs.
        CreateAccountDialog(
                QWidget * parent,
                tt3::ws::User user,
                const tt3::ws::Credentials & credentials
            );

        /// \brief
        ///     The class destructor.
        virtual ~CreateAccountDialog();

        //////////
        //  Operations
    public:
        /// \brief
        ///     Runs the dialog modally.
        /// \return
        ///     The user's choice; on OK the new Account has been created.
        Result                  doModal();

        /// \brief
        ///     Returns the newly created Account.
        /// \return
        ///     The newly created account (on Ok) or nullptr
        ///     if the dialog was cancelled.
        tt3::ws::Account        createdAccount() const { return _createdAccount; }

        //////////
        //  Implementation
    private:
        tt3::ws::User           _user;
        tt3::ws::Credentials    _credentials;
        tt3::ws::Validator::Account  *const _validator;

        tt3::ws::Account        _createdAccount;

        //  Helpers
        QStringList         _selectedEmailAddresses();
        void                _setSelectedEmailAddresses(const QStringList & emailAddresses);
        QString             _selectedEmailAddress();
        void                _setSelectedEmailAddress(const QString & emailAddress);
        tt3::ws::Capabilities   _selectedCapabilities();
        void                _refresh();

        //////////
        //  Controls
    private:
        Ui::CreateAccountDialog *const  _ui;

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

//  End of tt3-gui/CreateAccountDialog.hpp
