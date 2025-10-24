//
//  tt3-gui/ConfirmDestroyWorkspaceDialog.cpp - tt3::gui::ConfirmDestroyWorkspaceDialog class implementation
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
ConfirmDestroyWorkspaceDialog::ConfirmDestroyWorkspaceDialog(
        ::QWidget * parent, tt3::ws::WorkspaceAddress workspaceAddress)
    :   AskYesNoDialog(
          parent,
          QIcon(":/tt3-gui/Resources/Images/Actions/DestroyWorkspaceLarge.png"),
          //  TODO localize via Resources
          "Destroy workspace",
          "Are you sure you really want to destroy the workspace\n" +
              workspaceAddress->displayForm() + " ?\n" +
              "IMPORTANT: YOU WILL NOT BE ABLE TO UNDO THIS ACTION LATER!")
{
}

//  End of tt3-gui/ConfirmDestroyWorkspaceDialog.cpp
