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
    //  The modal "Create user" dialog
    namespace Ui { class CreateUserDialog; }

    class TT3_GUI_PUBLIC CreateUserDialog final : private QDialog
    {
        Q_OBJECT
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(CreateUserDialog)

        //////////
        //  Types
    public:
        //  The dialog result after a modal invocation
        enum class Result { Ok, Cancel };

        //////////
        //  Construction/destruction
    public:
        CreateUserDialog(QWidget * parent,
                         tt3::ws::Workspace workspace, const tt3::ws::Credentials & credentials );
        virtual ~CreateUserDialog();

        //////////
        //  Operations
    public:
        //  Runs the dialog modally, returning user's choice
        Result              doModal();

        //  The newly created user (on Ok) or nullptr
        //  if the dialog was cancelled
        tt3::ws::User   createdUser() const { return _createdUser; }

        //////////
        //  Implementation
    private:
        tt3::ws::Workspace  _workspace;
        const tt3::ws::Credentials  _credentials;
        QList<QLocale>      _locales;   //  parallel to combo box items

        tt3::ws::User       _createdUser;

        //  Helpers
        static QString      _displayName(const QLocale & locale);
        void                _refresh();

        //////////
        //  Controls
    private:
        Ui::CreateUserDialog *  _ui;

        //////////
        //  Signal handlers
    private slots:
        void            _accept();
        void            _reject();
    };
}

//  End of tt3-gui/CreateUserDialog.hpp

