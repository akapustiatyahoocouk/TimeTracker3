//
//  tt3-gui/CreateActivityTypeDialog.cpp - tt3::gui::CreateActivityTypeDialog class implementation
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
#include "ui_CreateActivityTypeDialog.h"
using namespace tt3::gui;

//////////
//  Construction/destruction
CreateActivityTypeDialog::CreateActivityTypeDialog(
        QWidget * parent,
        tt3::ws::Workspace workspace,
        const tt3::ws::Credentials & credentials
    ) : QDialog(parent),
        //  Implementation
        _workspace(workspace),
        _credentials(credentials),
        _validator(workspace->validator()->activityType()),
        //  Controls
        _ui(new Ui::CreateActivityTypeDialog)
{
    static Component::Resources *const resources = Component::Resources::instance(); //  idempotent

    Q_ASSERT(_workspace != nullptr);
    Q_ASSERT(_credentials.isValid());

    _ui->setupUi(this);
    setWindowTitle(resources->string(RSID(CreateActivityTypeDialog), RID(Title)));

    //  Set up initial control values
    _ui->displayNameLabel->setText(
        resources->string(RSID(CreateActivityTypeDialog), RID(DisplayNameLabel)));
    _ui->descriptionLabel->setText(
        resources->string(RSID(CreateActivityTypeDialog), RID(DescriptionLabel)));

    _ui->buttonBox->button(QDialogButtonBox::StandardButton::Ok)->
        setText(resources->string(RSID(CreateActivityTypeDialog), RID(OkPushButton)));
    _ui->buttonBox->button(QDialogButtonBox::StandardButton::Ok)->
        setIcon(QIcon(":/tt3-gui/Resources/Images/Actions/OkSmall.png"));
    _ui->buttonBox->button(QDialogButtonBox::StandardButton::Cancel)->
        setText(resources->string(RSID(CreateActivityTypeDialog), RID(CancelPushButton)));
    _ui->buttonBox->button(QDialogButtonBox::StandardButton::Cancel)->
        setIcon(QIcon(":/tt3-gui/Resources/Images/Actions/CancelSmall.png"));

    //  Done
    adjustSize();
    _refresh();
}

CreateActivityTypeDialog::~CreateActivityTypeDialog()
{
    delete _ui;
}

//////////
//  Operations
CreateActivityTypeDialog::Result CreateActivityTypeDialog::doModal()
{
    return Result(this->exec());
}

//////////
//  Implementation helpers
void CreateActivityTypeDialog::_refresh()
{
    _ui->buttonBox->button(QDialogButtonBox::StandardButton::Ok)->setEnabled(
        _validator->isValidDisplayName(_ui->displayNameLineEdit->text()) &&
        _validator->isValidDescription(_ui->descriptionPlainTextEdit->toPlainText()));
}

//////////
//  Signal handlers
void CreateActivityTypeDialog::_displayNameLineEditTextChanged(QString)
{
    _refresh();
}

void CreateActivityTypeDialog::_descriptionPlainTextEditTextChanged()
{
    _refresh();
}

void CreateActivityTypeDialog::accept()
{
    try
    {
        _createdActivityType =
            _workspace->createActivityType(
                _credentials,
                _ui->displayNameLineEdit->text(),
                _ui->descriptionPlainTextEdit->toPlainText());
        done(int(Result::Ok));
    }
    catch (const tt3::util::Exception & ex)
    {
        ErrorDialog::show(this, ex);
    }
}

void CreateActivityTypeDialog::reject()
{
    done(int(Result::Cancel));
}

//  End of tt3-gui/CreateActivityTypeDialog.cpp
