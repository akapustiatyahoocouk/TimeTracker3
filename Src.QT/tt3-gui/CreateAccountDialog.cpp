//
//  tt3-gui/CreateAccountDialog.cpp - tt3::gui::CreateAccountDialog class implementation
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
#include "ui_CreateAccountDialog.h"
using namespace tt3::gui;

//////////
//  Construction/destruction
CreateAccountDialog::CreateAccountDialog(
        QWidget * parent,
        tt3::ws::User user,
        const tt3::ws::Credentials & credentials
    ) : QDialog(parent),
        //  Implementation
        _workspace(user->workspace()),
        _credentials(credentials),
        _validator(_workspace->validator()->account()),
        //  Controls
        _ui(new Ui::CreateAccountDialog)
{
    tt3::util::ResourceReader rr(Component::Resources::instance(), RSID(CreateAccountDialog));

    Q_ASSERT(_workspace != nullptr);
    Q_ASSERT(_credentials.isValid());
    Q_ASSERT(_validator != nullptr);

    _ui->setupUi(this);
    setWindowTitle(rr.string(RID(Title)));

    _ui->userLabel->setText(
        rr.string(RID(UserLabel)));
    _ui->loginLabel->setText(
        rr.string(RID(LoginLabel)));
    _ui->passwordLabel->setText(
        rr.string(RID(PasswordLabel)));
    _ui->confirmPasswordLabel->setText(
        rr.string(RID(ConfirmPasswordLabel)));
    _ui->emailAddressesLabel->setText(
        rr.string(RID(EmailAddressesLabel)));
    _ui->addEmailAddressPushButton->setText(
        rr.string(RID(AddEmailAddressPushButton)));
    _ui->modifyEmailAddressPushButton->setText(
        rr.string(RID(ModifyEmailAddressPushButton)));
    _ui->removeEmailAddressPushButton->setText(
        rr.string(RID(RemoveEmailAddressPushButton)));

    _ui->capabilitiesGroupBox->setTitle(
        rr.string(RID(CapabilitiesGroupBox)));
    _ui->administratorCheckBox->setText(
        rr.string(RID(AdministratorCheckBox)));
    _ui->manageUsersCheckBox->setText(
        rr.string(RID(ManageUsersCheckBox)));
    _ui->manageActivityTypesCheckBox->setText(
        rr.string(RID(ManageActivityTypesCheckBox)));
    _ui->manageBeneficiariesCheckBox->setText(
        rr.string(RID(ManageBeneficiariesCheckBox)));
    _ui->manageWorkloadsCheckBox->setText(
        rr.string(RID(ManageWorkloadsCheckBox)));
    _ui->managePublicActivitiesCheckBox->setText(
        rr.string(RID(ManagePublicActivitiesCheckBox)));
    _ui->managePublicTasksCheckBox->setText(
        rr.string(RID(ManagePublicTasksCheckBox)));
    _ui->managePrivateActivitiesCheckBox->setText(
        rr.string(RID(ManagePrivateActivitiesCheckBox)));
    _ui->managePrivateTasksCheckBox->setText(
        rr.string(RID(ManagePrivateTasksCheckBox)));
    _ui->logWorkCheckBox->setText(
        rr.string(RID(LogWorkCheckBox)));
    _ui->logEventsCheckBox->setText(
        rr.string(RID(LogEventsCheckBox)));
    _ui->generateReportsCheckBox->setText(
        rr.string(RID(GenerateReportsCheckBox)));
    _ui->backupAndRestoreCheckBox->setText(
        rr.string(RID(BackupAndRestoreCheckBox)));

    _ui->enabledCheckBox->setText(
        rr.string(RID(EnabledCheckBox)));

    _ui->buttonBox->button(QDialogButtonBox::StandardButton::Ok)->
        setText(rr.string(RID(OkPushButton)));
    _ui->buttonBox->button(QDialogButtonBox::StandardButton::Ok)->
        setIcon(QIcon(":/tt3-gui/Resources/Images/Actions/OkSmall.png"));
    _ui->buttonBox->button(QDialogButtonBox::StandardButton::Cancel)->
        setText(rr.string(RID(CancelPushButton)));
    _ui->buttonBox->button(QDialogButtonBox::StandardButton::Cancel)->
        setIcon(QIcon(":/tt3-gui/Resources/Images/Actions/CancelSmall.png"));

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
    _setSelectedUser(user);

    //  Done
    _ui->loginLineEdit->setFocus();
    adjustSize();
    _refresh();
}

CreateAccountDialog::~CreateAccountDialog()
{
    delete _ui;
}

//////////
//  Operations
CreateAccountDialog::Result CreateAccountDialog::doModal()
{
    return Result(this->exec());
}

//////////
//  Implementation helpers
tt3::ws::User CreateAccountDialog::_selectedUser()
{
    return _ui->userComboBox->currentData().value<tt3::ws::User>();
}

void CreateAccountDialog::_setSelectedUser(tt3::ws::User user)
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

QStringList CreateAccountDialog::_selectedEmailAddresses()
{
    QStringList result;
    for (int i = 0; i < _ui->emailAddressesListWidget->count(); i++)
    {
        result.append(_ui->emailAddressesListWidget->item(i)->text());
    }
    return result;
}

void CreateAccountDialog::_setSelectedEmailAddresses(const QStringList & emailAddresses)
{
    _ui->emailAddressesListWidget->clear();
    for (QString emailAddress : emailAddresses)
    {
        _ui->emailAddressesListWidget->addItem(emailAddress);
    }
    _refresh();
}

QString CreateAccountDialog::_selectedEmailAddress()
{
    int n = _ui->emailAddressesListWidget->currentRow();
    return (n != -1) ?
               _ui->emailAddressesListWidget->item(n)->text() :
               "";
}

void CreateAccountDialog::_setSelectedEmailAddress(const QString & emailAddress)
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

tt3::ws::Capabilities CreateAccountDialog::_selectedCapabilities()
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

void CreateAccountDialog::_refresh()
{
    _ui->modifyEmailAddressPushButton->setEnabled(!_selectedEmailAddress().isEmpty());
    _ui->removeEmailAddressPushButton->setEnabled(!_selectedEmailAddress().isEmpty());
    _ui->buttonBox->button(QDialogButtonBox::StandardButton::Ok)->setEnabled(
        _selectedUser() != nullptr &&
        _validator->isValidLogin(_ui->loginLineEdit->text()) &&
        _validator->isValidPassword(_ui->passwordLineEdit->text()) &&
        _ui->passwordLineEdit->text() == _ui->confirmPasswordLineEdit->text() &&
        _validator->isValidEmailAddresses(_selectedEmailAddresses()));
}

//////////
//  Signal handlers
void CreateAccountDialog::_loginLineEditTextChanged(QString)
{
    _refresh();
}

void CreateAccountDialog::_passwordLineEditTextChanged(QString)
{
    _refresh();
}

void CreateAccountDialog::_emailAddressesListWidgetCurrentRowChanged(int)
{
    _refresh();
}

void CreateAccountDialog::_addEmailAddressPushButtonClicked()
{
    AddEmailAddressDialog dlg(this, _workspace);
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

void CreateAccountDialog::_modifyEmailAddressPushButtonClicked()
{
    QString oldEmailAddress = _selectedEmailAddress();
    if (!oldEmailAddress.isEmpty())
    {
        ModifyEmailAddressDialog dlg(this, oldEmailAddress, _workspace);
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

void CreateAccountDialog::_removeEmailAddressPushButtonClicked()
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

void CreateAccountDialog::accept()
{
    try
    {
        _createdAccount =
            _selectedUser()->createAccount(
                _credentials,
                _ui->enabledCheckBox->isChecked(),
                _selectedEmailAddresses(),
                _ui->loginLineEdit->text(),
                _ui->passwordLineEdit->text(),
                _selectedCapabilities());
        done(int(Result::Ok));
    }
    catch (const tt3::util::Exception & ex)
    {
        ErrorDialog::show(this, ex);
    }
}

void CreateAccountDialog::reject()
{
    done(int(Result::Cancel));
}

//  End of tt3-gui/CreateAccountDialog.cpp
