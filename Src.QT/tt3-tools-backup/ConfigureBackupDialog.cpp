//
//  tt3-tools-backup/ConfigureBackupDialog.cpp - tt3::tools::backup::ConfigureBackupDialog class implementation
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
#include "tt3-tools-backup/API.hpp"
#include "ui_ConfigureBackupDialog.h"
using namespace tt3::tools::backup;

//////////
//  Construction/destruction
ConfigureBackupDialog::ConfigureBackupDialog(
        QWidget * parent
    ) : QDialog(parent),
        _ui(new Ui::ConfigureBackupDialog)
{
    tt3::util::ResourceReader rr(Component::Resources::instance(), RSID(ConfigureBackupDialog));

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
            return a->displayName() < b->displayName(); // Example: ascending order
        });
    for (auto workspaceType : qAsConst(workspaceTypes))
    {
        _ui->workspaceTypeComboBox->addItem(
            workspaceType->smallIcon(),
            workspaceType->displayName(),
            QVariant::fromValue(workspaceType));
    }

    //  Set static control values
    _ui->backupCurrentWorkspaceRadioButton->setText(
        rr.string(RID(BackupCurrentWorkspaceRadioButton)));
    _ui->orLabel->setText(
        rr.string(RID(OrLabel)));
    _ui->backupCustomWorkspaceRadioButton->setText(
        rr.string(RID(BackupCustomWorkspaceRadioButton)));
    _ui->workspaceTypeLabel->setText(
        rr.string(RID(WorkspaceTypeLabel)));
    _ui->locationLabel->setText(
        rr.string(RID(LocationLabel)));
    _ui->browsePushButton->setText(
        rr.string(RID(BrowsePushButton)));
    _ui->backupToLabel->setText(
        rr.string(RID(BackupToLabel)));
    _ui->backupToPushButton->setText(
        rr.string(RID(BackupToPushButton)));

    _ui->buttonBox->button(QDialogButtonBox::StandardButton::Ok)->
        setText(rr.string(RID(OkPushButton)));
    _ui->buttonBox->button(QDialogButtonBox::StandardButton::Ok)->
        setIcon(QIcon(":/tt3-tools-backup/Resources/Images/Actions/OkSmall.png"));
    _ui->buttonBox->button(QDialogButtonBox::StandardButton::Cancel)->
        setText(rr.string(RID(CancelPushButton)));
    _ui->buttonBox->button(QDialogButtonBox::StandardButton::Cancel)->
        setIcon(QIcon(":/tt3-tools-backup/Resources/Images/Actions/CancelSmall.png"));

    //  Set editable control values
    _setSelectedWorkspaceType(
        tt3::ws::WorkspaceTypeManager::findWorkspaceType(
            tt3::gui::Component::Settings::instance()->lastUsedWorkspaceType));

    //  Adjust controls
    if (tt3::gui::theCurrentWorkspace != nullptr)
    {
        _ui->backupCurrentWorkspaceRadioButton->setChecked(true);
    }
    else
    {
        _ui->backupCurrentWorkspaceRadioButton->setEnabled(false);
        _ui->orLabel->setEnabled(false);
        _ui->backupCustomWorkspaceRadioButton->setChecked(true);
    }

    //  Done
    _refresh();
    adjustSize();
}

ConfigureBackupDialog::~ConfigureBackupDialog()
{
    delete _ui;
}

//////////
//  Operations
auto ConfigureBackupDialog::doModal(
    ) -> Result
{
    return Result(this->exec());
}

auto ConfigureBackupDialog::selectedWorkspaceAddress(
    ) const -> tt3::ws::WorkspaceAddress
{
    return _workspaceAddress;
}

QString ConfigureBackupDialog::selectedBackupDestination() const
{
    return _backupDestination;
}

//////////
//  Implementation helpers
void ConfigureBackupDialog::_refresh()
{
    if (_ui->backupCurrentWorkspaceRadioButton->isChecked())
    {   //  Choosing the "current" workspace
        _ui->workspaceTypeLabel->setEnabled(false);
        _ui->workspaceTypeComboBox->setEnabled(false);
        _ui->locationLabel->setEnabled(false);
        _ui->locationLineEdit->setEnabled(false);
        _ui->browsePushButton->setEnabled(false);
        _ui->buttonBox->button(QDialogButtonBox::StandardButton::Ok)->setEnabled(
            !_ui->backupToLineEdit->text().trimmed().isEmpty());
    }
    else
    {   //  Choosing the "custom" workspace
        if (_ui->workspaceTypeComboBox->count() == 0)
        {   //  A special case - no workspace types are available
            _ui->workspaceTypeLabel->setEnabled(false);
            _ui->workspaceTypeComboBox->setEnabled(false);
            _ui->locationLabel->setEnabled(false);
            _ui->locationLineEdit->setEnabled(false);
            _ui->browsePushButton->setEnabled(false);
            _ui->buttonBox->button(QDialogButtonBox::StandardButton::Ok)->setEnabled(false);
        }
        else
        {
            _ui->workspaceTypeLabel->setEnabled(true);
            _ui->workspaceTypeComboBox->setEnabled(true);
            _ui->locationLabel->setEnabled(true);
            _ui->locationLineEdit->setEnabled(true);
            _ui->browsePushButton->setEnabled(true);
            _ui->locationLineEdit->setText(
                (_customWorkspaceAddress == nullptr) ?
                    "" :
                    _customWorkspaceAddress->displayForm());
            _ui->buttonBox->button(QDialogButtonBox::StandardButton::Ok)->setEnabled(
                _customWorkspaceAddress != nullptr &&
                !_ui->backupToLineEdit->text().trimmed().isEmpty());
        }
    }
}

auto ConfigureBackupDialog::_selectedWorkspaceType(
    ) -> tt3::ws::WorkspaceType
{
    return (_ui->workspaceTypeComboBox->currentIndex() >= 0) ?
               _ui->workspaceTypeComboBox->currentData(Qt::ItemDataRole::UserRole).value<tt3::ws::WorkspaceType>() :
               nullptr;
}

void ConfigureBackupDialog::_setSelectedWorkspaceType(
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
void ConfigureBackupDialog::_workspaceSourceRadioButtonClicked()
{
    _refresh();
}

void ConfigureBackupDialog::_workspaceTypeComboBoxCurrentIndexChanged(int)
{
    Q_ASSERT(_ui->workspaceTypeComboBox->currentIndex() != -1);
    Q_ASSERT(_selectedWorkspaceType() != nullptr);
    if (_customWorkspaceAddress != nullptr &&
        _customWorkspaceAddress->workspaceType() != _selectedWorkspaceType())
    {   //  Need to reset the workspace address
        _customWorkspaceAddress = nullptr;
    }
    _refresh();
}

void ConfigureBackupDialog::_browsePushButtonClicked()
{
    Q_ASSERT(_ui->workspaceTypeComboBox->currentIndex() != -1);
    tt3::ws::WorkspaceType workspaceType =
        _ui->workspaceTypeComboBox->currentData().value<tt3::ws::WorkspaceType>();
    Q_ASSERT(workspaceType != nullptr);

    if (auto workspaceAddress = workspaceType->enterExistingWorkspaceAddress(this))
    {
        _customWorkspaceAddress = workspaceAddress;
        _refresh();
    }
}

void ConfigureBackupDialog::_backupToPushButtonClicked()
{
    static Component::Resources *const resources = Component::Resources::instance();   //  idempotent

    QString path =
        QFileDialog::getSaveFileName(
            this,
            resources->string(RSID(ConfigureBackupDialog), RID(BackupToDialogTitle)),
            /*dir =*/ QString(),
            resources->string(RSID(ConfigureBackupDialog), RID(BackupToDialogFilter), BackupTool::PreferredFilenameExtension));
    if (!path.isEmpty())
    {
        _ui->backupToLineEdit->setText(path);
        _refresh();
    }
}

void ConfigureBackupDialog::accept()
{
    if (_ui->backupCurrentWorkspaceRadioButton->isChecked())
    {
        _workspaceAddress = tt3::gui::theCurrentWorkspace->address();
    }
    else
    {
        _workspaceAddress = _customWorkspaceAddress;
        tt3::gui::Component::Settings::instance()->lastUsedWorkspaceType =
            _selectedWorkspaceType()->mnemonic();
    }
    _backupDestination =
        QFileInfo(_ui->backupToLineEdit->text().trimmed()).absoluteFilePath();
    done(int(Result::Ok));
}

void ConfigureBackupDialog::reject()
{
    done(int(Result::Cancel));
}

//  End of tt3-tools-backup/ConfigureBackupDialog.cpp
