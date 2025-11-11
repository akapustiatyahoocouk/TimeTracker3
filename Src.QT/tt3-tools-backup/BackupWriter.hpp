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
        BackupWriter(
                tt3::ws::Workspace workspace,
                const tt3::ws::Credentials & credentials,
                const QString & backupFileName
            );
        ~BackupWriter();

        //////////
        //  Operations
    public:
        bool        backupWorkspace();

        //////////
        //  Implementation
    private:
        tt3::ws::Workspace  _workspace;
        tt3::ws::BackupCredentials  _credentials;
        QFile           _backupFile;
        QTextStream     _backupStream;

        const quint64   _objectsToWrite;
        const quint64   _associationsToWrite;
        quint64         _objectsWritten = 0;
        quint64         _associationsWritten = 0;
        const int       _oneObjectDelayMs;

        std::unique_ptr<BackupProgressDialog>   _progressDialog = nullptr;

        struct _CancelRequest {};

        //  Helpers
        //  All methods may throw
        void        _onObjectWritten();
        void        _onAssociationWritten();
        void        _reportProgress();

        template <class T>
        void        _backupObjects(
                            const QSet<T> & objects
                        )
        {
            for (const auto & object : _sortedByOid(objects))
            {
                _backupObject(object);
                _onObjectWritten();
            }
        }

        void        _backupObject(  //  incl. Accounts
                            tt3::ws::User user
                        );
        void        _backupObject(
                            tt3::ws::Account account
                        );
        void        _backupObject(
                            tt3::ws::ActivityType activityType
                        );
        void        _backupObject(  //  Not Task!
                            tt3::ws::PublicActivity publicActivity
                        );
        void        _backupObject(  //  incl. children
                            tt3::ws::PublicTask publicTask
                        );
        void        _backupObject(  //  Not Task!
                            tt3::ws::PrivateActivity privateActivity
                        );
        void        _backupObject(  //  incl. children
                            tt3::ws::PrivateTask privateTask
                        );
        void        _backupObject(  //  incl. children
                            tt3::ws::Project project
                        );
        void        _backupObject(
                            tt3::ws::WorkStream workStream
                        );
        void        _backupObject(
                            tt3::ws::Beneficiary beneficiary
                        );
        void        _backupObject(
                            tt3::ws::Work work
                        );
        void        _backupObject(
                            tt3::ws::Event event
                        );

        void        _writeObjectHeader(
                            const tt3::ws::Object & object
                        );
        void        _writeObjectProperty(
                            const QString & propertyName,
                            const tt3::ws::Oid & propertyValue
                        );
        void        _writeObjectProperty(
                            const QString & propertyName,
                            const QList<tt3::ws::Oid> & propertyValue
                        );
        void        _writeObjectProperty(
                            const QString & propertyName,
                            bool propertyValue
                        );
        void        _writeObjectProperty(
                            const QString & propertyName,
                            const QString & propertyValue
                        );
        void        _writeObjectProperty(
                            const QString & propertyName,
                            const QStringList & propertyValue
                        );
        void        _writeObjectProperty(
                            const QString & propertyName,
                            const tt3::ws::UiLocale & propertyValue
                        );
        void        _writeObjectProperty(
                            const QString & propertyName,
                            const tt3::ws::InactivityTimeout & propertyValue
                        );
        void        _writeObjectProperty(
                            const QString & propertyName,
                            const tt3::ws::Capabilities & propertyValue
                        );
        void        _writeObjectProperty(
                            const QString & propertyName,
                            const QDateTime & propertyValue
                        );

        template <class T>
        QList<T>        _sortedByOid(const QSet<T> & objects)
        {
            QList<T> result = objects.values();
            if (result.size() > 1)
            {
                std::sort(
                    result.begin(),
                    result.end(),
                    [](const T & a, const T & b)
                    {
                        return tt3::util::toString(a->oid()) < tt3::util::toString(b->oid());
                    });
            }
            return result;
        }
    };
}

//  End of tt3-tools-backup/BackupWriter.hpp
