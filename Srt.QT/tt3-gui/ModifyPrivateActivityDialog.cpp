//
//  tt3-gui/ModifyPrivateActivityDialog.cpp - tt3::gui::ModifyPrivateActivityDialog class implementation
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
#include "ui_ModifyPrivateActivityDialog.h"
using namespace tt3::gui;

//////////
//  Construction/destruction
ModifyPrivateActivityDialog::ModifyPrivateActivityDialog(
        QWidget * parent,
        tt3::ws::PrivateActivity privateActivity,
        const tt3::ws::Credentials & credentials
    ) : QDialog(parent),
        //  Implementation
        _privateActivity(privateActivity),
        _credentials(credentials),
        _validator(privateActivity->workspace()->validator()->privateActivity()),
        _readOnly(privateActivity == nullptr ||
                  !privateActivity->canModify(credentials) ||  //  may throw
                  privateActivity->workspace()->isReadOnly()),
        //  Controls
        _ui(new Ui::ModifyPrivateActivityDialog)
{
    Q_ASSERT(_privateActivity != nullptr);
    Q_ASSERT(_credentials.isValid());

    _ui->setupUi(this);

    _ui->buttonBox->button(QDialogButtonBox::StandardButton::Ok)->
        setIcon(QIcon(":/tt3-gui/Resources/Images/Actions/OkSmall.png"));
    _ui->buttonBox->button(QDialogButtonBox::StandardButton::Cancel)->
        setIcon(QIcon(":/tt3-gui/Resources/Images/Actions/CancelSmall.png"));

    //  Populate User combo box & select the proper user
    QList<tt3::ws::User> usersList =
        _privateActivity->workspace()->users(_credentials).values();   //  may throw
    std::sort(usersList.begin(),
              usersList.end(),
              [&](auto a, auto b)
              {
                  return a->realName(_credentials) < b->realName(_credentials);   //  may throw
              });
    for (tt3::ws::User u : usersList)
    {
        _ui->userComboBox->addItem(
            u->type()->smallIcon(),
            u->realName(_credentials),  //  may throw
            QVariant::fromValue(u));
    }
    _setSelectedUser(_privateActivity->owner(_credentials));
    _ui->userComboBox->setEnabled(false);   //  TODO for now!

    //  Fill the "activity type" combo box (may throw)
    QList<tt3::ws::ActivityType> activityTypes =
        _privateActivity->workspace()->activityTypes(_credentials).values();
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
    _ui->displayNameLineEdit->setText(_privateActivity->displayName(_credentials));
    _ui->descriptionPlainTextEdit->setPlainText(_privateActivity->description(_credentials));
    _setSelectedActivityType(_privateActivity->activityType(_credentials));
    _setSelectedWorkload(_privateActivity->workload(_credentials));
    _setSelectedTimeout(_privateActivity->timeout(_credentials));
    _ui->requireCommentOnStartCheckBox->setChecked(_privateActivity->requireCommentOnStart(_credentials));
    _ui->requireCommentOnStopCheckBox->setChecked(_privateActivity->requireCommentOnStop(_credentials));
    _ui->fullScreenReminderCheckBox->setChecked(_privateActivity->fullScreenReminder(_credentials));

    //  Adjust for "view only" mode
    if (_readOnly)
    {
        this->setWindowTitle("View private activity");
        this->setWindowIcon(QIcon(":/tt3-gui/Resources/Images/Actions/ViewPrivateActivityLarge.png"));
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
    _ui->displayNameLineEdit->setFocus();
    adjustSize();
    _refresh();
}

ModifyPrivateActivityDialog::~ModifyPrivateActivityDialog()
{
    delete _ui;
}

//////////
//  Operations
auto ModifyPrivateActivityDialog::doModal(
    ) -> ModifyPrivateActivityDialog::Result
{
    return Result(this->exec());
}

//////////
//  Implementation helpers
tt3::ws::User ModifyPrivateActivityDialog::_selectedUser()
{
    return _ui->userComboBox->currentData().value<tt3::ws::User>();
}

void ModifyPrivateActivityDialog::_setSelectedUser(tt3::ws::User user)
{
    for (int i = 0; i < _ui->userComboBox->count(); i++)
    {
        if (_ui->userComboBox->itemData(i).value<tt3::ws::User>() == user)
        {
            _ui->userComboBox->setCurrentIndex(i);
            break;
        }
    }
}

auto ModifyPrivateActivityDialog::_selectedActivityType(
    ) -> tt3::ws::ActivityType
{
    return _ui->activityTypeComboBox->currentData().value<tt3::ws::ActivityType>();
}

void ModifyPrivateActivityDialog::_setSelectedActivityType(
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

void ModifyPrivateActivityDialog::_setSelectedWorkload(
        tt3::ws::Workload workload
    )
{   //  TODO implement properly
    Q_ASSERT(workload == nullptr);
    _ui->workloadValueLabel->setText("-");
    _selectedWorkload = workload;
}

auto ModifyPrivateActivityDialog::_selectedTimeout(
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

void ModifyPrivateActivityDialog::_setSelectedTimeout(
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

void ModifyPrivateActivityDialog::_refresh()
{
    _ui->hoursComboBox->setEnabled(_ui->timeoutCheckBox->isChecked());
    _ui->minutesComboBox->setEnabled(_ui->timeoutCheckBox->isChecked());
    _ui->buttonBox->button(QDialogButtonBox::StandardButton::Ok)->setEnabled(
        _selectedUser() != nullptr &&
        _validator->isValidDisplayName(_ui->displayNameLineEdit->text()) &&
        _validator->isValidDescription(_ui->descriptionPlainTextEdit->toPlainText()) &&
        _validator->isValidTimeout(_selectedTimeout()));
}

//////////
//  Signal handlers
void ModifyPrivateActivityDialog::_displayNameLineEditTextChanged(QString)
{
    _refresh();
}

void ModifyPrivateActivityDialog::_descriptionPlainTextEditTextChanged()
{
    _refresh();
}

void ModifyPrivateActivityDialog::_selectWorkloadPushButtonClicked()
{
    ErrorDialog::show(this, "Not yet implemented");
}

void ModifyPrivateActivityDialog::_timeoutCheckBoxStateChanged(int)
{
    _refresh();
}

void ModifyPrivateActivityDialog::_hoursComboBoxCurrentIndexChanged(int)
{
    _refresh();
}

void ModifyPrivateActivityDialog::_minutesComboBoxCurrentIndexChanged(int)
{
    _refresh();
}

void ModifyPrivateActivityDialog::accept()
{
    try
    {   //  Any of the setters may throw
        if (!_readOnly)
        {
            if (_selectedUser() != _privateActivity->owner(_credentials))
            {   //  TODO confirm the owner change
                //  TODO move PrivateActivity to another User?
            }
            _privateActivity->setDisplayName(
                _credentials,
                _ui->displayNameLineEdit->text());
            _privateActivity->setDescription(
                _credentials,
                _ui->descriptionPlainTextEdit->toPlainText());
            _privateActivity->setTimeout(
                _credentials,
                _selectedTimeout());
            _privateActivity->setRequireCommentOnStart(
                _credentials,
                _ui->requireCommentOnStartCheckBox->isChecked());
            _privateActivity->setRequireCommentOnStop(
                _credentials,
                _ui->requireCommentOnStopCheckBox->isChecked());
            _privateActivity->setFullScreenReminder(
                _credentials,
                _ui->fullScreenReminderCheckBox->isChecked());
            _privateActivity->setActivityType(
                _credentials,
                _selectedActivityType());
            _privateActivity->setWorkload(
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

void ModifyPrivateActivityDialog::reject()
{
    done(int(Result::Cancel));
}

//  End of tt3-gui/ModifyPrivateActivityDialog.cpp
