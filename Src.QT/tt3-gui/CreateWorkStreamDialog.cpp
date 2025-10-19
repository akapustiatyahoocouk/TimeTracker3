//
//  tt3-gui/CreateWorkStreamDialog.cpp - tt3::gui::CreateWorkStreamDialog class implementation
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
#include "ui_CreateWorkStreamDialog.h"
using namespace tt3::gui;

//////////
//  Construction/destruction
CreateWorkStreamDialog::CreateWorkStreamDialog(
        QWidget * parent,
        tt3::ws::Workspace workspace,
        const tt3::ws::Credentials & credentials
    ) : QDialog(parent),
        //  Implementation
        _workspace(workspace),
        _credentials(credentials),
        _validator(workspace->validator()->workStream()),
        //  Controls
        _ui(new Ui::CreateWorkStreamDialog)
{
    Q_ASSERT(_workspace != nullptr);
    Q_ASSERT(_credentials.isValid());

    _ui->setupUi(this);

    _ui->buttonBox->button(QDialogButtonBox::StandardButton::Ok)->
        setIcon(QIcon(":/tt3-gui/Resources/Images/Actions/OkSmall.png"));
    _ui->buttonBox->button(QDialogButtonBox::StandardButton::Cancel)->
        setIcon(QIcon(":/tt3-gui/Resources/Images/Actions/CancelSmall.png"));

    //  Done
    _refresh();
    adjustSize();
    _ui->displayNameLineEdit->setFocus();
}

CreateWorkStreamDialog::~CreateWorkStreamDialog()
{
    delete _ui;
}

//////////
//  Operations
CreateWorkStreamDialog::Result CreateWorkStreamDialog::doModal()
{
    return Result(this->exec());
}

//////////
//  Implementation helpers
void CreateWorkStreamDialog::_refresh()
{
    _ui->buttonBox->button(QDialogButtonBox::StandardButton::Ok)->setEnabled(
        _validator->isValidDisplayName(_ui->displayNameLineEdit->text()) &&
        _validator->isValidDescription(_ui->descriptionPlainTextEdit->toPlainText()));
}

//////////
//  Signal handlers
void CreateWorkStreamDialog::_displayNameLineEditTextChanged(QString)
{
    _refresh();
}

void CreateWorkStreamDialog::_descriptionPlainTextEditTextChanged()
{
    _refresh();
}

void CreateWorkStreamDialog::_selectBeneficiariesPushButtonClicked()
{
    ErrorDialog::show(this, "Not yet implemented");
}

void CreateWorkStreamDialog::accept()
{
    try
    {
        _createdWorkStream =
            _workspace->createWorkStream(
                _credentials,
                _ui->displayNameLineEdit->text(),
                _ui->descriptionPlainTextEdit->toPlainText(),
                tt3::ws::Beneficiaries());  //  TODO for now
        done(int(Result::Ok));
    }
    catch (const tt3::util::Exception & ex)
    {
        qCritical() << ex.errorMessage();
        ErrorDialog::show(this, ex);
    }
}

void CreateWorkStreamDialog::reject()
{
    done(int(Result::Cancel));
}

//  End of tt3-gui/CreateWorkStreamDialog.cpp
