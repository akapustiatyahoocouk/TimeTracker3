//
//  tt3-gui/CreatePublicTaskDialog.cpp - tt3::gui::CreatePublicTaskDialog class implementation
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
    tt3::util::ResourceReader rr(Component::Resources::instance(), RSID(CreatePublicTaskDialog));

    Q_ASSERT(_workspace != nullptr);
    Q_ASSERT(_credentials.isValid());
    Q_ASSERT(initialParentTask == nullptr ||
             initialParentTask->workspace() == _workspace);

    _ui->setupUi(this);
    setWindowTitle(rr.string(RID(Title)));

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
    for (const auto & activityType : qAsConst(activityTypes))
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
    _setSelectedParentTask(initialParentTask);
    _setSelectedWorkload(nullptr);

    //  Done
    _refresh();
    adjustSize();
    _ui->displayNameLineEdit->setFocus();
}

CreatePublicTaskDialog::~CreatePublicTaskDialog()
{
    delete _ui;
}

//////////
//  Operations
auto CreatePublicTaskDialog::doModal(
    ) -> Result
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
    tt3::util::ResourceReader rr(Component::Resources::instance(), RSID(CreatePublicTaskDialog));

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

auto CreatePublicTaskDialog::_selectedActivityType(
    ) -> tt3::ws::ActivityType
{
    return _ui->activityTypeComboBox->currentData().value<tt3::ws::ActivityType>();
}

auto CreatePublicTaskDialog::_selectedWorkload(
    ) -> tt3::ws::Workload
{
    return _ui->workloadComboBox->currentData().value<tt3::ws::Workload>();
}

void CreatePublicTaskDialog::_setSelectedWorkload(
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
        qCritical() << ex;
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
    try
    {
        SelectWorkloadDialog dlg(
            this, _workspace, _credentials, _selectedWorkload());
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
                _selectedWorkload(),
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
                _selectedWorkload(),
                _ui->completedCheckBox->isChecked(),
                _ui->requiresCommentOnCompletionCeckBox->isChecked());
        }
        done(int(Result::Ok));
    }
    catch (const tt3::util::Exception & ex)
    {
        qCritical() << ex;
        ErrorDialog::show(this, ex);
    }
}

void CreatePublicTaskDialog::reject()
{
    done(int(Result::Cancel));
}

//  End of tt3-gui/CreatePublicTaskDialog.cpp
