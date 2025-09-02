//
//  tt3-gui/EditStringDialog.cpp - tt3::gui::EditStringDialog class implementation (+specializations)
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
#include "tt3-gui/API.hpp"
#include "ui_EditStringDialog.h"
using namespace tt3::gui;

EditStringDialog::EditStringDialog(QWidget * parent,
                                   const QIcon & icon, const QString & title, const QString & prompt,
                                   const QString & initialValue, Validator validator)
    :   QDialog(parent),
        _validator(validator),
        _ui(new Ui::EditStringDialog)
{
    _ui->setupUi(this);

    _ui->buttonBox->button(QDialogButtonBox::StandardButton::Ok)->
        setIcon(QIcon(":/tt3-gui/Resources/Images/Actions/OkSmall.png"));
    _ui->buttonBox->button(QDialogButtonBox::StandardButton::Cancel)->
        setIcon(QIcon(":/tt3-gui/Resources/Images/Actions/CancelSmall.png"));

    this->setWindowIcon(icon);
    this->setWindowTitle(title);
    _ui->promptLabel->setText(prompt);
    _ui->lineEdit->setText(initialValue);

    if (!initialValue.isEmpty())
    {
        _ui->lineEdit->selectAll();
    }

    //  Done
    adjustSize();
    _refresh();
}

EditStringDialog::~EditStringDialog()
{
    delete _ui;
}

//////////
//  Operations
EditStringDialog::Result EditStringDialog::doModal()
{
    return Result(this->exec());
}

//////////
//  Implementation helpers
void EditStringDialog::_refresh()
{
    _ui->buttonBox->button(QDialogButtonBox::StandardButton::Ok)->
        setEnabled((_validator == nullptr) ||
                   _validator(_ui->lineEdit->text()));
}

//////////
//  Signal handlers
void EditStringDialog::_lineEditTextChanged(QString)
{
    _refresh();
}

void EditStringDialog::accept()
{
    _editedValue = _ui->lineEdit->text();
    done(int(Result::Ok));
}

void EditStringDialog::reject()
{
    done(int(Result::Cancel));
}

//////////
//  AddEmailAddressDialog
AddEmailAddressDialog::AddEmailAddressDialog(::QWidget * parent, Validator validator)
    :   EditStringDialog(parent,
                         QIcon(":/tt3-gui/Resources/Images/Actions/AddEmailAddressLarge.png"),
                         "Add e-mail address",
                         "Enter the e-mail address",
                         "",
                         validator)
{
    Q_ASSERT(validator != nullptr);
}

//////////
//  ModifyEmailAddressDialog
ModifyEmailAddressDialog::ModifyEmailAddressDialog(::QWidget * parent, const QString & value, Validator validator)
    :   EditStringDialog(parent,
                       QIcon(":/tt3-gui/Resources/Images/Actions/AddEmailAddressLarge.png"),
                       "Modify e-mail address",
                       "Enter the e-mail address",
                       value,
                       validator)
{
    Q_ASSERT(validator != nullptr);
}

//  End of tt3-gui/EditStringDialog.cpp
