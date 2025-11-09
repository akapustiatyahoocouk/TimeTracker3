//
//  tt3-gui/SelectWorkloadsDialog.cpp - tt3::gui::SelectWorkloadsDialog class implementation
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
#include "ui_SelectWorkloadsDialog.h"
using namespace tt3::gui;

//////////
//  Construction/destruction
SelectWorkloadsDialog::SelectWorkloadsDialog(
        QWidget * parent,
        tt3::ws::Workspace workspace,
        const tt3::ws::Credentials & credentials,
        const tt3::ws::Workloads & workloads
    ) : QDialog(parent),
        _workspace(workspace),
        _credentials(credentials),
        _ui(new Ui::SelectWorkloadsDialog)
{
    tt3::util::ResourceReader rr(Component::Resources::instance(), RSID(SelectWorkloadsDialog));

    Q_ASSERT(_workspace != nullptr);

    _ui->setupUi(this);
    _treeWidgetDecorations = TreeWidgetDecorations(_ui->projectsTreeWidget);
    _listWidgetDecorations = ListWidgetDecorations(_ui->workloadsListWidget);
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
    _ui->selectedWorkloadsLabel->setText(
        rr.string(RID(SelectedWorkloadsLabel)));

    _ui->buttonBox->button(QDialogButtonBox::StandardButton::Ok)->
        setText(rr.string(RID(OkPushButton)));
    _ui->buttonBox->button(QDialogButtonBox::StandardButton::Ok)->
        setIcon(QIcon(":/tt3-gui/Resources/Images/Actions/OkSmall.png"));
    _ui->buttonBox->button(QDialogButtonBox::StandardButton::Cancel)->
        setText(rr.string(RID(CancelPushButton)));
    _ui->buttonBox->button(QDialogButtonBox::StandardButton::Cancel)->
        setIcon(QIcon(":/tt3-gui/Resources/Images/Actions/CancelSmall.png"));

    //  Set editable control values
    _setSelectedWorkloads(workloads);
    _ui->showCompletedProjectsCheckBox->setChecked(
        Component::Settings::instance()->showCompletedProjects);

    _refresh(); //  NOW, to adjust tree widget size to content
    _ui->projectsTreeWidget->expandAll();
    _ui->workloadsTabWidget->setCurrentIndex(
        Component::Settings::instance()->selectWorkloadsDialogTab);

    //  Done
    adjustSize();
}

SelectWorkloadsDialog::~SelectWorkloadsDialog()
{
    delete _ui;
}

//////////
//  Operations
auto SelectWorkloadsDialog::doModal(
    ) -> Result
{
    return Result(this->exec());
}

auto SelectWorkloadsDialog::selectedWorkloads(
    ) -> tt3::ws::Workloads
{
    return _selectedWorkloads();
}

//////////
//  Implementation helpers
auto SelectWorkloadsDialog::_currentWorkload(
    ) -> tt3::ws::Workload
{
    switch (_ui->workloadsTabWidget->currentIndex())
    {
        case 0: //  Projects
            if (QTreeWidgetItem * item = _ui->projectsTreeWidget->currentItem())
            {
                return item->data(0, Qt::ItemDataRole::UserRole).value<tt3::ws::Project>();
            }
            return nullptr;
        case 1: //  Work streams
            if (QTreeWidgetItem * item = _ui->workStreamsTreeWidget->currentItem())
            {
                return item->data(0, Qt::ItemDataRole::UserRole).value<tt3::ws::WorkStream>();
            }
            return nullptr;
        default:
            Q_ASSERT(false);
            return nullptr; //  be defensive in release mode
    }
}

bool SelectWorkloadsDialog::_setCurrentWorkload(
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

bool SelectWorkloadsDialog::_setCurrentProject(
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

bool SelectWorkloadsDialog::_setCurrentProject(
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

bool SelectWorkloadsDialog::_setCurrentWorkStream(
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

bool SelectWorkloadsDialog::_setCurrentWorkStream(
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

auto SelectWorkloadsDialog::_selectedWorkloads(
    ) -> tt3::ws::Workloads
{
    tt3::ws::Workloads result;
    for (int i = 0; i < _ui->workloadsListWidget->count(); i++)
    {
        result.insert(
            _ui->workloadsListWidget->item(i)->data(Qt::ItemDataRole::UserRole).value<tt3::ws::Workload>());
    }
    return result;
}

void SelectWorkloadsDialog::_setSelectedWorkloads(
        const tt3::ws::Workloads & workloads
    )
{
    static const QIcon errorIcon(":/tt3-gui/Resources/Images/Misc/ErrorSmall.png");
    tt3::util::ResourceReader rr(Component::Resources::instance(), RSID(SelectWorkloadsDialog));

    QList<tt3::ws::Workload> workloadsList = workloads.values();
    std::sort(
        workloadsList.begin(),
        workloadsList.end(),
        [&](auto a, auto b)
        {
            try
            {
                return a->displayName(_credentials) < b->displayName(_credentials); //  may throw
            }
            catch (tt3::util::Exception & ex)
            {   //  OOPS! Report & recover with a stable sort order
                qCritical() << ex;
                return a->oid() < b->oid();
            }
        });
    //  Make sure a proper number of list widget items...
    while (_ui->workloadsListWidget->count() < workloadsList.size())
    {   //  Too few items in the list widget
        _ui->workloadsListWidget->addItem("?");
    }
    while (_ui->workloadsListWidget->count() > workloadsList.size())
    {   //  Too many items in the list widget
        delete _ui->workloadsListWidget->takeItem(
            _ui->workloadsListWidget->count() - 1);
    }
    //  ...each represent a proper Workload
    for (int i = 0; i < workloadsList.count(); i++)
    {
        QListWidgetItem * item = _ui->workloadsListWidget->item(i);
        tt3::ws::Workload workload = workloadsList[i];
        try
        {
            QString text = workload->displayName(_credentials); //  may throw
            item->setIcon(workload->type()->smallIcon());
            if (auto project =
                std::dynamic_pointer_cast<tt3::ws::ProjectImpl>(workload))
            {
                if (project->completed(_credentials))  //  may throw
                {   //  Completed
                    text += " " + rr.string(RID(ProjectCompletedSuffix));
                    item->setForeground(_listWidgetDecorations.disabledItemForeground);
                }
                else
                {   //  Not completed
                    item->setForeground(_listWidgetDecorations.itemForeground);
                }
            }
            else
            {   //  Not a task
                item->setForeground(_listWidgetDecorations.itemForeground);
            }
            item->setText(text);
        }
        catch (tt3::util::Exception & ex)
        {   //  OOPS! Report & recover
            qCritical() << ex;
            item->setText(ex.errorMessage());
            item->setIcon(errorIcon);
            item->setForeground(_listWidgetDecorations.errorItemForeground);
        }
        item->setData(Qt::ItemDataRole::UserRole, QVariant::fromValue(workload));
    }
}

void SelectWorkloadsDialog::_refresh()
{
    //  We don't want a refresh() to trigger a recursive refresh()!
    if (auto _ = RefreshGuard(_refreshUnderway)) //  Don't recurse!
    {
        tt3::ws::Workloads selectedWorkloads = _selectedWorkloads();

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
        _refreshProjectCheckStates(selectedWorkloads);
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
        _refreshWorkStreamCheckStates(selectedWorkloads);
    }
}

void SelectWorkloadsDialog::_refreshProjectCheckStates(
        const tt3::ws::Workloads & selectedWorkloads
    )
{
    for (int i = 0; i < _ui->projectsTreeWidget->topLevelItemCount(); i++)
    {
        _refreshProjectCheckStates(_ui->projectsTreeWidget->topLevelItem(i), selectedWorkloads);
    }
}

void SelectWorkloadsDialog::_refreshProjectCheckStates(
        QTreeWidgetItem * projectItem,
        const tt3::ws::Workloads & selectedWorkloads
    )
{
    tt3::ws::Project project =
        projectItem->data(0, Qt::ItemDataRole::UserRole).value<tt3::ws::Project>();
    projectItem->setFlags(projectItem->flags() | Qt::ItemFlag::ItemIsUserCheckable);
    projectItem->setCheckState(
        0,
        selectedWorkloads.contains(project) ?
            Qt::CheckState::Checked :
            Qt::CheckState::Unchecked);
    for (int i = 0; i < projectItem->childCount(); i++)
    {
        _refreshProjectCheckStates(projectItem->child(i), selectedWorkloads);
    }
}

void SelectWorkloadsDialog::_refreshWorkStreamCheckStates(
        const tt3::ws::Workloads & selectedWorkloads
    )
{
    for (int i = 0; i < _ui->workStreamsTreeWidget->topLevelItemCount(); i++)
    {
        _refreshWorkStreamCheckStates(_ui->workStreamsTreeWidget->topLevelItem(i), selectedWorkloads);
    }
}

void SelectWorkloadsDialog::_refreshWorkStreamCheckStates(
        QTreeWidgetItem * workStreamItem,
        const tt3::ws::Workloads & selectedWorkloads
    )
{
    tt3::ws::WorkStream workStream =
        workStreamItem->data(0, Qt::ItemDataRole::UserRole).value<tt3::ws::WorkStream>();
    workStreamItem->setFlags(workStreamItem->flags() | Qt::ItemFlag::ItemIsUserCheckable);
    workStreamItem->setCheckState(
        0,
        selectedWorkloads.contains(workStream) ?
            Qt::CheckState::Checked :
            Qt::CheckState::Unchecked);
}

//////////
//  Signal handlers
void SelectWorkloadsDialog::_workloadsTabWidgetCurrentChanged(int n)
{
    Component::Settings::instance()->selectWorkloadsDialogTab = _ui->workloadsTabWidget->currentIndex();
    _refresh();
    _ui->workloadsTabWidget->setCurrentIndex(n);//  UI glitch if removed!
}

void SelectWorkloadsDialog::_projectsFilterLineEditTextChanged(QString)
{
    _refresh();
}

void SelectWorkloadsDialog::_workStreamsFilterLineEditTextChanged(QString)
{
    _refresh();
}

void SelectWorkloadsDialog::_workloadsListWidgetCurrentRowChanged(int)
{
    if (QListWidgetItem * item = _ui->workloadsListWidget->currentItem())
    {
        tt3::ws::Workload w =
            item->data(Qt::ItemDataRole::UserRole).value<tt3::ws::Workload>();
        _setCurrentWorkload(w);
    }
    _refresh();
}

void SelectWorkloadsDialog::_projectsTreeWidgetItemChanged(QTreeWidgetItem * item, int)
{
    if (!_refreshUnderway)
    {
        tt3::ws::Workloads selectedWorkloads = _selectedWorkloads();
        tt3::ws::Project project =
            item->data(0, Qt::ItemDataRole::UserRole).value<tt3::ws::Project>();
        if (item->checkState(0) == Qt::Checked &&
            !selectedWorkloads.contains(project))
        {   //  Item is newly "checked" - add to selection
            selectedWorkloads.insert(project);
            _setSelectedWorkloads(selectedWorkloads);
        }
        else if (item->checkState(0) == Qt::Unchecked &&
                 selectedWorkloads.contains(project))
        {   //  Item is newly "unckecked" - remove from selection
            selectedWorkloads.remove(project);
            _setSelectedWorkloads(selectedWorkloads);
        }
    }
    _refresh();
}

void SelectWorkloadsDialog::_workStreamsTreeWidgetItemChanged(QTreeWidgetItem * item, int)
{
    if (!_refreshUnderway)
    {
        tt3::ws::Workloads selectedWorkloads = _selectedWorkloads();
        tt3::ws::WorkStream workStream =
            item->data(0, Qt::ItemDataRole::UserRole).value<tt3::ws::WorkStream>();
        if (item->checkState(0) == Qt::Checked &&
            !selectedWorkloads.contains(workStream))
        {   //  Item is newly "checked" - add to selection
            selectedWorkloads.insert(workStream);
            _setSelectedWorkloads(selectedWorkloads);
        }
        else if (item->checkState(0) == Qt::Unchecked &&
                 selectedWorkloads.contains(workStream))
        {   //  Item is newly "unckecked" - remove from selection
            selectedWorkloads.remove(workStream);
            _setSelectedWorkloads(selectedWorkloads);
        }
    }
    _refresh();
}

void SelectWorkloadsDialog::_showCompletedProjectsCheckBoxStateChanged(int)
{
    _refresh();
}

void SelectWorkloadsDialog::accept()
{
    done(int(Result::Ok));
}

void SelectWorkloadsDialog::reject()
{
    done(int(Result::Cancel));
}

//  End of tt3-gui/SelectWorkloadsDialog.cpp
