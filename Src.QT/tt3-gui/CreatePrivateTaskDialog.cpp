//
//  tt3-gui/CreatePrivateTaskDialog.cpp - tt3::gui::CreatePrivateTaskDialog class implementation
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
#include "ui_CreatePrivateTaskDialog.h"
using namespace tt3::gui;

//////////
//  Construction/destruction
CreatePrivateTaskDialog::CreatePrivateTaskDialog(
        QWidget * parent,
        tt3::ws::User owner,
        const tt3::ws::Credentials & credentials
    ) : QDialog(parent),
        //  Implementation
        _workspace(owner->workspace()),
        _credentials(credentials),
        _validator(owner->workspace()->validator()->privateTask()),
        //  Controls
        _ui(new Ui::CreatePrivateTaskDialog)
{
    tt3::util::ResourceReader rr(Component::Resources::instance(), RSID(CreatePrivateTaskDialog));

    Q_ASSERT(_workspace != nullptr);
    Q_ASSERT(_credentials.isValid());

    _ui->setupUi(this);
    setWindowTitle(rr.string(RID(Title)));

    //  Set initial control values
    _ui->userLabel->setText(
        rr.string(RID(UserLabel)));
    _ui->parentTaskLabel->setText(
        rr.string(RID(ParentTaskLabel)));
    _ui->selectParentTaskPushButton->setText(
        rr.string(RID(SelectParentTaskPushButton)));
    _ui->displayNameLabel->setText(
        rr.string(RID(DisplayNameLabel)));
    _ui->descriptionLabel->setText(
        rr.string(RID(DescriptionLabel)));
    _ui->activityTypeLabel->setText(
        rr.string(RID(ActivityTypeLabel)));
    _ui->workloadLabel->setText(
        rr.string(RID(WorkloadLabel)));
    _ui->selectWorkloadPushButton->setText(
        rr.string(RID(SelectWorkloadPushButton)));
    _ui->timeoutLabel->setText(
        rr.string(RID(TimeoutLabel)));
    _ui->timeoutCheckBox->setText(
        rr.string(RID(TimeoutCheckBox)));

    _ui->requireCommentOnStartCheckBox->setText(
        rr.string(RID(RequireCommentOnStartCheckBox)));
    _ui->requireCommentOnStopCheckBox->setText(
        rr.string(RID(RequireCommentOnStopCheckBox)));
    _ui->fullScreenReminderCheckBox->setText(
        rr.string(RID(FullScreenReminderCheckBox)));
    _ui->requiresCommentOnCompletionCeckBox->setText(
        rr.string(RID(RequiresCommentOnCompletionCeckBox)));
    _ui->completedCheckBox->setText(
        rr.string(RID(CompletedCheckBox)));

    _ui->buttonBox->button(QDialogButtonBox::StandardButton::Ok)->
        setText(rr.string(RID(OkPushButton)));
    _ui->buttonBox->button(QDialogButtonBox::StandardButton::Ok)->
        setIcon(QIcon(":/tt3-gui/Resources/Images/Actions/OkSmall.png"));
    _ui->buttonBox->button(QDialogButtonBox::StandardButton::Cancel)->
        setText(rr.string(RID(CancelPushButton)));
    _ui->buttonBox->button(QDialogButtonBox::StandardButton::Cancel)->
        setIcon(QIcon(":/tt3-gui/Resources/Images/Actions/CancelSmall.png"));

    _setSelectedParentTask(nullptr);

    //  Populate User combo box & select the proper user
    QList<tt3::ws::User> usersList =
        _workspace->users(_credentials).values();   //  may throw
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
    _setSelectedUser(owner);

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
        _ui->hoursComboBox->addItem(
            rr.string(RID(HoursComboBoxItem), h),
            QVariant::fromValue(h));
    }
    for (int m = 0; m < 60; m += 15)
    {
        _ui->minutesComboBox->addItem(
            rr.string(RID(MinutesComboBoxItem), m),
            QVariant::fromValue(m));
    }

    //  Done
    _ui->displayNameLineEdit->setFocus();
    adjustSize();
    _refresh();
}

CreatePrivateTaskDialog::CreatePrivateTaskDialog(
        QWidget * parent,
        tt3::ws::PrivateTask parentTask,
        const tt3::ws::Credentials & credentials
    ) : CreatePrivateTaskDialog(
            parent,
            parentTask->owner(credentials),
            credentials)
{
    _setSelectedParentTask(parentTask);
    _refresh();
}

CreatePrivateTaskDialog::~CreatePrivateTaskDialog()
{
    delete _ui;
}

//////////
//  Operations
CreatePrivateTaskDialog::Result CreatePrivateTaskDialog::doModal()
{
    return Result(this->exec());
}

//////////
//  Implementation helpers
tt3::ws::User CreatePrivateTaskDialog::_selectedUser()
{
    return _ui->userComboBox->currentData().value<tt3::ws::User>();
}

void CreatePrivateTaskDialog::_setSelectedUser(tt3::ws::User user)
{
    for (int i = 0; i < _ui->userComboBox->count(); i++)
    {
        if (_ui->userComboBox->itemData(i).value<tt3::ws::User>() == user)
        {
            _ui->userComboBox->setCurrentIndex(i);
            _setSelectedParentTask(nullptr);
            break;
        }
    }
}

auto CreatePrivateTaskDialog::_selectedParentTask(
    ) -> tt3::ws::PrivateTask
{
    return _ui->parentTaskComboBox->currentData().value<tt3::ws::PrivateTask>();
}

void CreatePrivateTaskDialog::_setSelectedParentTask(
        tt3::ws::PrivateTask parentTask
    )
{
    tt3::util::ResourceReader rr(Component::Resources::instance(), RSID(CreatePrivateTaskDialog));

    //  Refill the "parent task" combo box
    _ui->parentTaskComboBox->clear();
    _ui->parentTaskComboBox->addItem(
        rr.string(RID(NoParent)),
        QVariant::fromValue<tt3::ws::PrivateTask>(nullptr));
    if (parentTask != nullptr)
    {
        _ui->parentTaskComboBox->addItem(
            parentTask->type()->smallIcon(),
            parentTask->displayName(_credentials),
            QVariant::fromValue(parentTask));
        _ui->parentTaskComboBox->setCurrentIndex(1);
    }
}

auto CreatePrivateTaskDialog::_selectedActivityType(
    ) -> tt3::ws::ActivityType
{
    return _ui->activityTypeComboBox->currentData().value<tt3::ws::ActivityType>();
}

auto CreatePrivateTaskDialog::_selectedTimeout(
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

void CreatePrivateTaskDialog::_refresh()
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
void CreatePrivateTaskDialog::_userComboBoxCurrentIndexChanged(int)
{
    _setSelectedParentTask(nullptr);
}

void CreatePrivateTaskDialog::_selectParentTaskPushButtonClicked()
{
    try
    {
        SelectPrivateTaskParentDialog dlg(
            this, _selectedUser(), _credentials, _selectedParentTask());
        if (dlg.doModal() == SelectPrivateTaskParentDialog::Result::Ok)
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

void CreatePrivateTaskDialog::_displayNameLineEditTextChanged(QString)
{
    _refresh();
}

void CreatePrivateTaskDialog::_descriptionPlainTextEditTextChanged()
{
    _refresh();
}

void CreatePrivateTaskDialog::_selectWorkloadPushButtonClicked()
{
    ErrorDialog::show(this, "Not yet implemented");
}

void CreatePrivateTaskDialog::_timeoutCheckBoxStateChanged(int)
{
    _refresh();
}

void CreatePrivateTaskDialog::_hoursComboBoxCurrentIndexChanged(int)
{
    _refresh();
}

void CreatePrivateTaskDialog::_minutesComboBoxCurrentIndexChanged(int)
{
    _refresh();
}

void CreatePrivateTaskDialog::accept()
{
    try
    {
        if (tt3::ws::PrivateTask parentTask = _selectedParentTask())
        {   //  Create child PrivateTask
            _createdPrivateTask =
                parentTask->createChild(
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
        {   //  Create root PrivateTask
            _createdPrivateTask =
                _selectedUser()->createPrivateTask(
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

void CreatePrivateTaskDialog::reject()
{
    done(int(Result::Cancel));
}

//  End of tt3-gui/CreatePrivateTaskDialog.cpp
