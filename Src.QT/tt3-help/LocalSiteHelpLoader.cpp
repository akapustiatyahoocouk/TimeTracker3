//
//  tt3-help/LocalSiteHelpLoader.cpp - tt3::help::LocalSiteHelpLoader class implementation
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
#include "tt3-help/API.hpp"
using namespace tt3::help;

//////////
//  Operations
auto LocalSiteHelpLoader::loadHelpCollection(
        const QString siteDirectory,
        ProgressListener progressListener
    ) -> HelpCollection *
{
    QDir absSiteDirectory(QFileInfo(siteDirectory).absoluteFilePath());
    if (!absSiteDirectory.exists())
    {   //  OOPS!
        throw HelpSiteDoesNotExistException(absSiteDirectory.path());
    }
    //  There are 2 possibilities here:
    //  1.  The absSiteDirectory contains a help tree, or
    //  2.  The absSiteDirectory contains one subdirectory for each locale
    //      supported by the help site, named after the locale ("en_GB", etc.)
    QStringList subdirs = absSiteDirectory.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
    QMap<QLocale, QString> dirsForLocales;
    for (QString subdir : subdirs)
    {
        QLocale locale = tt3::util::fromString(subdir, QLocale::c());
        if (locale != QLocale::c())
        {
            dirsForLocales[locale] = absSiteDirectory.filePath(subdir);
        }
    }
    if (dirsForLocales.isEmpty())
    {   //  A sigle simpe help collection
        return _loadSimpleHelpCollection(
            absSiteDirectory.absolutePath(),
            progressListener);
    }
    else if (dirsForLocales.size() == 1)
    {   //  1 locale only - no point in creating localized help
        return _loadSimpleHelpCollection(
            dirsForLocales.values()[0],
            progressListener);
    }
    else
    {   //  A localized help collection
        throw tt3::util::NotImplementedError();
        /*  TODO uncomment & implement
        QMap<QLocale, SimpleHelpCollection*> collectionsForLocales;
        for (QLocale locale : dirsForLocales.keys())
        {
            collectionsForLocales[locale] =
                _loadSimpleHelpCollection(dirsForLocales[locale]);
        }
        */
    }
    return nullptr;
}

//////////
//  Implementation
auto LocalSiteHelpLoader::_loadSimpleHelpCollection(
        const QString & rootDirectory,
        ProgressListener progressListener
    ) -> SimpleHelpCollection *
{
    std::unique_ptr<SimpleHelpCollection> helpCollection
    { new SimpleHelpCollection("", "", nullptr) };
    _loadTopicFromDirectory(helpCollection.get(), rootDirectory, progressListener);
    return helpCollection.release();
}

void LocalSiteHelpLoader::_loadTopicFromDirectory(
        SimpleHelpTopic * topic,
        const QString & directory,
        ProgressListener progressListener
    )
{
    QDir dir(directory);
    QStringList entries = dir.entryList(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot);
    for (auto entry : entries)
    {
        QString fileOrDirPath = dir.filePath(entry);
        if (progressListener != nullptr)
        {
            progressListener(fileOrDirPath);
        }
        QFileInfo fileOrDirInfo(fileOrDirPath);
        if (fileOrDirInfo.isFile())
        {   //  The .html file becomes a help topic
            //  UNLESS it's "index.html", in which case it
            //  specifies content & properties for the
            //  owner of "topics"
            if (entry == "index.html")
            {   //  This file is content for the enclising directory
                QString displayName;
                _analyzeHtmlFile(fileOrDirPath, displayName);
                topic->setDisplayName(displayName);
                topic->setContentUrl(QUrl::fromLocalFile(fileOrDirPath));
            }
            else if (entry.endsWith(".html"))
            {   //  This HTML file becomes a separate help topic
                QString displayName;
                _analyzeHtmlFile(fileOrDirPath, displayName);
                topic->children.createTopic(
                    entry.left(entry.length() - 5),
                    displayName,
                    new LocalFileContentLoader(fileOrDirPath));
            }
        }
        else if (fileOrDirInfo.isDir())
        {   //  The dir becomes a .html topic; the
            //  index.html within the dir gives the topic's
            //  properties and the dir itself is further
            //  analyzed for the topic's children
            auto childTopic =
                topic->children.createTopic(
                    entry,
                    "", //  displayName will be loaded from index.html
                    nullptr);
            _loadTopicFromDirectory(childTopic, fileOrDirPath, progressListener);
        }
    }
}

void LocalSiteHelpLoader::_analyzeHtmlBytes(
        const QByteArray & htmlBytes,
        QString & displayName
    )
{
    QString html = QString::fromUtf8(htmlBytes);

    static const QRegularExpression titleRegex("<title[^>]*>(.*?)</title>");
    QRegularExpressionMatch titleMatch = titleRegex.match(html);
    if (titleMatch.hasMatch())
    {
        displayName = QTextDocumentFragment::fromHtml(titleMatch.captured(1)).toPlainText().trimmed();
    }
}

void LocalSiteHelpLoader::_analyzeHtmlFile(
        const QString & htmlFileName,
        QString & displayName
    )
{
    QFile htmlFile(htmlFileName);
    if (htmlFile.open(QIODevice::ReadOnly))
    {
        _analyzeHtmlBytes(htmlFile.readAll(), displayName);
        htmlFile.close();
    }
}

//  End of tt3-help/LocalSiteHelpLoader.cpp
