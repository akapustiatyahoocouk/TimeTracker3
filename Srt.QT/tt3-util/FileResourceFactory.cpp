//
//  tt3-util/FileResourceFactory.cpp - tt3::util::FileResourceFactory class implementation
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
#include "tt3-util/API.hpp"
using namespace tt3::util;

//////////
//  Construction/destruction
FileResourceFactory::FileResourceFactory(const QString & baseFileName)
    :   _baseFileName(baseFileName)
{
}

FileResourceFactory::~FileResourceFactory()
{
}

//////////
//  IResourceFactory
QString FileResourceFactory::name() const
{
    return _baseFileName;
}

QLocale FileResourceFactory::baseLocale() const
{
    static QLocale result(QLocale::Language::English, QLocale::Country::UnitedKingdom);
    return result;
}

Locales FileResourceFactory::supportedLocales() const
{
    const_cast<FileResourceFactory*>(this)->_loadResources();
    return _supportedLocales;
}

QString FileResourceFactory::string(const ResourceSectionId & sectionId, const ResourceId & resourceId) const throws(MissingResourceException)
{
    const_cast<FileResourceFactory*>(this)->_loadResources();
    //  Do we have a mapping for the current default locale ?
    QString key = _key(sectionId, resourceId);
    if (_stringResources.contains(key))
    {   //  Use this one
        return _stringResources[key];
    }
    //  Do we have a mapping for the base locale ?
    key = _key(baseLocale(), sectionId, resourceId);
    if (_stringResources.contains(key))
    {   //  Use this one
        return _stringResources[key];
    }
    //  OOPS!
    throw MissingResourceException(_baseFileName, sectionId, resourceId);
}

//////////
//  Implementation helpers
void FileResourceFactory::_loadResources()
{
    //  Only load ONCE
    if (_loaded)
    {
        return;
    }
    _loaded = true;
    _supportedLocales.insert(baseLocale());
    //  Break base file name into prefix/suffix at
    //  the last extension separator
    qsizetype n = _baseFileName.lastIndexOf('.');
    Q_ASSERT(n >= 0 && n < _baseFileName.length());
    QString prefix = _baseFileName.left(n);
    QString suffix = _baseFileName.mid(n);
    //  Locate & load per-locale resource files
    QList<QLocale> allLocales =
        QLocale::matchingLocales(
            QLocale::AnyLanguage,
            QLocale::AnyScript,
            QLocale::AnyCountry);
    for (QLocale locale : allLocales)
    {
        _loadResourceFile(locale, prefix + "_" + locale.name() + suffix);
    }
}

void FileResourceFactory::_loadResourceFile(const QLocale & locale, const QString & fileName)
{
    QFile file(fileName);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        _supportedLocales.insert(locale);
        QTextStream stream(&file);
        QString sectionName = "";
        while (!stream.atEnd())
        {
            QString line = stream.readLine().trimmed();
            //  Ignore empty lines and comments
            if (line.isEmpty() ||
                line[0] == ';' || line[0] == '#' ||
                line.startsWith("--"))
            {
                continue;
            }
            //  New section ?
            if (line.startsWith("[") && line.endsWith("]"))
            {
                sectionName = line.mid(1, line.length() - 2).trimmed();
                continue;
            }
            //  <name>=<value>
            qsizetype eqIndex = line.indexOf('=');
            if (eqIndex == -1)
            {
                continue;
            }
            QString resourceName = line.left(eqIndex).trimmed();
            QString resourceValue = line.mid(eqIndex + 1).trimmed();
            QString key = _key(locale, ResourceSectionId(sectionName), ResourceId(resourceName));
            _stringResources[key] = resourceValue;
        }
    }
}

QString FileResourceFactory::_key(const QLocale & locale, const ResourceSectionId & sectionId, const ResourceId & resourceId)
{
    return locale.name() +
           "[" +
           sectionId.toString() +
           "]" +
           resourceId.toString();
}

QString FileResourceFactory::_key(const ResourceSectionId & sectionId, const ResourceId & resourceId)
{
    return _key(QLocale(), sectionId, resourceId);
}

//  End of tt3-util/FileResourceFactory.cpp
