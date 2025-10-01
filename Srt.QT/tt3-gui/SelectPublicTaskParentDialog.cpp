//
//  tt3-gui/SelectPublicTaskParentDialog.cpp - tt3::gui::SelectPublicTaskParentDialog class implementation
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
#include "ui_SelectPublicTaskParentDialog.h"
using namespace tt3::gui;

//////////
//  Construction/destruction
SelectPublicTaskParentDialog::SelectPublicTaskParentDialog(
        QWidget * parent,
        tt3::ws::PublicTask publicTask,
        const tt3::ws::Credentials & credentials,
        tt3::ws::PublicTask initialParentTask
    ) : SelectPublicTaskParentDialog(
            parent,
            publicTask->workspace(),
            credentials,
            initialParentTask)
{
    _publicTask = publicTask;

    //  Set initial control values
    _ui->promptLabel->setText(
        _prompt("Select new parent for public task", _publicTask));
    _ui->showCompletedTasksCheckBox->setChecked(
        _publicTask->completed(_credentials));
}

SelectPublicTaskParentDialog::SelectPublicTaskParentDialog(
        QWidget * parent,
        tt3::ws::Workspace workspace,
        const tt3::ws::Credentials & credentials,
        tt3::ws::PublicTask initialParentTask
    ) : QDialog(parent),
        //  Implementation
        _workspace(workspace),
        _publicTask(nullptr),
        _credentials(credentials),
        _selectedParentTask(initialParentTask),
        //  Controls
        _ui(new Ui::SelectPublicTaskParentDialog),
        _refreshTimer(this)
{
    Q_ASSERT(workspace != nullptr);

    _ui->setupUi(this);

    _ui->buttonBox->button(QDialogButtonBox::StandardButton::Ok)->
        setIcon(QIcon(":/tt3-gui/Resources/Images/Actions/OkSmall.png"));
    _ui->buttonBox->button(QDialogButtonBox::StandardButton::Cancel)->
        setIcon(QIcon(":/tt3-gui/Resources/Images/Actions/CancelSmall.png"));

    //  Set initial control values
    _ui->promptLabel->setText("Select new parent for public task");
    _ui->showCompletedTasksCheckBox->setChecked(false);
    _decorations = TreeWidgetDecorations(_ui->publicTasksTreeWidget);

    _refresh(); //  NOW, to adjust tree widget size to content
    _ui->publicTasksTreeWidget->expandAll();
    _setSelectedPublicTask(_selectedParentTask);
    _ui->publicTasksTreeWidget->setFocus();

    //  Start refreshing on timer
    //  We need to connect() by name (old-style)
    //  because we privately inherit from QDialog
    connect(&_refreshTimer,
            SIGNAL(timeout()),
            this,
            SLOT(_refreshTimerTimeout()));
    _refreshTimer.start(1000);

    //  Done
    _trackItemStateChanges = true;
    adjustSize();
}

SelectPublicTaskParentDialog::~SelectPublicTaskParentDialog()
{
    _refreshTimer.stop();
    delete _ui;
}

//////////
//  Operations
auto SelectPublicTaskParentDialog::doModal(
    ) -> SelectPublicTaskParentDialog::Result
{
    return Result(this->exec());
}

//////////
//  Model
void SelectPublicTaskParentDialog::_removeReparentedTask(
        PublicTaskManager::_WorkspaceModel workspaceModel
    )
{
    for (qsizetype i = workspaceModel->publicTaskModels.size() - 1; i >= 0; i--)
    {
        if (workspaceModel->publicTaskModels[i]->publicTask == _publicTask)
        {
            workspaceModel->publicTaskModels.removeAt(i);
        }
        else
        {
            _removeReparentedTask(workspaceModel->publicTaskModels[i]);
        }
    }
}

void SelectPublicTaskParentDialog::_removeReparentedTask(
        PublicTaskManager::_PublicTaskModel publicTaskModel
    )
{
    for (qsizetype i = publicTaskModel->childModels.size() - 1; i >= 0; i--)
    {
        if (publicTaskModel->childModels[i]->publicTask == _publicTask)
        {
            publicTaskModel->childModels.removeAt(i);
        }
        else
        {
            _removeReparentedTask(publicTaskModel->childModels[i]);
        }
    }
}

//////////
//  Implementation helpers
void SelectPublicTaskParentDialog::_refresh()
{
    //  We don't want a refresh() to trigger a recursive refresh()!
    static bool refreshUnderway = false;
    RefreshGuard refreshGuard(refreshUnderway);
    if (refreshGuard)   //  Don't recurse!
    {
        PublicTaskManager::_WorkspaceModel workspaceModel =
            PublicTaskManager::_createWorkspaceModel(
                _workspace, _credentials, _decorations);
        _removeReparentedTask(workspaceModel);
        if (!_ui->showCompletedTasksCheckBox->isChecked())
        {
            PublicTaskManager::_removeCompletedItems(workspaceModel, _credentials);
        }
        QString filter = _ui->filterLineEdit->text().trimmed();
        if (!filter.isEmpty())
        {
            PublicTaskManager::_filterItems(
                workspaceModel, filter, _decorations);
        }
        _trackItemStateChanges = false;
        PublicTaskManager::_refreshWorkspaceItem(
            _ui->publicTasksTreeWidget, workspaceModel);
        _refreshCheckStates();
        _trackItemStateChanges = true;
        if (!_ui->filterLineEdit->text().trimmed().isEmpty())
        {   //  Filtered - show all
            _ui->publicTasksTreeWidget->expandAll();
        }

        _ui->selectionLabel->setText(
            _prompt("Selected parent task", _selectedParentTask));
    }
}

void SelectPublicTaskParentDialog::_refreshCheckStates()
{
    for (int i = 0; i < _ui->publicTasksTreeWidget->topLevelItemCount(); i++)
    {
        _refreshCheckStates(_ui->publicTasksTreeWidget->topLevelItem(i));
    }
}

void SelectPublicTaskParentDialog::_refreshCheckStates(QTreeWidgetItem * item)
{
    tt3::ws::PublicTask publicTask =
        item->data(0, Qt::ItemDataRole::UserRole).value<tt3::ws::PublicTask>();
    item->setFlags(item->flags() | Qt::ItemFlag::ItemIsUserCheckable);
    item->setCheckState(
        0,
        (publicTask == _selectedParentTask) ?
            Qt::CheckState::Checked :
            Qt::CheckState::Unchecked);
    for (int i = 0; i < item->childCount(); i++)
    {
        _refreshCheckStates(item->child(i));
    }
}

bool SelectPublicTaskParentDialog::_setSelectedPublicTask(tt3::ws::PublicTask publicTask)
{
    for (int i = 0; i < _ui->publicTasksTreeWidget->topLevelItemCount(); i++)
    {
        if (_setSelectedPublicTask(_ui->publicTasksTreeWidget->topLevelItem(i), publicTask))
        {
            return true;
        }
    }
    return false;
}

bool SelectPublicTaskParentDialog::_setSelectedPublicTask(QTreeWidgetItem * item, tt3::ws::PublicTask publicTask)
{
    if (publicTask == item->data(0, Qt::ItemDataRole::UserRole).value<tt3::ws::PublicTask>())
    {
        _ui->publicTasksTreeWidget->setCurrentItem(item);
        return true;
    }
    for (int i = 0; i < item->childCount(); i++)
    {
        if (_setSelectedPublicTask(item->child(i), publicTask))
        {
            return true;
        }
    }
    return false;
}

QStringList SelectPublicTaskParentDialog::_taskDisplayPath(tt3::ws::PublicTask publicTask)
{
    QStringList result;
    if (publicTask == nullptr)
    {
        result.append("-");
    }
    else if (auto parentTask = publicTask->parent(_credentials))
    {
        result = _taskDisplayPath(parentTask);
        result.append(publicTask->displayName(_credentials));
    }
    else
    {
        result.append(publicTask->displayName(_credentials));
    }
    return result;
}

QString SelectPublicTaskParentDialog::_prompt(const QString & prompt, tt3::ws::PublicTask publicTask)
{
    QStringList path = _taskDisplayPath(publicTask);
    QString result = prompt;
    for (int i = 0; i < path.length(); i++)
    {
        result += "\n";
        result += (i == 0) ? "    " : QString(i * 4, ' ') + "+   ";
        result += path[i];
    }
    return result;
}

//////////
//  Signal handlers
void SelectPublicTaskParentDialog::_publicTasksTreeWidgetItemChanged(
        QTreeWidgetItem * item, int /*column*/)
{
    if (_trackItemStateChanges)
    {
        tt3::ws::PublicTask publicTask =
            item->data(0, Qt::ItemDataRole::UserRole).value<tt3::ws::PublicTask>();
        if (item->checkState(0) == Qt::Checked &&
            publicTask != _selectedParentTask)
        {   //  Item is mewly "checked" - select as "parent"
            _selectedParentTask = publicTask;
            _setSelectedPublicTask(publicTask);
        }
        else if (item->checkState(0) == Qt::Unchecked &&
                 publicTask == _selectedParentTask)
        {   //  Item is newly "unckecked" - unselect IF parent
            _selectedParentTask = nullptr;
            _setSelectedPublicTask(publicTask);
        }
    }
    _refresh();
}

void SelectPublicTaskParentDialog::_publicTasksTreeWidgetItemDoubleClicked(QTreeWidgetItem * item)
{
    _selectedParentTask =
        item->data(0, Qt::ItemDataRole::UserRole).value<tt3::ws::PublicTask>();
    _refresh();
    accept();
}

void SelectPublicTaskParentDialog::_refreshTimerTimeout()
{
    _refresh();
}

void SelectPublicTaskParentDialog::accept()
{
    done(int(Result::Ok));
}

void SelectPublicTaskParentDialog::reject()
{
    done(int(Result::Cancel));
}

//  End of tt3-gui/SelectPublicTaskParentDialog.cpp
