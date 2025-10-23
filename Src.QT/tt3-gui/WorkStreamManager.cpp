//
//  tt3-gui/WorkStreamManager.cpp - tt3::gui::WorkStreamManager class implementation
//  TODO translate UI via Resources
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
#include "ui_WorkStreamManager.h"
using namespace tt3::gui;

namespace tt3::gui
{
    extern CurrentTheme theCurrentTheme;
    extern CurrentCredentials theCurrentCredentials;
    extern CurrentWorkspace theCurrentWorkspace;
}

//////////
//  Construction/destruction
WorkStreamManager::WorkStreamManager(
        QWidget * parent
    ) : QWidget(parent),
        //  Implementation
        _workspace(theCurrentWorkspace),
        _credentials(theCurrentCredentials),
        //  Controls
        _ui(new Ui::WorkStreamManager)
{
    _ui->setupUi(this);
    _decorations = TreeWidgetDecorations(_ui->workStreamsTreeWidget);
    _applyCurrentLocale();

    //  Theme change means widget decorations change
    connect(&theCurrentTheme,
            &CurrentTheme::changed,
            this,
            &WorkStreamManager::_currentThemeChanged,
            Qt::ConnectionType::QueuedConnection);

    //  Locale change requires UI translation
    connect(&tt3::util::theCurrentLocale,
            &tt3::util::CurrentLocale::changed,
            this,
            &WorkStreamManager::_currentLocaleChanged,
            Qt::ConnectionType::QueuedConnection);

    //  Must listen to delayed refresh requests
    connect(this,
            &WorkStreamManager::refreshRequested,
            this,
            &WorkStreamManager::_refreshRequested,
            Qt::ConnectionType::QueuedConnection);

    //  Start listening for change notifications
    //  on the currently "viewed" Workspace
    _startListeningToWorkspaceChanges();
}

WorkStreamManager::~WorkStreamManager()
{
    _stopListeningToWorkspaceChanges();
    delete _ui;
}

//////////
//  Operaions
tt3::ws::Workspace WorkStreamManager::workspace() const
{
    return _workspace;
}

void WorkStreamManager::setWorkspace(tt3::ws::Workspace workspace)
{
    if (workspace != _workspace)
    {
        _stopListeningToWorkspaceChanges();
        _workspace = workspace;
        _startListeningToWorkspaceChanges();
        requestRefresh();
    }
}

tt3::ws::Credentials WorkStreamManager::credentials() const
{
    return _credentials;
}

void WorkStreamManager::setCredentials(const tt3::ws::Credentials & credentials)
{
    if (credentials != _credentials)
    {
        _credentials = credentials;
        requestRefresh();
    }
}

void WorkStreamManager::refresh()
{
    static const QIcon viewWorkStreamIcon(":/tt3-gui/Resources/Images/Actions/ViewWorkStreamLarge.png");
    static const QIcon modifyWorkStreamIcon(":/tt3-gui/Resources/Images/Actions/ModifyWorkStreamLarge.png");
    tt3::util::ResourceReader rr(Component::Resources::instance(), RSID(WorkStreamManager));

    //  We don't want a refresh() to trigger a recursive refresh()!
    if (auto _ = RefreshGuard(_refreshUnderway)) //  Don't recurse!
    {
        try
        {
            if (_workspace == nullptr || !_credentials.isValid() ||
                !_workspace->isOpen() ||
                !_workspace->canAccess(_credentials))   //  may throw
            {   //  Nothing to show...
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
        _ui->filterLabel->setEnabled(true);
        _ui->filterLineEdit->setEnabled(true);
        _ui->workStreamsTreeWidget->setEnabled(true);

        //  ...while others are enabled based on current
        //  selection and permissions granted by Credentials
        _WorkspaceModel workspaceModel =
            _createWorkspaceModel(_workspace, _credentials, _decorations);
        QString filter = _ui->filterLineEdit->text().trimmed();
        if (!filter.isEmpty())
        {
            _filterItems(workspaceModel, filter, _decorations);
        }
        _refreshWorkspaceTree(
            _ui->workStreamsTreeWidget,
            workspaceModel);

        tt3::ws::WorkStream selectedWorkStream = _selectedWorkStream();
        bool readOnly = _workspace->isReadOnly();
        try
        {
            _ui->createWorkStreamPushButton->setEnabled(
                !readOnly &&
                _workspace->grantsAny(  //  may throw
                    _credentials,
                    tt3::ws::Capability::Administrator |
                    tt3::ws::Capability::ManageWorkloads));
        }
        catch (const tt3::util::Exception & ex)
        {   //  OOPS! Log & disable
            qCritical() << ex;
            _ui->createWorkStreamPushButton->setEnabled(false);
        }
        _ui->modifyWorkStreamPushButton->setEnabled(
            selectedWorkStream != nullptr);
        try
        {
            _ui->destroyWorkStreamPushButton->setEnabled(
                !readOnly &&
                selectedWorkStream != nullptr &&
                selectedWorkStream->canDestroy(_credentials));    //  may throw
        }
        catch (const tt3::util::Exception & ex)
        {   //  OOPS! Log & disable
            qCritical() << ex;
            _ui->destroyWorkStreamPushButton->setEnabled(false);
        }

        //  Some buttons need to be adjusted for ReadOnoly mode
        try
        {
            if (selectedWorkStream != nullptr &&
                !selectedWorkStream->workspace()->isReadOnly() &&
                selectedWorkStream->canModify(_credentials))  //  may throw
            {   //  RW
                _ui->modifyWorkStreamPushButton->setIcon(modifyWorkStreamIcon);
                _ui->modifyWorkStreamPushButton->setText(
                    rr.string(RID(ModifyWorkStreamPushButton)));
            }
            else
            {   //  RO
                _ui->modifyWorkStreamPushButton->setIcon(viewWorkStreamIcon);
                _ui->modifyWorkStreamPushButton->setText(
                    rr.string(RID(ViewWorkStreamPushButton)));
            }
        }
        catch (const tt3::util::Exception & ex)
        {   //  OOPS! Log & simulate RO
            qCritical() << ex;
            _ui->modifyWorkStreamPushButton->setIcon(viewWorkStreamIcon);
            _ui->modifyWorkStreamPushButton->setText(
                rr.string(RID(ViewWorkStreamPushButton)));
        }
    }
}

void WorkStreamManager::requestRefresh()
{
    emit refreshRequested();
}

//////////
//  View model
auto WorkStreamManager::_createWorkspaceModel(
        tt3::ws::Workspace workspace,
        const tt3::ws::Credentials & credentials,
        const TreeWidgetDecorations & decorations
    ) -> WorkStreamManager::_WorkspaceModel
{
    _WorkspaceModel workspaceModel { new _WorkspaceModelImpl() };
    try
    {
        for (tt3::ws::WorkStream workStream : workspace->workStreams(credentials))    //  may throw
        {
            workspaceModel->workStreamModels.append(
                _createWorkStreamModel(workStream, credentials, decorations));
        }
        std::sort(
            workspaceModel->workStreamModels.begin(),
            workspaceModel->workStreamModels.end(),
            [&](auto a, auto b)
            { return a->text < b->text; });
    }
    catch (const tt3::util::Exception & ex)
    {
        qCritical() << ex;
        workspaceModel->workStreamModels.clear();
    }
    return workspaceModel;
}

auto WorkStreamManager::_createWorkStreamModel(
        tt3::ws::WorkStream workStream,
        const tt3::ws::Credentials & credentials,
        const TreeWidgetDecorations & decorations
    ) -> WorkStreamManager::_WorkStreamModel
{
    static const QIcon errorIcon(":/tt3-gui/Resources/Images/Misc/ErrorSmall.png");

    _WorkStreamModel workStreamModel { new _WorkStreamModelImpl(workStream) };
    try
    {
        workStreamModel->text = workStream->displayName(credentials);
        workStreamModel->icon = workStream->type()->smallIcon();
        workStreamModel->brush = decorations.itemForeground;
        workStreamModel->font = decorations.itemFont;
        workStreamModel->tooltip = workStream->description(credentials).trimmed();
    }
    catch (const tt3::util::Exception & ex)
    {
        qCritical() << ex;
        workStreamModel->text = ex.errorMessage();
        workStreamModel->icon = errorIcon;
        workStreamModel->font = decorations.itemFont;
        workStreamModel->brush = decorations.errorItemForeground;
        workStreamModel->tooltip = ex.errorMessage();
    }
    return workStreamModel;
}

void WorkStreamManager::_filterItems(
        _WorkspaceModel workspaceModel,
        const QString & filter,
        const TreeWidgetDecorations & decorations
    )
{
    Q_ASSERT(!filter.isEmpty());

    for (qsizetype i = workspaceModel->workStreamModels.size() - 1; i >= 0; i--)
    {
        _WorkStreamModel workStreamModel = workspaceModel->workStreamModels[i];
        if (workStreamModel->text.indexOf(filter, 0, Qt::CaseInsensitive) != -1)
        {   //  Item matches the filter - mark it as a match
            workStreamModel->brush = decorations.filterMatchItemForeground;
        }
        else
        {   //  Item does not match the filter
            workspaceModel->workStreamModels.removeAt(i);
        }
    }
}

void WorkStreamManager::_refreshWorkspaceTree(
        QTreeWidget * workStreamsTreeWidget,
        _WorkspaceModel workspaceModel
    )
{
    Q_ASSERT(workStreamsTreeWidget != nullptr);
    Q_ASSERT(workspaceModel != nullptr);

    //  Make sure the "activity types" tree contains
    //  a proper number of root (WorkStream) items...
    while (workStreamsTreeWidget->topLevelItemCount() < workspaceModel->workStreamModels.size())
    {   //  Too few root (WorkStream) items
        workStreamsTreeWidget->addTopLevelItem(new QTreeWidgetItem());
    }
    while (workStreamsTreeWidget->topLevelItemCount() > workspaceModel->workStreamModels.size())
    {   //  Too many root (WorkStream) items
        delete workStreamsTreeWidget->takeTopLevelItem(
            workStreamsTreeWidget->topLevelItemCount() - 1);
    }
    //  ...and that each top-level item represents
    //  a proper WorkStream and has proper children
    for (int i = 0; i < workspaceModel->workStreamModels.size(); i++)
    {
        _refreshWorkStreamItem(
            workStreamsTreeWidget->topLevelItem(i),
            workspaceModel->workStreamModels[i]);
    }
}

void WorkStreamManager::_refreshWorkStreamItem(
    QTreeWidgetItem * workStreamItem,
    _WorkStreamModel workStreamModel
    )
{
    Q_ASSERT(workStreamItem != nullptr);
    Q_ASSERT(workStreamModel != nullptr);

    //  Refresh WorkStream item properties
    workStreamItem->setText(0, workStreamModel->text);
    workStreamItem->setIcon(0, workStreamModel->icon);
    workStreamItem->setForeground(0, workStreamModel->brush);
    workStreamItem->setFont(0, workStreamModel->font);
    workStreamItem->setToolTip(0, workStreamModel->tooltip);
    workStreamItem->setData(0, Qt::ItemDataRole::UserRole, QVariant::fromValue(workStreamModel->workStream));
    //  There will be no further children
    Q_ASSERT(workStreamItem->childCount() == 0);
}

//////////
//  Implementation helpers
tt3::ws::WorkStream WorkStreamManager::_selectedWorkStream()
{
    QTreeWidgetItem * item = _ui->workStreamsTreeWidget->currentItem();
    return (item != nullptr) ?
               item->data(0, Qt::ItemDataRole::UserRole).value<tt3::ws::WorkStream>() :
               nullptr;
}

void WorkStreamManager::_setSelectedWorkStream(
        tt3::ws::WorkStream workStream
    )
{
    for (int i = 0; i < _ui->workStreamsTreeWidget->topLevelItemCount(); i++)
    {
        QTreeWidgetItem * workStreamItem = _ui->workStreamsTreeWidget->topLevelItem(i);
        if (workStream == workStreamItem->data(0, Qt::ItemDataRole::UserRole).value<tt3::ws::WorkStream>())
        {   //  This one!
            _ui->workStreamsTreeWidget->setCurrentItem(workStreamItem);
            return;
        }
    }
}

void WorkStreamManager::_startListeningToWorkspaceChanges()
{
    if (_workspace != nullptr)
    {
        connect(_workspace.get(),
                &tt3::ws::WorkspaceImpl::workspaceClosed,
                this,
                &WorkStreamManager::_workspaceClosed,
                Qt::ConnectionType::QueuedConnection);
        connect(_workspace.get(),
                &tt3::ws::WorkspaceImpl::objectCreated,
                this,
                &WorkStreamManager::_objectCreated,
                Qt::ConnectionType::QueuedConnection);
        connect(_workspace.get(),
                &tt3::ws::WorkspaceImpl::objectDestroyed,
                this,
                &WorkStreamManager::_objectDestroyed,
                Qt::ConnectionType::QueuedConnection);
        connect(_workspace.get(),
                &tt3::ws::WorkspaceImpl::objectModified,
                this,
                &WorkStreamManager::_objectModified,
                Qt::ConnectionType::QueuedConnection);
    }
}

void WorkStreamManager::_stopListeningToWorkspaceChanges()
{
    if (_workspace != nullptr)
    {
        disconnect(_workspace.get(),
                   &tt3::ws::WorkspaceImpl::workspaceClosed,
                   this,
                   &WorkStreamManager::_workspaceClosed);
        disconnect(_workspace.get(),
                   &tt3::ws::WorkspaceImpl::objectCreated,
                   this,
                   &WorkStreamManager::_objectCreated);
        disconnect(_workspace.get(),
                   &tt3::ws::WorkspaceImpl::objectDestroyed,
                   this,
                   &WorkStreamManager::_objectDestroyed);
        disconnect(_workspace.get(),
                   &tt3::ws::WorkspaceImpl::objectModified,
                   this,
                   &WorkStreamManager::_objectModified);
    }
}

void WorkStreamManager::_clearAndDisableAllControls()
{
    _ui->workStreamsTreeWidget->clear();
    _ui->filterLabel->setEnabled(false);
    _ui->filterLineEdit->setEnabled(false);
    _ui->workStreamsTreeWidget->setEnabled(false);
    _ui->createWorkStreamPushButton->setEnabled(false);
    _ui->modifyWorkStreamPushButton->setEnabled(false);
    _ui->destroyWorkStreamPushButton->setEnabled(false);
}

void WorkStreamManager::_applyCurrentLocale()
{
    tt3::util::ResourceReader rr(Component::Resources::instance(), RSID(WorkStreamManager));

    _ui->filterLabel->setText(
        rr.string(RID(FilterLabel)));
    _ui->createWorkStreamPushButton->setText(
        rr.string(RID(CreateWorkStreamPushButton)));
    _ui->modifyWorkStreamPushButton->setText(
        rr.string(RID(ModifyWorkStreamPushButton)));
    _ui->destroyWorkStreamPushButton->setText(
        rr.string(RID(DestroyWorkStreamPushButton)));
    refresh();
}

//////////
//  Signal handlers
void WorkStreamManager::_currentThemeChanged(ITheme *, ITheme *)
{
    _ui->workStreamsTreeWidget->clear();
    _decorations = TreeWidgetDecorations(_ui->workStreamsTreeWidget);
    requestRefresh();
}

void WorkStreamManager::_currentLocaleChanged(QLocale, QLocale)
{
    _applyCurrentLocale();
    refresh();
}

void WorkStreamManager::_workStreamsTreeWidgetCurrentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)
{
    requestRefresh();
}

void WorkStreamManager::_workStreamsTreeWidgetCustomContextMenuRequested(QPoint p)
{
    //  [re-]create the popup menu
    _workStreamsTreeContextMenu.reset(new QMenu());
    QAction * createWorkStreamAction =
        _workStreamsTreeContextMenu->addAction(
            _ui->createWorkStreamPushButton->icon(),
            _ui->createWorkStreamPushButton->text());
    QAction * modifyWorkStreamAction =
        _workStreamsTreeContextMenu->addAction(
            _ui->modifyWorkStreamPushButton->icon(),
            _ui->modifyWorkStreamPushButton->text());
    QAction * destroyWorkStreamAction =
        _workStreamsTreeContextMenu->addAction(
            _ui->destroyWorkStreamPushButton->icon(),
            _ui->destroyWorkStreamPushButton->text());
    //  Adjust menu item states
    createWorkStreamAction->setEnabled(_ui->createWorkStreamPushButton->isEnabled());
    modifyWorkStreamAction->setEnabled(_ui->modifyWorkStreamPushButton->isEnabled());
    destroyWorkStreamAction->setEnabled(_ui->destroyWorkStreamPushButton->isEnabled());
    //  Set up signal handling
    connect(createWorkStreamAction,
            &QAction::triggered,
            this,
            &WorkStreamManager::_createWorkStreamPushButtonClicked);
    connect(modifyWorkStreamAction,
            &QAction::triggered,
            this,
            &WorkStreamManager::_modifyWorkStreamPushButtonClicked);
    connect(destroyWorkStreamAction,
            &QAction::triggered,
            this,
            &WorkStreamManager::_destroyWorkStreamPushButtonClicked);
    //  Go!
    _workStreamsTreeContextMenu->popup(_ui->workStreamsTreeWidget->mapToGlobal(p));
}

void WorkStreamManager::_createWorkStreamPushButtonClicked()
{
    try
    {
        CreateWorkStreamDialog dlg(this, _workspace, _credentials);   //  may throw
        if (dlg.doModal() == CreateWorkStreamDialog::Result::Ok)
        {   //  WorkStream created
            refresh();  //  must refresh NOW
            _setSelectedWorkStream(dlg.createdWorkStream());
        }
    }
    catch (const tt3::util::Exception & ex)
    {
        qCritical() << ex;
        tt3::gui::ErrorDialog::show(this, ex);
    }
}

void WorkStreamManager::_modifyWorkStreamPushButtonClicked()
{
    if (auto workStream = _selectedWorkStream())
    {
        try
        {
            ModifyWorkStreamDialog dlg(this, workStream, _credentials); //  may throw
            if (dlg.doModal() == ModifyWorkStreamDialog::Result::Ok)
            {   //  WorkStream modified - its position in the activity types tree may have changed
                refresh();  //  must refresh NOW
                _setSelectedWorkStream(workStream);
            }
        }
        catch (const tt3::util::Exception & ex)
        {
            qCritical() << ex;
            ErrorDialog::show(this, ex);
            requestRefresh();
        }
    }
}

void WorkStreamManager::_destroyWorkStreamPushButtonClicked()
{
    if (auto workStream = _selectedWorkStream())
    {
        try
        {
            DestroyWorkStreamDialog dlg(this, workStream, _credentials); //  may throw
            if (dlg.doModal() == DestroyWorkStreamDialog::Result::Ok)
            {   //  Destroyed
                requestRefresh();
            }
        }
        catch (const tt3::util::Exception & ex)
        {
            qCritical() << ex;
            ErrorDialog::show(this, ex);
            requestRefresh();
        }
    }
}

void WorkStreamManager::_filterLineEditTextChanged(QString)
{
    requestRefresh();
}

void WorkStreamManager::_workspaceClosed(tt3::ws::WorkspaceClosedNotification /*notification*/)
{
    requestRefresh();
}

void WorkStreamManager::_objectCreated(tt3::ws::ObjectCreatedNotification /*notification*/)
{
    requestRefresh();
}

void WorkStreamManager::_objectDestroyed(tt3::ws::ObjectDestroyedNotification /*notification*/)
{
    requestRefresh();
}

void WorkStreamManager::_objectModified(tt3::ws::ObjectModifiedNotification /*notification*/)
{
    requestRefresh();
}

void WorkStreamManager::_refreshRequested()
{
    refresh();
}

//  End of tt3-gui/WorkStreamManager.cpp
