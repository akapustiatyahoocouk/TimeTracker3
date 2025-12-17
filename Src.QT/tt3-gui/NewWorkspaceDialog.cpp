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
    tt3::util::ResourceReader rr(Component::Resources::instance(), RSID(NewWorkspaceDialog));

    _ui->setupUi(this);
    setWindowTitle(rr.string(RID(Title)));

    //  Populate "Workspace type" combo box
    QList<tt3::ws::WorkspaceType> workspaceTypes =
        tt3::ws::WorkspaceTypeManager::allWorkspaceTypes().values();
    std::sort(
        workspaceTypes.begin(),
        workspaceTypes.end(),
        [](auto a, auto b)
        {
            return tt3::util::NaturalStringOrder::less(
                a->displayName(),
                b->displayName());
        }
        );
    for (auto workspaceType : std::as_const(workspaceTypes))
    {
        _ui->workspaceTypeComboBox->addItem(
            workspaceType->smallIcon(),
            workspaceType->displayName(),
            QVariant::fromValue(workspaceType));
    }

    //  Set static control values
    _ui->workspaceTypeLabel->setText(
        rr.string(RID(WorkspaceTypeLabel)));
    _ui->locationLabel->setText(
        rr.string(RID(LocationLabel)));
    _ui->browsePushButton->setText(
        rr.string(RID(BrowsePushButton)));
    _ui->userNameLabel->setText(
        rr.string(RID(UserNameLabel)));
    _ui->loginLabel->setText(
        rr.string(RID(LoginLabel)));
    _ui->passwordLabel->setText(
        rr.string(RID(PasswordLabel)));
    _ui->confirmPasswordLabel->setText(
        rr.string(RID(ConfirmPasswordLabel)));

    _ui->buttonBox->button(QDialogButtonBox::StandardButton::Ok)->
        setText(rr.string(RID(OkPushButton)));
    _ui->buttonBox->button(QDialogButtonBox::StandardButton::Ok)->
        setIcon(QIcon(":/tt3-gui/Resources/Images/Actions/OkSmall.png"));
    _ui->buttonBox->button(QDialogButtonBox::StandardButton::Cancel)->
        setText(rr.string(RID(CancelPushButton)));
    _ui->buttonBox->button(QDialogButtonBox::StandardButton::Cancel)->
        setIcon(QIcon(":/tt3-gui/Resources/Images/Actions/CancelSmall.png"));

    //  Set editable control values
    _setSelectedWorkspaceType(
        tt3::ws::WorkspaceTypeManager::findWorkspaceType(
            Component::Settings::instance()->lastUsedWorkspaceType));

    //  Done
    _refresh();
    adjustSize();
    _ui->workspaceTypeComboBox->setFocus();
}

NewWorkspaceDialog::~NewWorkspaceDialog()
{
    delete _ui;
}

//////////
//  Operations
auto NewWorkspaceDialog::doModal(
    ) -> Result
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
    tt3::ws::WorkspaceType workspaceType = _selectedWorkspaceType();
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

auto NewWorkspaceDialog::_selectedWorkspaceType(
    ) -> tt3::ws::WorkspaceType
{
    return (_ui->workspaceTypeComboBox->currentIndex() >= 0) ?
               _ui->workspaceTypeComboBox->currentData(Qt::ItemDataRole::UserRole).value<tt3::ws::WorkspaceType>() :
               nullptr;
}

void NewWorkspaceDialog::_setSelectedWorkspaceType(
        tt3::ws::WorkspaceType workspaceType
    )
{
    for (int i = 0; i < _ui->workspaceTypeComboBox->count(); i++)
    {
        if (workspaceType == _ui->workspaceTypeComboBox->itemData(i, Qt::ItemDataRole::UserRole).value<tt3::ws::WorkspaceType>())
        {
            _ui->workspaceTypeComboBox->setCurrentIndex(i);
            break;
        }
    }
}

//////////
//  Signal handlers
void NewWorkspaceDialog::_workspaceTypeComboBoxCurrentIndexChanged(int)
{
    Q_ASSERT(_ui->workspaceTypeComboBox->currentIndex() != -1);
    if (_workspaceAddress != nullptr &&
        _workspaceAddress->workspaceType() != _selectedWorkspaceType())
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
    Component::Settings::instance()->lastUsedWorkspaceType = _selectedWorkspaceType()->mnemonic();
    done(int(Result::Ok));
}

void NewWorkspaceDialog::reject()
{
    done(int(Result::Cancel));
}

//  End of tt3-gui/NewWorkspaceDialog.cpp
