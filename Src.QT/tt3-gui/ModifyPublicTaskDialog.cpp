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

namespace tt3::gui
{
    extern CurrentActivity theCurrentActivity;
}

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
    tt3::util::ResourceReader rr(Component::Resources::instance(), RSID(ModifyPublicTaskDialog));

    Q_ASSERT(_publicTask != nullptr);
    Q_ASSERT(_credentials.isValid());

    _ui->setupUi(this);
    setWindowTitle(rr.string(RID(Title)));

    //  Fill the "activity type" combo box (may throw)
    QList<tt3::ws::ActivityType> activityTypes =
        _publicTask->workspace()->activityTypes(_credentials).values();
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

    //  Fill "hours" and "minutes" combo boxes
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

    //  Set static control values
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

    //  Set editable control values (may throw)
    _setSelectedParentTask(_publicTask->parent(_credentials));
    _ui->displayNameLineEdit->setText(_publicTask->displayName(_credentials));
    _ui->descriptionPlainTextEdit->setPlainText(_publicTask->description(_credentials));
    _setSelectedActivityType(_publicTask->activityType(_credentials));
    _setSelectedWorkload(_publicTask->workload(_credentials));
    _setSelectedTimeout(_publicTask->timeout(_credentials));
    _ui->requireCommentOnStartCheckBox->setChecked(_publicTask->requireCommentOnStart(_credentials));
    _ui->requireCommentOnStopCheckBox->setChecked(_publicTask->requireCommentOnStop(_credentials));
    _ui->fullScreenReminderCheckBox->setChecked(_publicTask->fullScreenReminder(_credentials));
    _ui->completedCheckBox->setChecked(_publicTask->completed(_credentials));
    _ui->requiresCommentOnCompletionCeckBox->setChecked(_publicTask->requireCommentOnCompletion(_credentials));

    //  Adjust controls
    if (_readOnly)
    {   //  Adjust for "view only" mode
        setWindowTitle(rr.string(RID(ViewOnlyTitle)));
        setWindowIcon(QIcon(":/tt3-gui/Resources/Images/Actions/ViewPublicTaskLarge.png"));
        _ui->parentTaskComboBox->setEnabled(false);
        _ui->selectParentTaskPushButton->setEnabled(false);
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
        _ui->completedCheckBox->setEnabled(false);
        _ui->requiresCommentOnCompletionCeckBox->setEnabled(false);
    }
    else if (_publicTask->completed(_credentials))
    {   //  Only an Administrator can "uncomplete"
        //  a PublicTask - the ManagePublicTasks capability
        //  is not enough
        _ui->completedCheckBox->setEnabled(
            _publicTask->workspace()->grantsAll( //  may throw
                _credentials,
                tt3::ws::Capability::Administrator));
    }

    //  Done
    _refresh();
    adjustSize();
    _ui->displayNameLineEdit->setFocus();
}

ModifyPublicTaskDialog::~ModifyPublicTaskDialog()
{
    delete _ui;
}

//////////
//  Operations
auto ModifyPublicTaskDialog::doModal(
    ) -> Result
{
    return Result(this->exec());
}

//////////
//  Implementation helpers
auto ModifyPublicTaskDialog::_selectedParentTask(
    ) -> tt3::ws::PublicTask
{
    return _ui->parentTaskComboBox->currentData().value<tt3::ws::PublicTask>();
}

void ModifyPublicTaskDialog::_setSelectedParentTask(
        tt3::ws::PublicTask parentTask
    )
{
    tt3::util::ResourceReader rr(Component::Resources::instance(), RSID(ModifyPublicTaskDialog));

    //  Refill the "parent task" combo box
    _ui->parentTaskComboBox->clear();
    _ui->parentTaskComboBox->addItem(
        rr.string(RID(NoParent)),
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
            qCritical() << ex;
            Q_ASSERT(_ui->parentTaskComboBox->count() == 1);
        }
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

auto ModifyPublicTaskDialog::_selectedWorkload(
    ) -> tt3::ws::Workload
{
    return _ui->workloadComboBox->currentData().value<tt3::ws::Workload>();
}

void ModifyPublicTaskDialog::_setSelectedWorkload(
        tt3::ws::Workload workload
    )
{
    //  Refill the "workload" combo box
    _ui->workloadComboBox->clear();
    _ui->workloadComboBox->addItem(
        "-",
        QVariant::fromValue<tt3::ws::Workload>(nullptr));
    if (workload != nullptr)
    {
        try
        {
            _ui->workloadComboBox->addItem(
                workload->type()->smallIcon(),
                workload->displayName(_credentials),    //  may throw
                QVariant::fromValue(workload));
            _ui->workloadComboBox->setCurrentIndex(1);
        }
        catch (const tt3::util::Exception & ex)
        {  //  OOPS! Log & suppress
            qCritical() << ex;
        }
    }
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
void ModifyPublicTaskDialog::_selectParentTaskPushButtonClicked()
{
    try
    {
        SelectPublicTaskParentDialog dlg(
            this, _publicTask, _credentials, _selectedParentTask());
        if (dlg.doModal() == SelectPublicTaskParentDialog::Result::Ok)
        {
            _setSelectedParentTask(dlg.selectedParentTask());
            _refresh();
        }
    }
    catch (const tt3::util::Exception & ex)
    {
        qCritical() << ex;
        ErrorDialog::show(this, ex);
    }
}

void ModifyPublicTaskDialog::_displayNameLineEditTextChanged(QString)
{
    _refresh();
}

void ModifyPublicTaskDialog::_descriptionPlainTextEditTextChanged()
{
    _refresh();
}

void ModifyPublicTaskDialog::_selectWorkloadPushButtonClicked()
{
    try
    {
        SelectWorkloadDialog dlg(
            this, _publicTask->workspace(), _credentials, _selectedWorkload());
        if (dlg.doModal() == SelectWorkloadDialog::Result::Ok)
        {
            _setSelectedWorkload(dlg.selectedWorkload());
            _refresh();
        }
    }
    catch (const tt3::util::Exception & ex)
    {
        qCritical() << ex;
        ErrorDialog::show(this, ex);
    }
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
    {
        QString completionComment;
        if (!_publicTask->completed(_credentials) &&
            _ui->completedCheckBox->isChecked())
        {   //  Completing a PublicTask
            if (_publicTask->requireCommentOnCompletion(_credentials))
            {   //  ...that requires a comment
                EnterTaskCompletionCommentDialog dlg(this, _publicTask, _credentials);  //  may throw
                if (dlg.doModal() != EnterTaskCompletionCommentDialog::Result::Ok)
                {   //  OOPS! The user has cancelled!
                    return;
                }
                completionComment = dlg.comment();
            }
            //  A task that is being Completed must be stopped
            if (theCurrentActivity == _publicTask)
            {
                if (!theCurrentActivity.replaceWith(nullptr))
                {   //  OOPS! Stopping the task was cancelled by the user
                    return;
                }
            }
        }
        //  Any of the setters may throw
        if (!_readOnly)
        {
            _publicTask->setParent(
                _credentials,
                _selectedParentTask());
            _publicTask->setDisplayName(
                _credentials,
                _ui->displayNameLineEdit->text());
            _publicTask->setDescription(
                _credentials,
                _ui->descriptionPlainTextEdit->toPlainText());
            _publicTask->setTimeout(
                _credentials,
                _selectedTimeout());
            _publicTask->setRequireCommentOnStart(
                _credentials,
                _ui->requireCommentOnStartCheckBox->isChecked());
            _publicTask->setRequireCommentOnStop(
                _credentials,
                _ui->requireCommentOnStopCheckBox->isChecked());
            _publicTask->setFullScreenReminder(
                _credentials,
                _ui->fullScreenReminderCheckBox->isChecked());
            _publicTask->setActivityType(
                _credentials,
                _selectedActivityType());
            _publicTask->setWorkload(
                _credentials,
                _selectedWorkload());
            _publicTask->setCompleted(
                _credentials,
                _ui->completedCheckBox->isChecked());
            _publicTask->setRequireCommentOnCompletion(
                _credentials,
                _ui->requiresCommentOnCompletionCeckBox->isChecked());
            //  ...and record the completion comment if there is one
            if (!completionComment.isEmpty())
            {
                try
                {
                    tt3::ws::Account callerAccount =
                        _publicTask->workspace()->login(_credentials);  //  may throw
                    callerAccount->createEvent( //  may throw
                        _credentials,
                        QDateTime::currentDateTimeUtc(),
                        completionComment + ": " + _publicTask->displayName(_credentials), //  may throw
                        tt3::ws::Activities{_publicTask});
                }
                catch (const tt3::util::Exception & ex)
                {   //  OOPS! Log & suppress
                    qCritical() << ex;
                }
            }
        }
        done(int(Result::Ok));
    }
    catch (const tt3::util::Exception & ex)
    {
        qCritical() << ex;
        ErrorDialog::show(this, ex);
    }
}

void ModifyPublicTaskDialog::reject()
{
    done(int(Result::Cancel));
}

//  End oif tt3-gui/ModifyPublicTaskDialog.cpp
