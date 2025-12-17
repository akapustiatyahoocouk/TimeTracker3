//
//  tt3-tools-restore/ConfigureRestoreDialog.cpp - tt3::tools::restore::ConfigureRestoreDialog class implementation
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
#include "tt3-tools-restore/API.hpp"
#include "ui_ConfigureRestoreDialog.h"
using namespace tt3::tools::restore;

//////////
//  Construction/destruction
ConfigureRestoreDialog::ConfigureRestoreDialog(
        QWidget * parent
    ) : QDialog(parent),
        _ui(new Ui::ConfigureRestoreDialog)
{
    tt3::util::ResourceReader rr(Component::Resources::instance(), RSID(ConfigureRestoreDialog));

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
        });
    for (auto workspaceType : std::as_const(workspaceTypes))
    {
        _ui->workspaceTypeComboBox->addItem(
            workspaceType->smallIcon(),
            workspaceType->displayName(),
            QVariant::fromValue(workspaceType));
    }

    //  Set static control values
    _ui->restoreFromLabel->setText(
        rr.string(RID(RestoreFromLabel)));
    _ui->restoreFromPushButton->setText(
        rr.string(RID(RestoreFromPushButton)));
    _ui->restoreIntoLabel->setText(
        rr.string(RID(RestoreIntoLabel)));
    _ui->workspaceTypeLabel->setText(
        rr.string(RID(WorkspaceTypeLabel)));
    _ui->locationLabel->setText(
        rr.string(RID(LocationLabel)));
    _ui->browsePushButton->setText(
        rr.string(RID(BrowsePushButton)));

    _ui->buttonBox->button(QDialogButtonBox::StandardButton::Ok)->
        setText(rr.string(RID(OkPushButton)));
    _ui->buttonBox->button(QDialogButtonBox::StandardButton::Ok)->
        setIcon(QIcon(":/tt3-tools-restore/Resources/Images/Actions/OkSmall.png"));
    _ui->buttonBox->button(QDialogButtonBox::StandardButton::Cancel)->
        setText(rr.string(RID(CancelPushButton)));
    _ui->buttonBox->button(QDialogButtonBox::StandardButton::Cancel)->
        setIcon(QIcon(":/tt3-tools-restore/Resources/Images/Actions/CancelSmall.png"));

    //  Set editable control values
    _setSelectedWorkspaceType(
        tt3::ws::WorkspaceTypeManager::findWorkspaceType(
            tt3::gui::Component::Settings::instance()->lastUsedWorkspaceType));

    //  Done
    _refresh();
    adjustSize();
    _ui->workspaceTypeComboBox->setFocus();
}

ConfigureRestoreDialog::~ConfigureRestoreDialog()
{
    delete _ui;
}

//////////
//  Operations
auto ConfigureRestoreDialog::doModal(
    ) -> Result
{
    return Result(this->exec());
}

QString ConfigureRestoreDialog::restoreSource() const
{
    return _restoreSource;
}

auto ConfigureRestoreDialog::workspaceAddress(
    ) const -> tt3::ws::WorkspaceAddress
{
    return _workspaceAddress;
}

//////////
//  Implementation helpers
void ConfigureRestoreDialog::_refresh()
{
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
            (_workspaceAddress == nullptr) ?
                "" :
                _workspaceAddress->displayForm());
        _ui->buttonBox->button(QDialogButtonBox::StandardButton::Ok)->setEnabled(
            !_ui->restoreFromLineEdit->text().trimmed().isEmpty() &&
            _workspaceAddress != nullptr);
    }
}

auto ConfigureRestoreDialog::_selectedWorkspaceType(
    ) -> tt3::ws::WorkspaceType
{
    return (_ui->workspaceTypeComboBox->currentIndex() >= 0) ?
               _ui->workspaceTypeComboBox->currentData(Qt::ItemDataRole::UserRole).value<tt3::ws::WorkspaceType>() :
               nullptr;
}

void ConfigureRestoreDialog::_setSelectedWorkspaceType(
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
void ConfigureRestoreDialog::_restoreFromPushButtonClicked()
{
    static Component::Resources *const resources = Component::Resources::instance();   //  idempotent

    QString path =
        QFileDialog::getOpenFileName(
            this,
            resources->string(RSID(ConfigureRestoreDialog), RID(RestoreFromDialogTitle)),
            /*dir =*/ QString(),
            resources->string(RSID(ConfigureRestoreDialog), RID(RestoreFromDialogFilter), RestoreTool::PreferredExtension));
    if (!path.isEmpty())
    {
        _ui->restoreFromLineEdit->setText(path);
        _refresh();
    }
}

void ConfigureRestoreDialog::_workspaceTypeComboBoxCurrentIndexChanged(int)
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

void ConfigureRestoreDialog::_browsePushButtonClicked()
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

void ConfigureRestoreDialog::accept()
{
    tt3::gui::Component::Settings::instance()->lastUsedWorkspaceType =
        _selectedWorkspaceType()->mnemonic();
    _restoreSource =
        QFileInfo(_ui->restoreFromLineEdit->text().trimmed()).absoluteFilePath();
    done(int(Result::Ok));
}

void ConfigureRestoreDialog::reject()
{
    done(int(Result::Cancel));
}

//  End of tt3-tools-restore/ConfigureRestoreDialog.cpp
