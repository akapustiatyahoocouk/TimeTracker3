//
//  tt3-gui/ConfirmDropWorkspaceDialog.cpp - tt3::gui::ConfirmDropWorkspaceDialog class implementation
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
//  Construction/destruction
ConfirmDropWorkspaceDialog::ConfirmDropWorkspaceDialog(
        ::QWidget * parent,
        tt3::ws::WorkspaceAddress workspaceAddress
    ) : AskYesNoDialog(
            parent,
            QIcon(":/tt3-gui/Resources/Images/Actions/LoginLarge.png"),
            Component::Resources::instance()->string(
                RSID(ConfirmDropWorkspaceDialog),
                RID(Title)),
            Component::Resources::instance()->string(
                RSID(ConfirmDropWorkspaceDialog),
                RID(Prompt),
                workspaceAddress->displayForm()))
{
}

//  End of tt3-gui/ConfirmDropWorkspaceDialog.cpp
