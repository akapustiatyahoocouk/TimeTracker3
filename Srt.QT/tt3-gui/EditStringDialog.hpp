//
//  tt3-gui/EditStringDialog.hpp - The modal "Edit string" dialog
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
    //  The modal "Edit string" dialog
    namespace Ui { class EditStringDialog; }

    class TT3_GUI_PUBLIC EditStringDialog : private QDialog
    {
        Q_OBJECT
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(EditStringDialog)

        //////////
        //  Types
    public:
        //  The dialog result after a modal invocation
        enum class Result { Ok, Cancel };

        //  The validator is a string predicate
        using Validator = std::function<bool(QString)>;

        //////////
        //  Construction/destruction
    public:
        EditStringDialog(QWidget * parent,
                         const QIcon & icon, const QString & title, const QString & prompt,
                         const QString & initialValue, Validator validator = nullptr);
        virtual ~EditStringDialog();

        //////////
        //  Operations
    public:
        //  Runs the dialog modally, returning user's choice
        Result              doModal();

        //  The string as modified by the user
        QString             editedValue() const { return _editedValue; }

        //////////
        //  Implementation
    private:
        Validator           _validator;
        QString             _editedValue;

        //  Helpers
        void                _refresh();

        //////////
        //  Controls
    private:
        Ui::EditStringDialog *  _ui;

        //////////
        //  Signal handlers
    private slots:
        void            _lineEditTextChanged(QString);
        void            accept() override;
        void            reject() override;
    };

    //  The "Add e-mail address" modal dialog
    class TT3_GUI_PUBLIC AddEmailAddressDialog final : public EditStringDialog
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(AddEmailAddressDialog)

        //////////
        //  Construction/destruction
    public:
        AddEmailAddressDialog(::QWidget * parent, Validator validator);
    };

    //  The "Modify e-mail address" modal dialog
    class TT3_GUI_PUBLIC ModifyEmailAddressDialog final : public EditStringDialog
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(ModifyEmailAddressDialog)

        //////////
        //  Construction/destruction
    public:
        ModifyEmailAddressDialog(::QWidget * parent, const QString & value, Validator validator);
    };
}

//  End of tt3-gui/EditStringDialog.hpp

