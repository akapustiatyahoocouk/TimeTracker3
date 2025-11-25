//
//  tt3-tools-restore/RestoreReader.hpp - tt3 Restore Reader agent
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

namespace tt3::tools::restore
{
    /// \class RestoreReader tt3-tools-restore/API.hpp
    /// \brief The "TT3 Restore Reader" agent.
    class TT3_TOOLS_RESTORE_PUBLIC RestoreReader final
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(RestoreReader)

        //////////
        //  Construction/destruction
    public:
        RestoreReader(
                tt3::ws::Workspace workspace,
                const tt3::ws::Credentials & credentials,
                const QString & backupFileName
            );
        ~RestoreReader();

        //////////
        //  Operations
    public:
        /// \brief
        ///     Performs the workspace restore.
        /// \details
        ///     If called from a thread that currently runs
        ///     an event loop, will display a popup "restore
        ///     progress" window that allows cancelling the
        ///     restore process while it is underwau.
        /// \exception Exception
        ///     If an error occurs.
        bool        restoreWorkspace();

        //////////
        //  Implementation
    private:
        tt3::ws::Workspace      _workspace; //  to restore into
        tt3::ws::Credentials    _adminCredentials;   //  for _workspace
        const quint64   _bytesToRead;
        const quint64   _recordCount;   //  approximate! >0
        const int       _oneRecordDelayMs;
        const tt3::ws::RestoreCredentials _restoreCredentials;   //  for _workspace

        QFile           _restoreFile;   //  to read from
        QTextStream     _restoreStream; //  to read from
        quint64         _bytesRead = 0;

        std::unique_ptr<RestoreProgressDialog>  _progressDialog = nullptr;

        struct _CancelRequest {};   //  thrown to cancel

        //  Helpers
        //  All methods may throw
        struct _Record
        {
            QString     type;   //  == backup section name
            QMap<QString,QString>   fields;

            bool        isValid() const
            {
                return !type.isEmpty();
            }

            void        reset(const QString & recordType = "")
            {
                type = recordType;
                fields.clear();
            }

        };
        _Record         _record;    //  currently being read/processed
        
        void            _reportProgress();
        void            _processRecord();
    };
}

//  End of tt3-tools-restore/RestoreReader.hpp
