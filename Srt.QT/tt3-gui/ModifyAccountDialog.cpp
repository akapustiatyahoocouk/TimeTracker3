//
//  tt3-gui/ModifyAccountDialog.cpp - tt3::gui::ModifyAccountDialog class implementation
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
#include "ui_ModifyAccountDialog.h"
using namespace tt3::gui;

//////////
//  Construction/destruction
ModifyAccountDialog::ModifyAccountDialog(
        QWidget * parent,
        tt3::ws::Account account,
        const tt3::ws::Credentials & credentials)
    throws(tt3::ws::WorkspaceClosedException)
    :   QDialog(parent),
        //  Implementation
        _account(account),
        _credentials(credentials),
        _validator(account->workspace()->validator()->account()),
        _oldPasswordHash(account->passwordHash(credentials)),
        //  Controls
        _ui(new Ui::ModifyAccountDialog)
{
    Q_ASSERT(_account != nullptr);
    Q_ASSERT(_credentials.isValid());

    _ui->setupUi(this);

    _ui->buttonBox->button(QDialogButtonBox::StandardButton::Ok)->
        setIcon(QIcon(":/tt3-gui/Resources/Images/Actions/OkSmall.png"));
    _ui->buttonBox->button(QDialogButtonBox::StandardButton::Cancel)->
        setIcon(QIcon(":/tt3-gui/Resources/Images/Actions/CancelSmall.png"));

    //  Set initial control values (may throw)
    _ui->loginLineEdit->setText(_account->login(_credentials));
    _ui->passwordLineEdit->setText(_oldPasswordHash);
    _ui->confirmPasswordLineEdit->setText(_oldPasswordHash);
    _setSelectedEmailAddresses(_account->emailAddresses(_credentials));
    _setSelectedCapabilities(_account->capabilities(_credentials));
    _ui->enabledCheckBox->setChecked(_account->enabled(_credentials));

    tt3::ws::Capabilities clientCapabilities =
        _account->workspace()->capabilities(credentials);
    //  An ordinary user cannot change their login,
    //  capabilities or enabled status - only password
    //  and e-mail addresses
    if ((clientCapabilities & tt3::ws::Capabilities::Administrator) == tt3::ws::Capabilities::None &&
        (clientCapabilities & tt3::ws::Capabilities::Administrator) == tt3::ws::Capabilities::None)
    {
        _ui->loginLineEdit->setEnabled(false);
        _ui->capabilitiesGroupBox->setEnabled(false);
        _ui->enabledCheckBox->setEnabled(false);
    }

    //  Done
    adjustSize();
    _refresh();
}

ModifyAccountDialog::~ModifyAccountDialog()
{
    delete _ui;
}

//////////
//  Operations
ModifyAccountDialog::Result ModifyAccountDialog::doModal()
{
    return Result(this->exec());
}

//////////
//  Implementation helpers
QStringList ModifyAccountDialog::_selectedEmailAddresses()
{
    QStringList result;
    for (int i = 0; i < _ui->emailAddressesListWidget->count(); i++)
    {
        result.append(_ui->emailAddressesListWidget->item(i)->text());
    }
    return result;
}

void ModifyAccountDialog::_setSelectedEmailAddresses(const QStringList & emailAddresses)
{
    _ui->emailAddressesListWidget->clear();
    for (QString emailAddress : emailAddresses)
    {
        _ui->emailAddressesListWidget->addItem(emailAddress);
    }
    _refresh();
}

QString ModifyAccountDialog::_selectedEmailAddress()
{
    int n = _ui->emailAddressesListWidget->currentRow();
    return (n != -1) ?
               _ui->emailAddressesListWidget->item(n)->text() :
               "";
}

void ModifyAccountDialog::_setSelectedEmailAddress(const QString & emailAddress)
{
    for (int i = 0; i < _ui->emailAddressesListWidget->count(); i++)
    {
        if (emailAddress == _ui->emailAddressesListWidget->item(i)->text())
        {
            _ui->emailAddressesListWidget->setCurrentRow(i);
            break;
        }
    }
}

tt3::ws::Capabilities ModifyAccountDialog::_selectedCapabilities()
{
    tt3::ws::Capabilities result = tt3::ws::Capabilities::None;
    if (_ui->administratorCheckBox->isChecked())
    {
        result |= tt3::ws::Capabilities::Administrator;
    }
    if (_ui->manageUsersCheckBox->isChecked())
    {
        result |= tt3::ws::Capabilities::ManageUsers;
    }
    if (_ui->manageActivityTypesCheckBox->isChecked())
    {
        result |= tt3::ws::Capabilities::ManageActivityTypes;
    }
    if (_ui->manageBeneficiariesCheckBox->isChecked())
    {
        result |= tt3::ws::Capabilities::ManageBeheficiaries;
    }
    if (_ui->manageWorkloadsCheckBox->isChecked())
    {
        result |= tt3::ws::Capabilities::ManageWorkloads;
    }
    if (_ui->managePublicActivitiesCheckBox->isChecked())
    {
        result |= tt3::ws::Capabilities::ManagePublicActivities;
    }
    if (_ui->managePublicTasksCheckBox->isChecked())
    {
        result |= tt3::ws::Capabilities::ManagePublicTasks;
    }
    if (_ui->managePrivateActivitiesCheckBox->isChecked())
    {
        result |= tt3::ws::Capabilities::ManagePrivateActivities;
    }
    if (_ui->managePrivateTasksCheckBox->isChecked())
    {
        result |= tt3::ws::Capabilities::ManagePrivateTasks;
    }
    if (_ui->logWorkCheckBox->isChecked())
    {
        result |= tt3::ws::Capabilities::LogWork;
    }
    if (_ui->logEventsCheckBox->isChecked())
    {
        result |= tt3::ws::Capabilities::LogEvents;
    }
    if (_ui->generateReportsCheckBox->isChecked())
    {
        result |= tt3::ws::Capabilities::GenerateReports;
    }
    if (_ui->backupAndRestoreCheckBox->isChecked())
    {
        result |= tt3::ws::Capabilities::BackupAndRestore;
    }
    return result;
}

void ModifyAccountDialog::_setSelectedCapabilities(tt3::ws::Capabilities capabilities)
{
    _ui->administratorCheckBox->setChecked(
        (capabilities & tt3::ws::Capabilities::Administrator) != tt3::ws::Capabilities::None);
    _ui->manageUsersCheckBox->setChecked(
        (capabilities & tt3::ws::Capabilities::ManageUsers) != tt3::ws::Capabilities::None);
    _ui->manageActivityTypesCheckBox->setChecked(
        (capabilities & tt3::ws::Capabilities::ManageActivityTypes) != tt3::ws::Capabilities::None);
    _ui->manageBeneficiariesCheckBox->setChecked(
        (capabilities & tt3::ws::Capabilities::ManageBeheficiaries) != tt3::ws::Capabilities::None);
    _ui->manageWorkloadsCheckBox->setChecked(
        (capabilities & tt3::ws::Capabilities::ManageWorkloads) != tt3::ws::Capabilities::None);
    _ui->managePublicActivitiesCheckBox->setChecked(
        (capabilities & tt3::ws::Capabilities::ManagePublicActivities) != tt3::ws::Capabilities::None);
    _ui->managePublicTasksCheckBox->setChecked(
        (capabilities & tt3::ws::Capabilities::ManagePublicTasks) != tt3::ws::Capabilities::None);
    _ui->managePrivateActivitiesCheckBox->setChecked(
        (capabilities & tt3::ws::Capabilities::ManagePrivateActivities) != tt3::ws::Capabilities::None);
    _ui->managePrivateTasksCheckBox->setChecked(
        (capabilities & tt3::ws::Capabilities::ManagePrivateTasks) != tt3::ws::Capabilities::None);
    _ui->logWorkCheckBox->setChecked(
        (capabilities & tt3::ws::Capabilities::LogWork) != tt3::ws::Capabilities::None);
    _ui->logEventsCheckBox->setChecked(
        (capabilities & tt3::ws::Capabilities::LogEvents) != tt3::ws::Capabilities::None);
    _ui->generateReportsCheckBox->setChecked(
        (capabilities & tt3::ws::Capabilities::GenerateReports) != tt3::ws::Capabilities::None);
    _ui->backupAndRestoreCheckBox->setChecked(
        (capabilities & tt3::ws::Capabilities::BackupAndRestore) != tt3::ws::Capabilities::None);
}

void ModifyAccountDialog::_refresh()
{
    _ui->modifyEmailAddressPushButton->setEnabled(!_selectedEmailAddress().isEmpty());
    _ui->removeEmailAddressPushButton->setEnabled(!_selectedEmailAddress().isEmpty());
    _ui->buttonBox->button(QDialogButtonBox::StandardButton::Ok)->setEnabled(
        _validator->isValidLogin(_ui->loginLineEdit->text()) &&
        _validator->isValidPassword(_ui->passwordLineEdit->text()) &&
        _ui->passwordLineEdit->text() == _ui->confirmPasswordLineEdit->text() &&
        _validator->isValidEmailAddresses(_selectedEmailAddresses()));
}

//////////
//  Signal handlers
void ModifyAccountDialog::_loginLineEditTextChanged(QString)
{
    _refresh();
}

void ModifyAccountDialog::_passwordLineEditTextChanged(QString)
{
    _refresh();
}

void ModifyAccountDialog::_emailAddressesListWidgetCurrentRowChanged(int)
{
    _refresh();
}

void ModifyAccountDialog::_addEmailAddressPushButtonClicked()
{
    AddEmailAddressDialog dlg(
        this,
        [&](auto a) { return _validator->isValidEmailAddress(a); });
    if (dlg.doModal() == AddEmailAddressDialog::Result::Ok)
    {
        QStringList emailAddresses = _selectedEmailAddresses();
        emailAddresses.removeOne(dlg.editedValue());    //  in case it's already there
        emailAddresses.append(dlg.editedValue());
        emailAddresses.sort();
        _setSelectedEmailAddresses(emailAddresses);
        _setSelectedEmailAddress(dlg.editedValue());
        _refresh();
    }
}

void ModifyAccountDialog::_modifyEmailAddressPushButtonClicked()
{
    QString oldEmailAddress = _selectedEmailAddress();
    if (!oldEmailAddress.isEmpty())
    {
        ModifyEmailAddressDialog dlg(
            this,
            oldEmailAddress,
            [&](auto a) { return _validator->isValidEmailAddress(a); });
        if (dlg.doModal() == ModifyEmailAddressDialog::Result::Ok)
        {
            QStringList emailAddresses = _selectedEmailAddresses();
            emailAddresses.removeOne(oldEmailAddress);
            emailAddresses.removeOne(dlg.editedValue());
            emailAddresses.append(dlg.editedValue());
            emailAddresses.sort();
            _setSelectedEmailAddresses(emailAddresses);
            _setSelectedEmailAddress(dlg.editedValue());
            _refresh();
        }
    }
}

void ModifyAccountDialog::_removeEmailAddressPushButtonClicked()
{
    QString emailAddress = _selectedEmailAddress();
    if (!emailAddress.isEmpty())
    {
        QStringList emailAddresses = _selectedEmailAddresses();
        emailAddresses.removeOne(emailAddress);
        //  No need to sort() - sorted already
        _setSelectedEmailAddresses(emailAddresses);
        _refresh();
    }
}

void ModifyAccountDialog::accept()
{
    try
    {
        if (_ui->enabledCheckBox->isEnabled())
        {   //  i.e. editable. MUST come first!
            _account->setEnabled(_credentials, _ui->enabledCheckBox->isChecked());
        }
        _account->setEmailAddresses(_credentials, _selectedEmailAddresses());
        if (_ui->loginLineEdit->isEnabled())
        {   //  i.e. editable
            _account->setLogin(_credentials, _ui->loginLineEdit->text());
        }
        if (_ui->passwordLineEdit->text() != _oldPasswordHash)
        {   //  i.e. changed
            _account->setPassword(_credentials, _ui->passwordLineEdit->text());
        }
        if (_ui->capabilitiesGroupBox->isEnabled())
        {   //  i.e. editable
            _account->setCapabilities(_credentials, _selectedCapabilities());
        }

        done(int(Result::Ok));
    }
    catch (const tt3::util::Exception & ex)
    {
        ErrorDialog::show(this, ex);
    }
}

void ModifyAccountDialog::reject()
{
    done(int(Result::Cancel));
}

//  End of tt3-gui/ModifyAccountDialog.cpp
