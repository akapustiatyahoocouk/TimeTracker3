//
//  tt3-gui/DestroyPrivateActivityDialog.cpp - tt3::gui::DestroyPrivateActivityDialog class implementation
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
DestroyPrivateActivityDialog::DestroyPrivateActivityDialog(
        ::QWidget * parent,
        tt3::ws::PrivateActivity privateActivity,
        const tt3::ws::Credentials & credentials
    ) : AskYesNoDialog(
            parent,
            QIcon(":/tt3-gui/Resources/Images/Actions/DestroyPrivateActivityLarge.png"),
            "Destroy private activity",
            _prompt(privateActivity, credentials)),
    _privateActivity(privateActivity),
    _credentials(credentials)
{
}

//////////
//  Operations
DestroyPrivateActivityDialog::Result DestroyPrivateActivityDialog::doModal()
{
    return (AskYesNoDialog::doModal() == AskYesNoDialog::Result::Yes) ?
               Result::Ok :
               Result::Cancel;
}

//////////
//  Implementation helpers
QString DestroyPrivateActivityDialog::_prompt(
        tt3::ws::PrivateActivity privateActivity,
        const tt3::ws::Credentials & credentials
    )
{
    QString result =
        "Are you sure you want to destroy private activity\n" +
        privateActivity->displayName(credentials) + " ?";
    //  TODO if there are Works/Events logged against this
    //  activity, count them and add a line to the prompt.
    return result;
}

//////////
//  Signal handlers
void DestroyPrivateActivityDialog::accept()
{
    try
    {
        //  If the PrivateActivity is currently underway,
        //  stop it; there's no need to record a Work unit.
        if (theCurrentActivity == _privateActivity)
        {
            theCurrentActivity.drop();
        }
        //  Destroy!
        _privateActivity->destroy(_credentials);
        AskYesNoDialog::accept();
    }
    catch (const tt3::util::Exception & ex)
    {   //  OOPS! We cannot cast "this" to "QDialog*" directly
        ::QDialog * me =
            reinterpret_cast<::QDialog*>(
                reinterpret_cast<uintptr_t>(this));
        Q_ASSERT(reinterpret_cast<uintptr_t>(me) == reinterpret_cast<uintptr_t>(this));
        ErrorDialog::show(me, ex);
    }
}

//  End of tt3-gui/DestroyPrivateActivityDialog.cpp
