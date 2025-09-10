//
//  tt3-gui/ActivityTypeManager.cpp - tt3::gui::ActivityTypeManager class implementation
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
#include "ui_ActivityTypeManager.h"
using namespace tt3::gui;

namespace tt3::gui
{
    extern CurrentTheme theCurrentTheme;
}

//////////
//  Construction/destruction
ActivityTypeManager::ActivityTypeManager(QWidget * parent)
    :   QWidget(parent),
        //  Implementation
        _workspace(tt3::ws::theCurrentWorkspace),
        _credentials(tt3::ws::theCurrentCredentials),
        //  Controls
        _ui(new Ui::ActivityTypeManager)
{
    _ui->setupUi(this);

    _decorations = TreeWidgetDecorations(_ui->activityTypesTreeWidget);

    //  Theme change means widget decorations change
    connect(&theCurrentTheme,
            &CurrentTheme::changed,
            this,
            &ActivityTypeManager::_currentThemeChanged,
            Qt::ConnectionType::QueuedConnection);

    //  Must listen to delayed refresh requests
    connect(this,
            &ActivityTypeManager::refreshRequested,
            this,
            &ActivityTypeManager::_refreshRequested,
            Qt::ConnectionType::QueuedConnection);

    //  Start listening for change notifications
    //  on the currently "viewed" Workspace
    _startListeningToWorkspaceChanges();
}

ActivityTypeManager::~ActivityTypeManager()
{
    _stopListeningToWorkspaceChanges();
    delete _ui;
}

//////////
//  Operaions
tt3::ws::Workspace ActivityTypeManager::workspace() const
{
    return _workspace;
}

void ActivityTypeManager::setWorkspace(tt3::ws::Workspace workspace)
{
    if (workspace != _workspace)
    {
        _stopListeningToWorkspaceChanges();
        _workspace = workspace;
        _startListeningToWorkspaceChanges();
        requestRefresh();
    }
}

tt3::ws::Credentials ActivityTypeManager::credentials() const
{
    return _credentials;
}

void ActivityTypeManager::setCredentials(const tt3::ws::Credentials & credentials)
{
    if (credentials != _credentials)
    {
        _credentials = credentials;
        requestRefresh();
    }
}

void ActivityTypeManager::refresh()
{
    //  We don't want a refresh() to trigger a recursive refresh()!
    static bool refreshUnderway = false;
    if (refreshUnderway)
    {   //  Don't recurse!
        return;
    }
    refreshUnderway = true;

    if (_workspace == nullptr || !_credentials.isValid() ||
        !_workspace->isOpen() ||
        !_workspace->canAccess(_credentials)) //  TODO handle WorkspaceExceptions
    {   //  Nothing to show...
        _ui->activityTypesTreeWidget->clear();
        //  ...so disable all controls...
        _ui->filterLabel->setEnabled(false);
        _ui->filterLineEdit->setEnabled(false);
        _ui->activityTypesTreeWidget->setEnabled(false);
        _ui->createActivityTypePushButton->setEnabled(false);
        _ui->modifyActivityTypePushButton->setEnabled(false);
        _ui->destroyActivityTypePushButton->setEnabled(false);
        //  ...and we're done
        refreshUnderway = false;
        return;
    }

    //  Otherwise some controls are always enabled...
    _ui->filterLabel->setEnabled(true);
    _ui->filterLineEdit->setEnabled(true);
    _ui->activityTypesTreeWidget->setEnabled(true);

    //  ...while others are enabled based on current
    //  selection and permissions granted by Credentials
    _WorkspaceModel workspaceModel = _createWorkspaceModel();
    if (!_ui->filterLineEdit->text().trimmed().isEmpty())
    {
        _filterItems(workspaceModel);
    }
    _refreshActivityTypeItems(workspaceModel);


    tt3::ws::ActivityType selectedActivityType = _selectedActivityType();
    _ui->createActivityTypePushButton->setEnabled(
        _workspace->grantsCapability(_credentials, tt3::ws::Capabilities::Administrator) ||
        _workspace->grantsCapability(_credentials, tt3::ws::Capabilities::ManageActivityTypes));
    _ui->modifyActivityTypePushButton->setEnabled(
        selectedActivityType != nullptr &&
        selectedActivityType->canModify(_credentials));
    _ui->destroyActivityTypePushButton->setEnabled(
        selectedActivityType != nullptr &&
        selectedActivityType->canDestroy(_credentials));

    refreshUnderway = false;
}

void ActivityTypeManager::requestRefresh()
{
    emit refreshRequested();
}

//////////
//  View model
ActivityTypeManager::_WorkspaceModel ActivityTypeManager::_createWorkspaceModel()
{
    _WorkspaceModel workspaceModel { new _WorkspaceModelImpl() };
    try
    {
        for (tt3::ws::ActivityType activityType : _workspace->activityTypes(_credentials))    //  may throw
        {
            workspaceModel->activityTypeModels.append(_createActivityTypeModel(activityType));
        }
        std::sort(workspaceModel->activityTypeModels.begin(),
                  workspaceModel->activityTypeModels.end(),
                  [&](auto a, auto b)
                    { return a->text < b->text; });
    }
    catch (...)
    {
        workspaceModel->activityTypeModels.clear();
    }
    return workspaceModel;
}

ActivityTypeManager::_ActivityTypeModel ActivityTypeManager::_createActivityTypeModel(tt3::ws::ActivityType activityType)
{
    static const QIcon errorIcon(":/tt3-gui/Resources/Images/Misc/ErrorSmall.png");

    _ActivityTypeModel activityTypeModel { new _ActivityTypeModelImpl(activityType) };
    try
    {
        activityTypeModel->text = activityType->displayName(_credentials);
        activityTypeModel->icon = activityType->type()->smallIcon();
        activityTypeModel->brush = _decorations.itemForeground;
        activityTypeModel->font = _decorations.itemFont;
        activityTypeModel->tooltip = activityType->description(_credentials).trimmed();
    }
    catch (tt3::util::Exception & ex)
    {
        activityTypeModel->text = ex.errorMessage();
        activityTypeModel->icon = errorIcon;
        activityTypeModel->font = _decorations.itemFont;
        activityTypeModel->brush = _decorations.errorItemForeground;
        activityTypeModel->tooltip = ex.errorMessage();
    }
    return activityTypeModel;
}

void ActivityTypeManager::_filterItems(_WorkspaceModel workspaceModel)
{
    QString filter = _ui->filterLineEdit->text().trimmed();
    Q_ASSERT(!filter.isEmpty());

    for (qsizetype i = workspaceModel->activityTypeModels.size() - 1; i >= 0; i--)
    {
        _ActivityTypeModel activityTypeModel = workspaceModel->activityTypeModels[i];
        if (activityTypeModel->text.indexOf(filter, 0, Qt::CaseInsensitive) != -1)
        {   //  Item matches the filter - mark it as a match
            activityTypeModel->brush = _decorations.filterMatchItemForeground;
        }
        else
        {   //  Item does not match the filter
            workspaceModel->activityTypeModels.removeAt(i);
        }
    }
}

//////////
//  Implementation helpers
void ActivityTypeManager::_refreshActivityTypeItems(_WorkspaceModel workspaceModel)
{
    Q_ASSERT(_workspace != nullptr);
    Q_ASSERT(_credentials.isValid());

    //  Make sure the "activity types" tree contains
    //  a proper number of root (ActivityType) items...
    while (_ui->activityTypesTreeWidget->topLevelItemCount() < workspaceModel->activityTypeModels.size())
    {   //  Too few root (ActivityType) items
        _ActivityTypeModel activityTypeModel = workspaceModel->activityTypeModels[_ui->activityTypesTreeWidget->topLevelItemCount()];
        QTreeWidgetItem * activityTypeItem = new QTreeWidgetItem();
        activityTypeItem->setText(0, activityTypeModel->text);
        activityTypeItem->setIcon(0, activityTypeModel->icon);
        activityTypeItem->setForeground(0, activityTypeModel->brush);
        activityTypeItem->setFont(0, activityTypeModel->font);
        activityTypeItem->setToolTip(0, activityTypeModel->tooltip);
        activityTypeItem->setData(0, Qt::ItemDataRole::UserRole, QVariant::fromValue(activityTypeModel->activityType));
        _ui->activityTypesTreeWidget->addTopLevelItem(activityTypeItem);
    }
    while (_ui->activityTypesTreeWidget->topLevelItemCount() > workspaceModel->activityTypeModels.size())
    {   //  Too many root (ActivityType) items
        delete _ui->activityTypesTreeWidget->takeTopLevelItem(
            _ui->activityTypesTreeWidget->topLevelItemCount() - 1);
    }

    //  ...and that each top-level item represents
    //  a proper ActivityType and has proper children
    for (int i = 0; i < workspaceModel->activityTypeModels.size(); i++)
    {
        QTreeWidgetItem * activityTypeItem = _ui->activityTypesTreeWidget->topLevelItem(i);
        _ActivityTypeModel activityTypeModel = workspaceModel->activityTypeModels[i];
        activityTypeItem->setText(0, activityTypeModel->text);
        activityTypeItem->setIcon(0, activityTypeModel->icon);
        activityTypeItem->setForeground(0, activityTypeModel->brush);
        activityTypeItem->setFont(0, activityTypeModel->font);
        activityTypeItem->setToolTip(0, activityTypeModel->tooltip);
        activityTypeItem->setData(0, Qt::ItemDataRole::UserRole, QVariant::fromValue(activityTypeModel->activityType));
    }
}

tt3::ws::ActivityType ActivityTypeManager::_selectedActivityType()
{
    QTreeWidgetItem * item = _ui->activityTypesTreeWidget->currentItem();
    return (item != nullptr) ?
               item->data(0, Qt::ItemDataRole::UserRole).value<tt3::ws::ActivityType>() :
               nullptr;
}

void ActivityTypeManager::_setSelectedActivityType(tt3::ws::ActivityType activityType)
{
    for (int i = 0; i < _ui->activityTypesTreeWidget->topLevelItemCount(); i++)
    {
        QTreeWidgetItem * activityTypeItem = _ui->activityTypesTreeWidget->topLevelItem(i);
        if (activityType == activityTypeItem->data(0, Qt::ItemDataRole::UserRole).value<tt3::ws::ActivityType>())
        {   //  This one!
            _ui->activityTypesTreeWidget->setCurrentItem(activityTypeItem);
            return;
        }
    }
}

void ActivityTypeManager::_startListeningToWorkspaceChanges()
{
    if (_workspace != nullptr)
    {
        connect(_workspace.get(),
                &tt3::ws::WorkspaceImpl::workspaceClosed,
                this,
                &ActivityTypeManager::_workspaceClosed,
                Qt::ConnectionType::QueuedConnection);
        connect(_workspace.get(),
                &tt3::ws::WorkspaceImpl::objectCreated,
                this,
                &ActivityTypeManager::_objectCreated,
                Qt::ConnectionType::QueuedConnection);
        connect(_workspace.get(),
                &tt3::ws::WorkspaceImpl::objectDestroyed,
                this,
                &ActivityTypeManager::_objectDestroyed,
                Qt::ConnectionType::QueuedConnection);
        connect(_workspace.get(),
                &tt3::ws::WorkspaceImpl::objectModified,
                this,
                &ActivityTypeManager::_objectModified,
                Qt::ConnectionType::QueuedConnection);
    }
}

void ActivityTypeManager::_stopListeningToWorkspaceChanges()
{
    if (_workspace != nullptr)
    {
        disconnect(_workspace.get(),
                   &tt3::ws::WorkspaceImpl::workspaceClosed,
                   this,
                   &ActivityTypeManager::_workspaceClosed);
        disconnect(_workspace.get(),
                   &tt3::ws::WorkspaceImpl::objectCreated,
                   this,
                   &ActivityTypeManager::_objectCreated);
        disconnect(_workspace.get(),
                   &tt3::ws::WorkspaceImpl::objectDestroyed,
                   this,
                   &ActivityTypeManager::_objectDestroyed);
        disconnect(_workspace.get(),
                   &tt3::ws::WorkspaceImpl::objectModified,
                   this,
                   &ActivityTypeManager::_objectModified);
    }
}

//////////
//  Signal handlers
void ActivityTypeManager::_currentThemeChanged(ITheme *, ITheme *)
{
    _ui->activityTypesTreeWidget->clear();
    _decorations = TreeWidgetDecorations(_ui->activityTypesTreeWidget);
    requestRefresh();
}

void ActivityTypeManager::_activityTypesTreeWidgetCurrentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)
{
    requestRefresh();
}

void ActivityTypeManager::_activityTypesTreeWidgetCustomContextMenuRequested(QPoint p)
{
    //  [re-]create the popup menu
    _activityTypesTreeContextMenu.reset(new QMenu());
    QAction * createActivityTypeAction =
        _activityTypesTreeContextMenu->addAction(
            _ui->createActivityTypePushButton->icon(),
            _ui->createActivityTypePushButton->text());
    QAction * modifyActivityTypeAction =
        _activityTypesTreeContextMenu->addAction(
            _ui->modifyActivityTypePushButton->icon(),
            _ui->modifyActivityTypePushButton->text());
    QAction * destroyActivityTypeAction =
        _activityTypesTreeContextMenu->addAction(
            _ui->destroyActivityTypePushButton->icon(),
            _ui->destroyActivityTypePushButton->text());
    //  Adjust menu item states
    createActivityTypeAction->setEnabled(_ui->createActivityTypePushButton->isEnabled());
    modifyActivityTypeAction->setEnabled(_ui->modifyActivityTypePushButton->isEnabled());
    destroyActivityTypeAction->setEnabled(_ui->destroyActivityTypePushButton->isEnabled());
    //  Set up signal handling
    connect(createActivityTypeAction,
            &QAction::triggered,
            this,
            &ActivityTypeManager::_createActivityTypePushButtonClicked);
    connect(modifyActivityTypeAction,
            &QAction::triggered,
            this,
            &ActivityTypeManager::_modifyActivityTypePushButtonClicked);
    connect(destroyActivityTypeAction,
            &QAction::triggered,
            this,
            &ActivityTypeManager::_destroyActivityTypePushButtonClicked);
    //  Go!
    _activityTypesTreeContextMenu->popup(_ui->activityTypesTreeWidget->mapToGlobal(p));
}

void ActivityTypeManager::_createActivityTypePushButtonClicked()
{
    CreateActivityTypeDialog dlg(this, _workspace, _credentials);
    if (dlg.doModal() == CreateActivityTypeDialog::Result::Ok)
    {   //  ActivityType created
        refresh();  //  must refresh NOW
        _setSelectedActivityType(dlg.createdActivityType());
    }
}

void ActivityTypeManager::_modifyActivityTypePushButtonClicked()
{
    /*  TODO implement
    tt3::ws::User user = _selectedUser();
    if (user != nullptr)
    {
        try
        {
            ModifyUserDialog dlg(this, user, _credentials); //  may throw
            if (dlg.doModal() == ModifyUserDialog::Result::Ok)
            {   //  User modified - its position in the users tree may have changed
                refresh();  //  must refresh NOW
                _setSelectedUser(user);
            }
        }
        catch (tt3::util::Exception & ex)
        {
            ErrorDialog::show(this, ex);
            requestRefresh();
        }
    }
    */
}

void ActivityTypeManager::_destroyActivityTypePushButtonClicked()
{
    /*  TODO implement
    tt3::ws::User user = _selectedUser();
    if (user != nullptr)
    {
        try
        {
            ConfirmDestroyUserDialog dlg(this, user, _credentials); //  may throw
            if (dlg.doModal() == ConfirmDestroyUserDialog::Result::Yes)
            {   //  Do it!
                user->destroy(_credentials);    //  may throw
                requestRefresh();
            }
        }
        catch (tt3::util::Exception & ex)
        {
            ErrorDialog::show(this, ex);
            requestRefresh();
        }
    }
    */
}

void ActivityTypeManager::_filterLineEditTextChanged(QString)
{
    requestRefresh();
}

void ActivityTypeManager::_workspaceClosed(tt3::ws::WorkspaceClosedNotification /*notification*/)
{
    requestRefresh();
}

void ActivityTypeManager::_objectCreated(tt3::ws::ObjectCreatedNotification /*notification*/)
{
    requestRefresh();
}

void ActivityTypeManager::_objectDestroyed(tt3::ws::ObjectDestroyedNotification /*notification*/)
{
    requestRefresh();
}

void ActivityTypeManager::_objectModified(tt3::ws::ObjectModifiedNotification /*notification*/)
{
    requestRefresh();
}

void ActivityTypeManager::_refreshRequested()
{
    refresh();
}

//  End of tt3-gui/ActivityTypeManager.cpp
