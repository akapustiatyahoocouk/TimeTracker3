//
//  tt3-gui/SelectWorkloadDialog.cpp - tt3::gui::SelectWorkloadDialog class implementation
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
#include "ui_SelectWorkloadDialog.h"
using namespace tt3::gui;

//////////
//  Construction/destruction
SelectWorkloadDialog::SelectWorkloadDialog(
        QWidget * parent,
        tt3::ws::Workspace workspace,
        const tt3::ws::Credentials & credentials,
        tt3::ws::Workload workload
    ) : QDialog(parent),
        _workspace(workspace),
        _credentials(credentials),
        _selectedWorkload(workload),
        _ui(new Ui::SelectWorkloadDialog)
{
    tt3::util::ResourceReader rr(Component::Resources::instance(), RSID(SelectWorkloadDialog));

    Q_ASSERT(_workspace != nullptr);
    Q_ASSERT(_selectedWorkload == nullptr ||
             _selectedWorkload->workspace() == _workspace);

    _ui->setupUi(this);
    _treeWidgetDecorations = TreeWidgetDecorations(_ui->projectsTreeWidget);
    setWindowTitle(rr.string(RID(Title)));

    //  Set static control values
    _ui->workloadsTabWidget->setTabText(
        0, rr.string(RID(ProjectsTab)));
    _ui->workloadsTabWidget->setTabText(
        1, rr.string(RID(WorkStreamsTab)));
    _ui->projectsFilterLabel->setText(
        rr.string(RID(ProjectsFilterLabel)));
    _ui->workStreamsFilterLabel->setText(
        rr.string(RID(WorkStreamsFilterLabel)));
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
    _setSelectedWorkload(workload);
    _ui->showCompletedProjectsCheckBox->setChecked(
        Component::Settings::instance()->showCompletedProjects);

    _refresh(); //  NOW, to adjust tree widget size to content
    _ui->projectsTreeWidget->expandAll();
    _ui->workloadsTabWidget->setCurrentIndex(
        Component::Settings::instance()->selectWorkloadsDialogTab);

    //  Done
    adjustSize();
}

SelectWorkloadDialog::~SelectWorkloadDialog()
{
    delete _ui;
}

//////////
//  Operations
auto SelectWorkloadDialog::doModal() -> Result
{
    return Result(this->exec());
}

auto SelectWorkloadDialog::selectedWorkload(
    ) -> tt3::ws::Workload
{
    return _selectedWorkload;
}

//////////
//  Implementation helpers
bool SelectWorkloadDialog::_setCurrentWorkload(
        tt3::ws::Workload workload
    )
{
    if (auto project =
        std::dynamic_pointer_cast<tt3::ws::ProjectImpl>(workload))
    {
        return _setCurrentProject(project);
    }
    if (auto workStream =
        std::dynamic_pointer_cast<tt3::ws::WorkStreamImpl>(workload))
    {
        return _setCurrentWorkStream(workStream);
    }
    return false;
}

bool SelectWorkloadDialog::_setCurrentProject(
        tt3::ws::Project project
    )
{
    for (int i = 0; i < _ui->projectsTreeWidget->topLevelItemCount(); i++)
    {
        if (_setCurrentProject(_ui->projectsTreeWidget->topLevelItem(i), project))
        {
            return true;
        }
    }
    return false;
}

bool SelectWorkloadDialog::_setCurrentProject(
        QTreeWidgetItem * projectItem,
        tt3::ws::Project project
    )
{
    if (project == projectItem->data(0, Qt::ItemDataRole::UserRole).value<tt3::ws::Project>())
    {
        _ui->workloadsTabWidget->setCurrentIndex(0);
        _ui->projectsTreeWidget->setCurrentItem(projectItem);
        return true;
    }
    for (int i = 0; i < projectItem->childCount(); i++)
    {
        if (_setCurrentProject(projectItem->child(i), project))
        {
            return true;
        }
    }
    return false;
}

bool SelectWorkloadDialog::_setCurrentWorkStream(
        tt3::ws::WorkStream workStream
    )
{
    for (int i = 0; i < _ui->workStreamsTreeWidget->topLevelItemCount(); i++)
    {
        if (_setCurrentWorkStream(_ui->workStreamsTreeWidget->topLevelItem(i), workStream))
        {
            return true;
        }
    }
    return false;
}

bool SelectWorkloadDialog::_setCurrentWorkStream(
        QTreeWidgetItem * workStreamItem,
        tt3::ws::WorkStream workStream
    )
{
    if (workStream == workStreamItem->data(0, Qt::ItemDataRole::UserRole).value<tt3::ws::WorkStream>())
    {
        _ui->workloadsTabWidget->setCurrentIndex(1);
        _ui->workStreamsTreeWidget->setCurrentItem(workStreamItem);
        return true;
    }
    //  No need to recurse!
    return false;
}

void SelectWorkloadDialog::_setSelectedWorkload(
        tt3::ws::Workload workload
    )
{
    tt3::util::ResourceReader rr(Component::Resources::instance(), RSID(SelectWorkloadDialog));

    _selectedWorkload = workload;
    _setCurrentWorkload(workload);
    _ui->selectionLabel->setText(
        _prompt(
            rr.string(RID(SelectionLabel)),
            workload));
}

void SelectWorkloadDialog::_refresh()
{
    //  We don't want a refresh() to trigger a recursive refresh()!
    if (auto _ = RefreshGuard(_refreshUnderway)) //  Don't recurse!
    {
        //  Projects
        ProjectManager::_WorkspaceModel projectsModel =
            ProjectManager::_createWorkspaceModel(
                _workspace, _credentials, _treeWidgetDecorations);
        if (!_ui->showCompletedProjectsCheckBox->isChecked())
        {
            ProjectManager::_removeCompletedItems(projectsModel, _credentials);
        }
        QString projectsFilter = _ui->projectsFilterLineEdit->text().trimmed();
        if (!projectsFilter.isEmpty())
        {
            ProjectManager::_filterItems(
                projectsModel, projectsFilter, _treeWidgetDecorations);
        }
        ProjectManager::_refreshWorkspaceTree(
            _ui->projectsTreeWidget, projectsModel);
        _refreshProjectCheckStates();
        if (!_ui->projectsFilterLineEdit->text().trimmed().isEmpty())
        {   //  Filtered - show all
            _ui->projectsTreeWidget->expandAll();
        }

        //  Work streams
        WorkStreamManager::_WorkspaceModel workStreamsModel =
            WorkStreamManager::_createWorkspaceModel(
                _workspace, _credentials, _treeWidgetDecorations);
        QString workStreamsFilter = _ui->workStreamsFilterLineEdit->text().trimmed();
        if (!workStreamsFilter.isEmpty())
        {
            WorkStreamManager::_filterItems(
                workStreamsModel, workStreamsFilter, _treeWidgetDecorations);
        }
        WorkStreamManager::_refreshWorkspaceTree(
            _ui->workStreamsTreeWidget, workStreamsModel);
        _refreshWorkStreamCheckStates();
    }
}

void SelectWorkloadDialog::_refreshProjectCheckStates()
{
    for (int i = 0; i < _ui->projectsTreeWidget->topLevelItemCount(); i++)
    {
        _refreshProjectCheckStates(_ui->projectsTreeWidget->topLevelItem(i));
    }
}

void SelectWorkloadDialog::_refreshProjectCheckStates(
        QTreeWidgetItem * projectItem
    )
{
    tt3::ws::Project project =
        projectItem->data(0, Qt::ItemDataRole::UserRole).value<tt3::ws::Project>();
    projectItem->setFlags(projectItem->flags() | Qt::ItemFlag::ItemIsUserCheckable);
    projectItem->setCheckState(
        0,
        (project == _selectedWorkload) ?
            Qt::CheckState::Checked :
            Qt::CheckState::Unchecked);
    for (int i = 0; i < projectItem->childCount(); i++)
    {
        _refreshProjectCheckStates(projectItem->child(i));
    }
}

void SelectWorkloadDialog::_refreshWorkStreamCheckStates()
{
    for (int i = 0; i < _ui->workStreamsTreeWidget->topLevelItemCount(); i++)
    {
        _refreshWorkStreamCheckStates(_ui->workStreamsTreeWidget->topLevelItem(i));
    }
}

void SelectWorkloadDialog::_refreshWorkStreamCheckStates(
        QTreeWidgetItem * workStreamItem
    )
{
    tt3::ws::WorkStream workStream =
        workStreamItem->data(0, Qt::ItemDataRole::UserRole).value<tt3::ws::WorkStream>();
    workStreamItem->setFlags(workStreamItem->flags() | Qt::ItemFlag::ItemIsUserCheckable);
    workStreamItem->setCheckState(
        0,
        (workStream == _selectedWorkload) ?
            Qt::CheckState::Checked :
            Qt::CheckState::Unchecked);
}

QStringList SelectWorkloadDialog::_projectDisplayPath(
        tt3::ws::Project project
    )
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

QString SelectWorkloadDialog::_prompt(
        const QString & prompt,
        tt3::ws::Workload workload
    )
{
    if (auto project =
        std::dynamic_pointer_cast<tt3::ws::ProjectImpl>(workload))
    {
        QStringList path = _projectDisplayPath(project);
        QString result = prompt + project->type()->displayName();
        for (int i = 0; i < path.length(); i++)
        {
            result += "\n";
            result += (i == 0) ? "    " : QString(i * 4, ' ') + "+   ";
            result += path[i];
        }
        return result;
    }
    else if (auto workStream =
             std::dynamic_pointer_cast<tt3::ws::WorkStreamImpl>(workload))
    {
        return prompt +
               workStream->type()->displayName() +
               " " +
               workStream->displayName(_credentials);
    }
    else
    {
        return prompt + "-";
    }
}

//////////
//  Signal handlers
void SelectWorkloadDialog::_workloadsTabWidgetCurrentChanged(int n)
{
    Component::Settings::instance()->selectWorkloadsDialogTab = _ui->workloadsTabWidget->currentIndex();
    _refresh();
    _ui->workloadsTabWidget->setCurrentIndex(n);//  UI glitch if removed!
}

void SelectWorkloadDialog::_projectsFilterLineEditTextChanged(QString)
{
    _refresh();
}

void SelectWorkloadDialog::_workStreamsFilterLineEditTextChanged(QString)
{
    _refresh();
}

void SelectWorkloadDialog::_projectsTreeWidgetItemChanged(QTreeWidgetItem * item, int)
{
    if (!_refreshUnderway)
    {
        tt3::ws::Project project =
            item->data(0, Qt::ItemDataRole::UserRole).value<tt3::ws::Project>();
        if (item->checkState(0) == Qt::Checked &&
            _selectedWorkload != project)
        {   //  Item is newly "checked" - add to selection
            _setSelectedWorkload(project);
        }
        else if (item->checkState(0) == Qt::Unchecked &&
                 _selectedWorkload == project)
        {   //  Item is newly "unckecked" - remove from selection
            _setSelectedWorkload(nullptr);
        }
    }
    _refresh();
}

void SelectWorkloadDialog::_workStreamsTreeWidgetItemChanged(QTreeWidgetItem * item, int)
{
    if (!_refreshUnderway)
    {
        tt3::ws::WorkStream workStream =
            item->data(0, Qt::ItemDataRole::UserRole).value<tt3::ws::WorkStream>();
        if (item->checkState(0) == Qt::Checked &&
            _selectedWorkload != workStream)
        {   //  Item is newly "checked" - add to selection
            _setSelectedWorkload(workStream);
        }
        else if (item->checkState(0) == Qt::Unchecked &&
                 _selectedWorkload == workStream)
        {   //  Item is newly "unckecked" - remove from selection
            _setSelectedWorkload(nullptr);
        }
    }
    _refresh();
}

void SelectWorkloadDialog::_showCompletedProjectsCheckBoxStateChanged(int)
{
    _refresh();
}

void SelectWorkloadDialog::_projectsTreeWidgetItemDoubleClicked(QTreeWidgetItem * item,int)
{
    _selectedWorkload =
        item->data(0, Qt::ItemDataRole::UserRole).value<tt3::ws::Project>();
    accept();
}

void SelectWorkloadDialog::_workStreamsTreeWidgetItemDoubleClicked(QTreeWidgetItem * item,int)
{
    _selectedWorkload =
        item->data(0, Qt::ItemDataRole::UserRole).value<tt3::ws::WorkStream>();
    accept();
}

void SelectWorkloadDialog::accept()
{
    done(int(Result::Ok));
}

void SelectWorkloadDialog::reject()
{
    done(int(Result::Cancel));
}

//  End of tt3-gui/SelectWorkloadDialog.cpp
