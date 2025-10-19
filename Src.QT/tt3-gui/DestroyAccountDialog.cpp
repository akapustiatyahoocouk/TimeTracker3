//
//  tt3-gui/DestroyAccountDialog.cpp - tt3::gui::DestroyAccountDialog class implementation
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

//////////
//  Construction/destrution
DestroyAccountDialog::DestroyAccountDialog(
        ::QWidget * parent,
        tt3::ws::Account account,
        const tt3::ws::Credentials & credentials
    ) : AskYesNoDialog(
            parent,
            QIcon(":/tt3-gui/Resources/Images/Actions/DestroyAccountLarge.png"),
            "Destroy account",
            _prompt(account, credentials)),
        _account(account),
        _credentials(credentials)
{
}

//////////
//  Operations
DestroyAccountDialog::Result DestroyAccountDialog::doModal()
{
    return (AskYesNoDialog::doModal() == AskYesNoDialog::Result::Yes) ?
               Result::Ok :
               Result::Cancel;
}

//////////
//  Implementation helpers
QString DestroyAccountDialog::_prompt(
        tt3::ws::Account account,
        const tt3::ws::Credentials & credentials
    )
{
    QString result =
        "Are you sure you want to destroy account\n" +
        account->login(credentials) + " of user " +
        account->user(credentials)->realName(credentials) + " ?";
    //  If there are works/events logged by destroyed
    //  Account, count them and add a line
    try
    {
        qsizetype worksCount = 0, eventsCount = 0;
        int64_t worksDurationMs = 0;
        _collectDestructionClosure( //  may throw
            account,credentials,
            worksCount, eventsCount,
            worksDurationMs);
        if (worksCount > 0 && eventsCount > 0)
        {
            result += "\nThe account has ";
            result += tt3::util::toString(worksCount);
            result += " work unit(s) logged for it";
            result += "\n(with total duration of ";
            result += tt3::util::toString((worksDurationMs + 59999) / 60000);
            result += " minute(s)) and ";
            result += tt3::util::toString(eventsCount);
            result += " event(s).";
        }
        else if (worksCount > 0)
        {
            result += "\nThe account has ";
            result += tt3::util::toString(worksCount);
            result += " work unit(s) logged for it";
            result += "\n(with total duration of ";
            result += tt3::util::toString((worksDurationMs + 59999) / 60000);
            result += " minute(s)).";
        }
        else if (eventsCount > 0)
        {
            result += "\nThe account has ";
            result += tt3::util::toString(eventsCount);
            result += " event(s) logged for it.";
        }
    }
    catch (const tt3::util::Exception & ex)
    {   //  OOPS! Log, but suppress
        qCritical() << ex;
    }
    //  Done
    return result;
}

void DestroyAccountDialog::_collectDestructionClosure(
        tt3::ws::Account account,
        const tt3::ws::Credentials & credentials,
        qsizetype & worksCount, qsizetype & eventsCount,
        int64_t & worksDurationMs
    )
{
    for (tt3::ws::Work work : account->works(credentials))  //  may throw
    {
        worksCount++;
        worksDurationMs += work->startedAt(credentials).msecsTo(work->finishedAt(credentials));
    }
    eventsCount = account->events(credentials).size();  //  may throw
}

//////////
//  Signal handlers
void DestroyAccountDialog::accept()
{
    try
    {
        _account->destroy(_credentials);
        AskYesNoDialog::accept();
    }
    catch (const tt3::util::Exception & ex)
    {   //  OOPS! We cannot cast "this" to "QDialog*" directly
        qCritical() << ex;
        ErrorDialog::show(meAsParent(), ex);
    }
}

//  End of tt3-gui/DestroyAccountDialog.cpp
