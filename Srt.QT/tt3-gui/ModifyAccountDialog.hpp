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
    //  The modal "Create user" dialog
    namespace Ui { class ModifyAccountDialog; }

    class TT3_GUI_PUBLIC ModifyAccountDialog final : private QDialog
    {
        Q_OBJECT
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(ModifyAccountDialog)

        //////////
        //  Types
    public:
        //  The dialog result after a modal invocation
        enum class Result { Ok, Cancel };

        //////////
        //  Construction/destruction
    public:
        ModifyAccountDialog(QWidget * parent,
                            tt3::ws::Account account,
                            const tt3::ws::Credentials & credentials)
            throws(tt3::ws::WorkspaceClosedException);
        virtual ~ModifyAccountDialog();

        //////////
        //  Operations
    public:
        //  Runs the dialog modally, returning user's choice
        Result              doModal();

        //////////
        //  Implementation
    private:
        tt3::ws::Account        _account;
        tt3::ws::Credentials    _credentials;
        tt3::ws::Validator::Account  *const _validator;

        QString             _oldPasswordHash;   //  used to determine if password was changed

        //  Helpers
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
        Ui::ModifyAccountDialog *   _ui;

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
