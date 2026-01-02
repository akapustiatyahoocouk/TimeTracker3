//
//  tt3-gui/ProjectManager.cpp - tt3::gui::ProjectManager class implementation
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
#include "ui_ProjectManager.h"
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
ProjectManager::ProjectManager(
        QWidget * parent
    ) : QWidget(parent),
        //  Implementation
        _workspace(theCurrentWorkspace),
        _credentials(theCurrentCredentials),
        //  Controls
        _ui(new Ui::ProjectManager)
{
    _ui->setupUi(this);
    _decorations = TreeWidgetDecorations(_ui->projectsTreeWidget);
    _applyCurrentLocale();

    //  Theme change means widget decorations change
    connect(&theCurrentTheme,
            &CurrentTheme::changed,
            this,
            &ProjectManager::_currentThemeChanged,
            Qt::ConnectionType::QueuedConnection);

    //  Locale change requires UI translation
    connect(&tt3::util::theCurrentLocale,
            &tt3::util::CurrentLocale::changed,
            this,
            &ProjectManager::_currentLocaleChanged,
            Qt::ConnectionType::QueuedConnection);

    //  Current activity change means, at least, a refresh
    connect(&theCurrentActivity,
            &CurrentActivity::changed,
            this,
            &ProjectManager::_currentActivityChanged,
            Qt::ConnectionType::QueuedConnection);

    //  View options changes should cause a refresh
    connect(&Component::Settings::instance()->showCompletedProjects,
            &tt3::util::AbstractSetting::valueChanged,
            this,
            &ProjectManager::_viewOptionSettingValueChanged,
            Qt::ConnectionType::QueuedConnection);

    //  Must listen to delayed refresh requests
    connect(this,
            &ProjectManager::refreshRequested,
            this,
            &ProjectManager::_refreshRequested,
            Qt::ConnectionType::QueuedConnection);

    //  Start listening for change notifications
    //  on the currently "viewed" Workspace
    _startListeningToWorkspaceChanges();
}

ProjectManager::~ProjectManager()
{
    _stopListeningToWorkspaceChanges();
    delete _ui;
}

//////////
//  Operaions
tt3::ws::Workspace ProjectManager::workspace() const
{
    return _workspace;
}

void ProjectManager::setWorkspace(tt3::ws::Workspace workspace)
{
    if (workspace != _workspace)
    {
        _stopListeningToWorkspaceChanges();
        _workspace = workspace;
        _startListeningToWorkspaceChanges();
        requestRefresh();
    }
}

tt3::ws::Credentials ProjectManager::credentials() const
{
    return _credentials;
}

void ProjectManager::setCredentials(const tt3::ws::Credentials & credentials)
{
    if (credentials != _credentials)
    {
        _credentials = credentials;
        requestRefresh();
    }
}

void ProjectManager::refresh()
{
    static const QIcon viewProjectIcon(":/tt3-gui/Resources/Images/Actions/ViewProjectLarge.png");
    static const QIcon modifyProjectIcon(":/tt3-gui/Resources/Images/Actions/ModifyProjectLarge.png");
    tt3::util::ResourceReader rr(Component::Resources::instance(), RSID(ProjectManager));

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
        _ui->projectsTreeWidget->setEnabled(true);
        _ui->showCompletedCheckBox->setEnabled(true);

        //  ...while others are enabled based on current
        //  selection and permissions granted by Credentials
        _WorkspaceModel workspaceModel =
            _createWorkspaceModel(_workspace, _credentials, _decorations);
        if (!Component::Settings::instance()->showCompletedProjects)
        {
            _removeCompletedItems(workspaceModel, _credentials);
        }
        if (QString filter = _ui->filterLineEdit->text().trimmed();
            !filter.isEmpty())
        {
            _filterItems(workspaceModel, filter, _decorations);
        }
        _refreshWorkspaceTree(_ui->projectsTreeWidget, workspaceModel);
        if (!_ui->filterLineEdit->text().trimmed().isEmpty())
        {   //  Filtered - show all
            _ui->projectsTreeWidget->expandAll();
        }

        tt3::ws::Project currentProject = _currentProject();
        bool readOnly = _workspace->isReadOnly();
        try
        {
            _ui->createProjectPushButton->setEnabled(
                !readOnly &&
                _workspace->grantsAny(  //  may throw
                    _credentials,
                    tt3::ws::Capability::Administrator |
                    tt3::ws::Capability::ManageWorkloads));
        }
        catch (const tt3::util::Exception & ex)
        {   //  OOPS! Report & recover
            qCritical() << ex;
            _ui->createProjectPushButton->setEnabled(false);
        }
        _ui->modifyProjectPushButton->setEnabled(
            currentProject != nullptr);
        try
        {
            _ui->destroyProjectPushButton->setEnabled(
                !readOnly &&
                currentProject != nullptr &&
                currentProject->canDestroy(_credentials));  //  may throw
        }
        catch (const tt3::util::Exception & ex)
        {   //  OOPS! Report & recover
            qCritical() << ex;
            _ui->destroyProjectPushButton->setEnabled(false);
        }

        try
        {
            _ui->completeProjectPushButton->setEnabled(
                !readOnly &&
                currentProject != nullptr &&
                currentProject->canModify(_credentials) &&   //  may throw
                !currentProject->completed(_credentials));   //  may throw
        }
        catch (const tt3::util::Exception & ex)
        {   //  OOPS! Report & disable
            qCritical() << ex;
            _ui->completeProjectPushButton->setEnabled(false);
        }

        _ui->showCompletedCheckBox->setChecked(
            Component::Settings::instance()->showCompletedProjects);

        //  Some buttons need to be adjusted for ReadOnoly mode
        try
        {
            if (currentProject != nullptr &&
                !currentProject->workspace()->isReadOnly() &&
                currentProject->canModify(_credentials))    //  may throw
            {   //  RW
                _ui->modifyProjectPushButton->setIcon(modifyProjectIcon);
                _ui->modifyProjectPushButton->setText(
                    rr.string(RID(ModifyProjectPushButton)));
            }
            else
            {   //  RO
                _ui->modifyProjectPushButton->setIcon(viewProjectIcon);
                _ui->modifyProjectPushButton->setText(
                    rr.string(RID(ViewProjectPushButton)));
            }
        }
        catch (const tt3::util::Exception & ex)
        {   //  OOPS! Log & simulate RO
            qCritical() << ex;
            _ui->modifyProjectPushButton->setIcon(viewProjectIcon);
            _ui->modifyProjectPushButton->setText(
                rr.string(RID(ViewProjectPushButton)));
        }
    }
}

void ProjectManager::requestRefresh()
{
    emit refreshRequested();
}

//////////
//  View model
auto ProjectManager::_createWorkspaceModel(
        tt3::ws::Workspace workspace,
        const tt3::ws::Credentials & credentials,
        const TreeWidgetDecorations & decorations
    ) -> ProjectManager::_WorkspaceModel
{
    auto workspaceModel = std::make_shared<_WorkspaceModelImpl>();
    try
    {
        for (const auto & project : workspace->rootProjects(credentials))  //  may throw
        {
            workspaceModel->projectModels.append(
                _createProjectModel(project, credentials, decorations));
        }
        std::sort(
            workspaceModel->projectModels.begin(),
            workspaceModel->projectModels.end(),
            [&](auto a, auto b)
            {
                return tt3::util::NaturalStringOrder::less(a->text, b->text);
            });
    }
    catch (const tt3::util::Exception & ex)
    {
        qCritical() << ex;
        workspaceModel->projectModels.clear();
    }
    return workspaceModel;
}

auto ProjectManager::_createProjectModel(
        tt3::ws::Project project,
        const tt3::ws::Credentials & credentials,
        const TreeWidgetDecorations & decorations
    ) -> ProjectManager::_ProjectModel
{
    static const QIcon errorIcon(":/tt3-gui/Resources/Images/Misc/ErrorSmall.png");
    tt3::util::ResourceReader rr(Component::Resources::instance(), RSID(ProjectManager));

    auto projectModel = std::make_shared<_ProjectModelImpl>(project);
    try
    {
        projectModel->text = project->displayName(credentials);
        if (project->completed(credentials))
        {
            projectModel->text += " " + rr.string(RID(ProjectCompletedSuffix));
            projectModel->brush = decorations.disabledItemForeground;
        }
        else
        {
            projectModel->brush = decorations.itemForeground;
        }
        projectModel->icon = project->type()->smallIcon();
        projectModel->font = decorations.itemFont;
        projectModel->tooltip = project->description(credentials).trimmed();
        //  Do the children
        for (const auto & child : project->children(credentials)) //  may throw
        {
            projectModel->childModels.append(
                _createProjectModel(child, credentials, decorations));
        }
        std::sort(
            projectModel->childModels.begin(),
            projectModel->childModels.end(),
            [&](auto a, auto b)
            {
                return tt3::util::NaturalStringOrder::less(a->text, b->text);
            });
    }
    catch (const tt3::util::Exception & ex)
    {
        qCritical() << ex;
        projectModel->text = ex.errorMessage();
        projectModel->icon = errorIcon;
        projectModel->font = decorations.itemFont;
        projectModel->brush = decorations.errorItemForeground;
        projectModel->tooltip = ex.errorMessage();
        projectModel->childModels.clear();
    }
    return projectModel;
}

void ProjectManager::_removeCompletedItems(
        _WorkspaceModel workspaceModel,
        const tt3::ws::Credentials & credentials
    )
{
    for (qsizetype i = workspaceModel->projectModels.size() - 1; i >= 0; i--)
    {
        _ProjectModel projectModel = workspaceModel->projectModels[i];
        _removeCompletedItems(projectModel, credentials);
        //  If this Project is completed...
        try
        {
            if (projectModel->project->completed(credentials))    //  may throw
            {
                if (projectModel->childModels.isEmpty())
                {   //  ...and has no children models - delete it
                    workspaceModel->projectModels.removeAt(i);
                }
                //  ...else it has child models - keep it
            }
            //  ...else keep this Project item
        }
        catch (const tt3::util::Exception & ex)
        {   //  OOPS! Suppress, but log
            qCritical() << ex;
        }
    }
}

void ProjectManager::_removeCompletedItems(
        _ProjectModel projectModel,
        const tt3::ws::Credentials & credentials
    )
{
    for (qsizetype i = projectModel->childModels.size() - 1; i >= 0; i--)
    {
        _ProjectModel childModel = projectModel->childModels[i];
        _removeCompletedItems(childModel, credentials);
        //  If this Project is completed...
        try
        {
            if (childModel->project->completed(credentials)) //  may throw
            {
                if (childModel->childModels.isEmpty())
                {   //  ...and has no children models - delete it
                    projectModel->childModels.removeAt(i);
                }
                //  ...else it has child models - keep it
            }
            //  ...else keep this Project item
        }
        catch (const tt3::util::Exception & ex)
        {   //  OOPS! Suppress, but log
            qCritical() << ex;
        }
    }
}

void ProjectManager::_filterItems(
        _WorkspaceModel workspaceModel,
        const QString & filter,
        const TreeWidgetDecorations & decorations
    )
{
    Q_ASSERT(!filter.isEmpty());

    for (qsizetype i = workspaceModel->projectModels.size() - 1; i >= 0; i--)
    {
        _ProjectModel projectModel = workspaceModel->projectModels[i];
        _filterItems(projectModel, filter, decorations);
        if (projectModel->text.indexOf(filter, 0, Qt::CaseInsensitive) != -1)
        {   //  Item matches the filter - mark it as a match
            projectModel->brush = decorations.filterMatchItemForeground;
        }
        else if (projectModel->childModels.isEmpty())
        {   //  Item does not match the filter and has no children - remove it
            workspaceModel->projectModels.removeAt(i);
        }
        else
        {   //  Item does not match the filter but has children - show as disabled
            workspaceModel->projectModels[i]->brush = decorations.disabledItemForeground;
        }
    }
}

void ProjectManager::_filterItems(
        _ProjectModel projectModel,
        const QString & filter,
        const TreeWidgetDecorations & decorations
    )
{
    Q_ASSERT(!filter.isEmpty());

    for (qsizetype i = projectModel->childModels.size() - 1; i >= 0; i--)
    {
        _ProjectModel childModel = projectModel->childModels[i];
        _filterItems(childModel, filter, decorations);
        if (childModel->text.indexOf(filter, 0, Qt::CaseInsensitive) != -1)
        {   //  Item matches the filter - mark it as a match
            childModel->brush = decorations.filterMatchItemForeground;
        }
        else if (childModel->childModels.isEmpty())
        {   //  Item does not match the filter and has no children - remove it
            projectModel->childModels.removeAt(i);
        }
        else
        {   //  Item does not match the filter but has children - show as disabled
            projectModel->childModels[i]->brush = decorations.disabledItemForeground;
        }
    }
}

void ProjectManager::_refreshWorkspaceTree(
        QTreeWidget * projectsTreeWidget,
        _WorkspaceModel workspaceModel
    )
{
    Q_ASSERT(projectsTreeWidget != nullptr);
    Q_ASSERT(workspaceModel != nullptr);

    //  Make sure the "projects" tree contains
    //  a proper number of root (Project) items...
    while (projectsTreeWidget->topLevelItemCount() < workspaceModel->projectModels.size())
    {   //  Too few root (Project) items
        projectsTreeWidget->addTopLevelItem(new QTreeWidgetItem());
    }
    while (projectsTreeWidget->topLevelItemCount() > workspaceModel->projectModels.size())
    {   //  Too many root (Project) items
        delete projectsTreeWidget->takeTopLevelItem(
            projectsTreeWidget->topLevelItemCount() - 1);
    }
    //  ...and that each top-level item represents
    //  a proper Project and has proper children
    for (int i = 0; i < workspaceModel->projectModels.size(); i++)
    {
        _refreshProjectItem(
            projectsTreeWidget->topLevelItem(i),
            workspaceModel->projectModels[i]);
    }
}

void ProjectManager::_refreshProjectItem(
        QTreeWidgetItem * projectItem,
        _ProjectModel projectModel
    )
{
    Q_ASSERT(projectItem != nullptr);
    Q_ASSERT(projectModel != nullptr);

    //  Make sure the tree item has correct properties
    projectItem->setText(0, projectModel->text);
    projectItem->setIcon(0, projectModel->icon);
    projectItem->setForeground(0, projectModel->brush);
    projectItem->setFont(0, projectModel->font);
    projectItem->setToolTip(0, projectModel->tooltip);
    projectItem->setData(0, Qt::ItemDataRole::UserRole, QVariant::fromValue(projectModel->project));
    //  Make sure the projects, tree item tree contains
    //  a proper number of childs items...
    while (projectItem->childCount() < projectModel->childModels.size())
    {   //  Too few child items
        projectItem->addChild(new QTreeWidgetItem());
    }
    while (projectItem->childCount() > projectModel->childModels.size())
    {   //  Too many child items
        delete projectItem->takeChild(
            projectItem->childCount() - 1);
    }
    //  ...and that each child item represents
    //  a proper Project and has proper children
    for (int i = 0; i < projectModel->childModels.size(); i++)
    {
        _refreshProjectItem(
            projectItem->child(i),
            projectModel->childModels[i]);
    }
}

//////////
//  Implementation helpers
auto ProjectManager::_currentProject(
    ) -> tt3::ws::Project
{
    QTreeWidgetItem * item = _ui->projectsTreeWidget->currentItem();
    return (item != nullptr) ?
               item->data(0, Qt::ItemDataRole::UserRole).value<tt3::ws::Project>() :
               nullptr;
}

bool ProjectManager::_setCurrentProject(
        tt3::ws::Project project
    )
{
    for (int i = 0; i < _ui->projectsTreeWidget->topLevelItemCount(); i++)
    {
        QTreeWidgetItem * item = _ui->projectsTreeWidget->topLevelItem(i);
        if (project == item->data(0, Qt::ItemDataRole::UserRole).value<tt3::ws::Project>())
        {   //  This one!
            _ui->projectsTreeWidget->setCurrentItem(item);
            return true;
        }
        if (_setCurrentProject(item, project))
        {   //  One of descendants selected
            return true;
        }
    }
    return false;
}

bool ProjectManager::_setCurrentProject(
        QTreeWidgetItem * parentItem,
        tt3::ws::Project project
    )
{
    for (int i = 0; i < parentItem->childCount(); i++)
    {
        QTreeWidgetItem * item = parentItem->child(i);
        if (project == item->data(0, Qt::ItemDataRole::UserRole).value<tt3::ws::Project>())
        {   //  This one!
            _ui->projectsTreeWidget->setCurrentItem(item);
            return true;
        }
        if (_setCurrentProject(item, project))
        {   //  One of descendants selected
            return true;
        }
    }
    return false;
}

void ProjectManager::_startListeningToWorkspaceChanges()
{
    if (_workspace != nullptr)
    {
        connect(_workspace.get(),
                &tt3::ws::WorkspaceImpl::workspaceClosed,
                this,
                &ProjectManager::_workspaceClosed,
                Qt::ConnectionType::QueuedConnection);
        connect(_workspace.get(),
                &tt3::ws::WorkspaceImpl::objectCreated,
                this,
                &ProjectManager::_objectCreated,
                Qt::ConnectionType::QueuedConnection);
        connect(_workspace.get(),
                &tt3::ws::WorkspaceImpl::objectDestroyed,
                this,
                &ProjectManager::_objectDestroyed,
                Qt::ConnectionType::QueuedConnection);
        connect(_workspace.get(),
                &tt3::ws::WorkspaceImpl::objectModified,
                this,
                &ProjectManager::_objectModified,
                Qt::ConnectionType::QueuedConnection);
    }
}

void ProjectManager::_stopListeningToWorkspaceChanges()
{
    if (_workspace != nullptr)
    {
        disconnect(_workspace.get(),
                   &tt3::ws::WorkspaceImpl::workspaceClosed,
                   this,
                   &ProjectManager::_workspaceClosed);
        disconnect(_workspace.get(),
                   &tt3::ws::WorkspaceImpl::objectCreated,
                   this,
                   &ProjectManager::_objectCreated);
        disconnect(_workspace.get(),
                   &tt3::ws::WorkspaceImpl::objectDestroyed,
                   this,
                   &ProjectManager::_objectDestroyed);
        disconnect(_workspace.get(),
                   &tt3::ws::WorkspaceImpl::objectModified,
                   this,
                   &ProjectManager::_objectModified);
    }
}

void ProjectManager::_clearAndDisableAllControls()
{
    _ui->projectsTreeWidget->clear();
    _ui->filterLineEdit->setText("");
    _ui->filterLabel->setEnabled(false);
    _ui->filterLineEdit->setEnabled(false);
    _ui->projectsTreeWidget->setEnabled(false);
    _ui->createProjectPushButton->setEnabled(false);
    _ui->modifyProjectPushButton->setEnabled(false);
    _ui->destroyProjectPushButton->setEnabled(false);
    _ui->completeProjectPushButton->setEnabled(false);
    _ui->showCompletedCheckBox->setEnabled(false);
}

void ProjectManager::_applyCurrentLocale()
{
    tt3::util::ResourceReader rr(Component::Resources::instance(), RSID(ProjectManager));

    _ui->filterLabel->setText(
        rr.string(RID(FilterLabel)));
    _ui->createProjectPushButton->setText(
        rr.string(RID(CreateProjectPushButton)));
    _ui->modifyProjectPushButton->setText(
        rr.string(RID(ModifyProjectPushButton)));
    _ui->destroyProjectPushButton->setText(
        rr.string(RID(DestroyProjectPushButton)));
    _ui->completeProjectPushButton->setText(
        rr.string(RID(CompleteProjectPushButton)));
    _ui->showCompletedCheckBox->setText(
        rr.string(RID(ShowCompletedCheckBox)));
    refresh();
}

//////////
//  Signal handlers
void ProjectManager::_currentThemeChanged(ITheme *, ITheme *)
{
    _ui->projectsTreeWidget->clear();
    _decorations = TreeWidgetDecorations(_ui->projectsTreeWidget);
    requestRefresh();
}

void ProjectManager::_currentLocaleChanged(QLocale, QLocale)
{
    _applyCurrentLocale();
    refresh();
}

void ProjectManager::_currentActivityChanged(tt3::ws::Activity, tt3::ws::Activity)
{
    requestRefresh();
}

void ProjectManager::_projectsTreeWidgetCurrentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)
{
    requestRefresh();
}

void ProjectManager::_projectsTreeWidgetCustomContextMenuRequested(QPoint p)
{
    //  [re-]create the popup menu
    _projectsTreeContextMenu.reset(new QMenu());
    QAction * createProjectAction =
        _projectsTreeContextMenu->addAction(
            _ui->createProjectPushButton->icon(),
            _ui->createProjectPushButton->text());
    QAction * modifyProjectAction =
        _projectsTreeContextMenu->addAction(
            _ui->modifyProjectPushButton->icon(),
            _ui->modifyProjectPushButton->text());
    QAction * destroyProjectAction =
        _projectsTreeContextMenu->addAction(
            _ui->destroyProjectPushButton->icon(),
            _ui->destroyProjectPushButton->text());
    _projectsTreeContextMenu->addSeparator();
    QAction * completeProjectAction =
        _projectsTreeContextMenu->addAction(
            _ui->completeProjectPushButton->icon(),
            _ui->completeProjectPushButton->text());
    //  Adjust menu item states
    createProjectAction->setEnabled(_ui->createProjectPushButton->isEnabled());
    modifyProjectAction->setEnabled(_ui->modifyProjectPushButton->isEnabled());
    destroyProjectAction->setEnabled(_ui->destroyProjectPushButton->isEnabled());
    completeProjectAction->setEnabled(_ui->completeProjectPushButton->isEnabled());
    //  Set up signal handling
    connect(createProjectAction,
            &QAction::triggered,
            this,
            &ProjectManager::_createProjectPushButtonClicked);
    connect(modifyProjectAction,
            &QAction::triggered,
            this,
            &ProjectManager::_modifyProjectPushButtonClicked);
    connect(destroyProjectAction,
            &QAction::triggered,
            this,
            &ProjectManager::_destroyProjectPushButtonClicked);
    connect(completeProjectAction,
            &QAction::triggered,
            this,
            &ProjectManager::_completeProjectPushButtonClicked);
    //  Go!
    _projectsTreeContextMenu->popup(_ui->projectsTreeWidget->mapToGlobal(p));
}

void ProjectManager::_createProjectPushButtonClicked()
{
    try
    {
        CreateProjectDialog dlg(
            this, _workspace, _credentials, _currentProject()); //  may throw
        if (dlg.doModal() == CreateProjectDialog::Result::Ok)
        {   //  Project created
            refresh();  //  must refresh NOW
            _setCurrentProject(dlg.createdProject());
        }
    }
    catch (const tt3::util::Exception & ex)
    {
        qCritical() << ex;
        tt3::gui::ErrorDialog::show(this, ex);
    }
}

void ProjectManager::_modifyProjectPushButtonClicked()
{
    if (auto project = _currentProject())
    {
        try
        {
            ModifyProjectDialog dlg(this, project, _credentials); //  may throw
            if (dlg.doModal() == ModifyProjectDialog::Result::Ok)
            {   //  Project modified - its position in the projects tree may have changed
                refresh();  //  must refresh NOW
                _setCurrentProject(project);
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

void ProjectManager::_destroyProjectPushButtonClicked()
{
    if (auto project = _currentProject())
    {
        try
        {
            DestroyProjectDialog dlg(this, project, _credentials); //  may throw
            if (dlg.doModal() == DestroyProjectDialog::Result::Ok)
            {   //  Project destroyed
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

void ProjectManager::_completeProjectPushButtonClicked()
{
    if (auto project = _currentProject())
    {
        try
        {
            if (project->completed(_credentials))
            {   //  Nothing to do
                return;
            }
            project->setCompleted(_credentials, true);
        }
        catch (const tt3::util::Exception & ex)
        {
            qCritical() << ex;
            ErrorDialog::show(this, ex);
        }
        requestRefresh();
    }
}

void ProjectManager::_showCompletedCheckBoxToggled(bool)
{
    Component::Settings::instance()->showCompletedProjects =
        _ui->showCompletedCheckBox->isChecked();
    requestRefresh();
}

void ProjectManager::_viewOptionSettingValueChanged()
{
    requestRefresh();
}

void ProjectManager::_filterLineEditTextChanged(QString)
{
    requestRefresh();
}

void ProjectManager::_workspaceClosed(tt3::ws::WorkspaceClosedNotification /*notification*/)
{
    requestRefresh();
}

void ProjectManager::_objectCreated(tt3::ws::ObjectCreatedNotification /*notification*/)
{
    requestRefresh();
}

void ProjectManager::_objectDestroyed(tt3::ws::ObjectDestroyedNotification /*notification*/)
{
    requestRefresh();
}

void ProjectManager::_objectModified(tt3::ws::ObjectModifiedNotification /*notification*/)
{
    requestRefresh();
}

void ProjectManager::_refreshRequested()
{
    refresh();
}

//  End of tt3-gui/ProjectManager.cpp
