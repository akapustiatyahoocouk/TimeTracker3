//
//  tt3-gui/DestroyEventDialog.cpp - tt3::gui::DestroyEventDialog class implementation
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
DestroyEventDialog::DestroyEventDialog(
        ::QWidget * parent,
        tt3::ws::Event event,
        const tt3::ws::Credentials & credentials
    ) : AskYesNoDialog(
            parent,
            QIcon(":/tt3-gui/Resources/Images/Actions/DestroyEventLarge.png"),
            "Destroy event",
            _prompt(event, credentials)),
        _event(event),
        _credentials(credentials)
{
}

//////////
//  Operations
DestroyEventDialog::Result DestroyEventDialog::doModal()
{
    return (AskYesNoDialog::doModal() == AskYesNoDialog::Result::Yes) ?
               Result::Ok :
               Result::Cancel;
}

//////////
//  Implementation helpers
QString DestroyEventDialog::_prompt(
        tt3::ws::Event event,
        const tt3::ws::Credentials & credentials
    )
{
    QString result =
        "Are you sure you want to destroy event\n" +
        event->summary(credentials) +
        "\nlogged at " +
        event->occurredAt(credentials).toLocalTime().toString() +
        "\nby " +
        event->account(credentials)->login(credentials);
    for (tt3::ws::Activity activity : event->activities(credentials))
    {
        result +=
            "\nagainst " +
            activity->type()->displayName().toLower() +
            " " +
            activity->displayName(credentials);
    }
    return result + "?";
}

//////////
//  Signal handlers
void DestroyEventDialog::accept()
{
    try
    {   //  Destroy!
        _event->destroy(_credentials);
        AskYesNoDialog::accept();
    }
    catch (const tt3::util::Exception & ex)
    {   //  OOPS! We cannot cast "this" to "QDialog*" directly
        qCritical() << ex;
        ErrorDialog::show(meAsParent(), ex);
    }
}

//  End of tt3-gui/DestroyEventDialog.cpp
