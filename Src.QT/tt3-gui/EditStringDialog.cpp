//
//  tt3-gui/EditStringDialog.cpp - tt3::gui::EditStringDialog class implementation (+specializations)
//  TODO translate UI via Resources
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

//////////
//  Construction/destruction
EditStringDialog::EditStringDialog(
        QWidget * parent,
        const QIcon & icon,
        const QString & title,
        const QString & prompt,
        const QString & initialValue,
        Validator validator
    ) : QDialog(parent),
        _validator(validator),
        _ui(new Ui::EditStringDialog)
{
    static Component::Resources *const resources = Component::Resources::instance(); //  idempotent

    _ui->setupUi(this);
    this->setWindowIcon(icon);
    this->setWindowTitle(title);

    //  Set initial control values
    _ui->promptLabel->setText(prompt);
    _ui->lineEdit->setText(initialValue);

    _ui->buttonBox->button(QDialogButtonBox::StandardButton::Ok)->
        setText(resources->string(RSID(EditStringDialog), RID(OkPushButton)));
    _ui->buttonBox->button(QDialogButtonBox::StandardButton::Ok)->
        setIcon(QIcon(":/tt3-gui/Resources/Images/Actions/OkSmall.png"));
    _ui->buttonBox->button(QDialogButtonBox::StandardButton::Cancel)->
        setText(resources->string(RSID(EditStringDialog), RID(CancelPushButton)));
    _ui->buttonBox->button(QDialogButtonBox::StandardButton::Cancel)->
        setIcon(QIcon(":/tt3-gui/Resources/Images/Actions/CancelSmall.png"));

    //  Adjust control states
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

//  End of tt3-gui/EditStringDialog.cpp
