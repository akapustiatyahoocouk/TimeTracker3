//
//  tt3-skin-admin/Plugins.cpp - tt3::skin::admin::RecentWorkspaceOpener class implementation
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
#include "tt3-skin-admin/API.hpp"
using namespace tt3::skin::admin;

//////////
//  Construction/destruction
RecentWorkspaceOpener::RecentWorkspaceOpener(MainFrame * mainFrame, const tt3::ws::WorkspaceAddress & workspaceAddress)
    :   _mainFrame(mainFrame),
        _workspaceAddress(workspaceAddress)
{
    Q_ASSERT(_mainFrame != nullptr);
    Q_ASSERT(_workspaceAddress != nullptr);
}

RecentWorkspaceOpener::~RecentWorkspaceOpener()
{
}

//////////
//  Eveht handlers
void RecentWorkspaceOpener::_onTriggered()
{
    if (!_mainFrame->_openWorkspace(_workspaceAddress))
    {   //  May want to remove this workspace from the MRU
        tt3::gui::ConfirmForgetWorkspaceDialog dlg(_mainFrame, _workspaceAddress);
        if (dlg.doModal() == tt3::gui::ConfirmForgetWorkspaceDialog::Result::Yes)
        {   //  Do it!
            tt3::ws::Component::Settings::instance()->removeRecentWorkspace(_workspaceAddress);
            MainFrame * mainFrame = _mainFrame;
            mainFrame->_updateMruWorkspaces();  //  will delete "*this" instance!
            mainFrame->refresh();
        }
    }
}

//  End of tt3-skin-admin/RecentWorkspaceOpener.cpp
