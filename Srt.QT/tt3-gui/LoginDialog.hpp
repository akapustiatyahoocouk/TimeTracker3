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
    //  The modal "login" dialog
    namespace Ui { class LoginDialog; }

    class TT3_GUI_PUBLIC LoginDialog final : private QDialog
    {
        Q_OBJECT
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(LoginDialog)

        //////////
        //  Types
    public:
        //  The dialog result after a modal invocation
        enum class Result { Ok, Cancel };

        //////////
        //  Construction/destruction
    public:
        LoginDialog(QWidget * parent, const QString & login);
        virtual ~LoginDialog();

        //////////
        //  Operations
    public:
        //  Runs the dialog modally, returning user's choice
        Result                  doModal();

        //  The credentials entered by the user
        tt3::ws::Credentials    credentials() const { return _credentials; }

        //////////
        //  Implementation
    private:
        tt3::ws::Credentials    _credentials;

        //  Helpers
        void                    _refresh();

        //////////
        //  Controls
    private:
        Ui::LoginDialog *       _ui;

        //////////
        //  Event handlers
    private slots:
        void                    _loginLineEditTextChanged(QString);
        void                    _passwordLineEditTextChanged(QString);
        void                    _accept();
        void                    _reject();
    };
}

//  End of tt3-gui/LoginDialog.hpp

