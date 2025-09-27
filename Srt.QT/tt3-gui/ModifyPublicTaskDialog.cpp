//
//  tt3-gui/ModifyPublicTaskDialog.cpp - tt3::gui::ModifyPublicTaskDialog class implementation
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
#include "ui_ModifyPublicTaskDialog.h"
using namespace tt3::gui;

//////////
//  Construction/destruction
ModifyPublicTaskDialog::ModifyPublicTaskDialog(
        QWidget * parent,
        tt3::ws::PublicTask publicTask,
        const tt3::ws::Credentials & credentials
    ) : QDialog(parent),
        //  Implementation
        _publicTask(publicTask),
        _credentials(credentials),
        _validator(publicTask->workspace()->validator()->publicTask()),
        _readOnly(publicTask == nullptr ||
                  !publicTask->canModify(credentials) ||  //  may throw
                  publicTask->workspace()->isReadOnly()),
        //  Controls
        _ui(new Ui::ModifyPublicTaskDialog)
{
    Q_ASSERT(_publicTask != nullptr);
    Q_ASSERT(_credentials.isValid());

    _ui->setupUi(this);

    _ui->buttonBox->button(QDialogButtonBox::StandardButton::Ok)->
        setIcon(QIcon(":/tt3-gui/Resources/Images/Actions/OkSmall.png"));
    _ui->buttonBox->button(QDialogButtonBox::StandardButton::Cancel)->
        setIcon(QIcon(":/tt3-gui/Resources/Images/Actions/CancelSmall.png"));

    //  Fill the "activity type" combo box (may throw)
    QList<tt3::ws::ActivityType> activityTypes =
        _publicTask->workspace()->activityTypes(_credentials).values();
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
    _setSelectedParentTask(_publicTask->parent(_credentials));
    _ui->displayNameLineEdit->setText(_publicTask->displayName(_credentials));
    _ui->descriptionTextEdit->setPlainText(_publicTask->description(_credentials));
    _setSelectedActivityType(_publicTask->activityType(_credentials));
    _setSelectedWorkload(_publicTask->workload(_credentials));
    _setSelectedTimeout(_publicTask->timeout(_credentials));
    _ui->requireCommentOnStartCheckBox->setChecked(_publicTask->requireCommentOnStart(_credentials));
    _ui->requireCommentOnFinishCheckBox->setChecked(_publicTask->requireCommentOnFinish(_credentials));
    _ui->fullScreenReminderCheckBox->setChecked(_publicTask->fullScreenReminder(_credentials));
    _ui->completedCheckBox->setChecked(_publicTask->completed(_credentials));
    _ui->requiresCommentOnCompletionCeckBox->setChecked(_publicTask->requireCommentOnCompletion(_credentials));

    //  Adjust for "view only" mode
    if (_readOnly)
    {
        this->setWindowTitle("View public task");
        this->setWindowIcon(QIcon(":/tt3-gui/Resources/Images/Actions/ViewPublicTaskLarge.png"));
        _ui->parentTaskComboBox->setEnabled(false);
        _ui->selectParentRaskPushButton->setEnabled(false);
        _ui->displayNameLineEdit->setReadOnly(true);
        _ui->descriptionTextEdit->setReadOnly(true);
        _ui->activityTypeComboBox->setEnabled(false);
        _ui->selectWorkloadPushButton->setEnabled(false);
        _ui->timeoutCheckBox->setEnabled(false);
        _ui->hoursComboBox->setEnabled(false);
        _ui->minutesComboBox->setEnabled(false);
        _ui->requireCommentOnStartCheckBox->setEnabled(false);
        _ui->requireCommentOnFinishCheckBox->setEnabled(false);
        _ui->fullScreenReminderCheckBox->setEnabled(false);
        _ui->completedCheckBox->setEnabled(false);
        _ui->requiresCommentOnCompletionCeckBox->setEnabled(false);
    }

    //  Done
    adjustSize();
    _refresh();
}

ModifyPublicTaskDialog::~ModifyPublicTaskDialog()
{
    delete _ui;
}

//////////
//  Operations
auto ModifyPublicTaskDialog::doModal(
    ) -> ModifyPublicTaskDialog::Result
{
    return Result(this->exec());
}

//////////
//  Implementation helpers
void ModifyPublicTaskDialog::_setSelectedParentTask(
        tt3::ws::PublicTask parentTask
    )
{
    _selectedParentTask = parentTask;
    //  Refill the "parent task" combo box
    _ui->parentTaskComboBox->clear();
    _ui->parentTaskComboBox->addItem(
        "- (a root public task with no parent)",
        QVariant::fromValue<tt3::ws::PublicTask>(nullptr));
    if (parentTask != nullptr)
    {
        _ui->parentTaskComboBox->addItem(
            parentTask->type()->smallIcon(),
            parentTask->displayName(_credentials),
            QVariant::fromValue(parentTask));
        _ui->parentTaskComboBox->setCurrentIndex(1);
    }
}

auto ModifyPublicTaskDialog::_selectedActivityType(
    ) -> tt3::ws::ActivityType
{
    return _ui->activityTypeComboBox->currentData().value<tt3::ws::ActivityType>();
}

void ModifyPublicTaskDialog::_setSelectedActivityType(
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

void ModifyPublicTaskDialog::_setSelectedWorkload(
        tt3::ws::Workload workload
    )
{   //  TODO implement properly
    Q_ASSERT(workload == nullptr);
    _ui->workloadValueLabel->setText("-");
    _selectedWorkload = workload;
}

auto ModifyPublicTaskDialog::_selectedTimeout(
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

void ModifyPublicTaskDialog::_setSelectedTimeout(
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

void ModifyPublicTaskDialog::_refresh()
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
void ModifyPublicTaskDialog::_selectParentTaskPushButtonClicked()
{
    ErrorDialog::show(this, "Not yet implemented");
}

void ModifyPublicTaskDialog::_displayNameLineEditTextChanged(QString)
{
    _refresh();
}

void ModifyPublicTaskDialog::_descriptionTextEditTextChanged()
{
    _refresh();
}

void ModifyPublicTaskDialog::_selectWorkloadPushButtonClicked()
{
    ErrorDialog::show(this, "Not yet implemented");
}

void ModifyPublicTaskDialog::_timeoutCheckBoxStateChanged(int)
{
    _refresh();
}

void ModifyPublicTaskDialog::_hoursComboBoxCurrentIndexChanged(int)
{
    _refresh();
}

void ModifyPublicTaskDialog::_minutesComboBoxCurrentIndexChanged(int)
{
    _refresh();
}

void ModifyPublicTaskDialog::accept()
{
    try
    {   //  Any of the setters may throw
        if (!_readOnly)
        {
            //  TODO uncomment _publicTask->setParent(_credentials, _selectedParentTask);
            _publicTask->setDisplayName(
                _credentials,
                _ui->displayNameLineEdit->text());
            _publicTask->setDescription(
                _credentials,
                _ui->descriptionTextEdit->toPlainText());
            _publicTask->setTimeout(
                _credentials,
                _selectedTimeout());
            _publicTask->setRequireCommentOnStart(
                _credentials,
                _ui->requireCommentOnStartCheckBox->isChecked());
            _publicTask->setRequireCommentOnFinish(
                _credentials,
                _ui->requireCommentOnFinishCheckBox->isChecked());
            _publicTask->setFullScreenReminder(
                _credentials,
                _ui->fullScreenReminderCheckBox->isChecked());
            _publicTask->setActivityType(
                _credentials,
                _selectedActivityType());
            _publicTask->setWorkload(
                _credentials,
                _selectedWorkload);
            _publicTask->setCompleted(
                _credentials,
                _ui->completedCheckBox->isChecked());
            _publicTask->setRequireCommentOnCompletion(
                _credentials,
                _ui->requiresCommentOnCompletionCeckBox->isChecked());
        }
        done(int(Result::Ok));
    }
    catch (const tt3::util::Exception & ex)
    {
        ErrorDialog::show(this, ex);
    }
}

void ModifyPublicTaskDialog::reject()
{
    done(int(Result::Cancel));
}

//  End oif tt3-gui/ModifyPublicTaskDialog.cpp
