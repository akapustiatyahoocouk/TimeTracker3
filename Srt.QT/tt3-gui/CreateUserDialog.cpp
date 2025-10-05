//
//  tt3-gui/CreateUserDialog.cpp - tt3::gui::CreateUserDialog class implementation
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
#include "ui_CreateUserDialog.h"
using namespace tt3::gui;

//////////
//  Construction/destruction
CreateUserDialog::CreateUserDialog(QWidget * parent,
                                   tt3::ws::Workspace workspace, const tt3::ws::Credentials & credentials )
    :   QDialog(parent),
        //  Implementation
        _workspace(workspace),
        _credentials(credentials),
        _validator(workspace->validator()->user()),
        //  Controls
        _ui(new Ui::CreateUserDialog)
{
    Q_ASSERT(_workspace != nullptr);
    Q_ASSERT(_credentials.isValid());

    _ui->setupUi(this);

    _ui->buttonBox->button(QDialogButtonBox::StandardButton::Ok)->
        setIcon(QIcon(":/tt3-gui/Resources/Images/Actions/OkSmall.png"));
    _ui->buttonBox->button(QDialogButtonBox::StandardButton::Cancel)->
        setIcon(QIcon(":/tt3-gui/Resources/Images/Actions/CancelSmall.png"));

    //  Fill "hours" amd "minutes" combo boxes
    for (int h = 0; h < 12; h++)
    {
        _ui->hoursComboBox->addItem(tt3::util::toString(h) + " hrs", QVariant::fromValue(h));
    }
    for (int m = 0; m < 60; m += 15)
    {
        _ui->minutesComboBox->addItem(tt3::util::toString(m) + " min", QVariant::fromValue(m));
    }

    //  Fill "UI locale" combo box
    _ui->uiLocaleComboBox->addItem("- (system default)");
    for (QLocale locale : tt3::util::ComponentManager::supportedLocales())
    {
        _locales.append(locale);
    }
    std::sort(_locales.begin(),
              _locales.end(),
              [](auto a, auto b) { return _displayName(a) < _displayName(b); });

    for (QLocale locale : _locales)
    {
        _ui->uiLocaleComboBox->addItem(
            tt3::util::LocaleManager::smallIcon(locale),
            tt3::util::LocaleManager::displayName(locale));
    }
    _locales.insert(0, QLocale());  //  ...to make combo box and QList indexes match

    //  Done
    adjustSize();
    _refresh();
}

CreateUserDialog::~CreateUserDialog()
{
    delete _ui;
}

//////////
//  Operations
CreateUserDialog::Result CreateUserDialog::doModal()
{
    return Result(this->exec());
}

//////////
//  Implementation helpers
QString CreateUserDialog::_displayName(const QLocale & locale)
{
    return QLocale::languageToString(locale.language()) +
           " (" +
           QLocale::territoryToString(locale.territory()) +
           ")";
}

QStringList CreateUserDialog::_selectedEmailAddresses()
{
    QStringList result;
    for (int i = 0; i < _ui->emailAddressesListWidget->count(); i++)
    {
        result.append(_ui->emailAddressesListWidget->item(i)->text());
    }
    return result;
}

void CreateUserDialog::_setSelectedEmailAddresses(const QStringList & emailAddresses)
{
    _ui->emailAddressesListWidget->clear();
    for (QString emailAddress : emailAddresses)
    {
        _ui->emailAddressesListWidget->addItem(emailAddress);
    }
    _refresh();
}

QString CreateUserDialog::_selectedEmailAddress()
{
    int n = _ui->emailAddressesListWidget->currentRow();
    return (n != -1) ?
                _ui->emailAddressesListWidget->item(n)->text() :
                "";
}

void CreateUserDialog::_setSelectedEmailAddress(const QString & emailAddress)
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

tt3::ws::InactivityTimeout CreateUserDialog::_selectedInactivityTimeout()
{
    if (_ui->inactivityTimeoutCheckBox->isChecked())
    {
        int h = _ui->hoursComboBox->currentData().value<int>();
        int m = _ui->minutesComboBox->currentData().value<int>();
        return tt3::util::TimeSpan::hours(h) + tt3::util::TimeSpan::minutes(m);
    }
    return tt3::ws::InactivityTimeout();
}

tt3::ws::UiLocale CreateUserDialog::_selectedUiLocale()
{
    return (_ui->uiLocaleComboBox->currentIndex() == 0) ?
                tt3::ws::UiLocale() :
                tt3::ws::UiLocale(_locales[_ui->uiLocaleComboBox->currentIndex()]);
}

void CreateUserDialog::_refresh()
{
    _ui->modifyEmailAddressPushButton->setEnabled(!_selectedEmailAddress().isEmpty());
    _ui->removeEmailAddressPushButton->setEnabled(!_selectedEmailAddress().isEmpty());
    _ui->hoursComboBox->setEnabled(_ui->inactivityTimeoutCheckBox->isChecked());
    _ui->minutesComboBox->setEnabled(_ui->inactivityTimeoutCheckBox->isChecked());
    _ui->buttonBox->button(QDialogButtonBox::StandardButton::Ok)->setEnabled(
        _validator->isValidRealName(_ui->realNameLineEdit->text()) &&
        _validator->isValidInactivityTimeout(_selectedInactivityTimeout()) &&
        _validator->isValidEmailAddresses(_selectedEmailAddresses()) &&
        _validator->isValidUiLocale(_selectedUiLocale()));
}

//////////
//  Signal handlers
void CreateUserDialog::_realNameLineEditTextChanged(QString)
{
    _refresh();
}

void CreateUserDialog::_emailAddressesListWidgetCurrentRowChanged(int)
{
    _refresh();
}

void CreateUserDialog::_addEmailAddressPushButtonClicked()
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

void CreateUserDialog::_modifyEmailAddressPushButtonClicked()
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

void CreateUserDialog::_removeEmailAddressPushButtonClicked()
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

void CreateUserDialog::_inactivityTimeoutCheckBoxStateChanged(int)
{
    _refresh();
}

void CreateUserDialog::_hoursComboBoxCurrentIndexChanged(int)
{
    _refresh();
}

void CreateUserDialog::_minutesComboBoxCurrentIndexChanged(int)
{
    _refresh();
}

void CreateUserDialog::_uiLocaleComboBoxCurrentIndexChanged(int)
{
    _refresh();
}

void CreateUserDialog::_workingOnPushButtonClicked()
{
    tt3::gui::ErrorDialog::show(this, "Not yet implemented");
}

void CreateUserDialog::accept()
{
    try
    {
        _createdUser = _workspace->createUser(
            _credentials,
            _ui->enabledCheckBox->isChecked(),
            _selectedEmailAddresses(),
            _ui->realNameLineEdit->text(),
            _selectedInactivityTimeout(),
            _selectedUiLocale());
        done(int(Result::Ok));
    }
    catch (const tt3::util::Exception & ex)
    {
        ErrorDialog::show(this, ex);
    }
}

void CreateUserDialog::reject()
{
    done(int(Result::Cancel));
}

//  End of tt3-gui/CreateUserDialog.cpp
