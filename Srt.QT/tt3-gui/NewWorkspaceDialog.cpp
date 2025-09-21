//
//  tt3-gui/NewWorkspaceDialog.cpp - tt3::gui::NewWorkspaceDialog class implementation
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
#include "ui_NewWorkspaceDialog.h"
using namespace tt3::gui;

//////////
//  Construction/destruction
NewWorkspaceDialog::NewWorkspaceDialog(QWidget * parent)
    :   QDialog(parent),
        _ui(new Ui::NewWorkspaceDialog)
{
    _ui->setupUi(this);

    _ui->buttonBox->button(QDialogButtonBox::StandardButton::Ok)->
        setIcon(QIcon(":/tt3-gui/Resources/Images/Actions/OkSmall.png"));
    _ui->buttonBox->button(QDialogButtonBox::StandardButton::Cancel)->
        setIcon(QIcon(":/tt3-gui/Resources/Images/Actions/CancelSmall.png"));

    //  Populate "Workspace type" combo box
    QList<tt3::ws::WorkspaceType> workspaceTypes =
        tt3::ws::WorkspaceTypeManager::allWorkspaceTypes().values();
    std::sort(
        workspaceTypes.begin(),
        workspaceTypes.end(),
        [](auto& a, auto& b)
        {
            return a->displayName() < b->displayName(); // Example: ascending order
        }
        );
    for (auto workspaceType : workspaceTypes)
    {
        _ui->workspaceTypeComboBox->addItem(
            workspaceType->smallIcon(),
            workspaceType->displayName(),
            QVariant::fromValue(workspaceType));
    }

    //  Done
    adjustSize();
    _refresh();
}

NewWorkspaceDialog::~NewWorkspaceDialog()
{
    delete _ui;
}

//////////
//  Operations
NewWorkspaceDialog::Result NewWorkspaceDialog::doModal()
{
    return Result(this->exec());
}

//////////
//  Implementation helpers
void NewWorkspaceDialog::_refresh()
{
    if (_ui->workspaceTypeComboBox->count() == 0)
    {   //  A special case - no workspace types are available
        _ui->workspaceTypeComboBox->setEnabled(false);
        _ui->locationLabel->setEnabled(false);
        _ui->locationLineEdit->setEnabled(false);
        _ui->browsePushButton->setEnabled(false);
        _ui->buttonBox->button(QDialogButtonBox::StandardButton::Ok)->setEnabled(false);
        return;
    }
    //  General case
    tt3::ws::WorkspaceType workspaceType =
        _ui->workspaceTypeComboBox->currentData().value<tt3::ws::WorkspaceType>();
    Q_ASSERT(workspaceType != nullptr);
    tt3::ws::Validator::User * userValidator = workspaceType->validator()->user();
    tt3::ws::Validator::Account * accountValidator = workspaceType->validator()->account();

    _ui->locationLineEdit->setText((_workspaceAddress != nullptr) ? _workspaceAddress->displayForm() : "");
    _ui->buttonBox->button(QDialogButtonBox::StandardButton::Ok)->
        setEnabled(
           _workspaceAddress != nullptr &&
            userValidator->isValidRealName(_ui->userNameLineEdit->text()) &&
            accountValidator->isValidLogin(_ui->loginLineEdit->text()) &&
            accountValidator->isValidPassword(_ui->passwordLineEdit->text()) &&
            _ui->confirmPasswordLineEdit->text() == _ui->passwordLineEdit->text());
}

//////////
//  Event handlers
void NewWorkspaceDialog::_workspaceTypeComboBoxCurrentIndexChanged(int)
{
    Q_ASSERT(_ui->workspaceTypeComboBox->currentIndex() != -1);
    tt3::ws::WorkspaceType workspaceType =
        _ui->workspaceTypeComboBox->currentData().value<tt3::ws::WorkspaceType>();
    Q_ASSERT(workspaceType != nullptr);
    if (_workspaceAddress != nullptr && _workspaceAddress->workspaceType() != workspaceType)
    {   //  Need to reset the workspace address
        _workspaceAddress = nullptr;
    }
    _refresh();
}

void NewWorkspaceDialog::_browsePushButtonClicked()
{
    Q_ASSERT(_ui->workspaceTypeComboBox->currentIndex() != -1);
    tt3::ws::WorkspaceType workspaceType =
        _ui->workspaceTypeComboBox->currentData().value<tt3::ws::WorkspaceType>();
    Q_ASSERT(workspaceType != nullptr);

    if (auto workspaceAddress = workspaceType->enterNewWorkspaceAddress(this))
    {
        _workspaceAddress = workspaceAddress;
        _refresh();
    }
}

void NewWorkspaceDialog::_userNameLineEditTextChanged(QString)
{
    _adminUser = _ui->userNameLineEdit->text();
    _refresh();
}

void NewWorkspaceDialog::_loginLineEditTextChanged(QString)
{
    _adminLogin = _ui->loginLineEdit->text();
    _refresh();
}

void NewWorkspaceDialog::_passwordLineEditTextChanged(QString)
{
    _adminPassword = _ui->passwordLineEdit->text();
    _refresh();
}

void NewWorkspaceDialog::_confirmPasswordLineEditTextChanged(QString)
{
    _refresh();
}

void NewWorkspaceDialog::accept()
{
    done(int(Result::Ok));
}

void NewWorkspaceDialog::reject()
{
    done(int(Result::Cancel));
}

//  End of tt3-gui/NewWorkspaceDialog.cpp
