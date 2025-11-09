//
//  tt3-tools-backup/BackupWriter.cpp - BackupWriter class implementation
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
//  Construction/destruction
BackupWriter::BackupWriter()
{
}

BackupWriter::~BackupWriter()
{
}

//////////
//  Operations
void BackupWriter::backupWorkspace(
        tt3::ws::Workspace workspace,
        const tt3::ws::Credentials & credentials,
        const QString & backupFileName
    )
{
    Q_ASSERT(workspace != nullptr);

    //  Prepare to backup
    tt3::ws::BackupCredentials backupCredentials =
        workspace->beginBackup(credentials);    //  may throw
    try
    {
        //  Open file/stream
        QFile backupFile(backupFileName);
        if (!backupFile.open(QIODevice::WriteOnly))
        {   //  OOPS!
            throw tt3::ws::CustomWorkspaceException(backupFileName + ": " + backupFile.errorString());
        }
        QTextStream backupStream(&backupFile);
        //  Go!

        //  Cleanup & we're done
        workspace->releaseCredentials(backupCredentials);
    }
    catch (...)
    {   //  OOPS! Cleamup, then re-throw
        workspace->releaseCredentials(backupCredentials);
        QFile::remove(backupFileName);  //  may fail, but who cares at this point...
        throw;
    }
}

//  End of tt3-tools-backup/BackupWriter.cpp
