//
//  tt3-gui/DestroyWorkStreamDialog.cpp - tt3::gui::DestroyWorkStreamDialog class implementation
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
DestroyWorkStreamDialog::DestroyWorkStreamDialog(
        ::QWidget * parent,
        tt3::ws::WorkStream workStream,
        const tt3::ws::Credentials & credentials
    ) : AskYesNoDialog(
            parent,
            QIcon(":/tt3-gui/Resources/Images/Actions/DestroyWorkStreamLarge.png"),
            Component::Resources::instance()->string(
                RSID(DestroyWorkStreamDialog),
                RID(Title)),
            _prompt(workStream, credentials)),
        _workStream(workStream),
        _credentials(credentials)
{
}

//////////
//  Operations
auto DestroyWorkStreamDialog::doModal(
    ) -> Result
{
    return (AskYesNoDialog::doModal() == AskYesNoDialog::Result::Yes) ?
               Result::Ok :
               Result::Cancel;
}

//////////
//  Implementation helpers
QString DestroyWorkStreamDialog::_prompt(
        tt3::ws::WorkStream workStream,
        const tt3::ws::Credentials & credentials
    )
{
    tt3::util::ResourceReader rr(Component::Resources::instance(), RSID(DestroyWorkStreamDialog));

    QString result =
        rr.string(
            RID(Prompt),
            workStream->displayName(credentials)); //  may throw
    //  If there are activities assigned to this work stream,
    //  count their works/events, as these will lose their
    //  attribution to a work stream
    try
    {
        qsizetype worksCount = 0, eventsCount = 0;
        int64_t worksDurationMs = 0;
        _collectDestructionClosure(
            workStream,
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

void DestroyWorkStreamDialog::_collectDestructionClosure(
        tt3::ws::WorkStream workStream,
        const tt3::ws::Credentials & credentials,
        qsizetype & worksCount,
        qsizetype & eventsCount,
        int64_t & worksDurationMs
    )
{   //  Measure this item...
    tt3::ws::Events events;
    for (tt3::ws::Activity activity : workStream->contributingActivities(credentials))    //  may throw
    {
        for (tt3::ws::Work work : activity->works(credentials)) //  may throw
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
void DestroyWorkStreamDialog::accept()
{
    try
    {
        _workStream->destroy(_credentials);
        AskYesNoDialog::accept();
    }
    catch (const tt3::util::Exception & ex)
    {   //  OOPS! We cannot cast "this" to "QDialog*" directly
        qCritical() << ex;
        ErrorDialog::show(meAsParent(), ex);
    }
}

//  End of tt3-gui/DestroyWorkStreamDialog.cpp
