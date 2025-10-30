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
        ProgressListener /*progressListener*/
    ) -> IHelpCollection *
{
    QDir absSiteDirectory(QFileInfo(siteDirectory).absoluteFilePath());
    if (!absSiteDirectory.exists())
    {   //  OOPS! TODO throw
        throw new tt3::util::NotImplementedError();
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
            qDebug() << locale << " -> " << dirsForLocales[locale];
        }
    }
    if (dirsForLocales.isEmpty())
    {   //  A sigle simpe help collection
        return _loadSimpleHelpCollection(absSiteDirectory.absolutePath());
    }
    else
    {   //  A localized help collection
        QMap<QLocale, SimpleHelpCollection*> collectionsForLocales;
        for (QLocale locale : dirsForLocales.keys())
        {
            collectionsForLocales[locale] =
                _loadSimpleHelpCollection(dirsForLocales[locale]);
        }
    }
    return nullptr;
}

//////////
//  Implementation
auto LocalSiteHelpLoader::_loadSimpleHelpCollection(
        const QString & rootDirectory
    ) -> SimpleHelpCollection *
{
    std::unique_ptr<SimpleHelpCollection> helpCollection
        { new SimpleHelpCollection() };
    _loadTopicsFromDirectory(
        rootDirectory,
        helpCollection->roots);
    return helpCollection.release();
}

void LocalSiteHelpLoader::_loadTopicsFromDirectory(
        const QString & directoty,
        SimpleHelpTopicCollection & topics
    )
{
    QDir dir(directoty);
    QStringList entries = dir.entryList(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot);
    for (auto entry : entries)
    {
        QString fileOrDirPath = dir.filePath(entry);
        QFileInfo fileOrDirInfo(fileOrDirPath);
        if (fileOrDirInfo.isFile())
        {
            qDebug() << "File:" << fileOrDirPath;
        }
        else if (fileOrDirInfo.isDir())
        {
            qDebug() << "Dir:" << fileOrDirPath;
        }
    }
}

//  End of tt3-help/LocalSiteHelpLoader.cpp
