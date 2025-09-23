//
//  tt3-gui/CreatePublicActivityDialog.cpp - tt3::gui::CreatePublicActivityDialog class implementation
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
#include "ui_CreatePublicActivityDialog.h"
using namespace tt3::gui;

//////////
//  Construction/destruction
CreatePublicActivityDialog::CreatePublicActivityDialog(
        QWidget * parent,
        tt3::ws::Workspace workspace,
        const tt3::ws::Credentials & credentials
    ) : QDialog(parent),
        //  Implementation
        _workspace(workspace),
        _credentials(credentials),
        _validator(workspace->validator()->publicActivity()),
        //  Controls
        _ui(new Ui::CreatePublicActivityDialog)
{
    Q_ASSERT(_workspace != nullptr);
    Q_ASSERT(_credentials.isValid());

    _ui->setupUi(this);

    _ui->buttonBox->button(QDialogButtonBox::StandardButton::Ok)->
        setIcon(QIcon(":/tt3-gui/Resources/Images/Actions/OkSmall.png"));
    _ui->buttonBox->button(QDialogButtonBox::StandardButton::Cancel)->
        setIcon(QIcon(":/tt3-gui/Resources/Images/Actions/CancelSmall.png"));

    //  Fill the "activity type" combo box (may throw)
    QList<tt3::ws::ActivityType> activityTypes =
        _workspace->activityTypes(_credentials).values();
    std::sort(activityTypes.begin(),
              activityTypes.end(),
              [&](auto a, auto b)
              {
                  return a->displayName(_credentials) < b->displayName(_credentials);
              });

    _ui->activityTypeComboBox->addItem(
        "-",
        QVariant::fromValue<tt3::ws::ActivityType>(nullptr));
    for (tt3::ws::ActivityType activityType : activityTypes)
    {
        _ui->activityTypeComboBox->addItem(
            activityType->type()->smallIcon(),
            activityType->displayName(_credentials),
            QVariant::fromValue(activityType));
    }

    //  Fill "hours" amd "minutes" combo boxes
    for (int h = 0; h < 12; h++)
    {
        _ui->hoursComboBox->addItem(tt3::util::toString(h) + " hrs", QVariant::fromValue(h));
    }
    for (int m = 0; m < 60; m += 15)
    {
        _ui->minutesComboBox->addItem(tt3::util::toString(m) + " min", QVariant::fromValue(m));
    }

    //  Done
    adjustSize();
    _refresh();
}

CreatePublicActivityDialog::~CreatePublicActivityDialog()
{
    delete _ui;
}

//////////
//  Operations
CreatePublicActivityDialog::Result CreatePublicActivityDialog::doModal()
{
    return Result(this->exec());
}

//////////
//  Implementation helpers
auto CreatePublicActivityDialog::_selectedActivityType(
    ) -> tt3::ws::ActivityType
{
    return _ui->activityTypeComboBox->currentData().value<tt3::ws::ActivityType>();
}

auto CreatePublicActivityDialog::_selectedTimeout(
    ) -> tt3::ws::InactivityTimeout
{
    if (_ui->timeoutCheckBox->isChecked())
    {
        int h = _ui->hoursComboBox->currentData().value<int>();
        int m = _ui->minutesComboBox->currentData().value<int>();
        return tt3::util::TimeSpan::hours(h) + tt3::util::TimeSpan::minutes(m);
    }
    return tt3::ws::InactivityTimeout();
}

void CreatePublicActivityDialog::_refresh()
{
    _ui->hoursComboBox->setEnabled(_ui->timeoutCheckBox->isChecked());
    _ui->minutesComboBox->setEnabled(_ui->timeoutCheckBox->isChecked());
    _ui->buttonBox->button(QDialogButtonBox::StandardButton::Ok)->setEnabled(
        _validator->isValidDisplayName(_ui->displayNameLineEdit->text()) &&
        _validator->isValidDescription(_ui->descriptionTextEdit->toPlainText()) &&
        _validator->isValidTimeout(_selectedTimeout()));
}

//////////
//  Signal handlers
void CreatePublicActivityDialog::_displayNameLineEditTextChanged(QString)
{
    _refresh();
}

void CreatePublicActivityDialog::_descriptionTextEditTextChanged()
{
    _refresh();
}

void CreatePublicActivityDialog::_selectWorkloadPushButtonClicked()
{
    ErrorDialog::show(this, "Not yet implemented");
}

void CreatePublicActivityDialog::_timeoutCheckBoxStateChanged(int)
{
    _refresh();
}

void CreatePublicActivityDialog::_hoursComboBoxCurrentIndexChanged(int)
{
    _refresh();
}

void CreatePublicActivityDialog::_minutesComboBoxCurrentIndexChanged(int)
{
    _refresh();
}

void CreatePublicActivityDialog::accept()
{
    try
    {
        _createdPublicActivity = _workspace->createPublicActivity(
            _credentials,
            _ui->displayNameLineEdit->text(),
            _ui->descriptionTextEdit->toPlainText(),
            _selectedTimeout(),
            _ui->requireCommentOnStartCheckBox->isChecked(),
            _ui->requireCommentOnFinishCheckBox->isChecked(),
            _ui->fullScreenReminderCheckBox->isChecked(),
            _selectedActivityType(),
            _selectedWorkload);
        done(int(Result::Ok));
    }
    catch (const tt3::util::Exception & ex)
    {
        ErrorDialog::show(this, ex);
    }
}

void CreatePublicActivityDialog::reject()
{
    done(int(Result::Cancel));
}

//  End of tt3-gui/CreatePublicActivityDialog.cpp
