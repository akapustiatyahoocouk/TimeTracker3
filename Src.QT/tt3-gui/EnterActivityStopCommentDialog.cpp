//
//  tt3-gui/EnterActivityStopCommentDialog.cpp - tt3::gui::EnterActivityStopCommentDialog class implementation
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
EnterActivityStopCommentDialog::EnterActivityStopCommentDialog(
        ::QWidget * parent,
        tt3::ws::Activity activity,
        const tt3::ws::Credentials & credentials
    ) : EditStringDialog(
            parent,
            _dialogIcon(activity),
            _dialogTitle(activity, credentials),
            Component::Resources::instance()->string(
                RSID(EnterActivityStopCommentDialog),
                RID(Prompt)),
            "",
            [=](auto s)
            {
                return activity->workspace()->validator()->event()->isValidSummary(s);
            })
{
}

//////////
//  Implementation hepers
QIcon EnterActivityStopCommentDialog::_dialogIcon(
        tt3::ws::Activity activity
    )
{
    if (std::dynamic_pointer_cast<tt3::ws::PrivateTaskImpl>(activity))
    {
        return QIcon(":/tt3-gui/Resources/Images/Actions/StopPrivateTaskLarge.png");
    }
    else if (std::dynamic_pointer_cast<tt3::ws::PublicTaskImpl>(activity))
    {
        return QIcon(":/tt3-gui/Resources/Images/Actions/StopPublicTaskLarge.png");
    }
    else if (std::dynamic_pointer_cast<tt3::ws::PrivateActivityImpl>(activity))
    {
        return QIcon(":/tt3-gui/Resources/Images/Actions/StopPrivateActivityLarge.png");
    }
    else
    {
        Q_ASSERT(std::dynamic_pointer_cast<tt3::ws::PublicActivityImpl>(activity));
        return QIcon(":/tt3-gui/Resources/Images/Actions/StopPublicActivityLarge.png");
    }
}

QString EnterActivityStopCommentDialog::_dialogTitle(
        tt3::ws::Activity activity,
        const tt3::ws::Credentials & credentials
    )
{
    tt3::util::ResourceReader rr(Component::Resources::instance(), RSID(EnterActivityStopCommentDialog));

    return rr.string(
        RID(Title),
        activity->type()->displayName(),
        activity->displayName(credentials));
}

//  End of tt3-gui/EnterActivityStopCommentDialog.cpp
