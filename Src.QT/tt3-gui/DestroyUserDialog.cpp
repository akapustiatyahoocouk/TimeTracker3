//
//  tt3-gui/DestroyUserDialog.cpp - tt3::gui::DestroyUserDialog class implementation
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
DestroyUserDialog::DestroyUserDialog(
        ::QWidget * parent,
        tt3::ws::User user,
        const tt3::ws::Credentials & credentials
    ) : AskYesNoDialog(
            parent,
            QIcon(":/tt3-gui/Resources/Images/Actions/DestroyUserLarge.png"),
            Component::Resources::instance()->string(
                RSID(DestroyUserDialog),
                RID(Title)),
            _prompt(user, credentials)),
        _user(user),
        _credentials(credentials)
{
}

//////////
//  Operations
DestroyUserDialog::Result DestroyUserDialog::doModal()
{
    return (AskYesNoDialog::doModal() == AskYesNoDialog::Result::Yes) ?
                Result::Ok :
                Result::Cancel;
}

//////////
//  Implementation helpers
QString DestroyUserDialog::_prompt(
        tt3::ws::User user,
        const tt3::ws::Credentials & credentials
    )
{
    tt3::util::ResourceReader rr(Component::Resources::instance(), RSID(DestroyUserDialog));

    QString result =
        rr.string(
            RID(Prompt),
            user->realName(credentials),        //  may throw
            user->accounts(credentials).size());//  may throw
    //  If there are works/events logged by any of the
    //  destroyed User's accounts, count them and add a line
    try
    {
        qsizetype worksCount = 0, eventsCount = 0;
        int64_t worksDurationMs = 0;
        _collectDestructionClosure( //  may throw
            user,
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

void DestroyUserDialog::_collectDestructionClosure(
        tt3::ws::User user,
        const tt3::ws::Credentials & credentials,
        qsizetype & worksCount,
        qsizetype & eventsCount,
        int64_t & worksDurationMs
    )
{
    for (tt3::ws::Account account : user->accounts(credentials))  //  may throw
    {
        _collectDestructionClosure( //  may throw
            account,
            credentials,
            worksCount,
            eventsCount,
            worksDurationMs);
    }
}

void DestroyUserDialog::_collectDestructionClosure(
        tt3::ws::Account account,
        const tt3::ws::Credentials & credentials,
        qsizetype & worksCount,
        qsizetype & eventsCount,
        int64_t & worksDurationMs
    )
{
    for (tt3::ws::Work work : account->works(credentials))  //  may throw
    {
        worksCount++;
        worksDurationMs += work->startedAt(credentials).msecsTo(work->finishedAt(credentials)); //  may throw
    }
    eventsCount = account->events(credentials).size();  //  may throw
}

//////////
//  Signal handlers
void DestroyUserDialog::accept()
{
    try
    {
        _user->destroy(_credentials);
        AskYesNoDialog::accept();
    }
    catch (const tt3::util::Exception & ex)
    {   //  OOPS! We cannot cast "this" to "QDialog*" directly
        qCritical() << ex;
        ErrorDialog::show(meAsParent(), ex);
    }
}

//  End of tt3-gui/DestroyUserDialog.cpp
