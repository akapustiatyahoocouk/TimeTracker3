//
//  tt3-gui/PrivateTaskManager.cpp - tt3::gui::PrivateTaskManager class implementation
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
#include "ui_PrivateTaskManager.h"
using namespace tt3::gui;

namespace tt3::gui
{
    extern CurrentTheme theCurrentTheme;
    extern CurrentActivity theCurrentActivity;
    extern CurrentCredentials theCurrentCredentials;
    extern CurrentWorkspace theCurrentWorkspace;
}

//////////
//  Construction/destruction
PrivateTaskManager::PrivateTaskManager(
        QWidget * parent
    ) : QWidget(parent),
        //  Implementation
        _workspace(theCurrentWorkspace),
        _credentials(theCurrentCredentials),
        //  Controls
        _ui(new Ui::PrivateTaskManager),
        _refreshTimer(this)
{
    _ui->setupUi(this);

    //  Theme change means widget decorations change
    connect(&theCurrentTheme,
            &CurrentTheme::changed,
            this,
            &PrivateTaskManager::_currentThemeChanged,
            Qt::ConnectionType::QueuedConnection);

    //  Current activity change means, at least, a refresh
    connect(&theCurrentActivity,
            &CurrentActivity::changed,
            this,
            &PrivateTaskManager::_currentActivityChanged,
            Qt::ConnectionType::QueuedConnection);

    //  View options changes should cause a refresh
    connect(&Component::Settings::instance()->showCompletedPrivateTasks,
            &tt3::util::AbstractSetting::valueChanged,
            this,
            &PrivateTaskManager::_viewOptionSettingValueChanged,
            Qt::ConnectionType::QueuedConnection);

    //  Must listen to delayed refresh requests
    connect(this,
            &PrivateTaskManager::refreshRequested,
            this,
            &PrivateTaskManager::_refreshRequested,
            Qt::ConnectionType::QueuedConnection);

    //  Start listening for change notifications
    //  on the currently "viewed" Workspace
    _startListeningToWorkspaceChanges();

    //  Start refreshing on timer
    connect(&_refreshTimer,
            &QTimer::timeout,
            this,
            &PrivateTaskManager::_refreshTimerTimeout);
    _refreshTimer.start(1000);
}

PrivateTaskManager::~PrivateTaskManager()
{
    _refreshTimer.stop();
    _stopListeningToWorkspaceChanges();
    delete _ui;
}

//////////
//  Operaions
tt3::ws::Workspace PrivateTaskManager::workspace() const
{
    return _workspace;
}

void PrivateTaskManager::setWorkspace(tt3::ws::Workspace workspace)
{
    if (workspace != _workspace)
    {
        _stopListeningToWorkspaceChanges();
        _workspace = workspace;
        _startListeningToWorkspaceChanges();
        requestRefresh();
    }
}

tt3::ws::Credentials PrivateTaskManager::credentials() const
{
    return _credentials;
}

void PrivateTaskManager::setCredentials(const tt3::ws::Credentials & credentials)
{
    if (credentials != _credentials)
    {
        _credentials = credentials;
        requestRefresh();
    }
}

void PrivateTaskManager::refresh()
{
    static const QIcon viewPrivateTaskIcon(":/tt3-gui/Resources/Images/Actions/ViewPrivateTaskLarge.png");
    static const QIcon modifyPrivateTaskIcon(":/tt3-gui/Resources/Images/Actions/ModifyPrivateTaskLarge.png");

    //  We don't want a refresh() to trigger a recursive refresh()!
    static bool refreshUnderway = false;
    RefreshGuard refreshGuard(refreshUnderway);
    if (refreshGuard)   //  Don't recurse!
    {
        try
        {
            if (_workspace == nullptr || !_credentials.isValid() ||
                !_workspace->isOpen() ||
                !_workspace->canAccess(_credentials)) //  may throw
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
        _ui->filterLabel->setEnabled(true);
        _ui->filterLineEdit->setEnabled(true);
        _ui->privateTasksTreeWidget->setEnabled(true);
        _ui->showCompletedCheckBox->setEnabled(true);

        //  ...while others are enabled based on current
        //  selection and permissions granted by Credentials
        _WorkspaceModel workspaceModel =
            _createWorkspaceModel(_workspace, _credentials, _decorations);
        if (!Component::Settings::instance()->showCompletedPrivateTasks)
        {
            _removeCompletedItems(workspaceModel, _credentials);
        }
        QString filter = _ui->filterLineEdit->text().trimmed();
        if (!filter.isEmpty())
        {
            _filterItems(workspaceModel, filter, _decorations);
        }
        _refreshWorkspaceTree(_ui->privateTasksTreeWidget, workspaceModel);
        if (!_ui->filterLineEdit->text().trimmed().isEmpty())
        {   //  Filtered - show all
            _ui->privateTasksTreeWidget->expandAll();
        }

        tt3::ws::PrivateTask selectedPrivateTask = _selectedPrivateTask();
        bool readOnly = _workspace->isReadOnly();
        try
        {
            _ui->createPrivateTaskPushButton->setEnabled(
                !readOnly &&
                _workspace->grantsAny(  //  may throw
                    _credentials,
                    tt3::ws::Capability::Administrator |
                        tt3::ws::Capability::ManagePrivateActivities) &&
                (_selectedUser() != nullptr || _selectedPrivateTask() != nullptr));
        }
        catch (const tt3::util::Exception & ex)
        {   //  OOPS! Log & disable
            qCritical() << ex.errorMessage();
            _ui->createPrivateTaskPushButton->setEnabled(false);
        }
        _ui->modifyPrivateTaskPushButton->setEnabled(
            selectedPrivateTask != nullptr);
        try
        {
            _ui->destroyPrivateTaskPushButton->setEnabled(
                !readOnly &&
                selectedPrivateTask != nullptr &&
                selectedPrivateTask->canDestroy(_credentials));  //  may throw
        }
        catch (const tt3::util::Exception & ex)
        {   //  OOPS! Log & disable
            qCritical() << ex.errorMessage();
            _ui->destroyPrivateTaskPushButton->setEnabled(false);
        }

        try
        {
            _ui->startPrivateTaskPushButton->setEnabled(
                !readOnly &&
                selectedPrivateTask != nullptr &&
                theCurrentActivity != selectedPrivateTask &&
                !selectedPrivateTask->completed(_credentials) &&    //  may throw
                selectedPrivateTask->canStart(_credentials));       //  may throw
        }
        catch (const tt3::util::Exception & ex)
        {   //  OOPS! Log & disable
            qCritical() << ex.errorMessage();
            _ui->startPrivateTaskPushButton->setEnabled(false);
        }
        try
        {
            _ui->stopPrivateTaskPushButton->setEnabled(
                !readOnly &&
                selectedPrivateTask != nullptr &&
                theCurrentActivity == selectedPrivateTask &&
                selectedPrivateTask->canStop(_credentials));    //  may throw
        }
        catch (const tt3::util::Exception & ex)
        {   //  OOPS! Log & disable
            qCritical() << ex.errorMessage();
            _ui->stopPrivateTaskPushButton->setEnabled(false);
        }
        try
        {
            _ui->completePrivateTaskPushButton->setEnabled(
                !readOnly &&
                selectedPrivateTask != nullptr &&
                selectedPrivateTask->canModify(_credentials) &&   //  may throw
                !selectedPrivateTask->completed(_credentials));   //  may throw
        }
        catch (const tt3::util::Exception & ex)
        {   //  OOPS! Report & recover
            qCritical() << ex.errorMessage();
            _ui->completePrivateTaskPushButton->setEnabled(false);
        }

        _ui->showCompletedCheckBox->setChecked(
            Component::Settings::instance()->showCompletedPrivateTasks);

        //  Some buttons need to be adjusted for ReadOnoly mode
        try
        {
            if (selectedPrivateTask != nullptr &&
                !selectedPrivateTask->workspace()->isReadOnly() &&
                selectedPrivateTask->canModify(_credentials))    //  may throw
            {   //  RW
                _ui->modifyPrivateTaskPushButton->setIcon(modifyPrivateTaskIcon);
                _ui->modifyPrivateTaskPushButton->setText("Modify private task");
            }
            else
            {   //  RO
                _ui->modifyPrivateTaskPushButton->setIcon(viewPrivateTaskIcon);
                _ui->modifyPrivateTaskPushButton->setText("View private task");
            }
        }
        catch (const tt3::util::Exception & ex)
        {   //  OOPS! Simulate RO
            qCritical() << ex.errorMessage();
            _ui->modifyPrivateTaskPushButton->setIcon(viewPrivateTaskIcon);
            _ui->modifyPrivateTaskPushButton->setText("View private task");
        }
    }
}

void PrivateTaskManager::requestRefresh()
{
    emit refreshRequested();
}

//////////
//  View model
auto PrivateTaskManager::_createWorkspaceModel(
        tt3::ws::Workspace workspace,
        const tt3::ws::Credentials & credentials,
        const TreeWidgetDecorations & decorations
    ) -> PrivateTaskManager::_WorkspaceModel
{
    _WorkspaceModel workspaceModel { new _WorkspaceModelImpl() };
    try
    {
        if (workspace->grantsAll(credentials, tt3::ws::Capability::Administrator))
        {   //  See private tasks of all users
            for (tt3::ws::User user : workspace->users(credentials))    //  may throw
            {
                workspaceModel->userModels.append(
                    _createUserModel(user, credentials, decorations));
            }
        }
        else
        {   //  If not Administrator, show ONLY the caller User
            workspaceModel->userModels.append(
                _createUserModel(
                    workspace->login(credentials)->user(credentials),
                        credentials, decorations));
        }
        std::sort(workspaceModel->userModels.begin(),
                  workspaceModel->userModels.end(),
                  [&](auto a, auto b)
                  { return a->text < b->text; });
    }
    catch (const tt3::util::Exception & ex)
    {
        qCritical() << ex.errorMessage();
        workspaceModel->userModels.clear();
    }
    return workspaceModel;
}

auto PrivateTaskManager::_createUserModel(
        tt3::ws::User user,
        const tt3::ws::Credentials & credentials,
        const TreeWidgetDecorations & decorations
    ) -> PrivateTaskManager::_UserModel
{
    static const QIcon errorIcon(":/tt3-gui/Resources/Images/Misc/ErrorSmall.png");

    _UserModel userModel
        { new _UserModelImpl(user) };
    try
    {
        userModel->text = user->realName(credentials);
        if (!user->enabled(credentials))
        {
            userModel->text += " [disabled]";
            userModel->brush = decorations.disabledItemForeground;
        }
        else
        {
            userModel->brush = decorations.itemForeground;
        }
        userModel->icon = user->type()->smallIcon();
        userModel->font = decorations.itemFont;
        //  Do the children
        for (tt3::ws::PrivateTask privateTask : user->rootPrivateTasks(credentials))    //  may throw
        {
            userModel->privateTaskModels.append(
                _createPrivateTaskModel(privateTask, credentials, decorations));
        }
        std::sort(userModel->privateTaskModels.begin(),
                  userModel->privateTaskModels.end(),
                  [&](auto a, auto b)
                  { return a->text < b->text; });
    }
    catch (const tt3::util::Exception & ex)
    {
        qCritical() << ex.errorMessage();
        userModel->text = ex.errorMessage();
        userModel->icon = errorIcon;
        userModel->font = decorations.itemFont;
        userModel->brush = decorations.errorItemForeground;
        userModel->tooltip = ex.errorMessage();
        userModel->privateTaskModels.clear();
    }
    return userModel;
}

auto PrivateTaskManager::_createPrivateTaskModel(
        tt3::ws::PrivateTask privateTask,
        const tt3::ws::Credentials & credentials,
        const TreeWidgetDecorations & decorations
    ) -> PrivateTaskManager::_PrivateTaskModel
{
    static const QIcon errorIcon(":/tt3-gui/Resources/Images/Misc/ErrorSmall.png");

    _PrivateTaskModel privateTaskModel
        { new _PrivateTaskModelImpl(privateTask) };
    try
    {
        privateTaskModel->text = privateTask->displayName(credentials);
        if (privateTask->completed(credentials))
        {
            privateTaskModel->text += " [completed]";
            privateTaskModel->brush = decorations.disabledItemForeground;
        }
        else
        {
            privateTaskModel->brush = decorations.itemForeground;
        }
        privateTaskModel->icon = privateTask->type()->smallIcon();
        privateTaskModel->font = decorations.itemFont;
        privateTaskModel->tooltip = privateTask->description(credentials).trimmed();
        //  A "current" activity needs some extras
        if (theCurrentActivity == privateTask)
        {
            qint64 secs = qMax(0, theCurrentActivity.lastChangedAt().secsTo(QDateTime::currentDateTimeUtc()));
            char s[32];
            sprintf(s, " [%d:%02d:%02d]",
                    int(secs / (60 * 60)),
                    int((secs / 60) % 60),
                    int(secs % 60));
            privateTaskModel->text += s;
            privateTaskModel->font = decorations.itemEmphasisFont;
        }
        //  Do the children
        for (tt3::ws::PrivateTask child : privateTask->children(credentials)) //  may throw
        {
            privateTaskModel->childModels.append(
                _createPrivateTaskModel(child, credentials, decorations));
        }
        std::sort(privateTaskModel->childModels.begin(),
                  privateTaskModel->childModels.end(),
                  [&](auto a, auto b)
                  { return a->text < b->text; });
    }
    catch (const tt3::util::Exception & ex)
    {
        qCritical() << ex.errorMessage();
        privateTaskModel->text = ex.errorMessage();
        privateTaskModel->icon = errorIcon;
        privateTaskModel->font = decorations.itemFont;
        privateTaskModel->brush =
            decorations.errorItemForeground;
        privateTaskModel->tooltip = ex.errorMessage();
        privateTaskModel->childModels.clear();
    }
    return privateTaskModel;
}

void PrivateTaskManager::_removeCompletedItems(
        _WorkspaceModel workspaceModel,
        const tt3::ws::Credentials & credentials
    )
{
    for (qsizetype i = workspaceModel->userModels.size() - 1; i >= 0; i--)
    {
        _UserModel userModel = workspaceModel->userModels[i];
        _removeCompletedItems(userModel, credentials);
    }
}

void PrivateTaskManager::_removeCompletedItems(
        _UserModel userModel,
        const tt3::ws::Credentials & credentials
    )
{
    for (qsizetype i = userModel->privateTaskModels.size() - 1; i >= 0; i--)
    {
        _PrivateTaskModel privateTaskModel = userModel->privateTaskModels[i];
        _removeCompletedItems(privateTaskModel, credentials);
        //  If this PrivateTask is completed...
        try
        {
            if (privateTaskModel->privateTask->completed(credentials))    //  may throw
            {
                if (privateTaskModel->childModels.isEmpty())
                {   //  ...and has no children models - delete it
                    userModel->privateTaskModels.removeAt(i);
                }
                //  ...else it has child models - keep it
            }
            //  ...else keep this PrivateTask item
        }
        catch (const tt3::util::Exception & ex)
        {   //  OOPS! Suppress, but log
            qCritical() << ex.errorMessage();
        }
    }
}

void PrivateTaskManager::_removeCompletedItems(
        _PrivateTaskModel privateTaskModel,
        const tt3::ws::Credentials & credentials
    )
{
    for (qsizetype i = privateTaskModel->childModels.size() - 1; i >= 0; i--)
    {
        _PrivateTaskModel childModel = privateTaskModel->childModels[i];
        _removeCompletedItems(childModel, credentials);
        //  If this PrivateTask is completed...
        try
        {
            if (childModel->privateTask->completed(credentials)) //  may throw
            {
                if (childModel->childModels.isEmpty())
                {   //  ...and has no children models - delete it
                    privateTaskModel->childModels.removeAt(i);
                }
                //  ...else it has child models - keep it
            }
            //  ...else keep this PrivateTask item
        }
        catch (const tt3::util::Exception & ex)
        {   //  OOPS! Suppress, but log
            qCritical() << ex.errorMessage();
        }
    }
}

void PrivateTaskManager::_filterItems(
        _WorkspaceModel workspaceModel,
        const QString & filter,
        const TreeWidgetDecorations & decorations
    )
{
    Q_ASSERT(!filter.isEmpty());

    for (qsizetype i = workspaceModel->userModels.size() - 1; i >= 0; i--)
    {
        _UserModel userModel = workspaceModel->userModels[i];
        _filterItems(userModel, filter, decorations);
        if (userModel->text.indexOf(filter, 0, Qt::CaseInsensitive) != -1)
        {   //  Item matches the filter - mark it as a match
            userModel->brush = decorations.filterMatchItemForeground;
        }
        else if (userModel->privateTaskModels.isEmpty())
        {   //  Item does not match the filter and has no children - remove it
            workspaceModel->userModels.removeAt(i);
        }
        else
        {   //  Item does not match the filter but has children - show as disabled
            workspaceModel->userModels[i]->brush = decorations.disabledItemForeground;
        }
    }
}

void PrivateTaskManager::_filterItems(
        _UserModel userModel,
        const QString & filter,
        const TreeWidgetDecorations & decorations
    )
{
    Q_ASSERT(!filter.isEmpty());

    for (qsizetype i = userModel->privateTaskModels.size() - 1; i >= 0; i--)
    {
        _PrivateTaskModel privateTaskModel = userModel->privateTaskModels[i];
        _filterItems(privateTaskModel, filter, decorations);
        if (privateTaskModel->text.indexOf(filter, 0, Qt::CaseInsensitive) != -1)
        {   //  Item matches the filter - mark it as a match
            privateTaskModel->brush = decorations.filterMatchItemForeground;
        }
        else if (privateTaskModel->childModels.isEmpty())
        {   //  Item does not match the filter and has no children - remove it
            userModel->privateTaskModels.removeAt(i);
        }
        else
        {   //  Item does not match the filter but has children - show as disabled
            userModel->privateTaskModels[i]->brush = decorations.disabledItemForeground;
        }
    }
}

void PrivateTaskManager::_filterItems(
        _PrivateTaskModel privateTaskModel,
        const QString & filter,
        const TreeWidgetDecorations & decorations
    )
{
    Q_ASSERT(!filter.isEmpty());

    for (qsizetype i = privateTaskModel->childModels.size() - 1; i >= 0; i--)
    {
        _PrivateTaskModel childModel = privateTaskModel->childModels[i];
        _filterItems(childModel, filter, decorations);
        if (childModel->text.indexOf(filter, 0, Qt::CaseInsensitive) != -1)
        {   //  Item matches the filter - mark it as a match
            childModel->brush = decorations.filterMatchItemForeground;
        }
        else if (childModel->childModels.isEmpty())
        {   //  Item does not match the filter and has no children - remove it
            privateTaskModel->childModels.removeAt(i);
        }
        else
        {   //  Item does not match the filter but has children - show as disabled
            privateTaskModel->childModels[i]->brush = decorations.disabledItemForeground;
        }
    }
}

void PrivateTaskManager::_refreshWorkspaceTree(
        QTreeWidget * privateTasksTreeWidget,
        _WorkspaceModel workspaceModel
    )
{
    Q_ASSERT(privateTasksTreeWidget != nullptr);
    Q_ASSERT(workspaceModel != nullptr);

    //  Make sure the "private tasks" tree contains
    //  a proper number of root (User) items...
    while (privateTasksTreeWidget->topLevelItemCount() < workspaceModel->userModels.size())
    {   //  Too few root (User) items
        privateTasksTreeWidget->addTopLevelItem(new QTreeWidgetItem());
    }
    while (privateTasksTreeWidget->topLevelItemCount() > workspaceModel->userModels.size())
    {   //  Too many root (User) items
        delete privateTasksTreeWidget->takeTopLevelItem(
            privateTasksTreeWidget->topLevelItemCount() - 1);
    }
    //  ...and that each top-level item represents
    //  a proper User
    for (int i = 0; i < workspaceModel->userModels.size(); i++)
    {
        _refreshUserItem(
            privateTasksTreeWidget->topLevelItem(i),
            workspaceModel->userModels[i]);
    }
}

void PrivateTaskManager::_refreshUserItem(
        QTreeWidgetItem * userItem,
        _UserModel userModel
    )
{
    Q_ASSERT(userItem != nullptr);
    Q_ASSERT(userModel != nullptr);

    //  Refresh User item properties
    userItem->setText(0, userModel->text);
    userItem->setIcon(0, userModel->icon);
    userItem->setForeground(0, userModel->brush);
    userItem->setFont(0, userModel->font);
    userItem->setToolTip(0, userModel->tooltip);
    userItem->setData(0, Qt::ItemDataRole::UserRole, QVariant::fromValue(userModel->user));
    //  Make sure the User item contains a proper
    //  number of root (PrivateActivity) items...
    while (userItem->childCount() < userModel->privateTaskModels.size())
    {   //  Too few child (PrivateActivity) items
        userItem->addChild(new QTreeWidgetItem());
    }
    while (userItem->childCount() > userModel->privateTaskModels.size())
    {   //  Too many root (User) items
        delete userItem->takeChild(
            userItem->childCount() - 1);
    }
    //  ...and that each child item represents
    //  a proper PrivateTask
    for (int i = 0; i < userModel->privateTaskModels.size(); i++)
    {
        _refreshPrivateTaskItem(
            userItem->child(i),
            userModel->privateTaskModels[i]);
    }
}

void PrivateTaskManager::_refreshPrivateTaskItem(
        QTreeWidgetItem * privateTaskItem,
        _PrivateTaskModel privateTaskModel
    )
{
    Q_ASSERT(privateTaskItem != nullptr);
    Q_ASSERT(privateTaskModel != nullptr);

    //  Make sure the tree item has correct properties
    privateTaskItem->setText(0, privateTaskModel->text);
    privateTaskItem->setIcon(0, privateTaskModel->icon);
    privateTaskItem->setForeground(0, privateTaskModel->brush);
    privateTaskItem->setFont(0, privateTaskModel->font);
    privateTaskItem->setToolTip(0, privateTaskModel->tooltip);
    privateTaskItem->setData(0, Qt::ItemDataRole::UserRole, QVariant::fromValue(privateTaskModel->privateTask));
    //  Make sure the private task, tree item tree contains
    //  a proper number of childs items...
    while (privateTaskItem->childCount() < privateTaskModel->childModels.size())
    {   //  Too few child items
        privateTaskItem->addChild(new QTreeWidgetItem());
    }
    while (privateTaskItem->childCount() > privateTaskModel->childModels.size())
    {   //  Too many child items
        delete privateTaskItem->takeChild(
            privateTaskItem->childCount() - 1);
    }
    //  ...and that each child item represents
    //  a proper PrivateTask and has proper children
    for (int i = 0; i < privateTaskModel->childModels.size(); i++)
    {
        _refreshPrivateTaskItem(
            privateTaskItem->child(i),
            privateTaskModel->childModels[i]);
    }
}

//////////
//  Implementation helpers
tt3::ws::User PrivateTaskManager::_selectedUser()
{
    QTreeWidgetItem * item = _ui->privateTasksTreeWidget->currentItem();
    return (item != nullptr && item->parent() == nullptr) ?
               item->data(0, Qt::ItemDataRole::UserRole).value<tt3::ws::User>() :
               nullptr;
}

void PrivateTaskManager::_setSelectedUser(tt3::ws::User user)
{
    for (int i = 0; i < _ui->privateTasksTreeWidget->topLevelItemCount(); i++)
    {
        QTreeWidgetItem * userItem = _ui->privateTasksTreeWidget->topLevelItem(i);
        if (user == userItem->data(0, Qt::ItemDataRole::UserRole).value<tt3::ws::User>())
        {   //  This one!
            _ui->privateTasksTreeWidget->setCurrentItem(userItem);
            return;
        }
    }
}

auto PrivateTaskManager::_selectedPrivateTask(
    ) -> tt3::ws::PrivateTask
{
    QTreeWidgetItem * item = _ui->privateTasksTreeWidget->currentItem();
    return (item != nullptr && item->parent() != nullptr) ?
               item->data(0, Qt::ItemDataRole::UserRole).value<tt3::ws::PrivateTask>() :
               nullptr;
}

bool PrivateTaskManager::_setSelectedPrivateTask(
        tt3::ws::PrivateTask privateTask
    )
{
    for (int i = 0; i < _ui->privateTasksTreeWidget->topLevelItemCount(); i++)
    {
        QTreeWidgetItem * userItem = _ui->privateTasksTreeWidget->topLevelItem(i);
        for (int j = 0; j < userItem->childCount(); j++)
        {
            QTreeWidgetItem * item = userItem->child(j);
            if (privateTask == item->data(0, Qt::ItemDataRole::UserRole).value<tt3::ws::PrivateTask>())
            {   //  This one!
                _ui->privateTasksTreeWidget->setCurrentItem(item);
                return true;
            }
            if (_setSelectedPrivateTask(item, privateTask))
            {   //  One of descendants selected
                return true;
            }
        }
    }
    return false;
}

bool PrivateTaskManager::_setSelectedPrivateTask(
        QTreeWidgetItem * parentItem,
        tt3::ws::PrivateTask privateTask
    )
{
    for (int i = 0; i < parentItem->childCount(); i++)
    {
        QTreeWidgetItem * item = parentItem->child(i);
        if (privateTask == item->data(0, Qt::ItemDataRole::UserRole).value<tt3::ws::PrivateTask>())
        {   //  This one!
            _ui->privateTasksTreeWidget->setCurrentItem(item);
            return true;
        }
        if (_setSelectedPrivateTask(item, privateTask))
        {   //  One of descendants selected
            return true;
        }
    }
    return false;
}

void PrivateTaskManager::_startListeningToWorkspaceChanges()
{
    if (_workspace != nullptr)
    {
        connect(_workspace.get(),
                &tt3::ws::WorkspaceImpl::workspaceClosed,
                this,
                &PrivateTaskManager::_workspaceClosed,
                Qt::ConnectionType::QueuedConnection);
        connect(_workspace.get(),
                &tt3::ws::WorkspaceImpl::objectCreated,
                this,
                &PrivateTaskManager::_objectCreated,
                Qt::ConnectionType::QueuedConnection);
        connect(_workspace.get(),
                &tt3::ws::WorkspaceImpl::objectDestroyed,
                this,
                &PrivateTaskManager::_objectDestroyed,
                Qt::ConnectionType::QueuedConnection);
        connect(_workspace.get(),
                &tt3::ws::WorkspaceImpl::objectModified,
                this,
                &PrivateTaskManager::_objectModified,
                Qt::ConnectionType::QueuedConnection);
    }
}

void PrivateTaskManager::_stopListeningToWorkspaceChanges()
{
    if (_workspace != nullptr)
    {
        disconnect(_workspace.get(),
                   &tt3::ws::WorkspaceImpl::workspaceClosed,
                   this,
                   &PrivateTaskManager::_workspaceClosed);
        disconnect(_workspace.get(),
                   &tt3::ws::WorkspaceImpl::objectCreated,
                   this,
                   &PrivateTaskManager::_objectCreated);
        disconnect(_workspace.get(),
                   &tt3::ws::WorkspaceImpl::objectDestroyed,
                   this,
                   &PrivateTaskManager::_objectDestroyed);
        disconnect(_workspace.get(),
                   &tt3::ws::WorkspaceImpl::objectModified,
                   this,
                   &PrivateTaskManager::_objectModified);
    }
}

void PrivateTaskManager::_clearAndDisableAllControls()
{
    _ui->privateTasksTreeWidget->clear();
    _ui->filterLabel->setEnabled(false);
    _ui->filterLineEdit->setEnabled(false);
    _ui->privateTasksTreeWidget->setEnabled(false);
    _ui->createPrivateTaskPushButton->setEnabled(false);
    _ui->modifyPrivateTaskPushButton->setEnabled(false);
    _ui->destroyPrivateTaskPushButton->setEnabled(false);
    _ui->startPrivateTaskPushButton->setEnabled(false);
    _ui->stopPrivateTaskPushButton->setEnabled(false);
    _ui->completePrivateTaskPushButton->setEnabled(false);
    _ui->showCompletedCheckBox->setEnabled(false);
}


//////////
//  Signal handlers
void PrivateTaskManager::_currentThemeChanged(ITheme *, ITheme *)
{
    _ui->privateTasksTreeWidget->clear();
    _decorations = TreeWidgetDecorations(_ui->privateTasksTreeWidget);
    requestRefresh();
}

void PrivateTaskManager::_currentActivityChanged(tt3::ws::Activity, tt3::ws::Activity)
{
    requestRefresh();
}

void PrivateTaskManager::_privateTasksTreeWidgetCurrentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)
{
    requestRefresh();
}

void PrivateTaskManager::_privateTasksTreeWidgetCustomContextMenuRequested(QPoint p)
{
    //  [re-]create the popup menu
    _privateTasksTreeContextMenu.reset(new QMenu());
    QAction * createPrivateTaskAction =
        _privateTasksTreeContextMenu->addAction(
            _ui->createPrivateTaskPushButton->icon(),
            _ui->createPrivateTaskPushButton->text());
    QAction * modifyPrivateTaskAction =
        _privateTasksTreeContextMenu->addAction(
            _ui->modifyPrivateTaskPushButton->icon(),
            _ui->modifyPrivateTaskPushButton->text());
    QAction * destroyPrivateTaskAction =
        _privateTasksTreeContextMenu->addAction(
            _ui->destroyPrivateTaskPushButton->icon(),
            _ui->destroyPrivateTaskPushButton->text());
    _privateTasksTreeContextMenu->addSeparator();
    QAction * startPrivateTaskAction =
        _privateTasksTreeContextMenu->addAction(
            _ui->startPrivateTaskPushButton->icon(),
            _ui->startPrivateTaskPushButton->text());
    QAction * stopPrivateTaskAction =
        _privateTasksTreeContextMenu->addAction(
            _ui->stopPrivateTaskPushButton->icon(),
            _ui->stopPrivateTaskPushButton->text());
    QAction * completePrivateTaskAction =
        _privateTasksTreeContextMenu->addAction(
            _ui->completePrivateTaskPushButton->icon(),
            _ui->completePrivateTaskPushButton->text());
    //  Adjust menu item states
    createPrivateTaskAction->setEnabled(_ui->createPrivateTaskPushButton->isEnabled());
    modifyPrivateTaskAction->setEnabled(_ui->modifyPrivateTaskPushButton->isEnabled());
    destroyPrivateTaskAction->setEnabled(_ui->destroyPrivateTaskPushButton->isEnabled());
    startPrivateTaskAction->setEnabled(_ui->startPrivateTaskPushButton->isEnabled());
    stopPrivateTaskAction->setEnabled(_ui->stopPrivateTaskPushButton->isEnabled());
    completePrivateTaskAction->setEnabled(_ui->completePrivateTaskPushButton->isEnabled());
    //  Set up signal handling
    connect(createPrivateTaskAction,
            &QAction::triggered,
            this,
            &PrivateTaskManager::_createPrivateTaskPushButtonClicked);
    connect(modifyPrivateTaskAction,
            &QAction::triggered,
            this,
            &PrivateTaskManager::_modifyPrivateTaskPushButtonClicked);
    connect(destroyPrivateTaskAction,
            &QAction::triggered,
            this,
            &PrivateTaskManager::_destroyPrivateTaskPushButtonClicked);
    connect(startPrivateTaskAction,
            &QAction::triggered,
            this,
            &PrivateTaskManager::_startPrivateTaskPushButtonClicked);
    connect(stopPrivateTaskAction,
            &QAction::triggered,
            this,
            &PrivateTaskManager::_stopPrivateTaskPushButtonClicked);
    connect(completePrivateTaskAction,
            &QAction::triggered,
            this,
            &PrivateTaskManager::_completePrivateTaskPushButtonClicked);
    //  Go!
    _privateTasksTreeContextMenu->popup(_ui->privateTasksTreeWidget->mapToGlobal(p));
}

void PrivateTaskManager::_createPrivateTaskPushButtonClicked()
{
    try
    {
        if (auto user = _selectedUser())
        {
            CreatePrivateTaskDialog dlg(this, user, _credentials);  //  may throw
            if (dlg.doModal() == CreatePrivateTaskDialog::Result::Ok)
            {   //  User created
                refresh();  //  must refresh NOW
                _setSelectedPrivateTask(dlg.createdPrivateTask());
            }
        }
        else if (auto parentTask = _selectedPrivateTask())
        {
            CreatePrivateTaskDialog dlg(this, parentTask, _credentials);//  may throw
            if (dlg.doModal() == CreatePrivateTaskDialog::Result::Ok)
            {   //  User created
                refresh();  //  must refresh NOW
                _setSelectedPrivateTask(dlg.createdPrivateTask());
            }
        }
    }
    catch (const tt3::util::Exception & ex)
    {
        tt3::gui::ErrorDialog::show(this, ex);
    }
}

void PrivateTaskManager::_modifyPrivateTaskPushButtonClicked()
{
    if (auto privateTask = _selectedPrivateTask())
    {
        try
        {
            ModifyPrivateTaskDialog dlg(this, privateTask, _credentials); //  may throw
            if (dlg.doModal() == ModifyPrivateTaskDialog::Result::Ok)
            {   //  PrivateTask modified - its position in the private tasks tree may have changed
                refresh();  //  must refresh NOW
                _setSelectedPrivateTask(privateTask);
            }
        }
        catch (const tt3::util::Exception & ex)
        {
            ErrorDialog::show(this, ex);
            requestRefresh();
        }
    }
}

void PrivateTaskManager::_destroyPrivateTaskPushButtonClicked()
{
    if (auto privateTask = _selectedPrivateTask())
    {
        try
        {
            DestroyPrivateTaskDialog dlg(this, privateTask, _credentials); //  may throw
            if (dlg.doModal() == DestroyPrivateTaskDialog::Result::Ok)
            {   //  PrivateTask destroyed
                requestRefresh();
            }
        }
        catch (const tt3::util::Exception & ex)
        {
            ErrorDialog::show(this, ex);
            requestRefresh();
        }
    }
}

void PrivateTaskManager::_startPrivateTaskPushButtonClicked()
{
    if (auto privateTask = _selectedPrivateTask())
    {
        if (theCurrentActivity == privateTask)
        {   //  Nothing to do!
            return;
        }
        try
        {
            theCurrentActivity.replaceWith(privateTask);
        }
        catch (const tt3::util::Exception & ex)
        {
            ErrorDialog::show(this, ex);
        }
        requestRefresh();
    }
}

void PrivateTaskManager::_stopPrivateTaskPushButtonClicked()
{
    if (auto privateTask = _selectedPrivateTask())
    {
        if (theCurrentActivity != privateTask)
        {   //  Nothing to do!
            return;
        }
        try
        {
            theCurrentActivity.replaceWith(nullptr);
        }
        catch (const tt3::util::Exception & ex)
        {
            ErrorDialog::show(this, ex);
        }
        requestRefresh();
    }
}

void PrivateTaskManager::_completePrivateTaskPushButtonClicked()
{
    if (auto privateTask = _selectedPrivateTask())
    {
        try
        {
            if (privateTask->completed(_credentials))
            {   //  Nothing to do
                return;
            }
            //  Do we need a task completion comment ?
            QString completionComment;
            if (privateTask->requireCommentOnCompletion(_credentials))
            {
                EnterTaskCompletionCommentDialog dlg(this, privateTask);
                if (dlg.doModal() != EnterTaskCompletionCommentDialog::Result::Ok)
                {   //  OOPS! User has cancelled!
                    return;
                }
                completionComment = dlg.comment();
            }
            //  We are completing the PrivateTask...
            if (theCurrentActivity == privateTask)
            {   //  ...which is also "current" - stop it first
                if (!theCurrentActivity.replaceWith(nullptr))
                {   //  ...but if the user fails to provide a finish
                    //  comment, then don't proceed any further
                    return;
                }
            }
            //  Now we can enter the completion comment as an Event...
            if (!completionComment.isEmpty())
            {
                try
                {
                    tt3::ws::Account callerAccount =
                        privateTask->workspace()->login(_credentials); //  may throw
                    callerAccount->createEvent(
                        _credentials,
                        QDateTime::currentDateTimeUtc(),
                        completionComment + ": " +privateTask->displayName(_credentials),  //  may throw
                        tt3::ws::Activities{privateTask});  //  may throw
                }
                catch (const tt3::util::Exception & ex)
                {   //  OOPS! Log & suppress
                    qCritical() << ex.errorMessage();
                }
            }
            //  ...and complete the PrivateTask
            privateTask->setCompleted(_credentials, true);
        }
        catch (const tt3::util::Exception & ex)
        {
            ErrorDialog::show(this, ex);
        }
        requestRefresh();
    }
}

void PrivateTaskManager::_showCompletedCheckBoxToggled(bool)
{
    Component::Settings::instance()->showCompletedPrivateTasks =
        _ui->showCompletedCheckBox->isChecked();
    requestRefresh();
}

void PrivateTaskManager::_viewOptionSettingValueChanged()
{
    requestRefresh();
}

void PrivateTaskManager::_filterLineEditTextChanged(QString)
{
    requestRefresh();
}

void PrivateTaskManager::_workspaceClosed(tt3::ws::WorkspaceClosedNotification /*notification*/)
{
    requestRefresh();
}

void PrivateTaskManager::_objectCreated(tt3::ws::ObjectCreatedNotification /*notification*/)
{
    requestRefresh();
}

void PrivateTaskManager::_objectDestroyed(tt3::ws::ObjectDestroyedNotification /*notification*/)
{
    requestRefresh();
}

void PrivateTaskManager::_objectModified(tt3::ws::ObjectModifiedNotification /*notification*/)
{
    requestRefresh();
}

void PrivateTaskManager::_refreshRequested()
{
    refresh();
}

void PrivateTaskManager::_refreshTimerTimeout()
{
    if (tt3::ws::Activity activity = theCurrentActivity)
    {
        if (std::dynamic_pointer_cast<tt3::ws::PrivateTaskImpl>(activity))
        {
            refresh();
        }
    }
}

//  End of tt3-gui/PrivateTaskManager.cpp
