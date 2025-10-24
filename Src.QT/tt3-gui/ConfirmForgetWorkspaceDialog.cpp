//
//  tt3-gui/ConfirmForgetWorkspaceDialog.cpp - tt3::gui::ConfirmForgetWorkspaceDialog class implementation
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
ConfirmForgetWorkspaceDialog::ConfirmForgetWorkspaceDialog(
        ::QWidget * parent, tt3::ws::WorkspaceAddress workspaceAddress)
    :   AskYesNoDialog(
          parent,
          QIcon(":/tt3-gui/Resources/Images/Actions/OpenWorkspaceLarge.png"),
          //  TODO localize via Resources
          "Failed to open workspace",
          "Would you like to remove the workspace\n" +
              workspaceAddress->displayForm() +
              "\nfrom the recent workspaces list ?")
{
}

//  End of tt3-gui/ConfirmForgetWorkspaceDialog.cpp
