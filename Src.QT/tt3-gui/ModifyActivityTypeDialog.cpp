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
    tt3::util::ResourceReader rr(Component::Resources::instance(), RSID(ModifyActivityTypeDialog));

    Q_ASSERT(_activityType != nullptr);
    Q_ASSERT(_credentials.isValid());

    _ui->setupUi(this);
    setWindowTitle(rr.string(RID(Title)));

    //  Set static control values
    _ui->displayNameLabel->setText(
        rr.string(RID(DisplayNameLabel)));
    _ui->descriptionLabel->setText(
        rr.string(RID(DescriptionLabel)));

    _ui->buttonBox->button(QDialogButtonBox::StandardButton::Ok)->
        setText(rr.string(RID(OkPushButton)));
    _ui->buttonBox->button(QDialogButtonBox::StandardButton::Ok)->
        setIcon(QIcon(":/tt3-gui/Resources/Images/Actions/OkSmall.png"));
    _ui->buttonBox->button(QDialogButtonBox::StandardButton::Cancel)->
        setText(rr.string(RID(CancelPushButton)));
    _ui->buttonBox->button(QDialogButtonBox::StandardButton::Cancel)->
        setIcon(QIcon(":/tt3-gui/Resources/Images/Actions/CancelSmall.png"));

    //  Set editable control values (may throw)
    _ui->displayNameLineEdit->setText(
        _activityType->displayName(_credentials));
    _ui->descriptionPlainTextEdit->setPlainText(
        _activityType->description(_credentials));

    //  Adjust controls
    if (_readOnly)
    {   //  Adjust for "view only" mode
        setWindowTitle(rr.string(RID(ViewOnlyTitle)));
        setWindowIcon(QIcon(":/tt3-gui/Resources/Images/Actions/ViewActivityTypeLarge.png"));
        _ui->displayNameLineEdit->setReadOnly(true);
        _ui->descriptionPlainTextEdit->setReadOnly(true);
    }

    //  Done
    _refresh();
    adjustSize();
    _ui->displayNameLineEdit->setFocus();
}

ModifyActivityTypeDialog::~ModifyActivityTypeDialog()
{
    delete _ui;
}

//////////
//  Operations
auto ModifyActivityTypeDialog::doModal(
    ) -> Result
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
        qCritical() << ex;
        ErrorDialog::show(this, ex);
    }
}

void ModifyActivityTypeDialog::reject()
{
    done(int(Result::Cancel));
}

//  End of tt3-gui/ModifyActivityTypeDialog.cpp
