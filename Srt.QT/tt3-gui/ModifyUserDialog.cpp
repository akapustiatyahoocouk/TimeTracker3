//
//  tt3-gui/ModifyUserDialog.cpp - tt3::gui::ModifyUserDialog class implementation
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
#include "ui_ModifyUserDialog.h"
using namespace tt3::gui;

//////////
//  Construction/destruction
ModifyUserDialog::ModifyUserDialog(QWidget *parent,
                                   tt3::ws::User user,
                                   const tt3::ws::Credentials & credentials)
    throws(tt3::ws::WorkspaceClosedException)
    :   QDialog(parent),
        //  Implementation
        _user(user),
        _credentials(credentials),
        _validator(user->workspace()->validator()->user()),
        //  Controls
        _ui(new Ui::ModifyUserDialog)
{
    Q_ASSERT(_user != nullptr);
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

    //  Set initial control values (may throw)
    _ui->realNameLineEdit->setText(_user->realName(_credentials));
    _setSelectedEmailAddresses(_user->emailAddresses(_credentials));
    _setSelectedInactivityTimeout(_user->inactivityTimeout(_credentials));
    _setSelectedUiLocale(_user->uiLocale(_credentials));
    _ui->enabledCheckBox->setChecked(_user->enabled(_credentials));

    //  Done
    adjustSize();
    _refresh();
}

ModifyUserDialog::~ModifyUserDialog()
{
    delete _ui;
}

//////////
//  Operations
ModifyUserDialog::Result ModifyUserDialog::doModal()
{
    return Result(this->exec());
}

//////////
//  Implementation helpers
QString ModifyUserDialog::_displayName(const QLocale & locale)
{
    return QLocale::languageToString(locale.language()) +
           " (" +
           QLocale::territoryToString(locale.territory()) +
           ")";
}

QStringList ModifyUserDialog::_selectedEmailAddresses()
{
    QStringList result;
    for (int i = 0; i < _ui->emailAddressesListWidget->count(); i++)
    {
        result.append(_ui->emailAddressesListWidget->item(i)->text());
    }
    return result;
}

void ModifyUserDialog::_setSelectedEmailAddresses(const QStringList & emailAddresses)
{
    _ui->emailAddressesListWidget->clear();
    for (QString emailAddress : emailAddresses)
    {
        _ui->emailAddressesListWidget->addItem(emailAddress);
    }
    _refresh();
}


tt3::ws::InactivityTimeout ModifyUserDialog::_selectedInactivityTimeout()
{
    if (_ui->inactivityTimeoutCheckBox->isChecked())
    {
        int h = _ui->hoursComboBox->currentData().value<int>();
        int m = _ui->minutesComboBox->currentData().value<int>();
        return tt3::util::TimeSpan::hours(h) + tt3::util::TimeSpan::minutes(m);
    }
    return tt3::ws::InactivityTimeout();
}

void ModifyUserDialog::_setSelectedInactivityTimeout(const tt3::ws::InactivityTimeout & inactivityTimeout)
{
    _ui->inactivityTimeoutCheckBox->setChecked(false);
    if (!inactivityTimeout.has_value())
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
            if (tt3::util::TimeSpan::hours(h) + tt3::util::TimeSpan::minutes(m) == inactivityTimeout)
            {   //  These ones!
                _ui->inactivityTimeoutCheckBox->setChecked(true);
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

tt3::ws::UiLocale ModifyUserDialog::_selectedUiLocale()
{
    return (_ui->uiLocaleComboBox->currentIndex() == 0) ?
               tt3::ws::UiLocale() :
               tt3::ws::UiLocale(_locales[_ui->uiLocaleComboBox->currentIndex()]);
}

void ModifyUserDialog::_setSelectedUiLocale(const tt3::ws::UiLocale & uiLocale)
{
    _ui->uiLocaleComboBox->setCurrentIndex(0);
    if (uiLocale.has_value())
    {
        for (int i = 1; i < _locales.size(); i++)
        {
            if (_locales[i] == uiLocale)
            {   //  This one
                _ui->uiLocaleComboBox->setCurrentIndex(i);
                break;
            }
        }
    }
    _refresh();
}

void ModifyUserDialog::_refresh()
{
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
void ModifyUserDialog::_realNameLineEditTextChanged(QString)
{
    _refresh();
}

void ModifyUserDialog::_inactivityTimeoutCheckBoxStateChanged(int)
{
    _refresh();
}

void ModifyUserDialog::_hoursComboBoxCurrentIndexChanged(int)
{
    _refresh();
}

void ModifyUserDialog::_minutesComboBoxCurrentIndexChanged(int)
{
    _refresh();
}

void ModifyUserDialog::_uiLocaleComboBoxCurrentIndexChanged(int)
{
    _refresh();
}

void ModifyUserDialog::accept()
{
    try
    {
        _user->setEnabled(_credentials, _ui->enabledCheckBox->isChecked()); //  must be first!!!
        _user->setEmailAddresses(_credentials, _selectedEmailAddresses());
        _user->setRealName(_credentials, _ui->realNameLineEdit->text());
        _user->setInactivityTimeout(_credentials, _selectedInactivityTimeout());
        _user->setUiLocale(_credentials, _selectedUiLocale());
        done(int(Result::Ok));
    }
    catch (const tt3::util::Exception & ex)
    {
        ErrorDialog::show(this, ex);
    }
}

void ModifyUserDialog::reject()
{
    done(int(Result::Cancel));
}

//  End of tt3-gui/ModifyUserDialog.cpp
