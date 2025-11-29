//
//  tt3-tools-restore/RestoreTool.cpp - RestoreTool class implementation
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
#include "tt3-tools-restore/API.hpp"
using namespace tt3::tools::restore;

//////////
//  Constants
const QString RestoreTool::PreferredFilenameExtension = ".tt3-backup";

//////////
//  Singleton
IMPLEMENT_SINGLETON(RestoreTool)
RestoreTool::RestoreTool() {}
RestoreTool::~RestoreTool() {}

//////////
//  tt3::uti::ITool
RestoreTool::Mnemonic RestoreTool::mnemonic() const
{
    return M(Restore);
}

QString RestoreTool::displayName() const
{
    static Component::Resources *const resources = Component::Resources::instance();   //  idempotent
    return resources->string(RSID(RestoreTool), RID(DisplayName));
}

QString RestoreTool::description() const
{
    static Component::Resources *const resources = Component::Resources::instance();   //  idempotent
    return resources->string(RSID(RestoreTool), RID(Description));
}

QIcon RestoreTool::smallIcon() const
{
    static const QIcon icon(":/tt3-tools-restore/Resources/Images/Misc/RestoreSmall.png");
    return icon;
}

QIcon RestoreTool::largeIcon() const
{
    static const QIcon icon(":/tt3-tools-restore/Resources/Images/Misc/RestoreLarge.png");
    return icon;
}

bool RestoreTool::enabled() const
{
    return true;
}

void RestoreTool::run(QWidget * parent)
{
    Q_ASSERT(QThread::currentThread()->eventDispatcher() != nullptr);

    //  Configure the Restore operation
    ConfigureRestoreDialog dlg(parent);
    if (dlg.doModal() != ConfigureRestoreDialog::Result::Ok)
    {
        return;
    }
    //  Create a new workspace to reastore into, with
    //  fake Admin credentials
    tt3::ws::WorkspaceAddress workspaceAddress = dlg.workspaceAddress();
    QString adminUser = QUuid::createUuid().toString();
    QString adminLogin = QUuid::createUuid().toString();
    QString adminPassword = QUuid::createUuid().toString();
    tt3::ws::Workspace workspace =
        workspaceAddress->workspaceType()->createWorkspace(   //  may throw
            workspaceAddress,
            adminUser,
            adminLogin,
            adminPassword);
    //  Go!
    QString restoreSource = dlg.restoreSource();
    bool restoreSuccessful = false;
    tt3::ws::Credentials adminCredentials(adminLogin, adminPassword);
    try
    {
        RestoreReader restoreReader(
            workspace,
            adminCredentials,
            restoreSource);
        try
        {
            restoreSuccessful =
                restoreReader.restoreWorkspace(); //  may throw
        }
        catch (const tt3::util::ParseException & ex)
        {   //  Log & translate
            qCritical() << ex;
            throw BackupFileCorruptException(restoreSource);
        }
        //  Cleanup before returning
        workspace->close(); //  may throw
        //  RestoreReader's destructor closes the backup file
    }
    catch (...)
    {   //  Cleanup, then re-throw
        //  Cleanup calls may throw, but irrelevant at this point
        workspace->close(); //  may throw
        workspaceAddress->workspaceType()->destroyWorkspace( //  may throw
            adminCredentials, workspaceAddress);
        throw;
    }

    if (restoreSuccessful)
    {   //  Need to destroy the artificial "admin credentials"
        workspace =
            workspaceAddress->workspaceType()->openWorkspace(   //  may throw
                workspaceAddress,
                tt3::ws::OpenMode::ReadWrite);
        workspace->login(adminCredentials)->user(adminCredentials)->destroy(adminCredentials);  //  may throw
        workspace->close(); //  may throw
        //  Pop up the "restore completed" message
        tt3::util::ResourceReader rr(Component::Resources::instance(), RSID(RestoreCompletedDialog));
        tt3::gui::MessageDialog::show(
            parent,
            rr.string(RID(Title)),
            rr.string(RID(Message),
                      workspaceAddress->displayForm(),
                      restoreSource));
    }
    else
    {   //  Need to destroy the partually restored workspace
        workspaceAddress->workspaceType()->destroyWorkspace( //  may throw
            adminCredentials, workspaceAddress);
        //  Pop up the "restore cancelled" message
        tt3::util::ResourceReader rr(Component::Resources::instance(), RSID(RestoreCancelledDialog));
        tt3::gui::MessageDialog::show(
            parent,
            rr.string(RID(Title)),
            rr.string(RID(Message)));
    }
}

//  End of tt3-tools-restore/RestoreTool.cpp

