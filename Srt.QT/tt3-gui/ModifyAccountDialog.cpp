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
        const tt3::ws::Credentials & credentials
    ) : QDialog(parent),
        //  Implementation
        _account(account),
        _credentials(credentials),
        _validator(account->workspace()->validator()->account()),
        _readOnly(account == nullptr ||
                  !account->canModify(credentials) ||  //  may throw
                  account->workspace()->isReadOnly()),
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

    //  Populate User combo box & select the proper user
    QList<tt3::ws::User> usersList =
        _account->workspace()->users(_credentials).values();   //  may throw
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
    _setSelectedUser(_account->user(_credentials));
    _ui->userComboBox->setEnabled(false);   //  TODO for now!

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
    if (!clientCapabilities.contains(tt3::ws::Capability::Administrator) &&
        !clientCapabilities.contains(tt3::ws::Capability::ManageUsers))
    {
        _ui->loginLineEdit->setReadOnly(true);
        _ui->capabilitiesGroupBox->setEnabled(false);
        _ui->enabledCheckBox->setEnabled(false);
    }

    //  Adjust for "view only" mode
    if (_readOnly)
    {
        this->setWindowTitle("View account");
        this->setWindowIcon(QIcon(":/tt3-gui/Resources/Images/Actions/ViewAccountLarge.png"));
        _ui->loginLineEdit->setReadOnly(true);
        _ui->passwordLineEdit->setReadOnly(true);
        _ui->confirmPasswordLineEdit->setReadOnly(true);
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
tt3::ws::User ModifyAccountDialog::_selectedUser()
{
    return _ui->userComboBox->currentData().value<tt3::ws::User>();
}

void ModifyAccountDialog::_setSelectedUser(tt3::ws::User user)
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
    tt3::ws::Capabilities result;
    if (_ui->administratorCheckBox->isChecked())
    {
        result |= tt3::ws::Capability::Administrator;
    }
    if (_ui->manageUsersCheckBox->isChecked())
    {
        result |= tt3::ws::Capability::ManageUsers;
    }
    if (_ui->manageActivityTypesCheckBox->isChecked())
    {
        result |= tt3::ws::Capability::ManageActivityTypes;
    }
    if (_ui->manageBeneficiariesCheckBox->isChecked())
    {
        result |= tt3::ws::Capability::ManageBeneficiaries;
    }
    if (_ui->manageWorkloadsCheckBox->isChecked())
    {
        result |= tt3::ws::Capability::ManageWorkloads;
    }
    if (_ui->managePublicActivitiesCheckBox->isChecked())
    {
        result |= tt3::ws::Capability::ManagePublicActivities;
    }
    if (_ui->managePublicTasksCheckBox->isChecked())
    {
        result |= tt3::ws::Capability::ManagePublicTasks;
    }
    if (_ui->managePrivateActivitiesCheckBox->isChecked())
    {
        result |= tt3::ws::Capability::ManagePrivateActivities;
    }
    if (_ui->managePrivateTasksCheckBox->isChecked())
    {
        result |= tt3::ws::Capability::ManagePrivateTasks;
    }
    if (_ui->logWorkCheckBox->isChecked())
    {
        result |= tt3::ws::Capability::LogWork;
    }
    if (_ui->logEventsCheckBox->isChecked())
    {
        result |= tt3::ws::Capability::LogEvents;
    }
    if (_ui->generateReportsCheckBox->isChecked())
    {
        result |= tt3::ws::Capability::GenerateReports;
    }
    if (_ui->backupAndRestoreCheckBox->isChecked())
    {
        result |= tt3::ws::Capability::BackupAndRestore;
    }
    return result;
}

void ModifyAccountDialog::_setSelectedCapabilities(tt3::ws::Capabilities capabilities)
{
    _ui->administratorCheckBox->setChecked(
        capabilities.contains(tt3::ws::Capability::Administrator));
    _ui->manageUsersCheckBox->setChecked(
        capabilities.contains(tt3::ws::Capability::ManageUsers));
    _ui->manageActivityTypesCheckBox->setChecked(
        capabilities.contains(tt3::ws::Capability::ManageActivityTypes));
    _ui->manageBeneficiariesCheckBox->setChecked(
        capabilities.contains(tt3::ws::Capability::ManageBeneficiaries));
    _ui->manageWorkloadsCheckBox->setChecked(
        capabilities.contains(tt3::ws::Capability::ManageWorkloads));
    _ui->managePublicActivitiesCheckBox->setChecked(
        capabilities.contains(tt3::ws::Capability::ManagePublicActivities));
    _ui->managePublicTasksCheckBox->setChecked(
        capabilities.contains(tt3::ws::Capability::ManagePublicTasks));
    _ui->managePrivateActivitiesCheckBox->setChecked(
        capabilities.contains(tt3::ws::Capability::ManagePrivateActivities));
    _ui->managePrivateTasksCheckBox->setChecked(
        capabilities.contains(tt3::ws::Capability::ManagePrivateTasks));
    _ui->logWorkCheckBox->setChecked(
        capabilities.contains(tt3::ws::Capability::LogWork));
    _ui->logEventsCheckBox->setChecked(
        capabilities.contains(tt3::ws::Capability::LogEvents));
    _ui->generateReportsCheckBox->setChecked(
        capabilities.contains(tt3::ws::Capability::GenerateReports));
    _ui->backupAndRestoreCheckBox->setChecked(
        capabilities.contains(tt3::ws::Capability::BackupAndRestore));
}

void ModifyAccountDialog::_refresh()
{
    _ui->addEmailAddressPushButton->setEnabled(
        !_readOnly);
    _ui->modifyEmailAddressPushButton->setEnabled(
        !_readOnly &&
        !_selectedEmailAddress().isEmpty());
    _ui->removeEmailAddressPushButton->setEnabled(
        !_readOnly &&
        !_selectedEmailAddress().isEmpty());
    _ui->buttonBox->button(QDialogButtonBox::StandardButton::Ok)->setEnabled(
        !_readOnly &&
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
    AddEmailAddressDialog dlg(this, _account->workspace());
    if (dlg.doModal() == AddEmailAddressDialog::Result::Ok)
    {
        QStringList emailAddresses = _selectedEmailAddresses();
        emailAddresses.removeOne(dlg.emailAddress());    //  in case it's already there
        emailAddresses.append(dlg.emailAddress());
        emailAddresses.sort();
        _setSelectedEmailAddresses(emailAddresses);
        _setSelectedEmailAddress(dlg.emailAddress());
        _refresh();
    }
}

void ModifyAccountDialog::_modifyEmailAddressPushButtonClicked()
{
    QString oldEmailAddress = _selectedEmailAddress();
    if (!oldEmailAddress.isEmpty())
    {
        ModifyEmailAddressDialog dlg(this, oldEmailAddress, _account->workspace());
        if (dlg.doModal() == ModifyEmailAddressDialog::Result::Ok)
        {
            QStringList emailAddresses = _selectedEmailAddresses();
            emailAddresses.removeOne(oldEmailAddress);
            emailAddresses.removeOne(dlg.emailAddress());
            emailAddresses.append(dlg.emailAddress());
            emailAddresses.sort();
            _setSelectedEmailAddresses(emailAddresses);
            _setSelectedEmailAddress(dlg.emailAddress());
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
    {   //  Any of the setters may throw
        if (!_readOnly)
        {
            //  TODO move account to another User?
            _account->setEnabled(   //  MUST come first!
                _credentials,
                _ui->enabledCheckBox->isChecked());
            _account->setEmailAddresses(
                _credentials,
                _selectedEmailAddresses());
            _account->setLogin(
                _credentials,
                _ui->loginLineEdit->text());
            if (_ui->passwordLineEdit->text() != _oldPasswordHash)
            {   //  i.e. changed
                _account->setPassword(
                    _credentials,
                    _ui->passwordLineEdit->text());
            }
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
