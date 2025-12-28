//
//  tt3-gui/DestroyActivityTypeDialog.cpp - tt3::gui::DestroyActivityTypeDialog class implementation
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

//////////
//  Construction/destrution
DestroyActivityTypeDialog::DestroyActivityTypeDialog(
        ::QWidget * parent,
        tt3::ws::ActivityType activityType,
        const tt3::ws::Credentials & credentials
    ) : AskYesNoDialog(
            parent,
            QIcon(":/tt3-gui/Resources/Images/Actions/DestroyActivityTypeLarge.png"),
            Component::Resources::instance()->string(
                RSID(DestroyActivityTypeDialog),
                RID(Title)),
            _prompt(activityType, credentials)),
        _activityType(activityType),
        _credentials(credentials)
{
}

//////////
//  Operations
auto DestroyActivityTypeDialog::doModal(
    ) -> Result
{
    return (AskYesNoDialog::doModal() == AskYesNoDialog::Result::Yes) ?
               Result::Ok :
               Result::Cancel;
}

//////////
//  Implementation helpers
QString DestroyActivityTypeDialog::_prompt(
        tt3::ws::ActivityType activityType,
        const tt3::ws::Credentials & credentials
    )
{
    tt3::util::ResourceReader rr(Component::Resources::instance(), RSID(DestroyActivityTypeDialog));

    QString result =
        rr.string(
            RID(Prompt),
            activityType->displayName(credentials),         //  may throw
            activityType->activities(credentials).size());  //  may throw
    //  If there are activities of this type, count their
    //  works/events, as these will lose their attribution to
    //  an activity type
    try
    {
        qsizetype worksCount = 0, eventsCount = 0;
        int64_t worksDurationMs = 0;
        _collectDestructionClosure( //  may throw
            activityType,
            credentials,
            worksCount,
            eventsCount,
            worksDurationMs);
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

void DestroyActivityTypeDialog::_collectDestructionClosure(
        tt3::ws::ActivityType activityType,
        const tt3::ws::Credentials & credentials,
        qsizetype & worksCount,
        qsizetype & eventsCount,
        int64_t & worksDurationMs
    )
{
    tt3::ws::Events events;
    for (const auto & activity : activityType->activities(credentials))    //  may throw
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
    eventsCount = events.size();
}

//////////
//  Signal handlers
void DestroyActivityTypeDialog::accept()
{
    try
    {
        _activityType->destroy(_credentials);
        AskYesNoDialog::accept();
    }
    catch (const tt3::util::Exception & ex)
    {   //  OOPS! We cannot cast "this" to "QDialog*" directly
        qCritical() << ex;
        ErrorDialog::show(meAsParent(), ex);
    }
}

//  End of tt3-gui/DestroyActivityTypeDialog.cpp
