//
//  tt3-gui/ModifyPublicActivityDialog.cpp - tt3::gui::ModifyPublicActivityDialog class implementation
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
#include "ui_ModifyPublicActivityDialog.h"
using namespace tt3::gui;

//////////
//  Construction/destruction
ModifyPublicActivityDialog::ModifyPublicActivityDialog(
        QWidget * parent,
        tt3::ws::PublicActivity publicActivity,
        const tt3::ws::Credentials & credentials
    ) : QDialog(parent),
        //  Implementation
        _publicActivity(publicActivity),
        _credentials(credentials),
        _validator(publicActivity->workspace()->validator()->publicActivity()),
        _readOnly(publicActivity == nullptr ||
                  !publicActivity->canModify(credentials) ||  //  may throw
                  publicActivity->workspace()->isReadOnly()),
        //  Controls
        _ui(new Ui::ModifyPublicActivityDialog)
{
    Q_ASSERT(_publicActivity != nullptr);
    Q_ASSERT(_credentials.isValid());

    _ui->setupUi(this);

    _ui->buttonBox->button(QDialogButtonBox::StandardButton::Ok)->
        setIcon(QIcon(":/tt3-gui/Resources/Images/Actions/OkSmall.png"));
    _ui->buttonBox->button(QDialogButtonBox::StandardButton::Cancel)->
        setIcon(QIcon(":/tt3-gui/Resources/Images/Actions/CancelSmall.png"));

    //  Fill the "activity type" combo box (may throw)
    QList<tt3::ws::ActivityType> activityTypes =
        _publicActivity->workspace()->activityTypes(_credentials).values();
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

    //  Set initial control values (may throw)
    _ui->displayNameLineEdit->setText(_publicActivity->displayName(_credentials));
    _ui->descriptionPlainTextEdit->setPlainText(_publicActivity->description(_credentials));
    _setSelectedActivityType(_publicActivity->activityType(_credentials));
    _setSelectedWorkload(_publicActivity->workload(_credentials));
    _setSelectedTimeout(_publicActivity->timeout(_credentials));
    _ui->requireCommentOnStartCheckBox->setChecked(_publicActivity->requireCommentOnStart(_credentials));
    _ui->requireCommentOnStopCheckBox->setChecked(_publicActivity->requireCommentOnStop(_credentials));
    _ui->fullScreenReminderCheckBox->setChecked(_publicActivity->fullScreenReminder(_credentials));

    //  Adjust for "view only" mode
    if (_readOnly)
    {
        this->setWindowTitle("View public activity");
        this->setWindowIcon(QIcon(":/tt3-gui/Resources/Images/Actions/ViewPublicActivityLarge.png"));
        _ui->displayNameLineEdit->setReadOnly(true);
        _ui->descriptionPlainTextEdit->setReadOnly(true);
        _ui->activityTypeComboBox->setEnabled(false);
        _ui->selectWorkloadPushButton->setEnabled(false);
        _ui->timeoutCheckBox->setEnabled(false);
        _ui->hoursComboBox->setEnabled(false);
        _ui->minutesComboBox->setEnabled(false);
        _ui->requireCommentOnStartCheckBox->setEnabled(false);
        _ui->requireCommentOnStopCheckBox->setEnabled(false);
        _ui->fullScreenReminderCheckBox->setEnabled(false);
    }

    //  Done
    adjustSize();
    _refresh();
}

ModifyPublicActivityDialog::~ModifyPublicActivityDialog()
{
    delete _ui;
}

//////////
//  Operations
auto ModifyPublicActivityDialog::doModal(
    ) -> ModifyPublicActivityDialog::Result
{
    return Result(this->exec());
}

//////////
//  Implementation helpers
auto ModifyPublicActivityDialog::_selectedActivityType(
    ) -> tt3::ws::ActivityType
{
    return _ui->activityTypeComboBox->currentData().value<tt3::ws::ActivityType>();
}

void ModifyPublicActivityDialog::_setSelectedActivityType(
        tt3::ws::ActivityType activityType
    )
{
    for (int i = 0; i < _ui->activityTypeComboBox->count(); i++)
    {
        if (activityType == _ui->activityTypeComboBox->itemData(i).value<tt3::ws::ActivityType>())
        {
            _ui->activityTypeComboBox->setCurrentIndex(i);
            break;
        }
    }
}

void ModifyPublicActivityDialog::_setSelectedWorkload(
        tt3::ws::Workload workload
    )
{   //  TODO implement properly
    Q_ASSERT(workload == nullptr);
    _ui->workloadValueLabel->setText("-");
    _selectedWorkload = workload;
}

auto ModifyPublicActivityDialog::_selectedTimeout(
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

void ModifyPublicActivityDialog::_setSelectedTimeout(
        const tt3::ws::InactivityTimeout & timeout
    )
{
    _ui->timeoutCheckBox->setChecked(false);
    if (!timeout.has_value())
    {   //  No timeout is a special case
        _ui->hoursComboBox->setCurrentIndex(0);
        _ui->minutesComboBox->setCurrentIndex(0);
        _refresh();
        return;
    }
    for (int i = 0; i < _ui->hoursComboBox->count(); i++)
    {
        for (int j = 0; j < _ui->minutesComboBox->count(); j++)
        {
            int h = _ui->hoursComboBox->itemData(i).value<int>();
            int m = _ui->minutesComboBox->itemData(j).value<int>();
            if (tt3::util::TimeSpan::hours(h) + tt3::util::TimeSpan::minutes(m) == timeout)
            {   //  These ones!
                _ui->timeoutCheckBox->setChecked(true);
                _ui->hoursComboBox->setCurrentIndex(i);
                _ui->minutesComboBox->setCurrentIndex(j);
                _refresh();
                return;
            }
        }
    }
    //  If we're here, there was no h+m match
    _ui->hoursComboBox->setCurrentIndex(0);
    _ui->minutesComboBox->setCurrentIndex(0);
    _refresh();
}

void ModifyPublicActivityDialog::_refresh()
{
    _ui->hoursComboBox->setEnabled(
        !_readOnly &&
        _ui->timeoutCheckBox->isChecked());
    _ui->minutesComboBox->setEnabled(
        !_readOnly &&
        _ui->timeoutCheckBox->isChecked());
    _ui->buttonBox->button(QDialogButtonBox::StandardButton::Ok)->setEnabled(
        !_readOnly &&
        _validator->isValidDisplayName(_ui->displayNameLineEdit->text()) &&
        _validator->isValidDescription(_ui->descriptionPlainTextEdit->toPlainText()) &&
        _validator->isValidTimeout(_selectedTimeout()));
}

//////////
//  Signal handlers
void ModifyPublicActivityDialog::_displayNameLineEditTextChanged(QString)
{
    _refresh();
}

void ModifyPublicActivityDialog::_descriptionPlainTextEditTextChanged()
{
    _refresh();
}

void ModifyPublicActivityDialog::_selectWorkloadPushButtonClicked()
{
    ErrorDialog::show(this, "Not yet implemented");
}

void ModifyPublicActivityDialog::_timeoutCheckBoxStateChanged(int)
{
    _refresh();
}

void ModifyPublicActivityDialog::_hoursComboBoxCurrentIndexChanged(int)
{
    _refresh();
}

void ModifyPublicActivityDialog::_minutesComboBoxCurrentIndexChanged(int)
{
    _refresh();
}

void ModifyPublicActivityDialog::accept()
{
    try
    {   //  Any of the setters may throw
        if (!_readOnly)
        {
            _publicActivity->setDisplayName(
                _credentials,
                _ui->displayNameLineEdit->text());
            _publicActivity->setDescription(
                _credentials,
                _ui->descriptionPlainTextEdit->toPlainText());
            _publicActivity->setTimeout(
                _credentials,
                _selectedTimeout());
            _publicActivity->setRequireCommentOnStart(
                _credentials,
                _ui->requireCommentOnStartCheckBox->isChecked());
            _publicActivity->setRequireCommentOnStop(
                _credentials,
                _ui->requireCommentOnStopCheckBox->isChecked());
            _publicActivity->setFullScreenReminder(
                _credentials,
                _ui->fullScreenReminderCheckBox->isChecked());
            _publicActivity->setActivityType(
                _credentials,
                _selectedActivityType());
            _publicActivity->setWorkload(
                _credentials,
                _selectedWorkload);
        }
        done(int(Result::Ok));
    }
    catch (const tt3::util::Exception & ex)
    {
        ErrorDialog::show(this, ex);
    }
}

void ModifyPublicActivityDialog::reject()
{
    done(int(Result::Cancel));
}

//  End of tt3-gui/ModifyPublicActivityDialog.cpp
