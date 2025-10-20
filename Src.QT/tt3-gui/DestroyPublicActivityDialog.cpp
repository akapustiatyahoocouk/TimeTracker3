//
//  tt3-gui/DestroyPublicActivityDialog.cpp - tt3::gui::DestroyPublicActivityDialog class implementation
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
DestroyPublicActivityDialog::DestroyPublicActivityDialog(
        ::QWidget * parent,
        tt3::ws::PublicActivity publicActivity,
        const tt3::ws::Credentials & credentials
    ) : AskYesNoDialog(
            parent,
            QIcon(":/tt3-gui/Resources/Images/Actions/DestroyPublicActivityLarge.png"),
            Component::Resources::instance()->string(
                RSID(DestroyPublicActivityDialog),
                RID(Title)),
            _prompt(publicActivity, credentials)),
        _publicActivity(publicActivity),
        _credentials(credentials)
{
}

//////////
//  Operations
DestroyPublicActivityDialog::Result DestroyPublicActivityDialog::doModal()
{
    return (AskYesNoDialog::doModal() == AskYesNoDialog::Result::Yes) ?
               Result::Ok :
               Result::Cancel;
}

//////////
//  Implementation helpers
QString DestroyPublicActivityDialog::_prompt(
        tt3::ws::PublicActivity publicActivity,
        const tt3::ws::Credentials & credentials
    )
{
    tt3::util::ResourceReader rr(Component::Resources::instance(), RSID(DestroyPublicActivityDialog));

    QString result =
        rr.string(
            RID(Prompt),
            publicActivity->displayName(credentials)); //  may throw
    //  If there are Works/Events logged against this
    //  activity, count them and add a line to the prompt.
    try
    {
        qsizetype worksCount = 0, eventsCount = 0;
        int64_t worksDurationMs = 0;
        _collectDestructionClosure( //  may throw
            publicActivity,
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

void DestroyPublicActivityDialog::_collectDestructionClosure(
        tt3::ws::PublicActivity publicActivity,
        const tt3::ws::Credentials & credentials,
        qsizetype & worksCount,
        qsizetype & eventsCount,
        int64_t & worksDurationMs
    )
{
    for (tt3::ws::Work work : publicActivity->works(credentials))  //  may throw
    {
        worksCount++;
        worksDurationMs += work->startedAt(credentials).msecsTo(work->finishedAt(credentials)); //  may throw
    }
    eventsCount = publicActivity->events(credentials).size();  //  may throw
}

//////////
//  Signal handlers
void DestroyPublicActivityDialog::accept()
{
    try
    {
        //  If the PublicActivity is currently underway,
        //  stop it; there's no need to record a Work unit.
        if (theCurrentActivity == _publicActivity)
        {
            theCurrentActivity.drop();
        }
        //  Destroy!
        _publicActivity->destroy(_credentials);
        AskYesNoDialog::accept();
    }
    catch (const tt3::util::Exception & ex)
    {   //  OOPS! We cannot cast "this" to "QDialog*" directly
        qCritical() << ex;
        ErrorDialog::show(meAsParent(), ex);
    }
}

//  End of tt3-gui/DestroyPublicActivityDialog.cpp
