//
//  tt3-gui/ManageQuickPicksListDialog.cpp - tt3::gui::ManageQuickPicksListDialog class implementation
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
#include "ui_ManageQuickPicksListDialog.h"
using namespace tt3::gui;

//////////
//  Construction/destruction
ManageQuickPicksListDialog::ManageQuickPicksListDialog(
        QWidget * parent,
        tt3::ws::Account account,
        const tt3::ws::Credentials & credentials
    ) : QDialog(parent),
        //  Implementation
        _account(account),
        _credentials(credentials),
        _quickPicksList(account->quickPicksList(credentials)),
        //  Controls
        _ui(new Ui::ManageQuickPicksListDialog)
{
    Q_ASSERT(_account != nullptr);
    Q_ASSERT(_credentials.isValid());

    _ui->setupUi(this);

    _decorations = TreeWidgetDecorations(_ui->publicTasksTreeWidget);

    _ui->buttonBox->button(QDialogButtonBox::StandardButton::Ok)->
        setIcon(QIcon(":/tt3-gui/Resources/Images/Actions/OkSmall.png"));
    _ui->buttonBox->button(QDialogButtonBox::StandardButton::Cancel)->
        setIcon(QIcon(":/tt3-gui/Resources/Images/Actions/CancelSmall.png"));

    //  Set up initial control values
    _refillPublicActivitiesTree();
    _refillPublicTasksTree();
    _refillPrivateActivitiesTree();
    _refillPrivateTasksTree();

    //  Done
    _refresh();
    adjustSize();
    _trackItemStateChanges = true;
}

ManageQuickPicksListDialog::~ManageQuickPicksListDialog()
{
    delete _ui;
}

//////////
//  Operations
ManageQuickPicksListDialog::Result ManageQuickPicksListDialog::doModal()
{
    return Result(this->exec());
}

//////////
//  Model
void ManageQuickPicksListDialog::_refreshWorkspaceTree(
        PrivateActivityManager::_UserModel userModel
    )
{
    Q_ASSERT(userModel != nullptr);

    //  Make sure the "private activities" tree contains
    //  a proper number of root (User) items...
    while (_ui->privateActivitiesTreeWidget->topLevelItemCount() < userModel->privateActivityModels.size())
    {   //  Too few root (PrivateActivity) items
        _ui->privateActivitiesTreeWidget->addTopLevelItem(new QTreeWidgetItem());
    }
    while (_ui->privateActivitiesTreeWidget->topLevelItemCount() > userModel->privateActivityModels.size())
    {   //  Too many root (PrivateActivity) items
        delete _ui->privateActivitiesTreeWidget->takeTopLevelItem(
            _ui->privateActivitiesTreeWidget->topLevelItemCount() - 1);
    }
    //  ...and that each top-level item represents
    //  a proper PrivateActivity
    for (int i = 0; i < userModel->privateActivityModels.size(); i++)
    {
        PrivateActivityManager::_refreshPrivateActivityItem(
            _ui->privateActivitiesTreeWidget->topLevelItem(i),
            userModel->privateActivityModels[i]);
    }
}

void ManageQuickPicksListDialog::_refreshWorkspaceTree(
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
void ManageQuickPicksListDialog::_clearAndDisableAllControls()
{
    _ui->publicActivitiesTreeWidget->clear();
    _ui->publicActivitiesFilterLabel->setText("");  //  TODO for all _clearAndDisableAllControls()s everywhere
    _ui->publicActivitiesFilterLabel->setEnabled(false);
    _ui->publicActivitiesFilterLineEdit->setEnabled(false);
    _ui->publicActivitiesTreeWidget->setEnabled(false);

    _ui->publicTasksTreeWidget->clear();
    _ui->publicTasksFilterLineEdit->setText("");
    _ui->publicTasksFilterLabel->setEnabled(false);
    _ui->publicTasksFilterLineEdit->setEnabled(false);
    _ui->publicTasksTreeWidget->setEnabled(false);

    _ui->privateActivitiesTreeWidget->clear();
    _ui->privateActivitiesFilterLabel->setText("");  //  TODO for all _clearAndDisableAllControls()s everywhere
    _ui->privateActivitiesFilterLabel->setEnabled(false);
    _ui->privateActivitiesFilterLineEdit->setEnabled(false);
    _ui->privateActivitiesTreeWidget->setEnabled(false);

    _ui->privateTasksTreeWidget->clear();
    _ui->privateTasksFilterLineEdit->setText("");
    _ui->privateTasksFilterLabel->setEnabled(false);
    _ui->privateTasksFilterLineEdit->setEnabled(false);
    _ui->privateTasksTreeWidget->setEnabled(false);
}

void ManageQuickPicksListDialog::_refillPublicActivitiesTree()
{
    PublicActivityManager::_WorkspaceModel workspaceModel =
        PublicActivityManager::_createWorkspaceModel(
        _account->workspace(), _credentials, _decorations);
    QString filter = _ui->publicActivitiesFilterLineEdit->text().trimmed();
    if (!filter.isEmpty())
    {
        PublicActivityManager::_filterItems(workspaceModel, filter, _decorations);
    }
    PublicActivityManager::_refreshWorkspaceTree(
        _ui->publicActivitiesTreeWidget,
        workspaceModel);
}

void ManageQuickPicksListDialog::_refillPublicTasksTree()
{
    PublicTaskManager::_WorkspaceModel workspaceModel =
        PublicTaskManager::_createWorkspaceModel(
            _account->workspace(), _credentials, _decorations);
    PublicTaskManager::_removeCompletedItems(workspaceModel, _credentials);
    QString filter = _ui->publicTasksFilterLineEdit->text().trimmed();
    if (!filter.isEmpty())
    {
        PublicTaskManager::_filterItems(workspaceModel, filter, _decorations);
    }
    PublicTaskManager::_refreshWorkspaceTree(
        _ui->publicTasksTreeWidget,
        workspaceModel);
    //  TODO expand all ?
}

void ManageQuickPicksListDialog::_refillPrivateActivitiesTree()
{
    PrivateActivityManager::_UserModel userModel =
        PrivateActivityManager::_createUserModel(
            _account->user(_credentials), _credentials, _decorations);  //  TODO may throw
    QString filter = _ui->privateActivitiesFilterLineEdit->text().trimmed();
    if (!filter.isEmpty())
    {
        PrivateActivityManager::_filterItems(userModel, filter, _decorations);
    }
    _refreshWorkspaceTree(userModel);
}

void ManageQuickPicksListDialog::_refillPrivateTasksTree()
{
    PrivateTaskManager::_UserModel userModel =
        PrivateTaskManager::_createUserModel(
            _account->user(_credentials), _credentials, _decorations);
    PrivateTaskManager::_removeCompletedItems(userModel, _credentials);
    QString filter = _ui->privateTasksFilterLineEdit->text().trimmed();
    if (!filter.isEmpty())
    {
        PrivateTaskManager::_filterItems(
            userModel, filter, _decorations);
    }
    _refreshWorkspaceTree(userModel);
    //  TODO expand all ?
}

void ManageQuickPicksListDialog::_refresh()
{
    //  We don't want a refresh() to trigger a recursive refresh()!
    static bool refreshUnderway = false;
    RefreshGuard refreshGuard(refreshUnderway);
    if (refreshGuard)   //  Don't recurse!
    {
        try
        {
            if (_account->workspace() == nullptr || !_credentials.isValid() ||
                !_account->workspace()->isOpen() ||
                !_account->workspace()->canAccess(_credentials)) //  may throw
            {   //  Nothing to show
                _clearAndDisableAllControls();
                return;
            }
        }
        catch (const tt3::util::Exception & ex)
        {   //  OOPS! No point in proceesing.
            qCritical() << ex.errorMessage();
            _clearAndDisableAllControls();
            return;
        }

        //  Otherwise some controls are always enabled...
        _ui->publicActivitiesFilterLabel->setEnabled(true);
        _ui->publicActivitiesFilterLineEdit->setEnabled(true);
        _ui->publicActivitiesTreeWidget->setEnabled(true);

        _ui->publicTasksFilterLabel->setEnabled(true);
        _ui->publicTasksFilterLineEdit->setEnabled(true);
        _ui->publicTasksTreeWidget->setEnabled(true);

        _ui->privateActivitiesFilterLabel->setEnabled(true);
        _ui->privateActivitiesFilterLineEdit->setEnabled(true);
        _ui->privateActivitiesTreeWidget->setEnabled(true);

        _ui->privateTasksFilterLabel->setEnabled(true);
        _ui->privateTasksFilterLineEdit->setEnabled(true);
        _ui->privateTasksTreeWidget->setEnabled(true);
    }
}

//////////
//  Signal handlers
void ManageQuickPicksListDialog::accept()
{
    done(int(Result::Ok));
}

void ManageQuickPicksListDialog::reject()
{
    done(int(Result::Cancel));
}

//  End of tt3-gui/ManageQuickPicksListDialog.cpp
