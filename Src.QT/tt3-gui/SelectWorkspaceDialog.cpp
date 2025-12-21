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
SelectWorkspaceDialog::SelectWorkspaceDialog(
        QWidget * parent,
        OptionalControls optionalControls
    ) : QDialog(parent),
        _ui(new Ui::SelectWorkspaceDialog)
{
    tt3::util::ResourceReader rr(Component::Resources::instance(), RSID(SelectWorkspaceDialog));

    _ui->setupUi(this);
    setWindowTitle(rr.string(RID(Title)));

    //  Populate "Workspace type" combo box
    QList<tt3::ws::WorkspaceType> workspaceTypes =
        tt3::ws::WorkspaceTypeManager::all().values();
    std::sort(
        workspaceTypes.begin(),
        workspaceTypes.end(),
        [](auto a, auto b)
        {
            return tt3::util::NaturalStringOrder::less(
                a->displayName(), b->displayName()); 
        });
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
    _ui->openAsDefaultRadioButton->setText(
        rr.string(RID(OpenAsDefaultRadioButton)));
    _ui->openAsReadOnlyRadioButton->setText(
        rr.string(RID(OpenAsReadOnlyRadioButton)));
    _ui->openAsReadWriteRadioButton->setText(
        rr.string(RID(OpenAsReadWriteRadioButton)));

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
        tt3::ws::WorkspaceTypeManager::find(
            Component::Settings::instance()->lastUsedWorkspaceType));

    //  Hide optional controls if necessary
    if (optionalControls != OptionalControls::OpenModeSelection)
    {
        _ui->openAsLabel->setVisible(false);
        _ui->openAsPanel->setVisible(false);
    }

    //  Done
    _refresh();
    adjustSize();
    _ui->workspaceTypeComboBox->setFocus();
}

SelectWorkspaceDialog::~SelectWorkspaceDialog()
{
    delete _ui;
}

//////////
//  Operations
auto SelectWorkspaceDialog::doModal(
    ) -> Result
{
    return Result(this->exec());
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
    _ui->locationLineEdit->setText((_workspaceAddress != nullptr) ? _workspaceAddress->displayForm() : "");
    _ui->buttonBox->button(QDialogButtonBox::StandardButton::Ok)->setEnabled(_workspaceAddress != nullptr);
}

auto SelectWorkspaceDialog::_selectedWorkspaceType(
    ) -> tt3::ws::WorkspaceType
{
    return (_ui->workspaceTypeComboBox->currentIndex() >= 0) ?
               _ui->workspaceTypeComboBox->currentData(Qt::ItemDataRole::UserRole).value<tt3::ws::WorkspaceType>() :
                nullptr;
}

void SelectWorkspaceDialog::_setSelectedWorkspaceType(
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
void SelectWorkspaceDialog::_workspaceTypeComboBoxCurrentIndexChanged(int)
{
    Q_ASSERT(_ui->workspaceTypeComboBox->currentIndex() != -1);
    Q_ASSERT(_selectedWorkspaceType() != nullptr);
    if (_workspaceAddress != nullptr &&
        _workspaceAddress->workspaceType() != _selectedWorkspaceType())
    {   //  Need to reset the workspace address
        _workspaceAddress = nullptr;
    }
    _refresh();
}

void SelectWorkspaceDialog::_browsePushButtonClicked()
{
    Q_ASSERT(_ui->workspaceTypeComboBox->currentIndex() != -1);
    tt3::ws::WorkspaceType workspaceType =
        _ui->workspaceTypeComboBox->currentData().value<tt3::ws::WorkspaceType>();
    Q_ASSERT(workspaceType != nullptr);

    if (auto workspaceAddress = workspaceType->enterExistingWorkspaceAddress(this))
    {
        _workspaceAddress = workspaceAddress;
        _refresh();
    }
}

void SelectWorkspaceDialog::accept()
{
    if (_ui->openAsReadOnlyRadioButton->isChecked())
    {
        _openMode = tt3::ws::OpenMode::ReadOnly;
    }
    else if (_ui->openAsReadWriteRadioButton->isChecked())
    {
        _openMode = tt3::ws::OpenMode::ReadWrite;
    }
    else
    {
        _openMode = tt3::ws::OpenMode::Default;
    }
    Component::Settings::instance()->lastUsedWorkspaceType = _selectedWorkspaceType()->mnemonic();
    done(int(Result::Ok));
}

void SelectWorkspaceDialog::reject()
{
    done(int(Result::Cancel));
}

//  End of tt3-gui/SelectWorkspaceDialog.cpp
