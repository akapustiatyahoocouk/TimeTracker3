//
//  tt3-gui/ModifyPublicActivityDialog.cpp - tt3::gui::ModifyPublicActivityDialog class implementation
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
    tt3::util::ResourceReader rr(Component::Resources::instance(), RSID(ModifyPublicActivityDialog));

    Q_ASSERT(_publicActivity != nullptr);
    Q_ASSERT(_credentials.isValid());

    _ui->setupUi(this);
    setWindowTitle(rr.string(RID(Title)));

    //  Fill the "activity type" combo box (may throw)
    QList<tt3::ws::ActivityType> activityTypes =
        _publicActivity->workspace()->activityTypes(_credentials).values();
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
    for (const auto & activityType : std::as_const(activityTypes))
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
        setWindowTitle(rr.string(RID(ViewOnlyTitle)));
        setWindowIcon(QIcon(":/tt3-gui/Resources/Images/Actions/ViewPublicActivityLarge.png"));
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

ModifyPublicActivityDialog::~ModifyPublicActivityDialog()
{
    delete _ui;
}

//////////
//  Operations
auto ModifyPublicActivityDialog::doModal(
    ) -> Result
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

auto ModifyPublicActivityDialog::_selectedWorkload(
    ) -> tt3::ws::Workload
{
    return _ui->workloadComboBox->currentData().value<tt3::ws::Workload>();
}

void ModifyPublicActivityDialog::_setSelectedWorkload(
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
    try
    {
        SelectWorkloadDialog dlg(
            this, _publicActivity->workspace(), _credentials, _selectedWorkload());
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

void ModifyPublicActivityDialog::reject()
{
    done(int(Result::Cancel));
}

//  End of tt3-gui/ModifyPublicActivityDialog.cpp
