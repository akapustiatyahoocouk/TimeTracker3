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
    tt3::util::ResourceReader rr(Component::Resources::instance(), RSID(ManageQuickPicksListDialog));

    Q_ASSERT(_account != nullptr);
    Q_ASSERT(_credentials.isValid());

    _ui->setupUi(this);
    setWindowTitle(rr.string(RID(Title)));

    _treeWidgetDecorations = TreeWidgetDecorations(_ui->publicTasksTreeWidget);
    _listWidgetDecorations = ListWidgetDecorations(_ui->quickPicksListWidget);

    //  Set initial control values
    _ui->tasksTabWidget->setTabText(
        0, rr.string(RID(PublicActivitiesTab)));
    _ui->tasksTabWidget->setTabText(
        1, rr.string(RID(PublicTasksTab)));
    _ui->tasksTabWidget->setTabText(
        2, rr.string(RID(PrivateActivitiesTab)));
    _ui->tasksTabWidget->setTabText(
        3, rr.string(RID(PrivateTasksTab)));
    _ui->publicActivitiesFilterLabel->setText(
        rr.string(RID(PublicActivitiesFilterLabel)));
    _ui->publicTasksFilterLabel->setText(
        rr.string(RID(PublicTasksFilterLabel)));
    _ui->privateActivitiesFilterLabel->setText(
        rr.string(RID(PrivateActivitiesFilterLabel)));
    _ui->privateTasksFilterLabel->setText(
        rr.string(RID(PrivateTasksFilterLabel)));
    _ui->moveToTopPushButton->setText(
        rr.string(RID(MoveToTopPushButton)));
    _ui->moveUpPushButton->setText(
        rr.string(RID(MoveUpPushButton)));
    _ui->moveDownPushButton->setText(
        rr.string(RID(MoveDownPushButton)));
    _ui->moveToBottomPushButton->setText(
        rr.string(RID(MoveToBottomPushButton)));
    _ui->removePushButton->setText(
        rr.string(RID(RemovePushButton)));

    _ui->buttonBox->button(QDialogButtonBox::StandardButton::Ok)->
        setText(rr.string(RID(OkPushButton)));
    _ui->buttonBox->button(QDialogButtonBox::StandardButton::Ok)->
        setIcon(QIcon(":/tt3-gui/Resources/Images/Actions/OkSmall.png"));
    _ui->buttonBox->button(QDialogButtonBox::StandardButton::Cancel)->
        setText(rr.string(RID(CancelPushButton)));
    _ui->buttonBox->button(QDialogButtonBox::StandardButton::Cancel)->
        setIcon(QIcon(":/tt3-gui/Resources/Images/Actions/CancelSmall.png"));

    //  Set up initial control values
    if (auto _ = RefreshGuard(_refreshUnderway)) //  Don't recurse!
    {
        _refillPublicActivitiesTree();
        _refillPublicTasksTree();
        _refillPrivateActivitiesTree();
        _refillPrivateTasksTree();
        _refillQuickPicksListWidget();
    }

    _ui->tasksTabWidget->setCurrentIndex(
        Component::Settings::instance()->manageQuickPicksListDialogTab);

    //  Done
    _refresh();
    adjustSize();
}

ManageQuickPicksListDialog::~ManageQuickPicksListDialog()
{
    delete _ui;
}

//////////
//  Operations
auto ManageQuickPicksListDialog::doModal() -> Result
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
    _ui->publicActivitiesFilterLabel->setText("");
    _ui->publicActivitiesFilterLabel->setEnabled(false);
    _ui->publicActivitiesFilterLineEdit->setEnabled(false);
    _ui->publicActivitiesTreeWidget->setEnabled(false);

    _ui->publicTasksTreeWidget->clear();
    _ui->publicTasksFilterLineEdit->setText("");
    _ui->publicTasksFilterLabel->setEnabled(false);
    _ui->publicTasksFilterLineEdit->setEnabled(false);
    _ui->publicTasksTreeWidget->setEnabled(false);

    _ui->privateActivitiesTreeWidget->clear();
    _ui->privateActivitiesFilterLabel->setText("");
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
        _account->workspace(), _credentials, _treeWidgetDecorations);
    if (QString filter = _ui->publicActivitiesFilterLineEdit->text().trimmed();
        !filter.isEmpty())
    {
        PublicActivityManager::_filterItems(workspaceModel, filter, _treeWidgetDecorations);
    }
    PublicActivityManager::_refreshWorkspaceTree(
        _ui->publicActivitiesTreeWidget,
        workspaceModel);

    _refreshCheckMarks(
        _ui->publicActivitiesTreeWidget,
        [](auto item)
        {
            return item->data(0, Qt::ItemDataRole::UserRole)
                       .template value<tt3::ws::PublicActivity>();
        });
}

void ManageQuickPicksListDialog::_refillPublicTasksTree()
{
    PublicTaskManager::_WorkspaceModel workspaceModel =
        PublicTaskManager::_createWorkspaceModel(
            _account->workspace(), _credentials, _treeWidgetDecorations);
    PublicTaskManager::_removeCompletedItems(workspaceModel, _credentials);
    QString filter = _ui->publicTasksFilterLineEdit->text().trimmed();
    if (!filter.isEmpty())
    {
        PublicTaskManager::_filterItems(workspaceModel, filter, _treeWidgetDecorations);
    }
    PublicTaskManager::_refreshWorkspaceTree(
        _ui->publicTasksTreeWidget,
        workspaceModel);
    if (!filter.isEmpty())
    {
        _ui->publicTasksTreeWidget->expandAll();
    }

    _refreshCheckMarks(
        _ui->publicTasksTreeWidget,
        [](auto item)
        {
            return item->data(0, Qt::ItemDataRole::UserRole)
                       .template value<tt3::ws::PublicTask>();
        });
}

void ManageQuickPicksListDialog::_refillPrivateActivitiesTree()
{
    try
    {
        PrivateActivityManager::_UserModel userModel =
            PrivateActivityManager::_createUserModel(
                _account->user(_credentials), _credentials, _treeWidgetDecorations);  //  may throw
        if (QString filter = _ui->privateActivitiesFilterLineEdit->text().trimmed();
            !filter.isEmpty())
        {
            PrivateActivityManager::_filterItems(userModel, filter, _treeWidgetDecorations);
        }
        _refreshWorkspaceTree(userModel);

        _refreshCheckMarks(
            _ui->privateActivitiesTreeWidget,
            [](auto item)
            {
                return item->data(0, Qt::ItemDataRole::UserRole)
                           .template value<tt3::ws::PrivateActivity>();
            });
    }
    catch (const tt3::util::Exception & ex)
    {   //  OOPS! Log & suppress
        qCritical() << ex;
        _ui->privateActivitiesTreeWidget->clear();
    }
}

void ManageQuickPicksListDialog::_refillPrivateTasksTree()
{
    try
    {
        PrivateTaskManager::_UserModel userModel =
            PrivateTaskManager::_createUserModel(
                _account->user(_credentials), _credentials, _treeWidgetDecorations);    //  may throw
        PrivateTaskManager::_removeCompletedItems(userModel, _credentials);
        QString filter = _ui->privateTasksFilterLineEdit->text().trimmed();
        if (!filter.isEmpty())
        {
            PrivateTaskManager::_filterItems(
                userModel, filter, _treeWidgetDecorations);
        }
        _refreshWorkspaceTree(userModel);
        if (!filter.isEmpty())
        {
            _ui->privateTasksTreeWidget->expandAll();
        }

        _refreshCheckMarks(
            _ui->privateTasksTreeWidget,
            [](auto item)
            {
                return item->data(0, Qt::ItemDataRole::UserRole)
                           .template value<tt3::ws::PrivateTask>();
            });
    }
    catch (const tt3::util::Exception & ex)
    {   //  OOPS! Log & suppress
        qCritical() << ex;
        _ui->privateTasksTreeWidget->clear();
    }
}

void ManageQuickPicksListDialog::_refillQuickPicksListWidget()
{
    static const QIcon errorIcon(":/tt3-gui/Resources/Images/Misc/ErrorSmall.png");
    tt3::util::ResourceReader rr(Component::Resources::instance(), RSID(ManageQuickPicksListDialog));

    //  Make sure the list contains the proper number...
    while (_ui->quickPicksListWidget->count() < _quickPicksList.size())
    {   //  Too few items in the list widget
        _ui->quickPicksListWidget->addItem(new QListWidgetItem());
    }
    while (_ui->quickPicksListWidget->count() > _quickPicksList.size())
    {   //  Too many items in the list widget
        delete _ui->quickPicksListWidget->takeItem(
            _ui->quickPicksListWidget->count() - 1);
    }
    //  ...of proper items
    for (int i = 0; i < _quickPicksList.size(); i++)
    {
        QListWidgetItem * item = _ui->quickPicksListWidget->item(i);
        try
        {
            QBrush foregrund = _treeWidgetDecorations.itemForeground;
            QString suffix;
            if (tt3::ws::Task task =
                std::dynamic_pointer_cast<tt3::ws::TaskImpl>(_quickPicksList[i]))
            {
                if (task->completed(_credentials))
                {
                    suffix = " " + rr.string(RID(TaskCompletedSuffix));
                    foregrund = _treeWidgetDecorations.disabledItemForeground;
                }
            }
            item->setText(_quickPicksList[i]->displayName(_credentials) + suffix);  //  may throw
            item->setIcon(_quickPicksList[i]->type()->smallIcon()); //  may throw
            item->setForeground(foregrund);
        }
        catch (const tt3::util::Exception & ex)
        {   //  OOPS!
            qCritical() << ex;
            item->setText(ex.errorMessage());
            item->setIcon(errorIcon);
            item->setForeground(_listWidgetDecorations.errorItemForeground);
        }
        auto a = _quickPicksList[i];
        item->setData(Qt::ItemDataRole::UserRole, QVariant::fromValue(a));
    }
}

void ManageQuickPicksListDialog::_refresh()
{
    //  We don't want a refresh() to trigger a recursive refresh()!
    if (auto _ = RefreshGuard(_refreshUnderway)) //  Don't recurse!
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
            qCritical() << ex;
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

        _ui->moveToTopPushButton->setEnabled(
            _ui->quickPicksListWidget->currentItem() != nullptr &&
            _ui->quickPicksListWidget->currentIndex().row() > 0);
        _ui->moveUpPushButton->setEnabled(
            _ui->quickPicksListWidget->currentItem() != nullptr &&
            _ui->quickPicksListWidget->currentIndex().row() > 0);
        _ui->moveDownPushButton->setEnabled(
            _ui->quickPicksListWidget->currentItem() != nullptr &&
            _ui->quickPicksListWidget->currentIndex().row() + 1 < _ui->quickPicksListWidget->count());
        _ui->moveToBottomPushButton->setEnabled(
            _ui->quickPicksListWidget->currentItem() != nullptr &&
            _ui->quickPicksListWidget->currentIndex().row() + 1 < _ui->quickPicksListWidget->count());
        _ui->removePushButton->setEnabled(
            _ui->quickPicksListWidget->currentItem() != nullptr);
    }
}

void ManageQuickPicksListDialog::_refreshCheckMarks(
        QTreeWidget * treeWidget,
        _ActivityExtractor fn
    )
{
    for (int i = 0; i < treeWidget->topLevelItemCount(); i++)
    {
        _refreshCheckMarks(treeWidget->topLevelItem(i), fn);
    }
}

void ManageQuickPicksListDialog::_refreshCheckMarks(
        QTreeWidgetItem * item,
        _ActivityExtractor fn
    )
{
    item->setFlags(item->flags() | Qt::ItemFlag::ItemIsUserCheckable);
    item->setCheckState(
        0,
        _quickPicksList.contains(fn(item)) ?
            Qt::CheckState::Checked :
            Qt::CheckState::Unchecked);
    for (int i = 0; i < item->childCount(); i++)
    {
        _refreshCheckMarks(item->child(i), fn);
    }
}

auto ManageQuickPicksListDialog::_selectedQuickPicksListItem(
    ) -> tt3::ws::Activity
{
    if (QListWidgetItem * item = _ui->quickPicksListWidget->currentItem())
    {
        return item->data(Qt::ItemDataRole::UserRole).value<tt3::ws::Activity>();
    }
    return nullptr;
}

void ManageQuickPicksListDialog::_setSelectedQuickPicksListItem(
        tt3::ws::Activity activity
    )
{
    for (int i = 0; i < _ui->quickPicksListWidget->count(); i++)
    {
        QListWidgetItem * item = _ui->quickPicksListWidget->item(i);
        auto a = item->data(Qt::ItemDataRole::UserRole).value<tt3::ws::Activity>();
        if (activity == a)
        {
            _ui->quickPicksListWidget->setCurrentRow(i);
            break;
        }
    }
}

//////////
//  Signal handlers
void ManageQuickPicksListDialog::_publicActivitiesTreeWidgetItemChanged(QTreeWidgetItem * item, int)
{
    if (!_refreshUnderway)
    {
        tt3::ws::PublicActivity publicActivity =
            item->data(0, Qt::ItemDataRole::UserRole).value<tt3::ws::PublicActivity>();
        if (item->checkState(0) == Qt::Checked &&
            !_quickPicksList.contains(publicActivity))
        {   //  Item is newly "checked"
            _quickPicksList.append(publicActivity);
            _refillQuickPicksListWidget();
            _setSelectedQuickPicksListItem(publicActivity);
        }
        else if (item->checkState(0) == Qt::Unchecked &&
                 _quickPicksList.contains(publicActivity))
        {   //  Item is newly "unckecked"
            _quickPicksList.removeOne(publicActivity);
            _refillQuickPicksListWidget();
        }
    }
    _refresh();
}

void ManageQuickPicksListDialog::_publicTasksTreeWidgetItemChanged(QTreeWidgetItem * item, int)
{
    if (!_refreshUnderway)
    {
        tt3::ws::PublicTask publicTask =
            item->data(0, Qt::ItemDataRole::UserRole).value<tt3::ws::PublicTask>();
        if (item->checkState(0) == Qt::Checked &&
            !_quickPicksList.contains(publicTask))
        {   //  Item is newly "checked"
            _quickPicksList.append(publicTask);
            _refillQuickPicksListWidget();
            _setSelectedQuickPicksListItem(publicTask);
        }
        else if (item->checkState(0) == Qt::Unchecked &&
                 _quickPicksList.contains(publicTask))
        {   //  Item is newly "unckecked"
            _quickPicksList.removeOne(publicTask);
            _refillQuickPicksListWidget();
        }
    }
    _refresh();
}

void ManageQuickPicksListDialog::_privateActivitiesTreeWidgetItemChanged(QTreeWidgetItem * item, int)
{
    if (!_refreshUnderway)
    {
        tt3::ws::PrivateActivity privateActivity =
            item->data(0, Qt::ItemDataRole::UserRole).value<tt3::ws::PrivateActivity>();
        if (item->checkState(0) == Qt::Checked &&
            !_quickPicksList.contains(privateActivity))
        {   //  Item is newly "checked"
            _quickPicksList.append(privateActivity);
            _refillQuickPicksListWidget();
            _setSelectedQuickPicksListItem(privateActivity);
        }
        else if (item->checkState(0) == Qt::Unchecked &&
                 _quickPicksList.contains(privateActivity))
        {   //  Item is newly "unckecked"
            _quickPicksList.removeOne(privateActivity);
            _refillQuickPicksListWidget();
        }
    }
    _refresh();
}

void ManageQuickPicksListDialog::_privateTasksTreeWidgetItemChanged(QTreeWidgetItem * item, int)
{
    if (!_refreshUnderway)
    {
        tt3::ws::PrivateTask privateTask =
            item->data(0, Qt::ItemDataRole::UserRole).value<tt3::ws::PrivateTask>();
        if (item->checkState(0) == Qt::Checked &&
            !_quickPicksList.contains(privateTask))
        {   //  Item is newly "checked"
            _quickPicksList.append(privateTask);
            _refillQuickPicksListWidget();
            _setSelectedQuickPicksListItem(privateTask);
        }
        else if (item->checkState(0) == Qt::Unchecked &&
                 _quickPicksList.contains(privateTask))
        {   //  Item is newly "unckecked"
            _quickPicksList.removeOne(privateTask);
            _refillQuickPicksListWidget();
        }
    }
    _refresh();
}

void ManageQuickPicksListDialog::_quickPicksListWidgetCurrentRowChanged(int)
{
    _refresh();
}

void ManageQuickPicksListDialog::_moveUpPushButtonClicked()
{
    if (tt3::ws::Activity activity = _selectedQuickPicksListItem())
    {
        qsizetype n = _quickPicksList.indexOf(activity);
        if (n > 0)  //  includes a check for n != -1
        {
            _quickPicksList.remove(n);
            _quickPicksList.insert(n - 1, activity);
            _refillQuickPicksListWidget();
            _setSelectedQuickPicksListItem(activity);
        }
    }
}

void ManageQuickPicksListDialog::_moveDownPushButtonClicked()
{
    if (tt3::ws::Activity activity = _selectedQuickPicksListItem())
    {
        qsizetype n = _quickPicksList.indexOf(activity);
        if (n != -1 && n + 1 < _quickPicksList.size())
        {
            _quickPicksList.remove(n);
            _quickPicksList.insert(n + 1, activity);
            _refillQuickPicksListWidget();
            _setSelectedQuickPicksListItem(activity);
        }
    }
}

void ManageQuickPicksListDialog::_moveToTopPushButtonClicked()
{
    if (tt3::ws::Activity activity = _selectedQuickPicksListItem())
    {
        qsizetype n = _quickPicksList.indexOf(activity);
        if (n > 0)  //  includes a check for n != -1
        {
            _quickPicksList.remove(n);
            _quickPicksList.insert(0, activity);
            _refillQuickPicksListWidget();
            _setSelectedQuickPicksListItem(activity);
        }
    }
}

void ManageQuickPicksListDialog::_moveToBottomPushButtonClicked()
{
    if (tt3::ws::Activity activity = _selectedQuickPicksListItem())
    {
        qsizetype n = _quickPicksList.indexOf(activity);
        if (n != -1 && n + 1 < _quickPicksList.size())
        {
            _quickPicksList.remove(n);
            _quickPicksList.append(activity);
            _refillQuickPicksListWidget();
            _setSelectedQuickPicksListItem(activity);
        }
    }
}

void ManageQuickPicksListDialog::_removePushButtonClicked()
{
    if (tt3::ws::Activity activity = _selectedQuickPicksListItem())
    {
        if (auto _ = RefreshGuard(_refreshUnderway)) //  Don't recurse!
        {
            _quickPicksList.removeOne(activity);
            _refillQuickPicksListWidget();
            //  The check states of the tree items must be adjusted!
            _refreshCheckMarks(
                _ui->publicActivitiesTreeWidget,
                [](auto item)
                {
                    return item->data(0, Qt::ItemDataRole::UserRole)
                               .template value<tt3::ws::PublicActivity>();
                });
            _refreshCheckMarks(
                _ui->publicTasksTreeWidget,
                [](auto item)
                {
                    return item->data(0, Qt::ItemDataRole::UserRole)
                               .template value<tt3::ws::PublicTask>();
                });
            _refreshCheckMarks(
                _ui->privateActivitiesTreeWidget,
                [](auto item)
                {
                    return item->data(0, Qt::ItemDataRole::UserRole)
                               .template value<tt3::ws::PrivateActivity>();
                });
            _refreshCheckMarks(
                _ui->privateTasksTreeWidget,
                [](auto item)
                {
                    return item->data(0, Qt::ItemDataRole::UserRole)
                               .template value<tt3::ws::PrivateTask>();
                });
        }
    }
}

void ManageQuickPicksListDialog::_publicActivitiesFilterLineEditTextChanged(QString)
{
    _refillPublicActivitiesTree();
    _refresh();
}

void ManageQuickPicksListDialog::_publicTasksFilterLineEditTextChanged(QString)
{
    _refillPublicTasksTree();
    _refresh();
}

void ManageQuickPicksListDialog::_privateActivitiesFilterLineEditTextChanged(QString)
{
    _refillPrivateActivitiesTree();
    _refresh();
}

void ManageQuickPicksListDialog::_privateTasksFilterLineEditTextChanged(QString)
{
    _refillPrivateTasksTree();
    _refresh();
}

void ManageQuickPicksListDialog::accept()
{
    Component::Settings::instance()->manageQuickPicksListDialogTab =
        _ui->tasksTabWidget->currentIndex();
    done(int(Result::Ok));
}

void ManageQuickPicksListDialog::reject()
{
    Component::Settings::instance()->manageQuickPicksListDialogTab =
        _ui->tasksTabWidget->currentIndex();
    done(int(Result::Cancel));
}

//  End of tt3-gui/ManageQuickPicksListDialog.cpp
