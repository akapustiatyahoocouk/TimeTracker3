//
//  tt3-gui/ModifyActivityTypeDialog.cpp - tt3::gui::ModifyActivityTypeDialog class implementation
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
#include "ui_ModifyActivityTypeDialog.h"
using namespace tt3::gui;

//////////
//  Construction/destruction
ModifyActivityTypeDialog::ModifyActivityTypeDialog(
        QWidget * parent,
        tt3::ws::ActivityType activityType,
        const tt3::ws::Credentials & credentials
    ) : QDialog(parent),
        //  Implementation
        _activityType(activityType),
        _credentials(credentials),
        _validator(activityType->workspace()->validator()->activityType()),
        _readOnly(activityType == nullptr ||
                  !activityType->canModify(credentials) ||  //  may throw
                  activityType->workspace()->isReadOnly()),
        //  Controls
        _ui(new Ui::ModifyActivityTypeDialog)
{
    Q_ASSERT(_activityType != nullptr);
    Q_ASSERT(_credentials.isValid());

    _ui->setupUi(this);

    _ui->buttonBox->button(QDialogButtonBox::StandardButton::Ok)->
        setIcon(QIcon(":/tt3-gui/Resources/Images/Actions/OkSmall.png"));
    _ui->buttonBox->button(QDialogButtonBox::StandardButton::Cancel)->
        setIcon(QIcon(":/tt3-gui/Resources/Images/Actions/CancelSmall.png"));

    //  Set initial control values (may throw)
    _ui->displayNameLineEdit->setText(_activityType->displayName(_credentials));
    _ui->descriptionPlainTextEdit->setPlainText(_activityType->description(_credentials));

    //  Adjust for "view only" mode
    if (_readOnly)
    {
        this->setWindowTitle("View activity type");
        this->setWindowIcon(QIcon(":/tt3-gui/Resources/Images/Actions/ViewActivityTypeLarge.png"));
        _ui->displayNameLineEdit->setReadOnly(true);
        _ui->descriptionPlainTextEdit->setReadOnly(true);
    }

    //  Done
    _ui->displayNameLabel->setFocus();
    adjustSize();
    _refresh();
}

ModifyActivityTypeDialog::~ModifyActivityTypeDialog()
{
    delete _ui;
}

//////////
//  Operations
ModifyActivityTypeDialog::Result ModifyActivityTypeDialog::doModal()
{
    return Result(this->exec());
}

//////////
//  Implementation helpers
void ModifyActivityTypeDialog::_refresh()
{
    _ui->buttonBox->button(QDialogButtonBox::StandardButton::Ok)->setEnabled(
        !_readOnly &&
        _validator->isValidDisplayName(_ui->displayNameLineEdit->text()) &&
        _validator->isValidDescription(_ui->descriptionPlainTextEdit->toPlainText()));
}

//////////
//  Signal handlers
void ModifyActivityTypeDialog::_displayNameLineEditTextChanged(QString)
{
    _refresh();
}

void ModifyActivityTypeDialog::_descriptionPlainTextEditTextChanged()
{
    _refresh();
}

void ModifyActivityTypeDialog::accept()
{
    try
    {   //  Any of the setters may throw
        if (!_readOnly)
        {
            _activityType->setDisplayName(
                _credentials,
                _ui->displayNameLineEdit->text());
            _activityType->setDescription(
                _credentials,
                _ui->descriptionPlainTextEdit->toPlainText());
        }
        done(int(Result::Ok));
    }
    catch (const tt3::util::Exception & ex)
    {
        ErrorDialog::show(this, ex);
    }
}

void ModifyActivityTypeDialog::reject()
{
    done(int(Result::Cancel));
}

//  End of tt3-gui/ModifyActivityTypeDialog.cpp
