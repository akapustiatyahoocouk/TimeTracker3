//
//  tt3-gui/SelectProjectParentDialog.cpp - tt3::gui::SelectProjectParentDialog class implementation
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
#include "ui_SelectProjectParentDialog.h"
using namespace tt3::gui;

//////////
//  Construction/destruction
SelectProjectParentDialog::SelectProjectParentDialog(
        QWidget * parent,
        tt3::ws::Project project,
        const tt3::ws::Credentials & credentials,
        tt3::ws::Project initialParentProject
    ) : SelectProjectParentDialog(
            parent,
            project->workspace(),
            credentials,
            initialParentProject)
{
    tt3::util::ResourceReader rr(Component::Resources::instance(), RSID(SelectProjectParentDialog));

    _project = project;
    _refresh();

    //  Set static control values
    _ui->promptLabel->setText(
        _prompt(rr.string(RID(PromptLabel)), _project));

    //  Set editable control values
    _ui->showCompletedProjectsCheckBox->setChecked(
        _project->completed(_credentials));
    _refresh();
}

SelectProjectParentDialog::SelectProjectParentDialog(
        QWidget * parent,
        tt3::ws::Workspace workspace,
        const tt3::ws::Credentials & credentials,
        tt3::ws::Project initialParentProject
    ) : QDialog(parent),
        //  Implementation
        _workspace(workspace),
        _project(nullptr),
        _credentials(credentials),
        _selectedParentProject(initialParentProject),
        //  Controls
        _ui(new Ui::SelectProjectParentDialog)
{
    tt3::util::ResourceReader rr(Component::Resources::instance(), RSID(SelectProjectParentDialog));

    Q_ASSERT(workspace != nullptr);

    _ui->setupUi(this);
    _decorations = TreeWidgetDecorations(_ui->projectsTreeWidget);
    setWindowTitle(rr.string(RID(Title)));

    //  Set static control values
    _ui->promptLabel->setText(
        rr.string(RID(PromptLabel)));
    _ui->filterLabel->setText(
        rr.string(RID(FilterLabel)));
    _ui->showCompletedProjectsCheckBox->setText(
        rr.string(RID(ShowCompletedProjectsCheckBox)));

    _ui->buttonBox->button(QDialogButtonBox::StandardButton::Ok)->
        setText(rr.string(RID(OkPushButton)));
    _ui->buttonBox->button(QDialogButtonBox::StandardButton::Ok)->
        setIcon(QIcon(":/tt3-gui/Resources/Images/Actions/OkSmall.png"));
    _ui->buttonBox->button(QDialogButtonBox::StandardButton::Cancel)->
        setText(rr.string(RID(CancelPushButton)));
    _ui->buttonBox->button(QDialogButtonBox::StandardButton::Cancel)->
        setIcon(QIcon(":/tt3-gui/Resources/Images/Actions/CancelSmall.png"));

    //  Set editable control values
    _ui->showCompletedProjectsCheckBox->setChecked(false);

    //  Adjust controls
    _refresh(); //  NOW, to adjust tree widget size to content
    _ui->projectsTreeWidget->expandAll();
    _setSelectedProject(_selectedParentProject);

    //  Done
    adjustSize();
    _ui->projectsTreeWidget->setFocus();
}

SelectProjectParentDialog::~SelectProjectParentDialog()
{
    delete _ui;
}

//////////
//  Operations
auto SelectProjectParentDialog::doModal() -> Result
{
    return Result(this->exec());
}

//////////
//  Model
void SelectProjectParentDialog::_removeReparentedProject(
        ProjectManager::_WorkspaceModel workspaceModel
    )
{
    for (qsizetype i = workspaceModel->projectModels.size() - 1; i >= 0; i--)
    {
        if (workspaceModel->projectModels[i]->project == _project)
        {
            workspaceModel->projectModels.removeAt(i);
        }
        else
        {
            _removeReparentedProject(workspaceModel->projectModels[i]);
        }
    }
}

void SelectProjectParentDialog::_removeReparentedProject(
        ProjectManager::_ProjectModel projectModel
    )
{
    for (qsizetype i = projectModel->childModels.size() - 1; i >= 0; i--)
    {
        if (projectModel->childModels[i]->project == _project)
        {
            projectModel->childModels.removeAt(i);
        }
        else
        {
            _removeReparentedProject(projectModel->childModels[i]);
        }
    }
}

//////////
//  Implementation helpers
void SelectProjectParentDialog::_refresh()
{
    tt3::util::ResourceReader rr(Component::Resources::instance(), RSID(SelectProjectParentDialog));

    //  We don't want a refresh() to trigger a recursive refresh()!
    if (auto _ = RefreshGuard(_refreshUnderway)) //  Don't recurse!
    {
        ProjectManager::_WorkspaceModel workspaceModel =
            ProjectManager::_createWorkspaceModel(
                _workspace, _credentials, _decorations);
        _removeReparentedProject(workspaceModel);
        if (!_ui->showCompletedProjectsCheckBox->isChecked())
        {
            ProjectManager::_removeCompletedItems(workspaceModel, _credentials);
        }
        if (QString filter = _ui->filterLineEdit->text().trimmed();
            !filter.isEmpty())
        {
            ProjectManager::_filterItems(
                workspaceModel, filter, _decorations);
        }
        ProjectManager::_refreshWorkspaceTree(
            _ui->projectsTreeWidget, workspaceModel);
        _refreshCheckStates();
        if (!_ui->filterLineEdit->text().trimmed().isEmpty())
        {   //  Filtered - show all
            _ui->projectsTreeWidget->expandAll();
        }

        _ui->selectionLabel->setText(
            _prompt(
                rr.string(RID(SelectionLabel)),
                _selectedParentProject));
    }
}

void SelectProjectParentDialog::_refreshCheckStates()
{
    for (int i = 0; i < _ui->projectsTreeWidget->topLevelItemCount(); i++)
    {
        _refreshCheckStates(_ui->projectsTreeWidget->topLevelItem(i));
    }
}

void SelectProjectParentDialog::_refreshCheckStates(QTreeWidgetItem * item)
{
    tt3::ws::Project project =
        item->data(0, Qt::ItemDataRole::UserRole).value<tt3::ws::Project>();
    item->setFlags(item->flags() | Qt::ItemFlag::ItemIsUserCheckable);
    item->setCheckState(
        0,
        (project == _selectedParentProject) ?
            Qt::CheckState::Checked :
            Qt::CheckState::Unchecked);
    for (int i = 0; i < item->childCount(); i++)
    {
        _refreshCheckStates(item->child(i));
    }
}

bool SelectProjectParentDialog::_setSelectedProject(tt3::ws::Project project)
{
    for (int i = 0; i < _ui->projectsTreeWidget->topLevelItemCount(); i++)
    {
        if (_setSelectedProject(_ui->projectsTreeWidget->topLevelItem(i), project))
        {
            return true;
        }
    }
    return false;
}

bool SelectProjectParentDialog::_setSelectedProject(QTreeWidgetItem * item, tt3::ws::Project project)
{
    if (project == item->data(0, Qt::ItemDataRole::UserRole).value<tt3::ws::Project>())
    {
        _ui->projectsTreeWidget->setCurrentItem(item);
        return true;
    }
    for (int i = 0; i < item->childCount(); i++)
    {
        if (_setSelectedProject(item->child(i), project))
        {
            return true;
        }
    }
    return false;
}

QStringList SelectProjectParentDialog::_projectDisplayPath(tt3::ws::Project project)
{
    QStringList result;
    if (project == nullptr)
    {
        result.append("-");
    }
    else if (auto parentProject = project->parent(_credentials))
    {
        result = _projectDisplayPath(parentProject);
        result.append(project->displayName(_credentials));
    }
    else
    {
        result.append(project->displayName(_credentials));
    }
    return result;
}

QString SelectProjectParentDialog::_prompt(
        const QString & prompt,
        tt3::ws::Project project
    )
{
    QStringList path = _projectDisplayPath(project);
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
void SelectProjectParentDialog::_projectsTreeWidgetItemChanged(
    QTreeWidgetItem * item, int)
{
    if (!_refreshUnderway)
    {
        tt3::ws::Project project =
            item->data(0, Qt::ItemDataRole::UserRole).value<tt3::ws::Project>();
        if (item->checkState(0) == Qt::Checked &&
            project != _selectedParentProject)
        {   //  Item is newly "checked" - select as "parent"
            _selectedParentProject = project;
            _setSelectedProject(project);
        }
        else if (item->checkState(0) == Qt::Unchecked &&
                 project == _selectedParentProject)
        {   //  Item is newly "unckecked" - unselect IF parent
            _selectedParentProject = nullptr;
            _setSelectedProject(project);
        }
    }
    _refresh();
}

void SelectProjectParentDialog::_projectsTreeWidgetItemDoubleClicked(QTreeWidgetItem * item)
{
    _selectedParentProject =
        item->data(0, Qt::ItemDataRole::UserRole).value<tt3::ws::Project>();
    _refresh();
    accept();
}

void SelectProjectParentDialog::_showCompletedProjectsCheckBoxStateChanged(int)
{
    _refresh();
}

void SelectProjectParentDialog::accept()
{
    done(int(Result::Ok));
}

void SelectProjectParentDialog::reject()
{
    done(int(Result::Cancel));
}

//  End of tt3-gui/SelectProjectParentDialog.cpp
