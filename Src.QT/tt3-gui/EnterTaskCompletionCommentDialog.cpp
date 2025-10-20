//
//  tt3-gui/EnterTaskCompletionCommentDialog.cpp - tt3::gui::EnterTaskCompletionCommentDialog class implementation
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
EnterTaskCompletionCommentDialog::EnterTaskCompletionCommentDialog(
        ::QWidget * parent,
        tt3::ws::Task task,
        const tt3::ws::Credentials & credentials
    ) : EditStringDialog(
            parent,
            _dialogIcon(task),
            _dialogTitle(task, credentials),
            Component::Resources::instance()->string(
                RSID(EnterTaskCompletionCommentDialog),
                RID(Prompt)),
            "",
            [=](auto s)
            {
                return task->workspace()->validator()->event()->isValidSummary(s);
            })
{
}

//////////
//  Implementation hepers
QIcon EnterTaskCompletionCommentDialog::_dialogIcon(
        tt3::ws::Task task
    )
{
    if (std::dynamic_pointer_cast<tt3::ws::PrivateTaskImpl>(task))
    {
        return QIcon(":/tt3-gui/Resources/Images/Actions/CompletePrivateTaskLarge.png");
    }
    else
    {
        Q_ASSERT(std::dynamic_pointer_cast<tt3::ws::PublicTaskImpl>(task));
        return QIcon(":/tt3-gui/Resources/Images/Actions/CompletePublicTaskLarge.png");
    }
}

QString EnterTaskCompletionCommentDialog::_dialogTitle(
        tt3::ws::Task task,
        const tt3::ws::Credentials & credentials
    )
{
    tt3::util::ResourceReader rr(Component::Resources::instance(), RSID(EnterTaskCompletionCommentDialog));

    return rr.string(
        RID(Title),
        task->type()->displayName(),
        task->displayName(credentials));    //  may throw
}

//  End of tt3-gui/EnterTaskCompletionCommentDialog.cpp
