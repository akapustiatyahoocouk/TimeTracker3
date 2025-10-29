//
//  tt3-gui/HelpSiteBuilder.cpp - tt3::gui::HelpSiteBuilder class implementation
//  TODO localize via Resources
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
#include "tt3-gui/API.hpp"
using namespace tt3::gui;
#include <private/qzipreader_p.h>

//////////
//  Construction/destruction
HelpSiteBuilder::HelpSiteBuilder()
    :   _workerThread(this)
{
    //  Determine the directory where help .zips reside
    QString startupDirectory = QCoreApplication::applicationDirPath();
    qDebug() << "startupDirectory=" << startupDirectory;
    _zipFilesDirectory = QDir(startupDirectory).filePath("Help");
    qDebug() << "_zipFilesDirectory" << _zipFilesDirectory;

    QString tempDirectory = QStandardPaths::writableLocation(QStandardPaths::TempLocation);
    qDebug() << "tempDirectory=" << tempDirectory;
    _helpSiteDirectory = QDir(tempDirectory).filePath(".tt3-help-" TT3_VERSION);
    qDebug() << "_helpSiteDirectory=" << _helpSiteDirectory;

    //  Move signals to worker thread to ensure queued
    //  connections to all slots
    //  TODO kill off moveToThread(&_workerThread);

    //  Done
    _workerThread.start();
}

HelpSiteBuilder::~HelpSiteBuilder()
{
    _workerThread.requestStop();
    _workerThread.wait(5 * _WorkerThread::WaitChunkMs);
    _workerThread.terminate();
}

//////////
//  Operations
bool HelpSiteBuilder::buildHelpSite()
{
    //  Post a "help site rebuild" request
    //  and wit for its completion
    std::atomic<bool> comletionStatus = false;
    _workerThread.post(new _RebuildHelpRequest(comletionStatus));
    while (!comletionStatus)
    {
        QCoreApplication::processEvents(QEventLoop::AllEvents);
    }
    return true;
}

//////////
//  Implementation helpers
auto HelpSiteBuilder::_detectHelpSources(
    ) -> _HelpSources
{
    _HelpSources result;
    for (const auto & zipInfo : QDir(_zipFilesDirectory).entryInfoList())
    {
        if (zipInfo.isFile() && !zipInfo.isSymbolicLink() &&
            zipInfo.fileName().endsWith(".zip"))
        {
            _HelpSource helpSource;
            helpSource.zipFileName = zipInfo.absoluteFilePath();
            helpSource.zipFileTime = zipInfo.lastModified(QTimeZone::UTC);
            helpSource.zipFileSize = zipInfo.size();
            result.append(helpSource);
        }
    }
    std::sort(
        result.begin(),
        result.end(),
        [](auto a, auto b)
        {
            return a.zipFileName < b.zipFileName;
        });
    return result;
}

void HelpSiteBuilder::_processHelpSource(const _HelpSource & helpSource)
{
    QZipReader zipReader(helpSource.zipFileName);
    if (zipReader.exists())
    {
        for (auto entryInfo : zipReader.fileInfoList())
        {
            emit siteBuildingProgress(
                "Analyzing " + QFileInfo(helpSource.zipFileName).fileName(),
                "Extracting " + entryInfo.filePath);
            QThread::msleep(100);
            if (entryInfo.isFile)
            {
                QString helpFile = QDir(_helpSiteDirectory).filePath(entryInfo.filePath);
                QByteArray data = zipReader.fileData(entryInfo.filePath);
                //qDebug() << entryInfo.filePath << " -> " << helpFile;
                //qDebug() << data;
            }
        }
    }
}

void HelpSiteBuilder::_rebuildHelpSite(_RebuildHelpRequest & request)
{
    emit siteBuildingStarted();
    try
    {
        _HelpSources helpSources = _detectHelpSources();
        for (auto helpSource : helpSources)
        {
            emit siteBuildingProgress(
                "Analyzing " + QFileInfo(helpSource.zipFileName).fileName(),
                "-");
            QThread::msleep(100);
            _processHelpSource(helpSource);
        }
        throw tt3::util::NotImplementedError();
        //  TODO void        siteBuildingProgress(const QString & action);
        emit siteBuildingCompleted(true);
    }
    catch (const tt3::util::Exception & ex)
    {   //  Propagate the error message to event thread
        emit siteBuildingCompleted(false);
        emit siteBuildingError(ex.errorMessage());
    }
    catch (const tt3::util::Error & ex)
    {   //  We catch errors too - this is a WORKER thread
        emit siteBuildingCompleted(false);
        emit siteBuildingError(ex.errorMessage());
    }
    request.comletionStatus = true;
}

//////////
//  HelpSiteBuilder::_WorkerThread
void HelpSiteBuilder::_WorkerThread::run()
{
    while (!_stopRequested)
    {
        _ServiceRequest * serviceRequest = nullptr;
        if (_requestQueue.tryDequeue(serviceRequest, WaitChunkMs))
        {
            if (auto request = dynamic_cast<_RebuildHelpRequest*>(serviceRequest))
            {
                _helpSiteBuilder->_rebuildHelpSite(*request);
            }
            delete serviceRequest;
        }
    }
}

//  End of tt3-gui/HelpSiteBuilder.cpp
