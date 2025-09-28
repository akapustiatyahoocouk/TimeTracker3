//
//  tt3-gui/EnterActivityStartCommentDialog.cpp - tt3::gui::EnterActivityStartCommentDialog class implementation
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
EnterActivityStartCommentDialog::EnterActivityStartCommentDialog(
        ::QWidget * parent,
        tt3::ws::Activity activity
    ) : EditStringDialog(
          parent,
          _dialogIcon(activity),
          _dialogTitle(activity),
          "Enter start comment:",
          "",
          [=](auto c) { return activity->workspace()->validator()->event()->isValidComment(c); })
{
}

//////////
//  Implementation hepers
QIcon EnterActivityStartCommentDialog::_dialogIcon(
        tt3::ws::Activity activity
    )
{
    /*  TODO uncomment
    if (std::dynamic_pointer_cast<tt3::ws::PrivateTaskImpl>(activity))
    {
        return "Start private task";
    }
    else */ if (std::dynamic_pointer_cast<tt3::ws::PublicTaskImpl>(activity))
    {
        return QIcon(":/tt3-gui/Resources/Images/Actions/StartPublicTaskLarge.png");
    }
    /*  TODO uncomment
    else if (std::dynamic_pointer_cast<tt3::ws::PrivateActivityImpl>(activity))
    {
        return "Complete private activity";
    }
    else
    */
    {
        Q_ASSERT(std::dynamic_pointer_cast<tt3::ws::PublicActivityImpl>(activity));
        return QIcon(":/tt3-gui/Resources/Images/Actions/StartPublicActivityLarge.png");
    }
}

QString EnterActivityStartCommentDialog::_dialogTitle(
        tt3::ws::Activity activity
    )
{
    /*  TODO uncomment
    if (std::dynamic_pointer_cast<tt3::ws::PrivateTaskImpl>(activity))
    {
        return "Start private task";
    }
    else */ if (std::dynamic_pointer_cast<tt3::ws::PublicTaskImpl>(activity))
    {
        return "Start public task";
    }
    /*  TODO uncomment
    else if (std::dynamic_pointer_cast<tt3::ws::PrivateActivityImpl>(activity))
    {
        return "Start private activity";
    }
    else
    */
    {
        Q_ASSERT(std::dynamic_pointer_cast<tt3::ws::PublicActivityImpl>(activity));
        return "Start public activity";
    }
}

//  End of tt3-gui/EnterActivityStartCommentDialog.cpp
