//
//  tt3-tools-restore/RestoreReader.cpp - RestoreReader class implementation
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
//  Construction/destruction
RestoreReader::RestoreReader(
        tt3::ws::Workspace workspace,
        const tt3::ws::Credentials & credentials,
        const QString & backupFileName
    ) : _workspace(workspace),
        _adminCredentials(credentials),
        _bytesToRead(QFileInfo(backupFileName).size()),
        _recordCount(_bytesToRead / 217 + 1),   //  real backup files were measured
        _oneRecordDelayMs(int(10000 / _recordCount)),
        _restoreCredentials(
            workspace->beginRestore(    //  may throw
                credentials,
                _recordCount * 85 +     //  1,000,000 objects -> 1 day lease...
                    60 * 60 * 1000)),   //  ...+ 1 hour
        _restoreFile(backupFileName),
        _restoreStream(&_restoreFile)
{
    //  Prepare record handler dispatch table
    _recordHandlers["Object:User"] = &RestoreReader::_processUserRecord;
    _recordHandlers["Object:Account"] = &RestoreReader::_processAccountRecord;
    _recordHandlers["Object:ActivityType"] = &RestoreReader::_processActivityTypeRecord;
    _recordHandlers["Object:PublicActivity"] = &RestoreReader::_processPublicActivityRecord;
    _recordHandlers["Object:PublicTask"] = &RestoreReader::_processPublicTaskRecord;
    _recordHandlers["Object:PrivateActivity"] = &RestoreReader::_processPrivateActivityRecord;
    _recordHandlers["Object:PrivateTask"] = &RestoreReader::_processPrivateTaskRecord;
    _recordHandlers["Object:Project"] = &RestoreReader::_processProjectRecord;
    _recordHandlers["Object:WorkStream"] = &RestoreReader::_processWorkStreamRecord;
    _recordHandlers["Object:Beneficiary"] = &RestoreReader::_processBeneficiaryRecord;
    _recordHandlers["Object:Work"] = &RestoreReader::_processWorkRecord;
    _recordHandlers["Object:Event"] = &RestoreReader::_processEventRecord;
    _recordHandlers["Association:QuickPicksList"] = &RestoreReader::_processQuickPicksListAssociationRecord;
    _recordHandlers["Association:PermittedWorkloads"] = &RestoreReader::_processPermittedWorkloadsAssociationRecord;
    _recordHandlers["Association:ActivityType"] = &RestoreReader::_processActivityTypeAssociationRecord;
    _recordHandlers["Association:ActivityWorkload"] = &RestoreReader::_processActivityWorkloadAssociationRecord;
    _recordHandlers["Association:WorkloadBeneficiaries"] = &RestoreReader::_processWorkloadBeneficiariesAssociationRecord;
}

RestoreReader::~RestoreReader()
{
    _restoreFile.close();   //  destructors must be noexcept!
    try
    {
        _workspace->releaseCredentials(_restoreCredentials);
    }
    catch (const tt3::util::Exception & ex)
    {   //  destructors must be noexcept!
        qCritical() << ex;
    }
}

//////////
//  Operations
bool RestoreReader::restoreWorkspace()
{
    Q_ASSERT(_bytesRead == 0);

    //  Prepare to restore
    if (!_restoreFile.open(QIODevice::ReadOnly))
    {   //  OOPS!
        throw tt3::ws::CustomWorkspaceException(_restoreFile.fileName() + ": " + _restoreFile.errorString());
    }

    //  Do we need a progress dialog ?
    if (QThread::currentThread()->eventDispatcher() != nullptr)
    {
        _progressDialog.reset(
            new RestoreProgressDialog(
                QApplication::activeWindow(),
                _workspace->address()->displayForm(),
                _restoreFile.fileName()));
        _progressDialog->setVisible(true);
    }

    //  Go!
    _record.reset();
    try
    {
        //  All _restore...() services may throw
        while (!_restoreStream.atEnd())
        {
            QString line = _restoreStream.readLine().trimmed();
            if (line.startsWith("[") && line.endsWith("]"))
            {   //  New recpord starts here
                if (_record.isValid())
                {
                    _processRecord();
                }
                _record.reset(line.mid(1, line.length() - 2));
                continue;
            }
        }
        if (_record.isValid())
        {
            _processRecord();
        }

        //  Cleanup & we're done.
        //  Note, that these will be called from
        //  destructor as a last resort, but calling
        //  them leads to earlier lock release and
        //  better error diagnostics.
        _progressDialog.reset(nullptr);
        _restoreFile.close();
        _workspace->releaseCredentials(_restoreCredentials);   //  may throw
        return true;
    }
    catch (const _CancelRequest &)
    {   //  Cleanup & cancel
        _progressDialog.reset(nullptr);
        _restoreFile.close();
        _workspace->releaseCredentials(_restoreCredentials);
        return false;
    }
    catch (...)
    {   //  OOPS! Cleamup, then re-throw
        _progressDialog.reset(nullptr);
        _restoreFile.close();
        _workspace->releaseCredentials(_restoreCredentials);
        throw;
    }
}

//////////
//  Implementation helpers
void RestoreReader::_reportProgress()
{
    if (_progressDialog != nullptr)
    {
        double progress =
            (_bytesToRead == 0) ?
                0.0 :
                (double)_restoreFile.pos() / (double)_bytesToRead;
        _progressDialog->reportProgress(progress);
        QDateTime continueAt =
            QDateTime::currentDateTimeUtc().addMSecs(_oneRecordDelayMs);
        do
        {
            QCoreApplication::processEvents();
        }   while (QDateTime::currentDateTimeUtc() < continueAt);
        if (_progressDialog->cancelRequested())
        {
            throw _CancelRequest();
        }
    }
}

void RestoreReader::_processRecord()
{
    qDebug() << _record.type;
    Q_ASSERT(_recordHandlers.contains(_record.type));
    (this->*_recordHandlers[_record.type])();
    _reportProgress();
}

void RestoreReader::_processUserRecord()
{
    //  TODO implement
}

void RestoreReader::_processAccountRecord()
{
    //  TODO implement
}

void RestoreReader::_processActivityTypeRecord()
{
    //  TODO implement
}

void RestoreReader::_processPublicActivityRecord()
{
    //  TODO implement
}

void RestoreReader::_processPublicTaskRecord()
{
    //  TODO implement
}

void RestoreReader::_processPrivateActivityRecord()
{
    //  TODO implement
}

void RestoreReader::_processPrivateTaskRecord()
{
    //  TODO implement
}

void RestoreReader::_processProjectRecord()
{
    //  TODO implement
}

void RestoreReader::_processWorkStreamRecord()
{
    //  TODO implement
}

void RestoreReader::_processBeneficiaryRecord()
{
    //  TODO implement
}

void RestoreReader::_processWorkRecord()
{
    //  TODO implement
}

void RestoreReader::_processEventRecord()
{
    //  TODO implement
}

void RestoreReader::_processQuickPicksListAssociationRecord()
{
    //  TODO implement
}

void RestoreReader::_processPermittedWorkloadsAssociationRecord()
{
    //  TODO implement
}

void RestoreReader::_processActivityTypeAssociationRecord()
{
    //  TODO implement
}

void RestoreReader::_processActivityWorkloadAssociationRecord()
{
    //  TODO implement
}

void RestoreReader::_processWorkloadBeneficiariesAssociationRecord()
{
    //  TODO implement
}

//  End of tt3-tools-restore/RestoreReader.cpp
