//
//  tt3-gui/PrivateActivityManager.cpp - tt3::gui::PrivateActivityManager class implementation
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
#include "ui_PrivateActivityManager.h"
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
PrivateActivityManager::PrivateActivityManager(
        QWidget * parent
    )
    :   QWidget(parent),
        //  Implementation
        _workspace(theCurrentWorkspace),
        _credentials(theCurrentCredentials),
        //  Controls
        _ui(new Ui::PrivateActivityManager),
        _refreshTimer(this)
{
    _ui->setupUi(this);

    //  Theme change means widget decorations change
    connect(&theCurrentTheme,
            &CurrentTheme::changed,
            this,
            &PrivateActivityManager::_currentThemeChanged,
            Qt::ConnectionType::QueuedConnection);

    //  Current activity change means, at least, a refresh
    connect(&theCurrentActivity,
            &CurrentActivity::changed,
            this,
            &PrivateActivityManager::_currentActivityChanged,
            Qt::ConnectionType::QueuedConnection);

    //  Must listen to delayed refresh requests
    connect(this,
            &PrivateActivityManager::refreshRequested,
            this,
            &PrivateActivityManager::_refreshRequested,
            Qt::ConnectionType::QueuedConnection);

    //  Start listening for change notifications
    //  on the currently "viewed" Workspace
    _startListeningToWorkspaceChanges();

    //  Start refreshing on timer
    connect(&_refreshTimer,
            &QTimer::timeout,
            this,
            &PrivateActivityManager::_refreshTimerTimeout);
    _refreshTimer.start(1000);
}

PrivateActivityManager::~PrivateActivityManager()
{
    _refreshTimer.stop();
    _stopListeningToWorkspaceChanges();
    delete _ui;
}

//////////
//  Operaions
tt3::ws::Workspace PrivateActivityManager::workspace() const
{
    return _workspace;
}

void PrivateActivityManager::setWorkspace(tt3::ws::Workspace workspace)
{
    if (workspace != _workspace)
    {
        _stopListeningToWorkspaceChanges();
        _workspace = workspace;
        _startListeningToWorkspaceChanges();
        requestRefresh();
    }
}

tt3::ws::Credentials PrivateActivityManager::credentials() const
{
    return _credentials;
}

void PrivateActivityManager::setCredentials(const tt3::ws::Credentials & credentials)
{
    if (credentials != _credentials)
    {
        _credentials = credentials;
        requestRefresh();
    }
}

void PrivateActivityManager::refresh()
{
    static const QIcon viewPrivateActivityIcon(":/tt3-gui/Resources/Images/Actions/ViewPrivateActivityLarge.png");
    static const QIcon modifyPrivateActivityIcon(":/tt3-gui/Resources/Images/Actions/ModifyPrivateActivityLarge.png");

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
        _ui->privateActivitiesTreeWidget->setEnabled(true);

        //  ...while others are enabled based on current
        //  selection and permissions granted by Credentials
        _WorkspaceModel workspaceModel = _createWorkspaceModel();
        if (!_ui->filterLineEdit->text().trimmed().isEmpty())
        {
            _filterItems(workspaceModel);
        }
        _refreshWorkspaceTree(workspaceModel);

        tt3::ws::PrivateActivity selectedPrivateActivity = _selectedPrivateActivity();
        bool readOnly = _workspace->isReadOnly();
        try
        {
            _ui->createPrivateActivityPushButton->setEnabled(
                !readOnly &&
                _workspace->grantsAny(  //  may throw
                    _credentials,
                    tt3::ws::Capabilities::Administrator |
                        tt3::ws::Capabilities::ManagePrivateActivities));
        }
        catch (const tt3::util::Exception & ex)
        {   //  OOPS! Log & disable
            qCritical() << ex.errorMessage();
            _ui->createPrivateActivityPushButton->setEnabled(false);
        }
        _ui->modifyPrivateActivityPushButton->setEnabled(
            selectedPrivateActivity != nullptr);
        try
        {
            _ui->destroyPrivateActivityPushButton->setEnabled(
                !readOnly &&
                selectedPrivateActivity != nullptr &&
                selectedPrivateActivity->canDestroy(_credentials));  //  may throw
        }
        catch (const tt3::util::Exception & ex)
        {   //  OOPS! Log & disable
            qCritical() << ex.errorMessage();
            _ui->destroyPrivateActivityPushButton->setEnabled(false);
        }

        //  TODO if the current  credentials do not allow logging
        //       Work, "start" and "stop" shall be disabled
        //  TODO if the current credentials do not allow logging
        //       Events and the selectedPrivateActivity requires comment
        //       on start, "start" shall be disabled.
        //  TODO if the current credentials do not allow logging
        //       Events and the current activity requires comment on
        //       finish, "start" and "stop" shall be disabled.
        _ui->startPrivateActivityPushButton->setEnabled(
            !readOnly &&
            selectedPrivateActivity != nullptr &&
            theCurrentActivity != selectedPrivateActivity);
        _ui->stopPrivateActivityPushButton->setEnabled(
            !readOnly &&
            selectedPrivateActivity != nullptr &&
            theCurrentActivity == selectedPrivateActivity);

        //  Some buttons need to be adjusted for ReadOnoly mode
        try
        {
            if (selectedPrivateActivity != nullptr &&
                !selectedPrivateActivity->workspace()->isReadOnly() &&
                selectedPrivateActivity->canModify(_credentials))    //  may throw
            {   //  RW
                _ui->modifyPrivateActivityPushButton->setIcon(modifyPrivateActivityIcon);
                _ui->modifyPrivateActivityPushButton->setText("Modify public activity");
            }
            else
            {   //  RO
                _ui->modifyPrivateActivityPushButton->setIcon(viewPrivateActivityIcon);
                _ui->modifyPrivateActivityPushButton->setText("View public activity");
            }
        }
        catch (const tt3::util::Exception & ex)
        {   //  OOPS! Simulate RO
            qCritical() << ex.errorMessage();
            _ui->modifyPrivateActivityPushButton->setIcon(viewPrivateActivityIcon);
            _ui->modifyPrivateActivityPushButton->setText("View public activity");
        }
    }
}

void PrivateActivityManager::requestRefresh()
{
    emit refreshRequested();
}

//////////
//  View model
auto PrivateActivityManager::_createWorkspaceModel(
    )   -> PrivateActivityManager::_WorkspaceModel
{
    _WorkspaceModel workspaceModel { new _WorkspaceModelImpl() };
    try
    {
        if (_workspace->grantsAll(_credentials, tt3::ws::Capabilities::Administrator))
        {   //  See private activities of all users
            for (tt3::ws::User user : _workspace->users(_credentials))    //  may throw
            {
                workspaceModel->userModels.append(_createUserModel(user));
            }
        }
        else
        {   //  If not Administrator, show ONLY the caller User
            workspaceModel->userModels.append(
                _createUserModel(
                    _workspace->login(_credentials)->user(_credentials)));
        }
        std::sort(workspaceModel->userModels.begin(),
                  workspaceModel->userModels.end(),
                  [&](auto a, auto b)
                  { return a->text < b->text; });
    }
    catch (const tt3::util::Exception &)
    {
        workspaceModel->userModels.clear();
    }
    return workspaceModel;
}

auto PrivateActivityManager::_createUserModel(
        tt3::ws::User user
    ) -> PrivateActivityManager::_UserModel
{
    static const QIcon errorIcon(":/tt3-gui/Resources/Images/Misc/ErrorSmall.png");

    _UserModel userModel
        { new _UserModelImpl(user) };
    try
    {
        userModel->text = user->realName(_credentials);
        if (!user->enabled(_credentials))
        {
            userModel->text += " [disabled]";
            userModel->brush = _decorations.disabledItemForeground;
        }
        else
        {
            userModel->brush = _decorations.itemForeground;
        }
        userModel->icon = user->type()->smallIcon();
        userModel->font = _decorations.itemFont;
        //  Do the children
        for (tt3::ws::PrivateActivity privateActivity : user->privateActivities(_credentials))    //  may throw
        {
            userModel->privateActivityModels.append(_createPrivateActivityModel(privateActivity));
        }
        std::sort(userModel->privateActivityModels.begin(),
                  userModel->privateActivityModels.end(),
                  [&](auto a, auto b)
                  { return a->text < b->text; });
    }
    catch (const tt3::util::Exception & ex)
    {
        userModel->text = ex.errorMessage();
        userModel->icon = errorIcon;
        userModel->font = _decorations.itemFont;
        userModel->brush = _decorations.errorItemForeground;
        userModel->tooltip = ex.errorMessage();
        userModel->privateActivityModels.clear();
    }
    return userModel;
}

auto PrivateActivityManager::_createPrivateActivityModel(
        tt3::ws::PrivateActivity privateActivity
    ) -> PrivateActivityManager::_PrivateActivityModel
{
    static const QIcon errorIcon(":/tt3-gui/Resources/Images/Misc/ErrorSmall.png");

    _PrivateActivityModel privateActivityModel
        { new _PrivateActivityModelImpl(privateActivity) };
    try
    {
        privateActivityModel->text = privateActivity->displayName(_credentials);
        privateActivityModel->icon = privateActivity->type()->smallIcon();
        privateActivityModel->brush = _decorations.itemForeground;
        privateActivityModel->font = _decorations.itemFont;
        privateActivityModel->tooltip = privateActivity->description(_credentials).trimmed();
        //  A "current" activity needs some extras
        if (theCurrentActivity == privateActivity)
        {
            qint64 secs = qMax(0, theCurrentActivity.lastChangedAt().secsTo(QDateTime::currentDateTimeUtc()));
            char s[32];
            sprintf(s, " [%d:%02d:%02d]",
                    int(secs / (60 * 60)),
                    int((secs / 60) % 60),
                    int(secs % 60));
            privateActivityModel->text += s;
            privateActivityModel->font = _decorations.itemEmphasisFont;
        }
    }
    catch (const tt3::util::Exception & ex)
    {
        privateActivityModel->text = ex.errorMessage();
        privateActivityModel->icon = errorIcon;
        privateActivityModel->font = _decorations.itemFont;
        privateActivityModel->brush = _decorations.errorItemForeground;
        privateActivityModel->tooltip = ex.errorMessage();
    }
    return privateActivityModel;
}

void PrivateActivityManager::_filterItems(
        _WorkspaceModel workspaceModel
    )
{
    QString filter = _ui->filterLineEdit->text().trimmed();
    Q_ASSERT(!filter.isEmpty());

    for (qsizetype i = workspaceModel->userModels.size() - 1; i >= 0; i--)
    {
        _UserModel userModel = workspaceModel->userModels[i];
        _filterItems(userModel);
        if (userModel->text.indexOf(filter, 0, Qt::CaseInsensitive) != -1)
        {   //  Item matches the filter - mark it as a match
            userModel->brush = _decorations.filterMatchItemForeground;
        }
        else if (userModel->privateActivityModels.isEmpty())
        {   //  Item does not match the filter and has no children - remove it
            workspaceModel->userModels.removeAt(i);
        }
        else
        {   //  Item does not match the filter but has children - show as disabled
            workspaceModel->userModels[i]->brush = _decorations.disabledItemForeground;
        }
    }
}

void PrivateActivityManager::_filterItems(
        _UserModel userModel
    )
{
    QString filter = _ui->filterLineEdit->text().trimmed();
    Q_ASSERT(!filter.isEmpty());

    for (qsizetype i = userModel->privateActivityModels.size() - 1; i >= 0; i--)
    {
        _PrivateActivityModel privateActivityModel = userModel->privateActivityModels[i];
        if (privateActivityModel->text.indexOf(filter, 0, Qt::CaseInsensitive) != -1)
        {   //  Item matches the filter - mark it as a match
            privateActivityModel->brush = _decorations.filterMatchItemForeground;
        }
        else
        {   //  Item does not match the filter
            userModel->privateActivityModels.removeAt(i);
        }
    }
}

void PrivateActivityManager::_refreshWorkspaceTree(
        _WorkspaceModel workspaceModel
    )
{
    Q_ASSERT(_workspace != nullptr);
    Q_ASSERT(_credentials.isValid());

    //  Make sure the "private activities" tree contains
    //  a proper number of root (User) items...
    while (_ui->privateActivitiesTreeWidget->topLevelItemCount() < workspaceModel->userModels.size())
    {   //  Too few root (User) items
        _ui->privateActivitiesTreeWidget->addTopLevelItem(new QTreeWidgetItem());
    }
    while (_ui->privateActivitiesTreeWidget->topLevelItemCount() > workspaceModel->userModels.size())
    {   //  Too many root (User) items
        delete _ui->privateActivitiesTreeWidget->takeTopLevelItem(
            _ui->privateActivitiesTreeWidget->topLevelItemCount() - 1);
    }
    //  ...and that each top-level item represents
    //  a proper User
    for (int i = 0; i < workspaceModel->userModels.size(); i++)
    {
        _refreshUserItem(
            _ui->privateActivitiesTreeWidget->topLevelItem(i),
            workspaceModel->userModels[i]);
    }
}

void PrivateActivityManager::_refreshUserItem(
        QTreeWidgetItem * userItem,
        _UserModel userModel
    )
{
    Q_ASSERT(_workspace != nullptr);
    Q_ASSERT(_credentials.isValid());

    //  Refresh User item properties
    userItem->setText(0, userModel->text);
    userItem->setIcon(0, userModel->icon);
    userItem->setForeground(0, userModel->brush);
    userItem->setFont(0, userModel->font);
    userItem->setToolTip(0, userModel->tooltip);
    userItem->setData(0, Qt::ItemDataRole::UserRole, QVariant::fromValue(userModel->user));
    //  Make sure the User item contains a proper
    //  number of root (PrivateActivity) items...
    while (userItem->childCount() < userModel->privateActivityModels.size())
    {   //  Too few child (PrivateActivity) items
        userItem->addChild(new QTreeWidgetItem());
    }
    while (userItem->childCount() > userModel->privateActivityModels.size())
    {   //  Too many root (User) items
        delete userItem->takeChild(
            userItem->childCount() - 1);
    }
    //  ...and that each child item represents
    //  a proper PrivateActivity
    for (int i = 0; i < userModel->privateActivityModels.size(); i++)
    {
        _refreshPrivateActivityItem(
            userItem->child(i),
            userModel->privateActivityModels[i]);
    }
}

void PrivateActivityManager::_refreshPrivateActivityItem(
        QTreeWidgetItem * privateActivityItem,
        _PrivateActivityModel privateActivityModel
    )
{
    Q_ASSERT(_workspace != nullptr);
    Q_ASSERT(_credentials.isValid());

    //  Refresh PrivateActivity item properties
    privateActivityItem->setText(0, privateActivityModel->text);
    privateActivityItem->setIcon(0, privateActivityModel->icon);
    privateActivityItem->setForeground(0, privateActivityModel->brush);
    privateActivityItem->setFont(0, privateActivityModel->font);
    privateActivityItem->setToolTip(0, privateActivityModel->tooltip);
    privateActivityItem->setData(0, Qt::ItemDataRole::UserRole, QVariant::fromValue(privateActivityModel->privateActivity));
}

//////////
//  Implementation helpers
auto PrivateActivityManager::_selectedPrivateActivity(
    ) -> tt3::ws::PrivateActivity
{
    QTreeWidgetItem * item = _ui->privateActivitiesTreeWidget->currentItem();
    return (item != nullptr && item->parent() != nullptr) ?
               item->data(0, Qt::ItemDataRole::UserRole).value<tt3::ws::PrivateActivity>() :
               nullptr;
}

void PrivateActivityManager::_setSelectedPrivateActivity(
        tt3::ws::PrivateActivity privateActivity
    )
{
    for (int i = 0; i < _ui->privateActivitiesTreeWidget->topLevelItemCount(); i++)
    {
        QTreeWidgetItem * userItem = _ui->privateActivitiesTreeWidget->topLevelItem(i);
        for (int j = 0; j < userItem->childCount(); j++)
        {
            QTreeWidgetItem * privateActivityItem = userItem->child(j);
            if (privateActivity == privateActivityItem->data(0, Qt::ItemDataRole::UserRole).value<tt3::ws::PrivateActivity>())
            {   //  This one!
                _ui->privateActivitiesTreeWidget->setCurrentItem(privateActivityItem);
                return;
            }
        }
    }
}

void PrivateActivityManager::_startListeningToWorkspaceChanges()
{
    if (_workspace != nullptr)
    {
        connect(_workspace.get(),
                &tt3::ws::WorkspaceImpl::workspaceClosed,
                this,
                &PrivateActivityManager::_workspaceClosed,
                Qt::ConnectionType::QueuedConnection);
        connect(_workspace.get(),
                &tt3::ws::WorkspaceImpl::objectCreated,
                this,
                &PrivateActivityManager::_objectCreated,
                Qt::ConnectionType::QueuedConnection);
        connect(_workspace.get(),
                &tt3::ws::WorkspaceImpl::objectDestroyed,
                this,
                &PrivateActivityManager::_objectDestroyed,
                Qt::ConnectionType::QueuedConnection);
        connect(_workspace.get(),
                &tt3::ws::WorkspaceImpl::objectModified,
                this,
                &PrivateActivityManager::_objectModified,
                Qt::ConnectionType::QueuedConnection);
    }
}

void PrivateActivityManager::_stopListeningToWorkspaceChanges()
{
    if (_workspace != nullptr)
    {
        disconnect(_workspace.get(),
                   &tt3::ws::WorkspaceImpl::workspaceClosed,
                   this,
                   &PrivateActivityManager::_workspaceClosed);
        disconnect(_workspace.get(),
                   &tt3::ws::WorkspaceImpl::objectCreated,
                   this,
                   &PrivateActivityManager::_objectCreated);
        disconnect(_workspace.get(),
                   &tt3::ws::WorkspaceImpl::objectDestroyed,
                   this,
                   &PrivateActivityManager::_objectDestroyed);
        disconnect(_workspace.get(),
                   &tt3::ws::WorkspaceImpl::objectModified,
                   this,
                   &PrivateActivityManager::_objectModified);
    }
}

void PrivateActivityManager::_clearAndDisableAllControls()
{
    _ui->privateActivitiesTreeWidget->clear();
    _ui->filterLabel->setEnabled(false);
    _ui->filterLineEdit->setEnabled(false);
    _ui->privateActivitiesTreeWidget->setEnabled(false);
    _ui->createPrivateActivityPushButton->setEnabled(false);
    _ui->modifyPrivateActivityPushButton->setEnabled(false);
    _ui->destroyPrivateActivityPushButton->setEnabled(false);
    _ui->startPrivateActivityPushButton->setEnabled(false);
    _ui->stopPrivateActivityPushButton->setEnabled(false);
}

//////////
//  Signal handlers
void PrivateActivityManager::_currentThemeChanged(ITheme *, ITheme *)
{
    _ui->privateActivitiesTreeWidget->clear();
    _decorations = TreeWidgetDecorations(_ui->privateActivitiesTreeWidget);
    requestRefresh();
}

void PrivateActivityManager::_currentActivityChanged(tt3::ws::Activity, tt3::ws::Activity)
{
    requestRefresh();
}

void PrivateActivityManager::_privateActivitiesTreeWidgetCurrentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)
{
    requestRefresh();
}

void PrivateActivityManager::_privateActivitiesTreeWidgetCustomContextMenuRequested(QPoint p)
{
    //  [re-]create the popup menu
    _privateActivitiesTreeContextMenu.reset(new QMenu());
    QAction * createPrivateActivityAction =
        _privateActivitiesTreeContextMenu->addAction(
            _ui->createPrivateActivityPushButton->icon(),
            _ui->createPrivateActivityPushButton->text());
    QAction * modifyPrivateActivityAction =
        _privateActivitiesTreeContextMenu->addAction(
            _ui->modifyPrivateActivityPushButton->icon(),
            _ui->modifyPrivateActivityPushButton->text());
    QAction * destroyPrivateActivityAction =
        _privateActivitiesTreeContextMenu->addAction(
            _ui->destroyPrivateActivityPushButton->icon(),
            _ui->destroyPrivateActivityPushButton->text());
    _privateActivitiesTreeContextMenu->addSeparator();
    QAction * startPrivateActivityAction =
        _privateActivitiesTreeContextMenu->addAction(
            _ui->startPrivateActivityPushButton->icon(),
            _ui->startPrivateActivityPushButton->text());
    QAction * stopPrivateActivityAction =
        _privateActivitiesTreeContextMenu->addAction(
            _ui->stopPrivateActivityPushButton->icon(),
            _ui->stopPrivateActivityPushButton->text());
    //  Adjust menu item states
    createPrivateActivityAction->setEnabled(_ui->createPrivateActivityPushButton->isEnabled());
    modifyPrivateActivityAction->setEnabled(_ui->modifyPrivateActivityPushButton->isEnabled());
    destroyPrivateActivityAction->setEnabled(_ui->destroyPrivateActivityPushButton->isEnabled());
    startPrivateActivityAction->setEnabled(_ui->startPrivateActivityPushButton->isEnabled());
    stopPrivateActivityAction->setEnabled(_ui->stopPrivateActivityPushButton->isEnabled());
    //  Set up signal handling
    connect(createPrivateActivityAction,
            &QAction::triggered,
            this,
            &PrivateActivityManager::_createPrivateActivityPushButtonClicked);
    connect(modifyPrivateActivityAction,
            &QAction::triggered,
            this,
            &PrivateActivityManager::_modifyPrivateActivityPushButtonClicked);
    connect(destroyPrivateActivityAction,
            &QAction::triggered,
            this,
            &PrivateActivityManager::_destroyPrivateActivityPushButtonClicked);
    connect(startPrivateActivityAction,
            &QAction::triggered,
            this,
            &PrivateActivityManager::_startPrivateActivityPushButtonClicked);
    connect(stopPrivateActivityAction,
            &QAction::triggered,
            this,
            &PrivateActivityManager::_stopPrivateActivityPushButtonClicked);
    //  Go!
    _privateActivitiesTreeContextMenu->popup(_ui->privateActivitiesTreeWidget->mapToGlobal(p));
}

void PrivateActivityManager::_createPrivateActivityPushButtonClicked()
{
    ErrorDialog::show(this, "Not yet implemented");
    /*  TODO uncomment
    try
    {
        CreatePrivateActivityDialog dlg(this, _workspace, _credentials);   //  may throw
        if (dlg.doModal() == CreatePrivateActivityDialog::Result::Ok)
        {   //  PrivateActivity created
            refresh();  //  must refresh NOW
            _setSelectedPrivateActivity(dlg.createdPrivateActivity());
        }
    }
    catch (const tt3::util::Exception & ex)
    {
        tt3::gui::ErrorDialog::show(this, ex);
    }
    */
}

void PrivateActivityManager::_modifyPrivateActivityPushButtonClicked()
{
    ErrorDialog::show(this, "Not yet implemented");
    /*  TODO uncomment
    if (auto privateActivity = _selectedPrivateActivity())
    {   //  TODO use the same slection/condition tandem in other similar contexts
        try
        {
            ModifyPrivateActivityDialog dlg(this, privateActivity, _credentials); //  may throw
            if (dlg.doModal() == ModifyPrivateActivityDialog::Result::Ok)
            {   //  PrivateActivity modified - its position in the activity types tree may have changed
                refresh();  //  must refresh NOW
                _setSelectedPrivateActivity(privateActivity);
            }
        }
        catch (const tt3::util::Exception & ex)
        {
            ErrorDialog::show(this, ex);
            requestRefresh();
        }
    }
    */
}

void PrivateActivityManager::_destroyPrivateActivityPushButtonClicked()
{
    ErrorDialog::show(this, "Not yet implemented");
    /*  TODO uncomment
    if (auto privateActivity = _selectedPrivateActivity())
    {
        try
        {
            DestroyPrivateActivityDialog dlg(this, privateActivity, _credentials); //  may throw
            if (dlg.doModal() == DestroyPrivateActivityDialog::Result::Ok)
            {   //  Destroyed
                requestRefresh();
            }
        }
        catch (const tt3::util::Exception & ex)
        {
            ErrorDialog::show(this, ex);
            requestRefresh();
        }
    }
    */
}

void PrivateActivityManager::_startPrivateActivityPushButtonClicked()
{
    if (auto privateActivity = _selectedPrivateActivity())
    {
        if (theCurrentActivity == privateActivity)
        {   //  Nothing to do!
            return;
        }
        try
        {
            theCurrentActivity.replaceWith(privateActivity);
        }
        catch (const tt3::util::Exception & ex)
        {
            ErrorDialog::show(this, ex);
        }
        requestRefresh();
    }
}

void PrivateActivityManager::_stopPrivateActivityPushButtonClicked()
{
    if (auto privateActivity = _selectedPrivateActivity())
    {
        if (theCurrentActivity != privateActivity)
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

void PrivateActivityManager::_filterLineEditTextChanged(QString)
{
    requestRefresh();
}

void PrivateActivityManager::_workspaceClosed(tt3::ws::WorkspaceClosedNotification /*notification*/)
{
    requestRefresh();
}

void PrivateActivityManager::_objectCreated(tt3::ws::ObjectCreatedNotification /*notification*/)
{
    requestRefresh();
}

void PrivateActivityManager::_objectDestroyed(tt3::ws::ObjectDestroyedNotification /*notification*/)
{
    requestRefresh();
}

void PrivateActivityManager::_objectModified(tt3::ws::ObjectModifiedNotification /*notification*/)
{
    requestRefresh();
}

void PrivateActivityManager::_refreshRequested()
{
    refresh();
}

void PrivateActivityManager::_refreshTimerTimeout()
{
    if (tt3::ws::Activity activity = theCurrentActivity)
    {
        if (std::dynamic_pointer_cast<tt3::ws::PrivateActivityImpl>(activity))
        {
            refresh();
        }
    }
}

//  End of tt3-gui/PrivateActivityManager.cpp
