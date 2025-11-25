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
    _reportProgress();
}

//  End of tt3-tools-restore/RestoreReader.cpp
