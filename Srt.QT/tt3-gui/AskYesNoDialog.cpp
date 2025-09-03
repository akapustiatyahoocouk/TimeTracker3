//
//  tt3-gui/AskYesNoDialog.cpp - tt3::gui::AskYesNoDialog class implementation
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
#include "ui_AskYesNoDialog.h"
using namespace tt3::gui;

//////////
//  Construction/destruction
AskYesNoDialog::AskYesNoDialog(QWidget * parent, const QIcon & icon,
                               const QString & title, const QString & prompt,
                               tt3::util::Setting<bool> * confirmActionSetting)
    :   QDialog(parent),
        _confirmActionSetting(confirmActionSetting),
        _ui(new Ui::AskYesNoDialog)
{
    _ui->setupUi(this);

    _ui->buttonBox->button(QDialogButtonBox::StandardButton::Yes)->
        setIcon(QIcon(":/tt3-gui/Resources/Images/Actions/OkSmall.png"));
    _ui->buttonBox->button(QDialogButtonBox::StandardButton::No)->
        setIcon(QIcon(":/tt3-gui/Resources/Images/Actions/CancelSmall.png"));

    this->setWindowIcon(icon);
    this->setWindowTitle(title);
    _ui->promptLabel->setText(prompt);
    if (_confirmActionSetting != nullptr)
    {
        _ui->assumeYesCheckBox->setChecked(!_confirmActionSetting->value());
    }
    else
    {
        _ui->assumeYesCheckBox->setVisible(false);
    }

    //  Done
    adjustSize();
}

AskYesNoDialog::~AskYesNoDialog()
{
    delete _ui;
}

//////////
//  Operations
AskYesNoDialog::Result AskYesNoDialog::doModal()
{
    return Result(this->exec());
}

//////////
//  Signal handlers
void AskYesNoDialog::accept()
{
    if (_confirmActionSetting != nullptr)
    {
        _confirmActionSetting->setValue(!_ui->assumeYesCheckBox->isChecked());
    }
    done(int(Result::Yes));
}

void AskYesNoDialog::reject()
{
    done(int(Result::No));
}

//////////
//  ConfirmForgetWorkspaceDialog
ConfirmForgetWorkspaceDialog::ConfirmForgetWorkspaceDialog(
    ::QWidget * parent, tt3::ws::WorkspaceAddress workspaceAddress)
    :   AskYesNoDialog(
            parent,
            QIcon(":/tt3-gui/Resources/Images/Actions/OpenWorkspaceLarge.png"),
            "Failed to open workspace",
            "Would you like to remove the workspace\n" +
                workspaceAddress->displayForm() +
                "\nfrom the recent workspaces list ?")
{
}

//////////
//  ConfirmCloseWorkspaceDialog
ConfirmCloseWorkspaceDialog::ConfirmCloseWorkspaceDialog(
    ::QWidget * parent, tt3::ws::WorkspaceAddress workspaceAddress)
    :   AskYesNoDialog(
          parent,
          QIcon(":/tt3-gui/Resources/Images/Actions/CloseWorkspaceLarge.png"),
          "Close workspace",
          "Are you sure you really want to close the workspace\n" +
              workspaceAddress->displayForm() + " ?",
          &Component::Settings::instance()->confirmCloseWorkspace)
{
}

//////////
//  ConfirmRestartDialog
ConfirmRestartDialog::ConfirmRestartDialog(::QWidget * parent)
    :   AskYesNoDialog(
          parent,
          QIcon(":/tt3-gui/Resources/Images/Actions/RestartLarge.png"),
          "Restart TimeTracker3",
          "Are you sure you want to restart TimeTracker3 ?",
          &Component::Settings::instance()->confirmRestart)
{
}

//////////
//  ConfirmExitDialog
ConfirmExitDialog::ConfirmExitDialog(::QWidget * parent)
    :   AskYesNoDialog(
          parent,
          QIcon(":/tt3-gui/Resources/Images/Actions/ExitLarge.png"),
          "Exit TimeTracker3",
          "Are you sure you want to exit TimeTracker3 ?",
          &Component::Settings::instance()->confirmExit)
{
}

//////////
//  ChooseReloginDialog
ChooseReloginDialog::ChooseReloginDialog(
    ::QWidget * parent, tt3::ws::WorkspaceAddress workspaceAddress)
    :   AskYesNoDialog(
          parent,
          QIcon(":/tt3-gui/Resources/Images/Actions/LoginLarge.png"),
          "Access denied",
          "Current credentials do not allow access to\n" +
              workspaceAddress->displayForm() +
              "\nDo you want to log in with different credentials ?")
{
}

//////////
//  ConfirmResetPageSettingsDialog
ConfirmResetPageSettingsDialog::ConfirmResetPageSettingsDialog(::QWidget * parent)
    :   AskYesNoDialog(
          parent,
          QIcon(":/tt3-gui/Resources/Images/Actions/ResetLarge.png"),
          "Reset page settings",
          "Are you sure you want to reset page settings to their default values ?"
            "\nThe effects will not be permanent until you press 'OK' to close the dialog.")
{
}

//////////
//  ConfirmResetAllSettingsDialog
ConfirmResetAllSettingsDialog::ConfirmResetAllSettingsDialog(::QWidget * parent)
    :   AskYesNoDialog(
          parent,
          QIcon(":/tt3-gui/Resources/Images/Actions/ResetLarge.png"),
          "Reset all settings",
          "Are you sure you want to reset all settings to their default values ?"
            "\nThe effects will not be permanent until you press 'OK' to close the dialog.")
{
}

//////////
//  RestartRequiredDialog
RestartRequiredDialog::RestartRequiredDialog(::QWidget * parent)
    :   AskYesNoDialog(
          parent,
          QIcon(":/tt3-gui/Resources/Images/Actions/RestartLarge.png"),
          "Restart required",
          "One of the changes you have made to application settings"
            "\nwill take place only after the application is restarted."
            "\nDo you want to restart TimeTracker3 now ?")
{
}

//////////
//  ConfirmDestroyUserDialog
ConfirmDestroyUserDialog::ConfirmDestroyUserDialog(
        ::QWidget * parent,
        tt3::ws::User user,
        const tt3::ws::Credentials & credentials)
    throws(tt3::ws::WorkspaceClosedException)
    :   AskYesNoDialog(
          parent,
          QIcon(":/tt3-gui/Resources/Images/Actions/DestroyUserLarge.png"),
          "Destroy user",
          _prompt(user, credentials))
{
}

QString ConfirmDestroyUserDialog::_prompt(tt3::ws::User user,
                                          const tt3::ws::Credentials & credentials) throws(tt3::ws::WorkspaceClosedException)
{
    QString result =
        "Are you sure you want to destroy user \n" +
        user->realName(credentials) + " ?";
    //  TODO if there are works/events, count them and add a line
    return result;
}

//  End of tt3-gui/AskYesNoDialog.cpp
