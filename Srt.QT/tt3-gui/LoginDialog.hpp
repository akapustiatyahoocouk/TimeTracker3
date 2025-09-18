//
//  tt3-gui/LoginDialog.hpp - The modal "Login" dialog
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
    namespace Ui { class LoginDialog; }

    /// \class LoginDialog tt3-gui/API.hpp
    /// \brief The modal "login" dialog.
    class TT3_GUI_PUBLIC LoginDialog final
        :   private QDialog
    {
        Q_OBJECT
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(LoginDialog)

        //////////
        //  Types
    public:
        /// \brief
        ///     The dialog result after a modal invocation.
        enum class Result
        {
            Ok,     ///<Login details confirmed by the user.
            Cancel  ///< The user has cancelled the dialog.
        };

        //////////
        //  Construction/destruction
    public:
        /// \brief
        ///     Constructs the dialog.
        /// \param parent
        ///     The parent widget for the dialog; nullptr == none.
        /// \param login
        ///     Thr login to display initially.
        LoginDialog(
                QWidget * parent,
                const QString & login
            );

        /// \brief
        ///     The class destructor.
        virtual ~LoginDialog();

        //////////
        //  Operations
    public:
        /// \brief
        ///     Runs the dialog modally.
        /// \return
        ///     the user's choice.
        Result          doModal();

        /// \brief
        ///     Returns the credentials entered by the user.
        /// \return
        ///     The credentials entered by the user.
        auto            credentials(
                            ) const -> tt3::ws::Credentials
        {
            return _credentials;
        }

        //////////
        //  Implementation
    private:
        tt3::ws::Credentials    _credentials;

        //  Helpers
        void            _refresh();

        //////////
        //  Controls
    private:
        Ui::LoginDialog *       _ui;

        //////////
        //  Event handlers
    private slots:
        void            _loginLineEditTextChanged(QString);
        void            _passwordLineEditTextChanged(QString);
        virtual void    accept() override;
        virtual void    reject() override;
    };
}

//  End of tt3-gui/LoginDialog.hpp

