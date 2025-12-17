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
    tt3::util::ResourceReader rr(Component::Resources::instance(), RSID(ModifyPrivateActivityDialog));

    Q_ASSERT(_privateActivity != nullptr);
    Q_ASSERT(_credentials.isValid());

    _ui->setupUi(this);
    setWindowTitle(rr.string(RID(Title)));

    //  Populate User combo box & select the proper user
    QList<tt3::ws::User> usersList =
        _privateActivity->workspace()->users(_credentials).values();   //  may throw
    std::sort(
        usersList.begin(),
        usersList.end(),
        [&](auto a, auto b)
        {
            return tt3::util::NaturalStringOrder::less(
                a->realName(_credentials),
                b->realName(_credentials)); //  may throw
        });
    for (const auto & u : std::as_const(usersList))
    {
        _ui->userComboBox->addItem(
            u->type()->smallIcon(),
            u->realName(_credentials),  //  may throw
            QVariant::fromValue(u));
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

    //  Fill the "activity type" combo box (may throw)
    QList<tt3::ws::ActivityType> activityTypes =
        _privateActivity->workspace()->activityTypes(_credentials).values();
    std::sort(
        activityTypes.begin(),
        activityTypes.end(),
        [&](auto a, auto b)
        {
            return tt3::util::NaturalStringOrder::less(
                a->displayName(_credentials),
                b->displayName(_credentials));
        });
    _ui->activityTypeComboBox->addItem(
        "-",
        QVariant::fromValue<tt3::ws::ActivityType>(nullptr));
    for (const auto & activityType : std::as_const(activityTypes))
    {
        _ui->activityTypeComboBox->addItem(
            activityType->type()->smallIcon(),
            activityType->displayName(_credentials),
            QVariant::fromValue(activityType));
    }

    //  Set static control values
    _ui->userLabel->setText(
        rr.string(RID(UserLabel)));
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

    _ui->buttonBox->button(QDialogButtonBox::StandardButton::Ok)->
        setText(rr.string(RID(OkPushButton)));
    _ui->buttonBox->button(QDialogButtonBox::StandardButton::Ok)->
        setIcon(QIcon(":/tt3-gui/Resources/Images/Actions/OkSmall.png"));
    _ui->buttonBox->button(QDialogButtonBox::StandardButton::Cancel)->
        setText(rr.string(RID(CancelPushButton)));
    _ui->buttonBox->button(QDialogButtonBox::StandardButton::Cancel)->
        setIcon(QIcon(":/tt3-gui/Resources/Images/Actions/CancelSmall.png"));

    //  Set editable control values (may throw)
    _setSelectedUser(_privateActivity->owner(_credentials));
    _ui->displayNameLineEdit->setText(_privateActivity->displayName(_credentials));
    _ui->descriptionPlainTextEdit->setPlainText(_privateActivity->description(_credentials));
    _setSelectedActivityType(_privateActivity->activityType(_credentials));
    _setSelectedWorkload(_privateActivity->workload(_credentials));
    _setSelectedTimeout(_privateActivity->timeout(_credentials));
    _ui->requireCommentOnStartCheckBox->setChecked(_privateActivity->requireCommentOnStart(_credentials));
    _ui->requireCommentOnStopCheckBox->setChecked(_privateActivity->requireCommentOnStop(_credentials));
    _ui->fullScreenReminderCheckBox->setChecked(_privateActivity->fullScreenReminder(_credentials));

    //  Adjust controls
    _ui->userComboBox->setEnabled(false);
    if (_readOnly)
    {   //  Adjust for "view only" mode
        setWindowTitle(rr.string(RID(ViewOnlyTitle)));
        setWindowIcon(QIcon(":/tt3-gui/Resources/Images/Actions/ViewPrivateActivityLarge.png"));
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
    _refresh();
    adjustSize();
    _ui->displayNameLineEdit->setFocus();
}

ModifyPrivateActivityDialog::~ModifyPrivateActivityDialog()
{
    delete _ui;
}

//////////
//  Operations
auto ModifyPrivateActivityDialog::doModal(
    ) -> Result
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

auto ModifyPrivateActivityDialog::_selectedWorkload(
    ) -> tt3::ws::Workload
{
    return _ui->workloadComboBox->currentData().value<tt3::ws::Workload>();
}

void ModifyPrivateActivityDialog::_setSelectedWorkload(
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
    try
    {
        SelectWorkloadDialog dlg(
            this, _privateActivity->workspace(), _credentials, _selectedWorkload());
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
                _selectedWorkload());
        }
        done(int(Result::Ok));
    }
    catch (const tt3::util::Exception & ex)
    {
        qCritical() << ex;
        ErrorDialog::show(this, ex);
    }
}

void ModifyPrivateActivityDialog::reject()
{
    done(int(Result::Cancel));
}

//  End of tt3-gui/ModifyPrivateActivityDialog.cpp
