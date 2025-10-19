//
//  tt3-gui/CreatePublicTaskDialog.cpp - tt3::gui::CreatePublicTaskDialog class implementation
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
#include "ui_CreatePublicTaskDialog.h"
using namespace tt3::gui;

//////////
//  Construction/destruction
CreatePublicTaskDialog::CreatePublicTaskDialog(
        QWidget * parent,
        tt3::ws::Workspace workspace,
        const tt3::ws::Credentials & credentials,
        tt3::ws::PublicTask initialParentTask
    ) : QDialog(parent),
        //  Implementation
        _workspace(workspace),
        _credentials(credentials),
        _validator(workspace->validator()->publicTask()),
        //  Controls
        _ui(new Ui::CreatePublicTaskDialog)
{
    Q_ASSERT(_workspace != nullptr);
    Q_ASSERT(_credentials.isValid());
    Q_ASSERT(initialParentTask == nullptr ||
             initialParentTask->workspace() == _workspace);

    _ui->setupUi(this);

    _ui->buttonBox->button(QDialogButtonBox::StandardButton::Ok)->
        setIcon(QIcon(":/tt3-gui/Resources/Images/Actions/OkSmall.png"));
    _ui->buttonBox->button(QDialogButtonBox::StandardButton::Cancel)->
        setIcon(QIcon(":/tt3-gui/Resources/Images/Actions/CancelSmall.png"));

    //  Fill the "activity type" combo box (may throw)
    QList<tt3::ws::ActivityType> activityTypes =
        _workspace->activityTypes(_credentials).values();
    std::sort(
        activityTypes.begin(),
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
    _setSelectedParentTask(initialParentTask);

    //  Done
    _ui->displayNameLineEdit->setFocus();
    adjustSize();
    _refresh();
}

CreatePublicTaskDialog::~CreatePublicTaskDialog()
{
    delete _ui;
}

//////////
//  Operations
CreatePublicTaskDialog::Result CreatePublicTaskDialog::doModal()
{
    return Result(this->exec());
}

//////////
//  Implementation helpers
auto CreatePublicTaskDialog::_selectedParentTask(
    ) -> tt3::ws::PublicTask
{
    return _ui->parentTaskComboBox->currentData().value<tt3::ws::PublicTask>();
}

void CreatePublicTaskDialog::_setSelectedParentTask(
        tt3::ws::PublicTask parentTask
    )
{
    //  Refill the "parent task" combo box
    _ui->parentTaskComboBox->clear();
    _ui->parentTaskComboBox->addItem(
        "- (a root public task with no parent)",
        QVariant::fromValue<tt3::ws::PublicTask>(nullptr));
    if (parentTask != nullptr)
    {
        try
        {
            _ui->parentTaskComboBox->addItem(
                parentTask->type()->smallIcon(),
                parentTask->displayName(_credentials),  //  may throw
                QVariant::fromValue(parentTask));
            _ui->parentTaskComboBox->setCurrentIndex(1);
        }
        catch (const tt3::util::Exception & ex)
        {   //  OOPS! Log & suppress
            qCritical() << ex.errorMessage();
            Q_ASSERT(_ui->parentTaskComboBox->count() == 1);
        }
    }
}

auto CreatePublicTaskDialog::_selectedActivityType(
    ) -> tt3::ws::ActivityType
{
    return _ui->activityTypeComboBox->currentData().value<tt3::ws::ActivityType>();
}

auto CreatePublicTaskDialog::_selectedTimeout(
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

void CreatePublicTaskDialog::_refresh()
{
    _ui->hoursComboBox->setEnabled(_ui->timeoutCheckBox->isChecked());
    _ui->minutesComboBox->setEnabled(_ui->timeoutCheckBox->isChecked());
    _ui->buttonBox->button(QDialogButtonBox::StandardButton::Ok)->setEnabled(
        _validator->isValidDisplayName(_ui->displayNameLineEdit->text()) &&
        _validator->isValidDescription(_ui->descriptionPlainTextEdit->toPlainText()) &&
        _validator->isValidTimeout(_selectedTimeout()));
}

//////////
//  Signal handlers
void CreatePublicTaskDialog::_selectParentTaskPushButtonClicked()
{
    try
    {
        SelectPublicTaskParentDialog dlg(
            this, _workspace, _credentials, _selectedParentTask());
        if (dlg.doModal() == SelectPublicTaskParentDialog::Result::Ok)
        {
            _setSelectedParentTask(dlg.selectedParentTask());
            _refresh();
        }
    }
    catch (const tt3::util::Exception & ex)
    {
        qCritical() << ex.errorMessage();
        ErrorDialog::show(this, ex);
    }
}

void CreatePublicTaskDialog::_displayNameLineEditTextChanged(QString)
{
    _refresh();
}

void CreatePublicTaskDialog::_descriptionPlainTextEditTextChanged()
{
    _refresh();
}

void CreatePublicTaskDialog::_selectWorkloadPushButtonClicked()
{
    ErrorDialog::show(this, "Not yet implemented");
}

void CreatePublicTaskDialog::_timeoutCheckBoxStateChanged(int)
{
    _refresh();
}

void CreatePublicTaskDialog::_hoursComboBoxCurrentIndexChanged(int)
{
    _refresh();
}

void CreatePublicTaskDialog::_minutesComboBoxCurrentIndexChanged(int)
{
    _refresh();
}

void CreatePublicTaskDialog::accept()
{
    try
    {
        if (tt3::ws::PublicTask parentTask = _selectedParentTask())
        {   //  Create child PublicTask
            _createdPublicTask = parentTask->createChild(
                _credentials,
                _ui->displayNameLineEdit->text(),
                _ui->descriptionPlainTextEdit->toPlainText(),
                _selectedTimeout(),
                _ui->requireCommentOnStartCheckBox->isChecked(),
                _ui->requireCommentOnStopCheckBox->isChecked(),
                _ui->fullScreenReminderCheckBox->isChecked(),
                _selectedActivityType(),
                _selectedWorkload,
                _ui->completedCheckBox->isChecked(),
                _ui->requiresCommentOnCompletionCeckBox->isChecked());
        }
        else
        {   //  Create root PublicTask
            _createdPublicTask = _workspace->createPublicTask(
                _credentials,
                _ui->displayNameLineEdit->text(),
                _ui->descriptionPlainTextEdit->toPlainText(),
                _selectedTimeout(),
                _ui->requireCommentOnStartCheckBox->isChecked(),
                _ui->requireCommentOnStopCheckBox->isChecked(),
                _ui->fullScreenReminderCheckBox->isChecked(),
                _selectedActivityType(),
                _selectedWorkload,
                _ui->completedCheckBox->isChecked(),
                _ui->requiresCommentOnCompletionCeckBox->isChecked());
        }
        done(int(Result::Ok));
    }
    catch (const tt3::util::Exception & ex)
    {
        qCritical() << ex.errorMessage();
        ErrorDialog::show(this, ex);
    }
}

void CreatePublicTaskDialog::reject()
{
    done(int(Result::Cancel));
}

//  End of tt3-gui/CreatePublicTaskDialog.cpp
