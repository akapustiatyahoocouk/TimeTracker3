//
//  tt3-gui/DestroyUserDialog.cpp - tt3::gui::DestroyUserDialog class implementation
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
DestroyUserDialog::DestroyUserDialog(
        ::QWidget * parent,
        tt3::ws::User user,
        const tt3::ws::Credentials & credentials
    ) : AskYesNoDialog(
            parent,
            QIcon(":/tt3-gui/Resources/Images/Actions/DestroyUserLarge.png"),
            "Destroy user",
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
    QString result =
        "Are you sure you want to destroy user\n" +
        user->realName(credentials) + " ?";
    //  TODO if there are works/events logged by any of the
    //  destroyed User's accounts, count them and add a line
    return result;
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
