//
//  tt3-gui/DestroyPrivateTaskDialog.cpp - tt3::gui::DestroyPrivateTaskDialog class implementation
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
DestroyPrivateTaskDialog::DestroyPrivateTaskDialog(
        ::QWidget * parent,
        tt3::ws::PrivateTask privateTask,
        const tt3::ws::Credentials & credentials
    ) : AskYesNoDialog(
            parent,
            QIcon(":/tt3-gui/Resources/Images/Actions/DestroyPrivateTaskLarge.png"),
            Component::Resources::instance()->string(
                RSID(DestroyPrivateTaskDialog),
                RID(Title)),
            _prompt(privateTask, credentials)),
        _privateTask(privateTask),
    _credentials(credentials)
{
}

//////////
//  Operations
DestroyPrivateTaskDialog::Result DestroyPrivateTaskDialog::doModal()
{
    return (AskYesNoDialog::doModal() == AskYesNoDialog::Result::Yes) ?
               Result::Ok :
               Result::Cancel;
}

//////////
//  Implementation helpers
QString DestroyPrivateTaskDialog::_prompt(
        tt3::ws::PrivateTask privateTask,
        const tt3::ws::Credentials & credentials
    )
{
    tt3::util::ResourceReader rr(Component::Resources::instance(), RSID(DestroyPrivateTaskDialog));

    QString result =
        rr.string(
            RID(Prompt),
            privateTask->displayName(credentials)); //  may throw
    //  If there are Works/Events logged against this
    //  task OR ITS DESCENDNTS, count them and add a line
    //  to the prompt, including the number of descendants -
    //  they all will be delete-cascaded.
    try
    {
        qsizetype tasksCount = 0, worksCount = 0, eventsCount = 0;
        int64_t worksDurationMs = 0;
        _collectDestructionClosure( //  may throw
            privateTask,
            credentials,
            tasksCount,
            worksCount,
            eventsCount,
            worksDurationMs);
        if (tasksCount > 1)
        {
            result +=
                rr.string(
                    RID(Subtasks),
                    tasksCount - 1);
        }
        if (worksCount > 0 && eventsCount > 0)
        {
            result +=
                rr.string(
                    RID(DetailsWE),
                    worksCount,
                    (worksDurationMs + 59999) / 60000,
                    eventsCount);
        }
        else if (worksCount > 0)
        {
            result +=
                rr.string(
                    RID(DetailsW),
                    worksCount,
                    (worksDurationMs + 59999) / 60000);
        }
        else if (eventsCount > 0)
        {
            result +=
                rr.string(
                    RID(DetailsE),
                    eventsCount);
        }
    }
    catch (const tt3::util::Exception & ex)
    {   //  OOPS! Log, but suppress
        qCritical() << ex;
    }
    //  Done
    return result;
}

void DestroyPrivateTaskDialog::_collectDestructionClosure(
        tt3::ws::PrivateTask privateTask,
        const tt3::ws::Credentials & credentials,
        qsizetype & tasksCount,
        qsizetype & worksCount,
        qsizetype & eventsCount,
        int64_t & worksDurationMs
    )
{
    tasksCount++;
    for (tt3::ws::Work work : privateTask->works(credentials))  //  may throw
    {
        worksCount++;
        worksDurationMs += work->startedAt(credentials).msecsTo(work->finishedAt(credentials)); //  may throw
    }
    eventsCount = privateTask->events(credentials).size();  //  may throw
    for (tt3::ws::PrivateTask child : privateTask->children(credentials))   //  may throw
    {
        _collectDestructionClosure( //  may throw
            child,
            credentials,
            tasksCount,
            worksCount,
            eventsCount,
            worksDurationMs);
    }
}

//////////
//  Signal handlers
void DestroyPrivateTaskDialog::accept()
{
    try
    {
        //  If the PrivateTask is currently underway,
        //  stop it; there's no need to record a Work unit.
        if (theCurrentActivity == _privateTask)
        {
            theCurrentActivity.drop();
        }
        //  Destroy!
        _privateTask->destroy(_credentials);
        AskYesNoDialog::accept();
    }
    catch (const tt3::util::Exception & ex)
    {   //  OOPS! We cannot cast "this" to "QDialog*" directly
        qCritical() << ex;
        ErrorDialog::show(meAsParent(), ex);
    }
}

//  End of tt3-gui/DestroyPrivateTaskDialog.cpp
