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
            "Destroy public activity",
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
    QString result =
        "Are you sure you want to destroy public activity\n" +
        publicActivity->displayName(credentials) + " ?";
    //  TODO if there are Works/Events logged against this
    //  activity, count them and add a line to the prompt.
    return result;
}

//////////
//  Signal handlers
void DestroyPublicActivityDialog::accept()
{
    try
    {
        //  If the publicActivity is currently underway,
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
        ::QDialog * me =
            reinterpret_cast<::QDialog*>(
                reinterpret_cast<uintptr_t>(this));
        Q_ASSERT(reinterpret_cast<uintptr_t>(me) == reinterpret_cast<uintptr_t>(this));
        ErrorDialog::show(me, ex);
    }
}

//  End of tt3-gui/DestroyPublicActivityDialog.cpp
