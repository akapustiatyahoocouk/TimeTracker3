//
//  tt3-help/HelpSiteBuilder.cpp - tt3::help::HelpSiteBuilder class implementation
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
#include "tt3-help/API.hpp"
using namespace tt3::help;
#if defined(_MSC_VER)
    #pragma warning(disable: 4275)  //  non dll-interface class '<CLASS>' used as base for dll-interface class '<CLASS>'
#endif
#include <private/qzipreader_p.h>

//////////
//  Construction/destruction
HelpSiteBuilder::HelpSiteBuilder()
    :   _workerThread(this)
{
    //  Determine the directory where help .zips reside
    QString startupDirectory = QCoreApplication::applicationDirPath();
    _zipFilesDirectory = QDir(startupDirectory).filePath("Help");

    QString tempDirectory = QStandardPaths::writableLocation(QStandardPaths::TempLocation);
    _helpSiteDirectory = QDir(tempDirectory).filePath(".tt3-help-" TT3_VERSION);

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
            QString helpFile = QDir(_helpSiteDirectory).filePath(entryInfo.filePath);
            if (entryInfo.isFile)
            {   //  Extrac data...
                QByteArray data = zipReader.fileData(entryInfo.filePath);
                //  ...prepare the destination location...
                QString dir = QFileInfo(helpFile).absolutePath();
                if (!QDir().mkpath(dir))
                {   //  OOPS! TODO throw...
                    continue;
                }
                //  ...and save
                QFile file(helpFile);
                if (file.exists() && file.size() != entryInfo.size)
                {   //  OOPS! TODO throw
                    continue;
                }
                if (file.open(QIODevice::WriteOnly))
                {
                    file.write(data);
                    file.close();
                }
                else
                {   //  OOPS! TODO Throw...
                    continue;
                }
            }
        }
    }
}

void HelpSiteBuilder::_rebuildHelpSite(_RebuildHelpRequest & request)
{
    emit siteBuildingStarted();
    try
    {
        //  Extract content from help ZIPs into the help site directory
        QDir(_helpSiteDirectory).removeRecursively();
        const _HelpSources helpSources = _detectHelpSources();
        for (const auto & helpSource : helpSources)
        {
            emit siteBuildingProgress(
                "Analyzing " + QFileInfo(helpSource.zipFileName).fileName(),
                "-");
            QThread::msleep(100);
            _processHelpSource(helpSource);
        }
        //  Look at the "ll_CC" per-locale subdirectories
        //  in the help site directory - each of them needs
        //  a dynamically generated toc.htm, etc.
        for (QString subdir : QDir(_helpSiteDirectory).entryList(QDir::Dirs | QDir::NoDotAndDotDot))
        {
            QLocale locale = tt3::util::fromString(subdir, QLocale::c());
            if (locale != QLocale::c())
            {
                QString localeDirectory = QDir(_helpSiteDirectory).filePath(subdir);
                qDebug () << localeDirectory;
                QLocale loc = QLocale();
                QLocale::setDefault(locale);
                _buildToc(localeDirectory);
                QLocale::setDefault(loc);
            }
        }
        //  Done
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

void HelpSiteBuilder::_buildToc(const QString & helpCollectionDirectory)
{
    std::unique_ptr<HelpCollection> helpCollection
        {
            LocalSiteHelpLoader::loadHelpCollection(
                helpCollectionDirectory,
                [&](QString currentPath)
                {
                    emit siteBuildingProgress(
                        "Building table of content",
                        currentPath);
                    QThread::msleep(1000);
                })
        };
    QString tocFileName =
        QDir(helpCollectionDirectory).filePath("toc.htm");
    //  Read toc/html template
    QFile tocFile(tocFileName);
    if (!tocFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {   //  OOPS!
        throw CustomHelpException(
            tocFileName + ": " + tocFile.errorString());
    }
    QByteArray tocBytes = tocFile.readAll();
    QString tocText = QString::fromUtf8(tocBytes);
    tocFile.close();
    //  Update toc template with dynamically generated TOC HTML
    QString tocHtml;
    _writeTocEntry(tocHtml, helpCollection.get(), 0);
    tocText.replace("<p>==TOC==</p>", tocHtml);
    //  Write toc file back
    if (!tocFile.open(QIODevice::WriteOnly | QIODevice::Text))
    {   //  OOPS!
        throw CustomHelpException(
            tocFileName + ": " + tocFile.errorString());
    }
    QTextStream tocStream(&tocFile);
    tocStream << tocText;
    tocFile.close();
}

void HelpSiteBuilder::_writeTocEntry(QString & tocHtml, HelpTopic * helpTopic, int level)
{
    tocHtml += "<tr><td>\n";
    tocHtml += "<a class=\"toc";
    tocHtml += tt3::util::toString(qMax(level, 1));
    tocHtml += "\" href=\"";
    /* TODO kill off */auto sht =  dynamic_cast<SimpleHelpTopic*>(helpTopic);
    if (helpTopic->contentLoader() != nullptr)
    {
        tocHtml += helpTopic->contentLoader()->contentUrl().toString();
    }
    tocHtml += "\">";
    tocHtml += helpTopic->displayName();
    tocHtml += "</a>\n";
    tocHtml += "</td></tr>\n";
    for (HelpTopic * child : sht->children)
    {
        _writeTocEntry(tocHtml, child, level + 1);
    }
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

//  End of tt3-help/HelpSiteBuilder.cpp
