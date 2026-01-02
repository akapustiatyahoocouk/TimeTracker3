//
//  tt3-gui/PublicTaskManager.cpp - tt3::gui::PublicTaskManager class implementation
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
#include "ui_PublicTaskManager.h"
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
PublicTaskManager::PublicTaskManager(
        QWidget * parent
    ) : QWidget(parent),
        //  Implementation
        _workspace(theCurrentWorkspace),
        _credentials(theCurrentCredentials),
        //  Controls
        _ui(new Ui::PublicTaskManager),
        _refreshTimer(this)
{
    _ui->setupUi(this);
    _decorations = TreeWidgetDecorations(_ui->publicTasksTreeWidget);
    _applyCurrentLocale();

    //  Theme change means widget decorations change
    connect(&theCurrentTheme,
            &CurrentTheme::changed,
            this,
            &PublicTaskManager::_currentThemeChanged,
            Qt::ConnectionType::QueuedConnection);

    //  Locale change requires UI translation
    connect(&tt3::util::theCurrentLocale,
            &tt3::util::CurrentLocale::changed,
            this,
            &PublicTaskManager::_currentLocaleChanged,
            Qt::ConnectionType::QueuedConnection);

    //  Current activity change means, at least, a refresh
    connect(&theCurrentActivity,
            &CurrentActivity::changed,
            this,
            &PublicTaskManager::_currentActivityChanged,
            Qt::ConnectionType::QueuedConnection);

    //  View options changes should cause a refresh
    connect(&Component::Settings::instance()->showCompletedPublicTasks,
            &tt3::util::AbstractSetting::valueChanged,
            this,
            &PublicTaskManager::_viewOptionSettingValueChanged,
            Qt::ConnectionType::QueuedConnection);

    //  Must listen to delayed refresh requests
    connect(this,
            &PublicTaskManager::refreshRequested,
            this,
            &PublicTaskManager::_refreshRequested,
            Qt::ConnectionType::QueuedConnection);

    //  Start listening for change notifications
    //  on the currently "viewed" Workspace
    _startListeningToWorkspaceChanges();

    //  Start refreshing on timer
    connect(&_refreshTimer,
            &QTimer::timeout,
            this,
            &PublicTaskManager::_refreshTimerTimeout);
    _refreshTimer.start(1000);
}

PublicTaskManager::~PublicTaskManager()
{
    _refreshTimer.stop();
    _stopListeningToWorkspaceChanges();
    delete _ui;
}

//////////
//  Operaions
tt3::ws::Workspace PublicTaskManager::workspace() const
{
    return _workspace;
}

void PublicTaskManager::setWorkspace(tt3::ws::Workspace workspace)
{
    if (workspace != _workspace)
    {
        _stopListeningToWorkspaceChanges();
        _workspace = workspace;
        _startListeningToWorkspaceChanges();
        requestRefresh();
    }
}

tt3::ws::Credentials PublicTaskManager::credentials() const
{
    return _credentials;
}

void PublicTaskManager::setCredentials(const tt3::ws::Credentials & credentials)
{
    if (credentials != _credentials)
    {
        _credentials = credentials;
        requestRefresh();
    }
}

void PublicTaskManager::refresh()
{
    static const QIcon viewPublicTaskIcon(":/tt3-gui/Resources/Images/Actions/ViewPublicTaskLarge.png");
    static const QIcon modifyPublicTaskIcon(":/tt3-gui/Resources/Images/Actions/ModifyPublicTaskLarge.png");
    tt3::util::ResourceReader rr(Component::Resources::instance(), RSID(PublicTaskManager));

    //  We don't want a refresh() to trigger a recursive refresh()!
    if (auto _ = RefreshGuard(_refreshUnderway)) //  Don't recurse!
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
            qCritical() << ex;
            _clearAndDisableAllControls();
            return;
        }

        //  Otherwise some controls are always enabled...
        _ui->filterLabel->setEnabled(true);
        _ui->filterLineEdit->setEnabled(true);
        _ui->publicTasksTreeWidget->setEnabled(true);
        _ui->showCompletedCheckBox->setEnabled(true);

        //  ...while others are enabled based on current
        //  selection and permissions granted by Credentials
        _WorkspaceModel workspaceModel =
            _createWorkspaceModel(_workspace, _credentials, _decorations);
        if (!Component::Settings::instance()->showCompletedPublicTasks)
        {
            _removeCompletedItems(workspaceModel, _credentials);
        }
        if (QString filter = _ui->filterLineEdit->text().trimmed();
            !filter.isEmpty())
        {
            _filterItems(workspaceModel, filter, _decorations);
        }
        _refreshWorkspaceTree(_ui->publicTasksTreeWidget, workspaceModel);
        if (!_ui->filterLineEdit->text().trimmed().isEmpty())
        {   //  Filtered - show all
            _ui->publicTasksTreeWidget->expandAll();
        }

        tt3::ws::PublicTask selectedPublicTask = _selectedPublicTask();
        bool readOnly = _workspace->isReadOnly();
        try
        {
            _ui->createPublicTaskPushButton->setEnabled(
                !readOnly &&
                _workspace->grantsAny(  //  may throw
                    _credentials,
                    tt3::ws::Capability::Administrator |
                    tt3::ws::Capability::ManagePublicTasks));
        }
        catch (const tt3::util::Exception & ex)
        {   //  OOPS! Report & recover
            qCritical() << ex;
            _ui->createPublicTaskPushButton->setEnabled(false);
        }
        _ui->modifyPublicTaskPushButton->setEnabled(
            selectedPublicTask != nullptr);
        try
        {
            _ui->destroyPublicTaskPushButton->setEnabled(
                !readOnly &&
                selectedPublicTask != nullptr &&
                selectedPublicTask->canDestroy(_credentials));  //  may throw
        }
        catch (const tt3::util::Exception & ex)
        {   //  OOPS! Report & recover
            qCritical() << ex;
            _ui->destroyPublicTaskPushButton->setEnabled(false);
        }

        try
        {
            _ui->startPublicTaskPushButton->setEnabled(
                !readOnly &&
                selectedPublicTask != nullptr &&
                theCurrentActivity != selectedPublicTask &&
                !selectedPublicTask->completed(_credentials) && //  may throw
                selectedPublicTask->canStart(_credentials));    //  may throw
        }
        catch (const tt3::util::Exception & ex)
        {   //  OOPS! Log & disable
            qCritical() << ex;
            _ui->startPublicTaskPushButton->setEnabled(false);
        }
        try
        {
            _ui->stopPublicTaskPushButton->setEnabled(
                !readOnly &&
                selectedPublicTask != nullptr &&
                theCurrentActivity == selectedPublicTask &&
                selectedPublicTask->canStop(_credentials)); //  may throw
        }
        catch (const tt3::util::Exception & ex)
        {   //  OOPS! Log & disable
            qCritical() << ex;
            _ui->stopPublicTaskPushButton->setEnabled(false);
        }
        try
        {
            _ui->completePublicTaskPushButton->setEnabled(
                !readOnly &&
                selectedPublicTask != nullptr &&
                selectedPublicTask->canModify(_credentials) &&   //  may throw
                !selectedPublicTask->completed(_credentials));   //  may throw
        }
        catch (const tt3::util::Exception & ex)
        {   //  OOPS! Report & disable
            qCritical() << ex;
            _ui->completePublicTaskPushButton->setEnabled(false);
        }

        _ui->showCompletedCheckBox->setChecked(
            Component::Settings::instance()->showCompletedPublicTasks);

        //  Some buttons need to be adjusted for ReadOnoly mode
        try
        {
            if (selectedPublicTask != nullptr &&
                !selectedPublicTask->workspace()->isReadOnly() &&
                selectedPublicTask->canModify(_credentials))    //  may throw
            {   //  RW
                _ui->modifyPublicTaskPushButton->setIcon(modifyPublicTaskIcon);
                _ui->modifyPublicTaskPushButton->setText(
                    rr.string(RID(ModifyPublicTaskPushButton)));
            }
            else
            {   //  RO
                _ui->modifyPublicTaskPushButton->setIcon(viewPublicTaskIcon);
                _ui->modifyPublicTaskPushButton->setText(
                    rr.string(RID(ViewPublicTaskPushButton)));
            }
        }
        catch (const tt3::util::Exception & ex)
        {   //  OOPS! Log & simulate RO
            qCritical() << ex;
            _ui->modifyPublicTaskPushButton->setIcon(viewPublicTaskIcon);
            _ui->modifyPublicTaskPushButton->setText(
                rr.string(RID(ViewPublicTaskPushButton)));
        }
    }
}

void PublicTaskManager::requestRefresh()
{
    emit refreshRequested();
}

//////////
//  View model
auto PublicTaskManager::_createWorkspaceModel(
        tt3::ws::Workspace workspace,
        const tt3::ws::Credentials & credentials,
        const TreeWidgetDecorations & decorations
    ) -> PublicTaskManager::_WorkspaceModel
{
    auto workspaceModel = std::make_shared<_WorkspaceModelImpl>();
    try
    {
        for (const auto & publicTask : workspace->rootPublicTasks(credentials))  //  may throw
        {
            workspaceModel->publicTaskModels.append(
                _createPublicTaskModel(publicTask, credentials, decorations));
        }
        std::sort(
            workspaceModel->publicTaskModels.begin(),
            workspaceModel->publicTaskModels.end(),
            [&](auto a, auto b)
            {
                return tt3::util::NaturalStringOrder::less(a->text, b->text);
            });
    }
    catch (const tt3::util::Exception & ex)
    {
        qCritical() << ex;
        workspaceModel->publicTaskModels.clear();
    }
    return workspaceModel;
}

auto PublicTaskManager::_createPublicTaskModel(
        tt3::ws::PublicTask publicTask,
        const tt3::ws::Credentials & credentials,
        const TreeWidgetDecorations & decorations
    ) -> PublicTaskManager::_PublicTaskModel
{
    static const QIcon errorIcon(":/tt3-gui/Resources/Images/Misc/ErrorSmall.png");
    tt3::util::ResourceReader rr(Component::Resources::instance(), RSID(PublicTaskManager));

    auto publicTaskModel = std::make_shared<_PublicTaskModelImpl>(publicTask);
    try
    {
        publicTaskModel->text = publicTask->displayName(credentials);
        if (publicTask->completed(credentials))
        {
            publicTaskModel->text += " " + rr.string(RID(TaskCompletedSuffix));
            publicTaskModel->brush = decorations.disabledItemForeground;
        }
        else
        {
            publicTaskModel->brush = decorations.itemForeground;
        }
        publicTaskModel->icon = publicTask->type()->smallIcon();
        publicTaskModel->font = decorations.itemFont;
        publicTaskModel->tooltip = publicTask->description(credentials).trimmed();
        //  A "current" activity needs some extras
        if (theCurrentActivity == publicTask)
        {
            qint64 secs = qMax(0, theCurrentActivity.lastChangedAt().secsTo(QDateTime::currentDateTimeUtc()));
            char s[32];
            sprintf(s, " [%d:%02d:%02d]",
                    int(secs / (60 * 60)),
                    int((secs / 60) % 60),
                    int(secs % 60));
            publicTaskModel->text += s;
            publicTaskModel->font = decorations.itemEmphasisFont;
        }
        //  Do the children
        for (const auto & child : publicTask->children(credentials)) //  may throw
        {
            publicTaskModel->childModels.append(
                _createPublicTaskModel(child, credentials, decorations));
        }
        std::sort(
            publicTaskModel->childModels.begin(),
            publicTaskModel->childModels.end(),
            [&](auto a, auto b)
            {
                return tt3::util::NaturalStringOrder::less(a->text, b->text);
            });
    }
    catch (const tt3::util::Exception & ex)
    {
        qCritical() << ex;
        publicTaskModel->text = ex.errorMessage();
        publicTaskModel->icon = errorIcon;
        publicTaskModel->font = decorations.itemFont;
        publicTaskModel->brush = decorations.errorItemForeground;
        publicTaskModel->tooltip = ex.errorMessage();
        publicTaskModel->childModels.clear();
    }
    return publicTaskModel;
}

void PublicTaskManager::_removeCompletedItems(
        _WorkspaceModel workspaceModel,
        const tt3::ws::Credentials & credentials
    )
{
    for (qsizetype i = workspaceModel->publicTaskModels.size() - 1; i >= 0; i--)
    {
        _PublicTaskModel publicTaskModel = workspaceModel->publicTaskModels[i];
        _removeCompletedItems(publicTaskModel, credentials);
        //  If this PublicTask is completed...
        try
        {
            if (publicTaskModel->publicTask->completed(credentials))    //  may throw
            {
                if (publicTaskModel->childModels.isEmpty())
                {   //  ...and has no children models - delete it
                    workspaceModel->publicTaskModels.removeAt(i);
                }
                //  ...else it has child models - keep it
            }
            //  ...else keep this PublicTask item
        }
        catch (const tt3::util::Exception & ex)
        {   //  OOPS! Suppress, but log
            qCritical() << ex;
        }
    }
}

void PublicTaskManager::_removeCompletedItems(
        _PublicTaskModel publicTaskModel,
        const tt3::ws::Credentials & credentials
    )
{
    for (qsizetype i = publicTaskModel->childModels.size() - 1; i >= 0; i--)
    {
        _PublicTaskModel childModel = publicTaskModel->childModels[i];
        _removeCompletedItems(childModel, credentials);
        //  If this PublicTask is completed...
        try
        {
            if (childModel->publicTask->completed(credentials)) //  may throw
            {
                if (childModel->childModels.isEmpty())
                {   //  ...and has no children models - delete it
                    publicTaskModel->childModels.removeAt(i);
                }
                //  ...else it has child models - keep it
            }
            //  ...else keep this PublicTask item
        }
        catch (const tt3::util::Exception & ex)
        {   //  OOPS! Suppress, but log
            qCritical() << ex;
        }
    }
}

void PublicTaskManager::_filterItems(
        _WorkspaceModel workspaceModel,
        const QString & filter,
        const TreeWidgetDecorations & decorations
    )
{
    Q_ASSERT(!filter.isEmpty());

    for (qsizetype i = workspaceModel->publicTaskModels.size() - 1; i >= 0; i--)
    {
        _PublicTaskModel publicTaskModel = workspaceModel->publicTaskModels[i];
        _filterItems(publicTaskModel, filter, decorations);
        if (publicTaskModel->text.indexOf(filter, 0, Qt::CaseInsensitive) != -1)
        {   //  Item matches the filter - mark it as a match
            publicTaskModel->brush = decorations.filterMatchItemForeground;
        }
        else if (publicTaskModel->childModels.isEmpty())
        {   //  Item does not match the filter and has no children - remove it
            workspaceModel->publicTaskModels.removeAt(i);
        }
        else
        {   //  Item does not match the filter but has children - show as disabled
            workspaceModel->publicTaskModels[i]->brush = decorations.disabledItemForeground;
        }
    }
}

void PublicTaskManager::_filterItems(
        _PublicTaskModel publicTaskModel,
        const QString & filter,
        const TreeWidgetDecorations & decorations
    )
{
    Q_ASSERT(!filter.isEmpty());

    for (qsizetype i = publicTaskModel->childModels.size() - 1; i >= 0; i--)
    {
        _PublicTaskModel childModel = publicTaskModel->childModels[i];
        _filterItems(childModel, filter, decorations);
        if (childModel->text.indexOf(filter, 0, Qt::CaseInsensitive) != -1)
        {   //  Item matches the filter - mark it as a match
            childModel->brush = decorations.filterMatchItemForeground;
        }
        else if (childModel->childModels.isEmpty())
        {   //  Item does not match the filter and has no children - remove it
            publicTaskModel->childModels.removeAt(i);
        }
        else
        {   //  Item does not match the filter but has children - show as disabled
            publicTaskModel->childModels[i]->brush = decorations.disabledItemForeground;
        }
    }
}

void PublicTaskManager::_refreshWorkspaceTree(
        QTreeWidget * publicTasksTreeWidget,
        _WorkspaceModel workspaceModel
    )
{
    Q_ASSERT(publicTasksTreeWidget != nullptr);
    Q_ASSERT(workspaceModel != nullptr);

    //  Make sure the "public tasks" tree contains
    //  a proper number of root (PublicTask) items...
    while (publicTasksTreeWidget->topLevelItemCount() < workspaceModel->publicTaskModels.size())
    {   //  Too few root (PublicTask) items
        publicTasksTreeWidget->addTopLevelItem(new QTreeWidgetItem());
    }
    while (publicTasksTreeWidget->topLevelItemCount() > workspaceModel->publicTaskModels.size())
    {   //  Too many root (PublicTask) items
        delete publicTasksTreeWidget->takeTopLevelItem(
            publicTasksTreeWidget->topLevelItemCount() - 1);
    }
    //  ...and that each top-level item represents
    //  a proper PublicTask and has proper children
    for (int i = 0; i < workspaceModel->publicTaskModels.size(); i++)
    {
        _refreshPublicTaskItem(
            publicTasksTreeWidget->topLevelItem(i),
            workspaceModel->publicTaskModels[i]);
    }
}

void PublicTaskManager::_refreshPublicTaskItem(
        QTreeWidgetItem * publicTaskItem,
        _PublicTaskModel publicTaskModel
    )
{
    Q_ASSERT(publicTaskItem != nullptr);
    Q_ASSERT(publicTaskModel != nullptr);

    //  Make sure the tree item has correct properties
    publicTaskItem->setText(0, publicTaskModel->text);
    publicTaskItem->setIcon(0, publicTaskModel->icon);
    publicTaskItem->setForeground(0, publicTaskModel->brush);
    publicTaskItem->setFont(0, publicTaskModel->font);
    publicTaskItem->setToolTip(0, publicTaskModel->tooltip);
    publicTaskItem->setData(0, Qt::ItemDataRole::UserRole, QVariant::fromValue(publicTaskModel->publicTask));
    //  Make sure the public task, tree item tree contains
    //  a proper number of childs items...
    while (publicTaskItem->childCount() < publicTaskModel->childModels.size())
    {   //  Too few child items
        publicTaskItem->addChild(new QTreeWidgetItem());
    }
    while (publicTaskItem->childCount() > publicTaskModel->childModels.size())
    {   //  Too many child items
        delete publicTaskItem->takeChild(
            publicTaskItem->childCount() - 1);
    }
    //  ...and that each child item represents
    //  a proper PublicTask and has proper children
    for (int i = 0; i < publicTaskModel->childModels.size(); i++)
    {
        _refreshPublicTaskItem(
            publicTaskItem->child(i),
            publicTaskModel->childModels[i]);
    }
}

//////////
//  Implementation helpers
auto PublicTaskManager::_selectedPublicTask(
    ) -> tt3::ws::PublicTask
{
    QTreeWidgetItem * item = _ui->publicTasksTreeWidget->currentItem();
    return (item != nullptr) ?
               item->data(0, Qt::ItemDataRole::UserRole).value<tt3::ws::PublicTask>() :
               nullptr;
}

bool PublicTaskManager::_setSelectedPublicTask(
        tt3::ws::PublicTask publicTask
    )
{
    for (int i = 0; i < _ui->publicTasksTreeWidget->topLevelItemCount(); i++)
    {
        QTreeWidgetItem * item = _ui->publicTasksTreeWidget->topLevelItem(i);
        if (publicTask == item->data(0, Qt::ItemDataRole::UserRole).value<tt3::ws::PublicTask>())
        {   //  This one!
            _ui->publicTasksTreeWidget->setCurrentItem(item);
            return true;
        }
        if (_setSelectedPublicTask(item, publicTask))
        {   //  One of descendants selected
            return true;
        }
    }
    return false;
}

bool PublicTaskManager::_setSelectedPublicTask(
        QTreeWidgetItem * parentItem,
        tt3::ws::PublicTask publicTask
    )
{
    for (int i = 0; i < parentItem->childCount(); i++)
    {
        QTreeWidgetItem * item = parentItem->child(i);
        if (publicTask == item->data(0, Qt::ItemDataRole::UserRole).value<tt3::ws::PublicTask>())
        {   //  This one!
            _ui->publicTasksTreeWidget->setCurrentItem(item);
            return true;
        }
        if (_setSelectedPublicTask(item, publicTask))
        {   //  One of descendants selected
            return true;
        }
    }
    return false;
}

void PublicTaskManager::_startListeningToWorkspaceChanges()
{
    if (_workspace != nullptr)
    {
        connect(_workspace.get(),
                &tt3::ws::WorkspaceImpl::workspaceClosed,
                this,
                &PublicTaskManager::_workspaceClosed,
                Qt::ConnectionType::QueuedConnection);
        connect(_workspace.get(),
                &tt3::ws::WorkspaceImpl::objectCreated,
                this,
                &PublicTaskManager::_objectCreated,
                Qt::ConnectionType::QueuedConnection);
        connect(_workspace.get(),
                &tt3::ws::WorkspaceImpl::objectDestroyed,
                this,
                &PublicTaskManager::_objectDestroyed,
                Qt::ConnectionType::QueuedConnection);
        connect(_workspace.get(),
                &tt3::ws::WorkspaceImpl::objectModified,
                this,
                &PublicTaskManager::_objectModified,
                Qt::ConnectionType::QueuedConnection);
    }
}

void PublicTaskManager::_stopListeningToWorkspaceChanges()
{
    if (_workspace != nullptr)
    {
        disconnect(_workspace.get(),
                   &tt3::ws::WorkspaceImpl::workspaceClosed,
                   this,
                   &PublicTaskManager::_workspaceClosed);
        disconnect(_workspace.get(),
                   &tt3::ws::WorkspaceImpl::objectCreated,
                   this,
                   &PublicTaskManager::_objectCreated);
        disconnect(_workspace.get(),
                   &tt3::ws::WorkspaceImpl::objectDestroyed,
                   this,
                   &PublicTaskManager::_objectDestroyed);
        disconnect(_workspace.get(),
                   &tt3::ws::WorkspaceImpl::objectModified,
                   this,
                   &PublicTaskManager::_objectModified);
    }
}

void PublicTaskManager::_clearAndDisableAllControls()
{
    _ui->publicTasksTreeWidget->clear();
    _ui->filterLineEdit->setText("");
    _ui->filterLabel->setEnabled(false);
    _ui->filterLineEdit->setEnabled(false);
    _ui->publicTasksTreeWidget->setEnabled(false);
    _ui->createPublicTaskPushButton->setEnabled(false);
    _ui->modifyPublicTaskPushButton->setEnabled(false);
    _ui->destroyPublicTaskPushButton->setEnabled(false);
    _ui->startPublicTaskPushButton->setEnabled(false);
    _ui->stopPublicTaskPushButton->setEnabled(false);
    _ui->completePublicTaskPushButton->setEnabled(false);
    _ui->showCompletedCheckBox->setEnabled(false);
}

void PublicTaskManager::_applyCurrentLocale()
{
    tt3::util::ResourceReader rr(Component::Resources::instance(), RSID(PublicTaskManager));

    _ui->filterLabel->setText(
        rr.string(RID(FilterLabel)));
    _ui->createPublicTaskPushButton->setText(
        rr.string(RID(CreatePublicTaskPushButton)));
    _ui->modifyPublicTaskPushButton->setText(
        rr.string(RID(ModifyPublicTaskPushButton)));
    _ui->destroyPublicTaskPushButton->setText(
        rr.string(RID(DestroyPublicTaskPushButton)));
    _ui->startPublicTaskPushButton->setText(
        rr.string(RID(StartPublicTaskPushButton)));
    _ui->stopPublicTaskPushButton->setText(
        rr.string(RID(StopPublicTaskPushButton)));
    _ui->completePublicTaskPushButton->setText(
        rr.string(RID(CompletePublicTaskPushButton)));
    _ui->showCompletedCheckBox->setText(
        rr.string(RID(ShowCompletedCheckBox)));
    refresh();
}

//////////
//  Signal handlers
void PublicTaskManager::_currentThemeChanged(ITheme *, ITheme *)
{
    _ui->publicTasksTreeWidget->clear();
    _decorations = TreeWidgetDecorations(_ui->publicTasksTreeWidget);
    requestRefresh();
}

void PublicTaskManager::_currentLocaleChanged(QLocale, QLocale)
{
    _applyCurrentLocale();
    refresh();
}

void PublicTaskManager::_currentActivityChanged(tt3::ws::Activity, tt3::ws::Activity)
{
    requestRefresh();
}

void PublicTaskManager::_publicTasksTreeWidgetCurrentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)
{
    requestRefresh();
}

void PublicTaskManager::_publicTasksTreeWidgetCustomContextMenuRequested(QPoint p)
{
    //  [re-]create the popup menu
    _publicTasksTreeContextMenu.reset(new QMenu());
    QAction * createPublicTaskAction =
        _publicTasksTreeContextMenu->addAction(
            _ui->createPublicTaskPushButton->icon(),
            _ui->createPublicTaskPushButton->text());
    QAction * modifyPublicTaskAction =
        _publicTasksTreeContextMenu->addAction(
            _ui->modifyPublicTaskPushButton->icon(),
            _ui->modifyPublicTaskPushButton->text());
    QAction * destroyPublicTaskAction =
        _publicTasksTreeContextMenu->addAction(
            _ui->destroyPublicTaskPushButton->icon(),
            _ui->destroyPublicTaskPushButton->text());
    _publicTasksTreeContextMenu->addSeparator();
    QAction * startPublicTaskAction =
        _publicTasksTreeContextMenu->addAction(
            _ui->startPublicTaskPushButton->icon(),
            _ui->startPublicTaskPushButton->text());
    QAction * stopPublicTaskAction =
        _publicTasksTreeContextMenu->addAction(
            _ui->stopPublicTaskPushButton->icon(),
            _ui->stopPublicTaskPushButton->text());
    QAction * completePublicTaskAction =
        _publicTasksTreeContextMenu->addAction(
            _ui->completePublicTaskPushButton->icon(),
            _ui->completePublicTaskPushButton->text());
    //  Adjust menu item states
    createPublicTaskAction->setEnabled(_ui->createPublicTaskPushButton->isEnabled());
    modifyPublicTaskAction->setEnabled(_ui->modifyPublicTaskPushButton->isEnabled());
    destroyPublicTaskAction->setEnabled(_ui->destroyPublicTaskPushButton->isEnabled());
    startPublicTaskAction->setEnabled(_ui->startPublicTaskPushButton->isEnabled());
    stopPublicTaskAction->setEnabled(_ui->stopPublicTaskPushButton->isEnabled());
    completePublicTaskAction->setEnabled(_ui->completePublicTaskPushButton->isEnabled());
    //  Set up signal handling
    connect(createPublicTaskAction,
            &QAction::triggered,
            this,
            &PublicTaskManager::_createPublicTaskPushButtonClicked);
    connect(modifyPublicTaskAction,
            &QAction::triggered,
            this,
            &PublicTaskManager::_modifyPublicTaskPushButtonClicked);
    connect(destroyPublicTaskAction,
            &QAction::triggered,
            this,
            &PublicTaskManager::_destroyPublicTaskPushButtonClicked);
    connect(startPublicTaskAction,
            &QAction::triggered,
            this,
            &PublicTaskManager::_startPublicTaskPushButtonClicked);
    connect(stopPublicTaskAction,
            &QAction::triggered,
            this,
            &PublicTaskManager::_stopPublicTaskPushButtonClicked);
    connect(completePublicTaskAction,
            &QAction::triggered,
            this,
            &PublicTaskManager::_completePublicTaskPushButtonClicked);
    //  Go!
    _publicTasksTreeContextMenu->popup(_ui->publicTasksTreeWidget->mapToGlobal(p));
}

void PublicTaskManager::_createPublicTaskPushButtonClicked()
{
    try
    {
        CreatePublicTaskDialog dlg(
            this, _workspace, _credentials, _selectedPublicTask()); //  may throw
        if (dlg.doModal() == CreatePublicTaskDialog::Result::Ok)
        {   //  User created
            refresh();  //  must refresh NOW
            _setSelectedPublicTask(dlg.createdPublicTask());
        }
    }
    catch (const tt3::util::Exception & ex)
    {
        qCritical() << ex;
        tt3::gui::ErrorDialog::show(this, ex);
    }
}

void PublicTaskManager::_modifyPublicTaskPushButtonClicked()
{
    if (auto publicTask = _selectedPublicTask())
    {
        try
        {
            ModifyPublicTaskDialog dlg(this, publicTask, _credentials); //  may throw
            if (dlg.doModal() == ModifyPublicTaskDialog::Result::Ok)
            {   //  PublicTask modified - its position in the public tasks tree may have changed
                refresh();  //  must refresh NOW
                _setSelectedPublicTask(publicTask);
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

void PublicTaskManager::_destroyPublicTaskPushButtonClicked()
{
    if (auto publicTask = _selectedPublicTask())
    {
        try
        {
            DestroyPublicTaskDialog dlg(this, publicTask, _credentials); //  may throw
            if (dlg.doModal() == DestroyPublicTaskDialog::Result::Ok)
            {   //  PublicTask destroyed
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

void PublicTaskManager::_startPublicTaskPushButtonClicked()
{
    if (auto publicTask = _selectedPublicTask())
    {
        if (theCurrentActivity == publicTask)
        {   //  Nothing to do!
            return;
        }
        try
        {
            theCurrentActivity.replaceWith(publicTask);
        }
        catch (const tt3::util::Exception & ex)
        {
            qCritical() << ex;
            ErrorDialog::show(this, ex);
        }
        requestRefresh();
    }
}

void PublicTaskManager::_stopPublicTaskPushButtonClicked()
{
    if (auto publicTask = _selectedPublicTask())
    {
        if (theCurrentActivity != publicTask)
        {   //  Nothing to do!
            return;
        }
        try
        {
            theCurrentActivity.replaceWith(nullptr);
        }
        catch (const tt3::util::Exception & ex)
        {
            qCritical() << ex;
            ErrorDialog::show(this, ex);
        }
        requestRefresh();
    }
}

void PublicTaskManager::_completePublicTaskPushButtonClicked()
{
    if (auto publicTask = _selectedPublicTask())
    {
        try
        {
            if (publicTask->completed(_credentials))
            {   //  Nothing to do
                return;
            }
            //  Do we need a task completion comment ?
            QString completionComment;
            if (publicTask->requireCommentOnCompletion(_credentials))
            {
                EnterTaskCompletionCommentDialog dlg(this, publicTask, _credentials);   //  may throw
                if (dlg.doModal() != EnterTaskCompletionCommentDialog::Result::Ok)
                {   //  OOPS! User has cancelled!
                    return;
                }
                completionComment = dlg.comment();
            }
            //  We are completing the PublicTask...
            if (theCurrentActivity == publicTask)
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
                        publicTask->workspace()->login(_credentials);   //  may throw
                    callerAccount->createEvent( //  may throw
                        _credentials,
                        QDateTime::currentDateTimeUtc(),
                        completionComment + ": " + publicTask->displayName(_credentials),   //  may throw
                        tt3::ws::Activities{publicTask});
                }
                catch (const tt3::util::Exception & ex)
                {   //  OOPS! Log & suppress
                    qCritical() << ex;
                }
            }
            //  ...and complete the PublicTask
            publicTask->setCompleted(_credentials, true);
        }
        catch (const tt3::util::Exception & ex)
        {
            qCritical() << ex;
            ErrorDialog::show(this, ex);
        }
        requestRefresh();
    }
}

void PublicTaskManager::_showCompletedCheckBoxToggled(bool)
{
    Component::Settings::instance()->showCompletedPublicTasks =
        _ui->showCompletedCheckBox->isChecked();
    requestRefresh();
}

void PublicTaskManager::_viewOptionSettingValueChanged()
{
    requestRefresh();
}

void PublicTaskManager::_filterLineEditTextChanged(QString)
{
    requestRefresh();
}

void PublicTaskManager::_workspaceClosed(tt3::ws::WorkspaceClosedNotification /*notification*/)
{
    requestRefresh();
}

void PublicTaskManager::_objectCreated(tt3::ws::ObjectCreatedNotification /*notification*/)
{
    requestRefresh();
}

void PublicTaskManager::_objectDestroyed(tt3::ws::ObjectDestroyedNotification /*notification*/)
{
    requestRefresh();
}

void PublicTaskManager::_objectModified(tt3::ws::ObjectModifiedNotification /*notification*/)
{
    requestRefresh();
}

void PublicTaskManager::_refreshRequested()
{
    refresh();
}

void PublicTaskManager::_refreshTimerTimeout()
{
    if (tt3::ws::Activity activity = theCurrentActivity)
    {
        if (std::dynamic_pointer_cast<tt3::ws::PublicTaskImpl>(activity))
        {
            refresh();
        }
    }
}

//  End of tt3-gui/PublicTaskManager.cpp
