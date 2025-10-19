//
//  tt3-gui/ModifyWorkStreamDialog.cpp - tt3::gui::ModifyWorkStreamDialog class implementation
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
#include "ui_ModifyWorkStreamDialog.h"
using namespace tt3::gui;

//////////
//  Construction/destruction
ModifyWorkStreamDialog::ModifyWorkStreamDialog(
        QWidget * parent,
        tt3::ws::WorkStream workStream,
        const tt3::ws::Credentials & credentials
    ) : QDialog(parent),
        //  Implementation
        _workStream(workStream),
        _credentials(credentials),
        _validator(workStream->workspace()->validator()->workStream()),
        _readOnly(workStream == nullptr ||
                  !workStream->canModify(credentials) ||  //  may throw
                  workStream->workspace()->isReadOnly()),
        //  Controls
        _ui(new Ui::ModifyWorkStreamDialog)
{
    Q_ASSERT(_workStream != nullptr);
    Q_ASSERT(_credentials.isValid());

    _ui->setupUi(this);

    _ui->buttonBox->button(QDialogButtonBox::StandardButton::Ok)->
        setIcon(QIcon(":/tt3-gui/Resources/Images/Actions/OkSmall.png"));
    _ui->buttonBox->button(QDialogButtonBox::StandardButton::Cancel)->
        setIcon(QIcon(":/tt3-gui/Resources/Images/Actions/CancelSmall.png"));

    //  Set initial control values (may throw)
    _ui->displayNameLineEdit->setText(_workStream->displayName(_credentials));
    _ui->descriptionPlainTextEdit->setPlainText(_workStream->description(_credentials));
    //  TODO Beneficiaries

    //  Adjust for "view only" mode
    if (_readOnly)
    {
        this->setWindowTitle("View work stream");
        this->setWindowIcon(QIcon(":/tt3-gui/Resources/Images/Actions/ViewWorkStreamLarge.png"));
        _ui->displayNameLineEdit->setReadOnly(true);
        _ui->descriptionPlainTextEdit->setReadOnly(true);
    }

    //  Done
    _refresh();
    adjustSize();
    _ui->displayNameLineEdit->setFocus();
}

ModifyWorkStreamDialog::~ModifyWorkStreamDialog()
{
    delete _ui;
}

//////////
//  Operations
ModifyWorkStreamDialog::Result ModifyWorkStreamDialog::doModal()
{
    return Result(this->exec());
}

//////////
//  Implementation helpers
void ModifyWorkStreamDialog::_refresh()
{
    _ui->selectBeneficiariesPushButton->setEnabled(!_readOnly);
    _ui->buttonBox->button(QDialogButtonBox::StandardButton::Ok)->setEnabled(
        !_readOnly &&
        _validator->isValidDisplayName(_ui->displayNameLineEdit->text()) &&
        _validator->isValidDescription(_ui->descriptionPlainTextEdit->toPlainText()));
}

//////////
//  Signal handlers
void ModifyWorkStreamDialog::_displayNameLineEditTextChanged(QString)
{
    _refresh();
}

void ModifyWorkStreamDialog::_descriptionPlainTextEditTextChanged()
{
    _refresh();
}

void ModifyWorkStreamDialog::_selectBeneficiariesPushButtonClicked()
{
    ErrorDialog::show(this, "Not yet implemented");
}

void ModifyWorkStreamDialog::accept()
{
    try
    {   //  Any of the setters may throw
        if (!_readOnly)
        {
            _workStream->setDisplayName(
                _credentials,
                _ui->displayNameLineEdit->text());
            _workStream->setDescription(
                _credentials,
                _ui->descriptionPlainTextEdit->toPlainText());
            //  TODO Beneficiaries
        }
        done(int(Result::Ok));
    }
    catch (const tt3::util::Exception & ex)
    {
        qCritical() << ex;
        ErrorDialog::show(this, ex);
    }
}

void ModifyWorkStreamDialog::reject()
{
    done(int(Result::Cancel));
}

//  End of tt3-gui/ModifyWorkStreamDialog.cpp
