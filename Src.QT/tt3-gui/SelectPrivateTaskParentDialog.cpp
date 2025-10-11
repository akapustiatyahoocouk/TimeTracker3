//
//  tt3-gui/SelectPrivateTaskParentDialog.cpp - tt3::gui::SelectPrivateTaskParentDialog class implementation
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
#include "ui_SelectPrivateTaskParentDialog.h"
using namespace tt3::gui;

//////////
//  Construction/destruction
SelectPrivateTaskParentDialog::SelectPrivateTaskParentDialog(
        QWidget * parent,
        tt3::ws::PrivateTask privateTask,
        const tt3::ws::Credentials & credentials,
        tt3::ws::PrivateTask initialParentTask
    ) : SelectPrivateTaskParentDialog(
            parent,
            privateTask->owner(credentials),
            credentials,
            initialParentTask)
{
    _privateTask = privateTask;
    _refresh();

    //  Set initial control values
    _ui->promptLabel->setText(
        _prompt("Select new parent for private task", _privateTask));
    _ui->showCompletedTasksCheckBox->setChecked(
        _privateTask->completed(_credentials));
}

SelectPrivateTaskParentDialog::SelectPrivateTaskParentDialog(
        QWidget * parent,
        tt3::ws::User owner,
        const tt3::ws::Credentials & credentials,
        tt3::ws::PrivateTask initialParentTask
    ) : QDialog(parent),
        //  Implementation
        _owner(owner),
        _privateTask(nullptr),
        _credentials(credentials),
        _selectedParentTask(initialParentTask),
        //  Controls
        _ui(new Ui::SelectPrivateTaskParentDialog),
        _refreshTimer(this)
{
    Q_ASSERT(_owner != nullptr);

    _ui->setupUi(this);

    _ui->buttonBox->button(QDialogButtonBox::StandardButton::Ok)->
        setIcon(QIcon(":/tt3-gui/Resources/Images/Actions/OkSmall.png"));
    _ui->buttonBox->button(QDialogButtonBox::StandardButton::Cancel)->
        setIcon(QIcon(":/tt3-gui/Resources/Images/Actions/CancelSmall.png"));

    //  Populate User combo box & select the proper user
    QList<tt3::ws::User> usersList =
        _owner->workspace()->users(_credentials).values();   //  may throw
    std::sort(usersList.begin(),
              usersList.end(),
              [&](auto a, auto b)
              {
                  return a->realName(_credentials) < b->realName(_credentials);   //  may throw
              });
    for (tt3::ws::User u : usersList)
    {
        _ui->userComboBox->addItem(
            u->type()->smallIcon(),
            u->realName(_credentials),  //  may throw
            QVariant::fromValue(u));
    }
    _setSelectedUser(_owner);
    _ui->userComboBox->setEnabled(false);

    //  Set initial control values
    _ui->promptLabel->setText("Select new parent for public task");
    _ui->showCompletedTasksCheckBox->setChecked(false);
    _decorations = TreeWidgetDecorations(_ui->privateTasksTreeWidget);

    _refresh(); //  NOW, to adjust tree widget size to content
    _ui->privateTasksTreeWidget->expandAll();
    _setSelectedPrivateTask(_selectedParentTask);

    //  Start refreshing on timer
    //  We need to connect() by name (old-style)
    //  because we privately inherit from QDialog
    connect(&_refreshTimer,
            SIGNAL(timeout()),
            this,
            SLOT(_refreshTimerTimeout()));
    _refreshTimer.start(1000);

    //  Done
    _ui->privateTasksTreeWidget->setFocus();
    _trackItemStateChanges = true;
    adjustSize();
}

SelectPrivateTaskParentDialog::~SelectPrivateTaskParentDialog()
{
    _refreshTimer.stop();
    delete _ui;
}

//////////
//  Operations
auto SelectPrivateTaskParentDialog::doModal(
    ) -> SelectPrivateTaskParentDialog::Result
{
    return Result(this->exec());
}

//////////
//  Model
void SelectPrivateTaskParentDialog::_removeReparentedTask(
        PrivateTaskManager::_WorkspaceModel workspaceModel
    )
{
    for (qsizetype i = workspaceModel->userModels.size() - 1; i >= 0; i--)
    {
        _removeReparentedTask(workspaceModel->userModels[i]);
    }
}

void SelectPrivateTaskParentDialog::_removeReparentedTask(
        PrivateTaskManager::_UserModel userModel
    )
{
    for (qsizetype i = userModel->privateTaskModels.size() - 1; i >= 0; i--)
    {
        if (userModel->privateTaskModels[i]->privateTask == _privateTask)
        {
            userModel->privateTaskModels.removeAt(i);
        }
        else
        {
            _removeReparentedTask(userModel->privateTaskModels[i]);
        }
    }
}

void SelectPrivateTaskParentDialog::_removeReparentedTask(
        PrivateTaskManager::_PrivateTaskModel privateTaskModel
    )
{
    for (qsizetype i = privateTaskModel->childModels.size() - 1; i >= 0; i--)
    {
        if (privateTaskModel->childModels[i]->privateTask == _privateTask)
        {
            privateTaskModel->childModels.removeAt(i);
        }
        else
        {
            _removeReparentedTask(privateTaskModel->childModels[i]);
        }
    }
}

void SelectPrivateTaskParentDialog::_refreshWorkspaceTree(
        PrivateTaskManager::_UserModel userModel
    )
{
    Q_ASSERT(userModel != nullptr);

    //  Make sure the "private tasks" tree contains
    //  a proper number of root (User) items...
    while (_ui->privateTasksTreeWidget->topLevelItemCount() < userModel->privateTaskModels.size())
    {   //  Too few root (User) items
        _ui->privateTasksTreeWidget->addTopLevelItem(new QTreeWidgetItem());
    }
    while (_ui->privateTasksTreeWidget->topLevelItemCount() > userModel->privateTaskModels.size())
    {   //  Too many root (User) items
        delete _ui->privateTasksTreeWidget->takeTopLevelItem(
            _ui->privateTasksTreeWidget->topLevelItemCount() - 1);
    }
    //  ...and that each top-level item represents
    //  a proper root PrivateTask
    for (int i = 0; i < userModel->privateTaskModels.size(); i++)
    {
        PrivateTaskManager::_refreshPrivateTaskItem(
            _ui->privateTasksTreeWidget->topLevelItem(i),
            userModel->privateTaskModels[i]);
    }
}

//////////
//  Implementation helpers
void SelectPrivateTaskParentDialog::_refresh()
{
    //  We don't want a refresh() to trigger a recursive refresh()!
    static bool refreshUnderway = false;
    RefreshGuard refreshGuard(refreshUnderway);
    if (refreshGuard)   //  Don't recurse!
    {
        PrivateTaskManager::_UserModel userModel =
            PrivateTaskManager::_createUserModel(
                _owner, _credentials, _decorations);
        _removeReparentedTask(userModel);
        if (!_ui->showCompletedTasksCheckBox->isChecked())
        {
            PrivateTaskManager::_removeCompletedItems(userModel, _credentials);
        }
        QString filter = _ui->filterLineEdit->text().trimmed();
        if (!filter.isEmpty())
        {
            PrivateTaskManager::_filterItems(
                userModel, filter, _decorations);
        }
        _trackItemStateChanges = false;
        _refreshWorkspaceTree(userModel);
        _refreshCheckStates();
        _trackItemStateChanges = true;
        if (!_ui->filterLineEdit->text().trimmed().isEmpty())
        {   //  Filtered - show all
            _ui->privateTasksTreeWidget->expandAll();
        }

        _ui->selectionLabel->setText(
            _prompt("Selected parent task", _selectedParentTask));
    }
}

void SelectPrivateTaskParentDialog::_refreshCheckStates()
{
    for (int i = 0; i < _ui->privateTasksTreeWidget->topLevelItemCount(); i++)
    {
        _refreshCheckStates(_ui->privateTasksTreeWidget->topLevelItem(i));
    }
}

void SelectPrivateTaskParentDialog::_refreshCheckStates(QTreeWidgetItem * item)
{
    tt3::ws::PrivateTask privateTask =
        item->data(0, Qt::ItemDataRole::UserRole).value<tt3::ws::PrivateTask>();
    item->setFlags(item->flags() | Qt::ItemFlag::ItemIsUserCheckable);
    item->setCheckState(
        0,
        (privateTask == _selectedParentTask) ?
            Qt::CheckState::Checked :
            Qt::CheckState::Unchecked);
    for (int i = 0; i < item->childCount(); i++)
    {
        _refreshCheckStates(item->child(i));
    }
}

void SelectPrivateTaskParentDialog::_setSelectedUser(tt3::ws::User user)
{
    for (int i = 0; i < _ui->userComboBox->count(); i++)
    {
        if (_ui->userComboBox->itemData(i).value<tt3::ws::User>() == user)
        {
            _ui->userComboBox->setCurrentIndex(i);
            _setSelectedPrivateTask(nullptr);
            break;
        }
    }
}


bool SelectPrivateTaskParentDialog::_setSelectedPrivateTask(tt3::ws::PrivateTask privateTask)
{
    for (int i = 0; i < _ui->privateTasksTreeWidget->topLevelItemCount(); i++)
    {
        if (_setSelectedPrivateTask(_ui->privateTasksTreeWidget->topLevelItem(i), privateTask))
        {
            return true;
        }
    }
    return false;
}

bool SelectPrivateTaskParentDialog::_setSelectedPrivateTask(QTreeWidgetItem * item, tt3::ws::PrivateTask privateTask)
{
    if (privateTask == item->data(0, Qt::ItemDataRole::UserRole).value<tt3::ws::PrivateTask>())
    {
        _ui->privateTasksTreeWidget->setCurrentItem(item);
        return true;
    }
    for (int i = 0; i < item->childCount(); i++)
    {
        if (_setSelectedPrivateTask(item->child(i), privateTask))
        {
            return true;
        }
    }
    return false;
}

QStringList SelectPrivateTaskParentDialog::_taskDisplayPath(tt3::ws::PrivateTask privateTask)
{
    QStringList result;
    if (privateTask == nullptr)
    {
        result.append("-");
    }
    else if (auto parentTask = privateTask->parent(_credentials))
    {
        result = _taskDisplayPath(parentTask);
        result.append(privateTask->displayName(_credentials));
    }
    else
    {
        result.append(privateTask->displayName(_credentials));
    }
    return result;
}

QString SelectPrivateTaskParentDialog::_prompt(const QString & prompt, tt3::ws::PrivateTask privateTask)
{
    QStringList path = _taskDisplayPath(privateTask);
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
void SelectPrivateTaskParentDialog::_privateTasksTreeWidgetItemChanged(
    QTreeWidgetItem * item, int /*column*/)
{
    if (_trackItemStateChanges)
    {
        tt3::ws::PrivateTask privateTask =
            item->data(0, Qt::ItemDataRole::UserRole).value<tt3::ws::PrivateTask>();
        if (item->checkState(0) == Qt::Checked &&
            privateTask != _selectedParentTask)
        {   //  Item is newly "checked" - select as "parent"
            _selectedParentTask = privateTask;
            _setSelectedPrivateTask(privateTask);
        }
        else if (item->checkState(0) == Qt::Unchecked &&
                 privateTask == _selectedParentTask)
        {   //  Item is newly "unckecked" - unselect IF parent
            _selectedParentTask = nullptr;
            _setSelectedPrivateTask(privateTask);
        }
    }
    _refresh();
}

void SelectPrivateTaskParentDialog::_privateTasksTreeWidgetItemDoubleClicked(QTreeWidgetItem * item)
{
    _selectedParentTask =
        item->data(0, Qt::ItemDataRole::UserRole).value<tt3::ws::PrivateTask>();
    _refresh();
    accept();
}

void SelectPrivateTaskParentDialog::_refreshTimerTimeout()
{
    _refresh();
}

void SelectPrivateTaskParentDialog::accept()
{
    done(int(Result::Ok));
}

void SelectPrivateTaskParentDialog::reject()
{
    done(int(Result::Cancel));
}

//  End of tt3-gui/SelectPrivateTaskParentDialog.cpp
