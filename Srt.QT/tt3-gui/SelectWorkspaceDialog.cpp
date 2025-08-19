//
//  tt3-gui/SelectWorkspaceDialog.cpp - tt3::gui::SelectWorkspaceDialog class implementation
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
#include "ui_SelectWorkspaceDialog.h"
using namespace tt3::gui;

//////////
//  Construction/destruction
SelectWorkspaceDialog::SelectWorkspaceDialog(QWidget * parent)
    :   QDialog(parent),
        _ui(new Ui::SelectWorkspaceDialog)
{
    _ui->setupUi(this);

    _ui->buttonBox->button(QDialogButtonBox::StandardButton::Ok)->
        setIcon(QIcon(":/tt3-gui/Resources/Images/Actions/OkSmall.png"));
    _ui->buttonBox->button(QDialogButtonBox::StandardButton::Cancel)->
        setIcon(QIcon(":/tt3-gui/Resources/Images/Actions/CancelSmall.png"));

    //  Populate "Workspace type" combo box
    QList<tt3::ws::WorkspaceType*> workspaceTypes =
        tt3::ws::WorkspaceTypeRegistry::allWorkspaceTypes().values();
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
    _refresh();
}

SelectWorkspaceDialog::~SelectWorkspaceDialog()
{
    delete _ui;
}

//////////
//  Implementation helpers
void SelectWorkspaceDialog::_refresh()
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
    _ui->locationLineEdit->setText(_workspaceAddress.displayForm());
    _ui->buttonBox->button(QDialogButtonBox::StandardButton::Ok)->setEnabled(_workspaceAddress.isValid());
}

//////////
//  Event handlers
void SelectWorkspaceDialog::_workspaceTypeComboBoxCurrentIndexChanged(int)
{
    Q_ASSERT(_ui->workspaceTypeComboBox->currentIndex() != -1);
    tt3::ws::WorkspaceType * workspaceType =
        _ui->workspaceTypeComboBox->currentData().value<tt3::ws::WorkspaceType*>();
    Q_ASSERT(workspaceType != nullptr);
    if (_workspaceAddress.isValid() && _workspaceAddress.workspaceType() != workspaceType)
    {   //  Need to reset the workspace address
        _workspaceAddress = tt3::ws::WorkspaceAddress();
    }
    _refresh();
}

void SelectWorkspaceDialog::_browsePushButtonClicked()
{
    Q_ASSERT(_ui->workspaceTypeComboBox->currentIndex() != -1);
    tt3::ws::WorkspaceType * workspaceType =
        _ui->workspaceTypeComboBox->currentData().value<tt3::ws::WorkspaceType*>();
    Q_ASSERT(workspaceType != nullptr);
    tt3::ws::WorkspaceAddress workspaceAddress = workspaceType->enterExistingWorkspaceAddress(this);
    if (workspaceAddress.isValid())
    {
        _workspaceAddress = workspaceAddress;
        _refresh();
    }
}

//  End of tt3-gui/SelectWorkspaceDialog.cpp
