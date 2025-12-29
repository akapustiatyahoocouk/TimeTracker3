//
//  tt3-gui/DestroyProjectDialog.cpp - tt3::gui::DestroyProjectDialog class implementation
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
            Component::Resources::instance()->string(
                RSID(DestroyProjectDialog),
                RID(Title)),
            _prompt(project, credentials)),
        _project(project),
        _credentials(credentials)
{
}

//////////
//  Operations
auto DestroyProjectDialog::doModal() -> Result
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
    tt3::util::ResourceReader rr(Component::Resources::instance(), RSID(DestroyProjectDialog));

    QString result =
        rr.string(
            RID(Prompt),
            project->displayName(credentials)); //  may throw
    //  If there are activities assigned to this project (or a
    //  sub-project thereof), count their works/events, as these
    //  will lose their attribution to a project
    try
    {
        qsizetype projectsCount = 0, worksCount = 0;
        tt3::ws::Events events;
        int64_t worksDurationMs = 0;
        _collectDestructionClosure(
            project,
            credentials,
            projectsCount,
            worksCount,
            events,
            worksDurationMs);
        Q_ASSERT(projectsCount >= 1);
        if (projectsCount > 1)
        {
            result +=
                rr.string(
                    RID(Subprojects),
                    projectsCount - 1);
        }
        if (worksCount > 0 && events.size() > 0)
        {
            result +=
                rr.string(
                    RID(DetailsWE),
                    worksCount,
                    (worksDurationMs + 59999) / 60000,
                    events.size());
        }
        else if (worksCount > 0)
        {
            result +=
                rr.string(
                    RID(DetailsW),
                    worksCount,
                    (worksDurationMs + 59999) / 60000);
        }
        else if (events.size() > 0)
        {
            result +=
                rr.string(
                    RID(DetailsE),
                    events.size());
        }
    }
    catch (const tt3::util::Exception & ex)
    {   //  OOPS! Log, but suppress
        qCritical() << ex;
    }
    //  Done
    return result;
}

void DestroyProjectDialog::_collectDestructionClosure(
        tt3::ws::Project project,
        const tt3::ws::Credentials & credentials,
        qsizetype & projectsCount,
        qsizetype & worksCount,
        tt3::ws::Events & events,
        int64_t & worksDurationMs
    )
{   //  Measure this item...
    projectsCount++;
    for (const auto & activity : project->contributingActivities(credentials))    //  may throw
    {
        for (const auto & work : activity->works(credentials)) //  may throw
        {
            worksCount++;
            worksDurationMs += work->startedAt(credentials).msecsTo(work->finishedAt(credentials)); //  may throw
        }
        //  Some events may be related to more tha one
        //  activity, so DON'T count them twice
        events += activity->events(credentials);
    }
    //  ...and children, recursively
    for (const auto & child : project->children(credentials))
    {
        _collectDestructionClosure(
            child,
            credentials,
            projectsCount,
            worksCount,
            events,
            worksDurationMs);
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
        qCritical() << ex;
        ErrorDialog::show(meAsParent(), ex);
    }
}

//  End of tt3-gui/DestroyProjectDialog.cpp
