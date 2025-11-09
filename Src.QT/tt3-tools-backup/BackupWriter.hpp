//
//  tt3-tools-backup/BackupWriter.hpp - tt3 Backup writer agent
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

namespace tt3::tools::backup
{
    /// \class BackupWriter tt3-tools-backup/API.hpp
    /// \brief The "TT3 Backup writer" agent.
    class TT3_TOOLS_BACKUP_PUBLIC BackupWriter final
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(BackupWriter)

        //////////
        //  Construction/destruction
    public:
        /// \brief
        ///     The class constructor.
        BackupWriter();

        /// \brief
        ///     The class destructor.
        ~BackupWriter();

        //////////
        //  Operations
    public:
        void        backupWorkspace(
                            tt3::ws::Workspace workspace,
                            const tt3::ws::Credentials & credentials,
                            const QString & backupFileName
                        );
    };
}

//  End of tt3-tools-backup/BackupWriter.hpp
