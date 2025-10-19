//
//  tt3-gui/DestroyProjectDialog.cpp - tt3::gui::DestroyProjectDialog class implementation
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
using namespace tt3::gui;

namespace tt3::gui
{
extern CurrentActivity theCurrentActivity;
}

//////////
//  Construction/destrution
DestroyProjectDialog::DestroyProjectDialog(
        ::QWidget * parent,
        tt3::ws::Project project,
        const tt3::ws::Credentials & credentials
    ) : AskYesNoDialog(
            parent,
            QIcon(":/tt3-gui/Resources/Images/Actions/DestroyProjectLarge.png"),
            "Destroy project",
            _prompt(project, credentials)),
        _project(project),
        _credentials(credentials)
{
}

//////////
//  Operations
DestroyProjectDialog::Result DestroyProjectDialog::doModal()
{
    return (AskYesNoDialog::doModal() == AskYesNoDialog::Result::Yes) ?
               Result::Ok :
               Result::Cancel;
}

//////////
//  Implementation helpers
QString DestroyProjectDialog::_prompt(
        tt3::ws::Project project,
        const tt3::ws::Credentials & credentials
    )
{
    QString result =
        "Are you sure you want to destroy project\n" +
        project->displayName(credentials) + " ?";
    //  Extra line?
    int projectsCount = 0;
    _collectDestructionClosure(project, credentials, projectsCount);
    Q_ASSERT(projectsCount >= 1);
    if (projectsCount > 1)
    {
        result +=
            "\nThe project has " +
            tt3::util::toString(projectsCount - 1) +
            " sub-projects, which will be destroyed as well.";
    }
    return result;
}

void DestroyProjectDialog::_collectDestructionClosure(
        tt3::ws::Project project,
        const tt3::ws::Credentials & credentials,
        int & projectsCount
    )
{   //  Measure this item...
    projectsCount++;
    //  ...and children, recursively
    for (tt3::ws::Project child : project->children(credentials))
    {
        _collectDestructionClosure(child, credentials, projectsCount);
    }
}

//////////
//  Signal handlers
void DestroyProjectDialog::accept()
{
    try
    {
        _project->destroy(_credentials);
        AskYesNoDialog::accept();
    }
    catch (const tt3::util::Exception & ex)
    {   //  OOPS! We cannot cast "this" to "QDialog*" directly
        qCritical() << ex.errorMessage();
        ::QDialog * me =
            reinterpret_cast<::QDialog*>(
                reinterpret_cast<uintptr_t>(this));
        Q_ASSERT(reinterpret_cast<uintptr_t>(me) == reinterpret_cast<uintptr_t>(this));
        ErrorDialog::show(me, ex);
    }
}

//  End of tt3-gui/DestroyProjectDialog.cpp
