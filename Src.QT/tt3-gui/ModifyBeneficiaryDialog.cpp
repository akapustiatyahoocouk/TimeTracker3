//
//  tt3-gui/ModifyBeneficiaryDialog.cpp - tt3::gui::ModifyBeneficiaryDialog class implementation
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
#include "ui_ModifyBeneficiaryDialog.h"
using namespace tt3::gui;

//////////
//  Construction/destruction
ModifyBeneficiaryDialog::ModifyBeneficiaryDialog(
        QWidget * parent,
        tt3::ws::Beneficiary beneficiary,
        const tt3::ws::Credentials & credentials
    ) : QDialog(parent),
        //  Implementation
        _beneficiary(beneficiary),
        _credentials(credentials),
        _validator(beneficiary->workspace()->validator()->beneficiary()),
        _readOnly(beneficiary == nullptr ||
                  !beneficiary->canModify(credentials) ||  //  may throw
                  beneficiary->workspace()->isReadOnly()),
        //  Controls
        _ui(new Ui::ModifyBeneficiaryDialog)
{
    Q_ASSERT(_beneficiary != nullptr);
    Q_ASSERT(_credentials.isValid());

    _ui->setupUi(this);

    _ui->buttonBox->button(QDialogButtonBox::StandardButton::Ok)->
        setIcon(QIcon(":/tt3-gui/Resources/Images/Actions/OkSmall.png"));
    _ui->buttonBox->button(QDialogButtonBox::StandardButton::Cancel)->
        setIcon(QIcon(":/tt3-gui/Resources/Images/Actions/CancelSmall.png"));

    //  Set initial control values (may throw)
    _ui->displayNameLineEdit->setText(_beneficiary->displayName(_credentials));
    _ui->descriptionPlainTextEdit->setPlainText(_beneficiary->description(_credentials));
    _setSelectedWorkloads(_beneficiary->workloads(_credentials));

    //  Adjust for "view only" mode
    if (_readOnly)
    {
        this->setWindowTitle("View beneficiary");
        this->setWindowIcon(QIcon(":/tt3-gui/Resources/Images/Actions/ViewBeneficiaryLarge.png"));
        _ui->displayNameLineEdit->setReadOnly(true);
        _ui->descriptionPlainTextEdit->setReadOnly(true);
        _ui->selectWorkloadsPushButton->setEnabled(false);
    }

    //  Done
    _refresh();
    adjustSize();
    _ui->displayNameLineEdit->setFocus();
}

ModifyBeneficiaryDialog::~ModifyBeneficiaryDialog()
{
    delete _ui;
}

//////////
//  Operations
ModifyBeneficiaryDialog::Result ModifyBeneficiaryDialog::doModal()
{
    return Result(this->exec());
}

//////////
//  Implementation helpers
auto ModifyBeneficiaryDialog::_selectedWorkloads(
    ) -> tt3::ws::Workloads
{   //  TODO implement
    return tt3::ws::Workloads();
}

void ModifyBeneficiaryDialog::_setSelectedWorkloads(
        const tt3::ws::Workloads & /*workloads*/
    )
{   //  TODO implement
}

void ModifyBeneficiaryDialog::_refresh()
{
    _ui->buttonBox->button(QDialogButtonBox::StandardButton::Ok)->setEnabled(
        !_readOnly &&
        _validator->isValidDisplayName(_ui->displayNameLineEdit->text()) &&
        _validator->isValidDescription(_ui->descriptionPlainTextEdit->toPlainText()));
}

//////////
//  Signal handlers
void ModifyBeneficiaryDialog::_displayNameLineEditTextChanged(QString)
{
    _refresh();
}

void ModifyBeneficiaryDialog::_descriptionPlainTextEditTextChanged()
{
    _refresh();
}

void ModifyBeneficiaryDialog::_selectWorkloadsPushButtonClicked()
{
    ErrorDialog::show(this, "Not yet implemented");
}

void ModifyBeneficiaryDialog::accept()
{
    try
    {   //  Any of the setters may throw
        if (!_readOnly)
        {
            _beneficiary->setDisplayName(
                _credentials,
                _ui->displayNameLineEdit->text());
            _beneficiary->setDescription(
                _credentials,
                _ui->descriptionPlainTextEdit->toPlainText());
            _beneficiary->setWorkloads(
                _credentials,
                _selectedWorkloads());
        }
        done(int(Result::Ok));
    }
    catch (const tt3::util::Exception & ex)
    {
        qCritical() << ex;
        ErrorDialog::show(this, ex);
    }
}

void ModifyBeneficiaryDialog::reject()
{
    done(int(Result::Cancel));
}

//  End of tt3-gui/ModifyBeneficiaryDialog.cpp
