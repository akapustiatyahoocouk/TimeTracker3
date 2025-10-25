//
//  tt3-gui/CreateUserDialog.hpp - The modal "Create user" dialog
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
    namespace Ui { class CreateUserDialog; }

    /// \class CreateUserDialog tt3-gui/API.hpp
    /// \brief The modal "Create user" dialog
    class TT3_GUI_PUBLIC CreateUserDialog final
        :   private QDialog
    {
        Q_OBJECT
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(CreateUserDialog)

        //////////
        //  Types
    public:
        /// \brief
        ///     The dialog result after a modal invocation.
        enum class Result
        {
            Ok,     ///< Changes confirme; User has been created.
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
        ///     The workspace to create a new User in.
        /// \param credentials
        ///     The credentials to use for data access.
        /// \exception WorkspaceException
        ///     If a data access error occurs.
        CreateUserDialog(
                QWidget * parent,
                tt3::ws::Workspace workspace,
                const tt3::ws::Credentials & credentials
            );

        /// \brief
        ///     The class destructor.
        virtual ~CreateUserDialog();

        //////////
        //  Operations
    public:
        /// \brief
        ///     Runs the dialog modally.
        /// \return
        ///     The user's choice; on OK a new User has been created.
        Result              doModal();

        /// \brief
        ///     Returns the newly created user.
        /// \return
        ///     The newly created user (on Ok) or nullptr
        ///     if the dialog was cancelled.
        tt3::ws::User       createdUser() const { return _createdUser; }

        //////////
        //  Implementation
    private:
        tt3::ws::Workspace  _workspace;
        const tt3::ws::Credentials  _credentials;
        QList<QLocale>      _locales;   //  parallel to combo box items
        tt3::ws::Validator::User *const _validator;

        tt3::ws::User       _createdUser;

        //  Helpers
        QStringList     _selectedEmailAddresses();
        void            _setSelectedEmailAddresses(const QStringList & emailAddresses);
        QString         _selectedEmailAddress();
        void            _setSelectedEmailAddress(const QString & emailAddress);
        auto            _selectedInactivityTimeout(
                            ) -> tt3::ws::InactivityTimeout;
        auto            _selectedUiLocale(
                            ) -> tt3::ws::UiLocale;
        auto            _selectedWorkloads(
                            ) -> tt3::ws::Workloads;
        void            _setSelectedWorkloads(
                                const tt3::ws::Workloads & workloads
                            );
        void            _refresh();

        //////////
        //  Controls
    private:
        Ui::CreateUserDialog *const _ui;
        //  Drawing resources
        ListWidgetDecorations   _listWidgetDecorations;

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

//  End of tt3-gui/CreateUserDialog.hpp

