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
        using _RecordHandler = void (RestoreReader::*)();
        QMap<QString, _RecordHandler>   _recordHandlers;

        //  All methods may throw
        static int      _xdigit(QChar c);

        template <class T>
        T               _parse(const QString & s, qsizetype & scan)
        {
            return tt3::util::fromString<T>(s, scan);
        }
#define DECLARE_PARSE(T)        \
        template <> T  _parse<T>(const QString & s, qsizetype & scan);
        DECLARE_PARSE(QString)
        DECLARE_PARSE(QStringList)
        DECLARE_PARSE(tt3::ws::InactivityTimeout)
        DECLARE_PARSE(tt3::ws::UiLocale)
#undef DECLARE_PARSE

        struct _Record
        {
            _Record(RestoreReader * rr) : restoreReader(rr) {}

            RestoreReader *const    restoreReader;
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

            template <class T>
            T           fetchField(const QString & field)
            {
                if (!fields.contains(field))
                {   //  OOPS!
                    throw BackupFileCorruptException(restoreReader->_restoreFile.fileName());
                }
                qsizetype scan = 0;
                T result= restoreReader->_parse<T>(fields[field], scan);
                if (scan != fields[field].length())
                {   //  OOPS!
                    throw tt3::util::ParseException(fields[field], scan);
                }
                return result;
            }

            template <class T>
            T           fetchOptionalField(const QString & field)
            {
                return fields.contains(field) ?
                        fetchField<T>(field) :  //  may throw
                        T();
            }
        };
        _Record         _record{this};  //  currently being read/processed

        void            _reportProgress();
        void            _processRecord();
        void            _processUserRecord();
        void            _processAccountRecord();
        void            _processActivityTypeRecord();
        void            _processPublicActivityRecord();
        void            _processPublicTaskRecord();
        void            _processPrivateActivityRecord();
        void            _processPrivateTaskRecord();
        void            _processProjectRecord();
        void            _processWorkStreamRecord();
        void            _processBeneficiaryRecord();
        void            _processWorkRecord();
        void            _processEventRecord();
        void            _processQuickPicksListAssociationRecord();
        void            _processPermittedWorkloadsAssociationRecord();
        void            _processActivityTypeAssociationRecord();
        void            _processActivityWorkloadAssociationRecord();
        void            _processWorkloadBeneficiariesAssociationRecord();
    };
}

//  End of tt3-tools-restore/RestoreReader.hpp
