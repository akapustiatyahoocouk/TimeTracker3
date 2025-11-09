//
//  tt3-tools-backup/BackupTool.cpp - BackupTool class implementation
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
#include "tt3-tools-backup/API.hpp"
using namespace tt3::tools::backup;

//////////
//  Constants
const QString BackupTool::PreferredFilenameExtension = ".tt3-backup";

//////////
//  Singleton
IMPLEMENT_SINGLETON(BackupTool)
BackupTool::BackupTool() {}
BackupTool::~BackupTool() {}

//////////
//  tt3::uti::ITool
BackupTool::Mnemonic BackupTool::mnemonic() const
{
    return M(Backup);
}

QString BackupTool::displayName() const
{
    static Component::Resources *const resources = Component::Resources::instance();   //  idempotent
    return resources->string(RSID(BackupTool), RID(DisplayName));
}

QString BackupTool::description() const
{
    static Component::Resources *const resources = Component::Resources::instance();   //  idempotent
    return resources->string(RSID(BackupTool), RID(Description));
}

QIcon BackupTool::smallIcon() const
{
    static const QIcon icon(":/tt3-tools-backup/Resources/Images/Misc/BackupSmall.png");
    return icon;
}

QIcon BackupTool::largeIcon() const
{
    static const QIcon icon(":/tt3-tools-vackup/Resources/Images/Misc/BackupLarge.png");
    return icon;
}

bool BackupTool::enabled() const
{
    return true;
}

void BackupTool::run(QWidget * parent)
{
    //  Select workspace to backup
    ConfigureBackupDialog dlg(parent);
    if (dlg.doModal() != ConfigureBackupDialog::Result::Ok)
    {
        return;
    }
    tt3::ws::WorkspaceAddress workspaceAddress = dlg.selectedWorkspaceAddress();
    tt3::ws::Workspace workspace;
    if (tt3::gui::theCurrentWorkspace != nullptr &&
        tt3::gui::theCurrentWorkspace->address() == workspaceAddress)
    {   //  Use current workspace
        workspace = tt3::gui::theCurrentWorkspace;
    }
    else
    {   //  Use custom workspace
        workspace =
            workspaceAddress->workspaceType()->openWorkspace(   //  may throw
                workspaceAddress,
                tt3::ws::OpenMode::ReadOnly);
    }
    //  Make sure we're using the Credentials that grant
    //  the BackupAndRestore (or Administrator) capability
    tt3::ws::Credentials credentials = tt3::gui::theCurrentCredentials;
    while (!credentials.isValid() ||
           !workspace->grantsAny(   //  may throw
                credentials,
                tt3::ws::Capability::Administrator |
                tt3::ws::Capability::BackupAndRestore))
    {   //  Need to use different credentials
        tt3::gui::ChooseReloginDialog dlg1(parent, workspaceAddress);
        if (dlg1.doModal() != tt3::gui::ChooseReloginDialog::Result::Yes)
        {   //  Cleanup & abort
            if (tt3::gui::theCurrentWorkspace != workspace)
            {
                workspace->close();     //  may throw, but irrelevant at this point
            }
            return;
        }
        //  The user has confirmed they want to re-login
        tt3::gui::LoginDialog dlg2(parent, QString());
        if (dlg2.doModal() != tt3::gui::LoginDialog::Result::Ok)
        {   //  Cleanup & abort
            if (tt3::gui::theCurrentWorkspace != workspace)
            {
                workspace->close();     //  may throw, but irrelevant at this point
            }
            return;
        }
        credentials = dlg2.credentials();
    }
    //  At this point, we have a) the workspace to backup
    //  and b) the credentials that allow to do so
    try
    {
        BackupWriter backupWriter;
        backupWriter.backupWorkspace(   //  may throw
            workspace,
            credentials,
            dlg.selectedBackupDestination());
        //  Cleanup before returning
        if (tt3::gui::theCurrentWorkspace != workspace)
        {
            workspace->close();     //  may throw, but irrelevant at this point
        }
    }
    catch (...)
    {   //  Cleanup, then re-throw
        if (workspace != tt3::gui::theCurrentWorkspace.operator ->())
        {
            workspace->close();     //  may throw, but irrelevant at this point
        }
        throw;
    }
}

//  End of tt3-tools-backup/BackupTool.cpp
