//
//  tt3-gui/ModifyUserDialog.hpp - The modal "Modify user" dialog
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
    namespace Ui { class ModifyUserDialog; }

    /// \class ModifyUserDialog "tt3-gui/API.hpp"
    /// \brief
    ///     The modal "Modify user" dialog
    class TT3_GUI_PUBLIC ModifyUserDialog final
        :   private QDialog
    {
        Q_OBJECT
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(ModifyUserDialog)

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
        /// @param user
        ///     The user to modify.
        /// @param credentials
        ///     The credentials to use for accessing User's data.
        /// @exception WorkspaceException
        ///     If an error occurs retrieving User details.
        ModifyUserDialog(
                QWidget * parent,
                tt3::ws::User user,
                const tt3::ws::Credentials & credentials
            );

        /// \brief
        ///     The class destructor.
        virtual ~ModifyUserDialog();

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
        tt3::ws::User       _user;
        const tt3::ws::Credentials      _credentials;
        tt3::ws::Validator::User *const _validator;
        const bool          _readOnly;
        QList<QLocale>      _locales;   //  parallel to combo box items

        //  Helpers
        static QString      _displayName(const QLocale & locale);
        QStringList         _selectedEmailAddresses();
        void                _setSelectedEmailAddresses(const QStringList & emailAddresses);
        QString             _selectedEmailAddress();
        void                _setSelectedEmailAddress(const QString & emailAddress);
        tt3::ws::InactivityTimeout  _selectedInactivityTimeout();
        void                _setSelectedInactivityTimeout(const tt3::ws::InactivityTimeout & inactivityTimeout);
        tt3::ws::UiLocale   _selectedUiLocale();
        void                _setSelectedUiLocale(const tt3::ws::UiLocale & uiLocale);
        void                _refresh();

        //////////
        //  Controls
    private:
        Ui::ModifyUserDialog *  _ui;

        //////////
        //  Signal handlers
    private slots:
        void            _realNameLineEditTextChanged(QString);
        void            _emailAddressesListWidgetCurrentRowChanged(int);
        void            _addEmailAddressPushButtonClicked();
        void            _modifyEmailAddressPushButtonClicked();
        void            _removeEmailAddressPushButtonClicked();
        void            _inactivityTimeoutCheckBoxStateChanged(int);
        void            _hoursComboBoxCurrentIndexChanged(int);
        void            _minutesComboBoxCurrentIndexChanged(int);
        void            _uiLocaleComboBoxCurrentIndexChanged(int);
        void            _workingOnPushButtonClicked();
        void            accept() override;
        void            reject() override;
    };
}

//  End of tt3-gui/ModifyUserDialog.hpp

