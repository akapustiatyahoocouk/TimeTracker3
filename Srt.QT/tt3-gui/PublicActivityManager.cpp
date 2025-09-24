//
//  tt3-gui/PublicActivityManager.cpp - tt3::gui::PublicActivityManager class implementation
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
#include "ui_PublicActivityManager.h"
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
PublicActivityManager::PublicActivityManager(
        QWidget * parent
    ) : QWidget(parent),
        //  Implementation
        _workspace(theCurrentWorkspace),
        _credentials(theCurrentCredentials),
        //  Controls
        _ui(new Ui::PublicActivityManager),
        _refreshTimer(this)
{
    _ui->setupUi(this);


    _decorations = TreeWidgetDecorations(_ui->publicActivitiesTreeWidget);

    //  Theme change means widget decorations change
    connect(&theCurrentTheme,
            &CurrentTheme::changed,
            this,
            &PublicActivityManager::_currentThemeChanged,
            Qt::ConnectionType::QueuedConnection);

    //  Current activity change means, at least, a refresh
    connect(&theCurrentActivity,
            &CurrentActivity::changed,
            this,
            &PublicActivityManager::_currentActivityChanged,
            Qt::ConnectionType::QueuedConnection);

    //  Must listen to delayed refresh requests
    connect(this,
            &PublicActivityManager::refreshRequested,
            this,
            &PublicActivityManager::_refreshRequested,
            Qt::ConnectionType::QueuedConnection);

    //  Start listening for change notifications
    //  on the currently "viewed" Workspace
    _startListeningToWorkspaceChanges();

    //  Start refreshing on timer
    connect(&_refreshTimer,
            &QTimer::timeout,
            this,
            &PublicActivityManager::_refreshTimerTimeout);
    _refreshTimer.start(1000);
}

PublicActivityManager::~PublicActivityManager()
{
    _refreshTimer.stop();
    _stopListeningToWorkspaceChanges();
    delete _ui;
}

//////////
//  Operaions
tt3::ws::Workspace PublicActivityManager::workspace() const
{
    return _workspace;
}

void PublicActivityManager::setWorkspace(tt3::ws::Workspace workspace)
{
    if (workspace != _workspace)
    {
        _stopListeningToWorkspaceChanges();
        _workspace = workspace;
        _startListeningToWorkspaceChanges();
        requestRefresh();
    }
}

tt3::ws::Credentials PublicActivityManager::credentials() const
{
    return _credentials;
}

void PublicActivityManager::setCredentials(const tt3::ws::Credentials & credentials)
{
    if (credentials != _credentials)
    {
        _credentials = credentials;
        requestRefresh();
    }
}

void PublicActivityManager::refresh()
{
    static const QIcon viewPublicActivityIcon(":/tt3-gui/Resources/Images/Actions/ViewPublicActivityLarge.png");
    static const QIcon modifyPublicActivityIcon(":/tt3-gui/Resources/Images/Actions/ModifyPublicActivityLarge.png");

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
                refreshUnderway = false;
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
        _ui->publicActivitiesTreeWidget->setEnabled(true);

        //  ...while others are enabled based on current
        //  selection and permissions granted by Credentials
        _WorkspaceModel workspaceModel = _createWorkspaceModel();
        if (!_ui->filterLineEdit->text().trimmed().isEmpty())
        {
            _filterItems(workspaceModel);
        }
        _refreshPublicActivityItems(workspaceModel);

        tt3::ws::PublicActivity selectedPublicActivity = _selectedPublicActivity();
        bool readOnly = _workspace->isReadOnly();
        _ui->createPublicActivityPushButton->setEnabled(
            !readOnly &&
            (_workspace->grantsCapability(_credentials, tt3::ws::Capabilities::Administrator) ||
             _workspace->grantsCapability(_credentials, tt3::ws::Capabilities::ManagePublicActivities)));
        _ui->modifyPublicActivityPushButton->setEnabled(
            selectedPublicActivity != nullptr);
        _ui->destroyPublicActivityPushButton->setEnabled(
            !readOnly &&
            selectedPublicActivity != nullptr &&
            selectedPublicActivity->canDestroy(_credentials));

        //  TODO if the current  credentials do not allow logging
        //       Work, "start" and "stop" shall be disabled
        //  TODO if the current credentials do not allow logging
        //       Events and the selectedPublicActivity requires comment
        //       on start, "start" shall be disabled.
        //  TODO if the current credentials do not allow logging
        //       Events and the current activity requires comment on
        //       finish, "start" and "stop" shall be disabled.
        _ui->startPublicActivityPushButton->setEnabled(
            !readOnly &&
            selectedPublicActivity != nullptr &&
            theCurrentActivity != selectedPublicActivity);
        _ui->stopPublicActivityPushButton->setEnabled(
            !readOnly &&
            selectedPublicActivity != nullptr &&
            theCurrentActivity == selectedPublicActivity);

        //  Some buttons need to be adjusted for ReadOnoly mode
        if (selectedPublicActivity != nullptr &&
            !selectedPublicActivity->workspace()->isReadOnly() &&
            selectedPublicActivity->canModify(_credentials))
        {   //  RW
            _ui->modifyPublicActivityPushButton->setIcon(modifyPublicActivityIcon);
            _ui->modifyPublicActivityPushButton->setText("Modify public activity");
        }
        else
        {   //  RO
            _ui->modifyPublicActivityPushButton->setIcon(viewPublicActivityIcon);
            _ui->modifyPublicActivityPushButton->setText("View public activity");
        }
    }
}

void PublicActivityManager::requestRefresh()
{
    emit refreshRequested();
}

//////////
//  View model
auto PublicActivityManager::_createWorkspaceModel(
    )   -> PublicActivityManager::_WorkspaceModel
{
    _WorkspaceModel workspaceModel { new _WorkspaceModelImpl() };
    try
    {
        for (tt3::ws::PublicActivity publicActivity : _workspace->publicActivities(_credentials))    //  may throw
        {
            workspaceModel->publicActivityModels.append(_createPublicActivityModel(publicActivity));
        }
        std::sort(workspaceModel->publicActivityModels.begin(),
                  workspaceModel->publicActivityModels.end(),
                  [&](auto a, auto b)
                  { return a->text < b->text; });
    }
    catch (const tt3::util::Exception &)
    {
        workspaceModel->publicActivityModels.clear();
    }
    return workspaceModel;
}

auto PublicActivityManager::_createPublicActivityModel(
        tt3::ws::PublicActivity publicActivity
    ) -> PublicActivityManager::_PublicActivityModel
{
    static const QIcon errorIcon(":/tt3-gui/Resources/Images/Misc/ErrorSmall.png");

    _PublicActivityModel publicActivityModel
        { new _PublicActivityModelImpl(publicActivity) };
    try
    {
        publicActivityModel->text = publicActivity->displayName(_credentials);
        publicActivityModel->icon = publicActivity->type()->smallIcon();
        publicActivityModel->brush = _decorations.itemForeground;
        publicActivityModel->font = _decorations.itemFont;
        publicActivityModel->tooltip = publicActivity->description(_credentials).trimmed();
        //  A "current" activity needs some extras
        if (theCurrentActivity == publicActivity)
        {
            qint64 secs = qMax(0, theCurrentActivity.lastChangedAt().secsTo(QDateTime::currentDateTimeUtc()));
            char s[32];
            sprintf(s, " [%d:%02d:%02d]",
                    int(secs / (60 * 60)),
                    int((secs / 60) % 60),
                    int(secs % 60));
            publicActivityModel->text += s;
            publicActivityModel->font = _decorations.itemEmphasisFont;
        }
    }
    catch (const tt3::util::Exception & ex)
    {
        publicActivityModel->text = ex.errorMessage();
        publicActivityModel->icon = errorIcon;
        publicActivityModel->font = _decorations.itemFont;
        publicActivityModel->brush = _decorations.errorItemForeground;
        publicActivityModel->tooltip = ex.errorMessage();
    }
    return publicActivityModel;
}

void PublicActivityManager::_filterItems(_WorkspaceModel workspaceModel)
{
    QString filter = _ui->filterLineEdit->text().trimmed();
    Q_ASSERT(!filter.isEmpty());

    for (qsizetype i = workspaceModel->publicActivityModels.size() - 1; i >= 0; i--)
    {
        _PublicActivityModel publicActivityModel = workspaceModel->publicActivityModels[i];
        if (publicActivityModel->text.indexOf(filter, 0, Qt::CaseInsensitive) != -1)
        {   //  Item matches the filter - mark it as a match
            publicActivityModel->brush = _decorations.filterMatchItemForeground;
        }
        else
        {   //  Item does not match the filter
            workspaceModel->publicActivityModels.removeAt(i);
        }
    }
}

//////////
//  Implementation helpers
void PublicActivityManager::_refreshPublicActivityItems(_WorkspaceModel workspaceModel)
{
    Q_ASSERT(_workspace != nullptr);
    Q_ASSERT(_credentials.isValid());

    //  Make sure the "activity types" tree contains
    //  a proper number of root (PublicActivity) items...
    while (_ui->publicActivitiesTreeWidget->topLevelItemCount() < workspaceModel->publicActivityModels.size())
    {   //  Too few root (PublicActivity) items
        _PublicActivityModel publicActivityModel = workspaceModel->publicActivityModels[_ui->publicActivitiesTreeWidget->topLevelItemCount()];
        QTreeWidgetItem * publicActivityItem = new QTreeWidgetItem();
        publicActivityItem->setText(0, publicActivityModel->text);
        publicActivityItem->setIcon(0, publicActivityModel->icon);
        publicActivityItem->setForeground(0, publicActivityModel->brush);
        publicActivityItem->setFont(0, publicActivityModel->font);
        publicActivityItem->setToolTip(0, publicActivityModel->tooltip);
        publicActivityItem->setData(0, Qt::ItemDataRole::UserRole, QVariant::fromValue(publicActivityModel->publicActivity));
        _ui->publicActivitiesTreeWidget->addTopLevelItem(publicActivityItem);
    }
    while (_ui->publicActivitiesTreeWidget->topLevelItemCount() > workspaceModel->publicActivityModels.size())
    {   //  Too many root (PublicActivity) items
        delete _ui->publicActivitiesTreeWidget->takeTopLevelItem(
            _ui->publicActivitiesTreeWidget->topLevelItemCount() - 1);
    }

    //  ...and that each top-level item represents
    //  a proper PublicActivity and has proper children
    for (int i = 0; i < workspaceModel->publicActivityModels.size(); i++)
    {
        QTreeWidgetItem * publicActivityItem = _ui->publicActivitiesTreeWidget->topLevelItem(i);
        _PublicActivityModel publicActivityModel = workspaceModel->publicActivityModels[i];
        publicActivityItem->setText(0, publicActivityModel->text);
        publicActivityItem->setIcon(0, publicActivityModel->icon);
        publicActivityItem->setForeground(0, publicActivityModel->brush);
        publicActivityItem->setFont(0, publicActivityModel->font);
        publicActivityItem->setToolTip(0, publicActivityModel->tooltip);
        publicActivityItem->setData(0, Qt::ItemDataRole::UserRole, QVariant::fromValue(publicActivityModel->publicActivity));
    }
}

auto PublicActivityManager::_selectedPublicActivity(
    ) -> tt3::ws::PublicActivity
{
    QTreeWidgetItem * item = _ui->publicActivitiesTreeWidget->currentItem();
    return (item != nullptr) ?
               item->data(0, Qt::ItemDataRole::UserRole).value<tt3::ws::PublicActivity>() :
               nullptr;
}

void PublicActivityManager::_setSelectedPublicActivity(
        tt3::ws::PublicActivity publicActivity
    )
{
    for (int i = 0; i < _ui->publicActivitiesTreeWidget->topLevelItemCount(); i++)
    {
        QTreeWidgetItem * publicActivityItem = _ui->publicActivitiesTreeWidget->topLevelItem(i);
        if (publicActivity == publicActivityItem->data(0, Qt::ItemDataRole::UserRole).value<tt3::ws::PublicActivity>())
        {   //  This one!
            _ui->publicActivitiesTreeWidget->setCurrentItem(publicActivityItem);
            return;
        }
    }
}

void PublicActivityManager::_startListeningToWorkspaceChanges()
{
    if (_workspace != nullptr)
    {
        connect(_workspace.get(),
                &tt3::ws::WorkspaceImpl::workspaceClosed,
                this,
                &PublicActivityManager::_workspaceClosed,
                Qt::ConnectionType::QueuedConnection);
        connect(_workspace.get(),
                &tt3::ws::WorkspaceImpl::objectCreated,
                this,
                &PublicActivityManager::_objectCreated,
                Qt::ConnectionType::QueuedConnection);
        connect(_workspace.get(),
                &tt3::ws::WorkspaceImpl::objectDestroyed,
                this,
                &PublicActivityManager::_objectDestroyed,
                Qt::ConnectionType::QueuedConnection);
        connect(_workspace.get(),
                &tt3::ws::WorkspaceImpl::objectModified,
                this,
                &PublicActivityManager::_objectModified,
                Qt::ConnectionType::QueuedConnection);
    }
}

void PublicActivityManager::_stopListeningToWorkspaceChanges()
{
    if (_workspace != nullptr)
    {
        disconnect(_workspace.get(),
                   &tt3::ws::WorkspaceImpl::workspaceClosed,
                   this,
                   &PublicActivityManager::_workspaceClosed);
        disconnect(_workspace.get(),
                   &tt3::ws::WorkspaceImpl::objectCreated,
                   this,
                   &PublicActivityManager::_objectCreated);
        disconnect(_workspace.get(),
                   &tt3::ws::WorkspaceImpl::objectDestroyed,
                   this,
                   &PublicActivityManager::_objectDestroyed);
        disconnect(_workspace.get(),
                   &tt3::ws::WorkspaceImpl::objectModified,
                   this,
                   &PublicActivityManager::_objectModified);
    }
}

void PublicActivityManager::_clearAndDisableAllControls()
{
    _ui->publicActivitiesTreeWidget->clear();
    _ui->filterLabel->setEnabled(false);
    _ui->filterLineEdit->setEnabled(false);
    _ui->publicActivitiesTreeWidget->setEnabled(false);
    _ui->createPublicActivityPushButton->setEnabled(false);
    _ui->modifyPublicActivityPushButton->setEnabled(false);
    _ui->destroyPublicActivityPushButton->setEnabled(false);
    _ui->startPublicActivityPushButton->setEnabled(false);
    _ui->stopPublicActivityPushButton->setEnabled(false);
}

//////////
//  Signal handlers
void PublicActivityManager::_currentThemeChanged(ITheme *, ITheme *)
{
    _ui->publicActivitiesTreeWidget->clear();
    _decorations = TreeWidgetDecorations(_ui->publicActivitiesTreeWidget);
    requestRefresh();
}

void PublicActivityManager::_currentActivityChanged(tt3::ws::Activity, tt3::ws::Activity)
{
    requestRefresh();
}

void PublicActivityManager::_publicActivitiesTreeWidgetCurrentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)
{
    requestRefresh();
}

void PublicActivityManager::_publicActivitiesTreeWidgetCustomContextMenuRequested(QPoint p)
{
    //  [re-]create the popup menu
    _publicActivitiesTreeContextMenu.reset(new QMenu());
    QAction * createPublicActivityAction =
        _publicActivitiesTreeContextMenu->addAction(
            _ui->createPublicActivityPushButton->icon(),
            _ui->createPublicActivityPushButton->text());
    QAction * modifyPublicActivityAction =
        _publicActivitiesTreeContextMenu->addAction(
            _ui->modifyPublicActivityPushButton->icon(),
            _ui->modifyPublicActivityPushButton->text());
    QAction * destroyPublicActivityAction =
        _publicActivitiesTreeContextMenu->addAction(
            _ui->destroyPublicActivityPushButton->icon(),
            _ui->destroyPublicActivityPushButton->text());
    _publicActivitiesTreeContextMenu->addSeparator();
    QAction * startPublicActivityAction =
        _publicActivitiesTreeContextMenu->addAction(
            _ui->startPublicActivityPushButton->icon(),
            _ui->startPublicActivityPushButton->text());
    QAction * stopPublicActivityAction =
        _publicActivitiesTreeContextMenu->addAction(
            _ui->stopPublicActivityPushButton->icon(),
            _ui->stopPublicActivityPushButton->text());
    //  Adjust menu item states
    createPublicActivityAction->setEnabled(_ui->createPublicActivityPushButton->isEnabled());
    modifyPublicActivityAction->setEnabled(_ui->modifyPublicActivityPushButton->isEnabled());
    destroyPublicActivityAction->setEnabled(_ui->destroyPublicActivityPushButton->isEnabled());
    startPublicActivityAction->setEnabled(_ui->startPublicActivityPushButton->isEnabled());
    stopPublicActivityAction->setEnabled(_ui->stopPublicActivityPushButton->isEnabled());
    //  Set up signal handling
    connect(createPublicActivityAction,
            &QAction::triggered,
            this,
            &PublicActivityManager::_createPublicActivityPushButtonClicked);
    connect(modifyPublicActivityAction,
            &QAction::triggered,
            this,
            &PublicActivityManager::_modifyPublicActivityPushButtonClicked);
    connect(destroyPublicActivityAction,
            &QAction::triggered,
            this,
            &PublicActivityManager::_destroyPublicActivityPushButtonClicked);
    connect(startPublicActivityAction,
            &QAction::triggered,
            this,
            &PublicActivityManager::_startPublicActivityPushButtonClicked);
    connect(stopPublicActivityAction,
            &QAction::triggered,
            this,
            &PublicActivityManager::_stopPublicActivityPushButtonClicked);
    //  Go!
    _publicActivitiesTreeContextMenu->popup(_ui->publicActivitiesTreeWidget->mapToGlobal(p));
}

void PublicActivityManager::_createPublicActivityPushButtonClicked()
{
    try
    {
        CreatePublicActivityDialog dlg(this, _workspace, _credentials);   //  may throw
        if (dlg.doModal() == CreatePublicActivityDialog::Result::Ok)
        {   //  PublicActivity created
            refresh();  //  must refresh NOW
            _setSelectedPublicActivity(dlg.createdPublicActivity());
        }
    }
    catch (const tt3::util::Exception & ex)
    {
        tt3::gui::ErrorDialog::show(this, ex);
    }
}

void PublicActivityManager::_modifyPublicActivityPushButtonClicked()
{
    if (auto publicActivity = _selectedPublicActivity())
    {   //  TODO use the same slection/condition tandem in other similar contexts
        try
        {
            ModifyPublicActivityDialog dlg(this, publicActivity, _credentials); //  may throw
            if (dlg.doModal() == ModifyPublicActivityDialog::Result::Ok)
            {   //  PublicActivity modified - its position in the activity types tree may have changed
                refresh();  //  must refresh NOW
                _setSelectedPublicActivity(publicActivity);
            }
        }
        catch (const tt3::util::Exception & ex)
        {
            ErrorDialog::show(this, ex);
            requestRefresh();
        }
    }
}

void PublicActivityManager::_destroyPublicActivityPushButtonClicked()
{
    if (auto publicActivity = _selectedPublicActivity())
    {
        try
        {
            DestroyPublicActivityDialog dlg(this, publicActivity, _credentials); //  may throw
            if (dlg.doModal() == DestroyPublicActivityDialog::Result::Ok)
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
}

void PublicActivityManager::_startPublicActivityPushButtonClicked()
{
    if (auto publicActivity = _selectedPublicActivity())
    {
        if (theCurrentActivity == publicActivity)
        {   //  Nothing to do!
            return;
        }
        try
        {
            theCurrentActivity.replaceWith(publicActivity);
        }
        catch (const tt3::util::Exception & ex)
        {
            ErrorDialog::show(this, ex);
        }
        requestRefresh();
    }
}

void PublicActivityManager::_stopPublicActivityPushButtonClicked()
{
    if (auto publicActivity = _selectedPublicActivity())
    {
        if (theCurrentActivity != publicActivity)
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

void PublicActivityManager::_filterLineEditTextChanged(QString)
{
    requestRefresh();
}

void PublicActivityManager::_workspaceClosed(tt3::ws::WorkspaceClosedNotification /*notification*/)
{
    requestRefresh();
}

void PublicActivityManager::_objectCreated(tt3::ws::ObjectCreatedNotification /*notification*/)
{
    requestRefresh();
}

void PublicActivityManager::_objectDestroyed(tt3::ws::ObjectDestroyedNotification /*notification*/)
{
    requestRefresh();
}

void PublicActivityManager::_objectModified(tt3::ws::ObjectModifiedNotification /*notification*/)
{
    requestRefresh();
}

void PublicActivityManager::_refreshRequested()
{
    refresh();
}

void PublicActivityManager::_refreshTimerTimeout()
{
    if (tt3::ws::Activity activity = theCurrentActivity)
    {
        if (std::dynamic_pointer_cast<tt3::ws::PublicActivityImpl>(activity))
        {
            refresh();
        }
    }
}

//  End of tt3-gui/PublicActivityManager.cpp
