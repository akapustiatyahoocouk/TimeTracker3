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

QString FileResourceFactory::string(const ResourceSectionId & sectionId, const ResourceId & resourceId) const
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
    {   //  There are locales (e.g. "en-Shaw") wjose "name()"
        //  is the same as that of the proper "en_GB" locale.
        //  We don't want these duplicates!
        _loadResourceFile(QLocale(locale.name()), prefix + "_" + locale.name() + suffix);
    }
    //  If _supportedLocales does not include baseLocale()
    //  we have an error, so TODO assert
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
            try
            {
                QString resourceName = line.left(eqIndex).trimmed();
                QString resourceValue = _unescape(line.mid(eqIndex + 1).trimmed());
                QString key = _key(locale, ResourceSectionId(sectionName), ResourceId(resourceName));
                _stringResources[key] = resourceValue;
            }
            catch (const tt3::util::ParseException & ex)
            {   //  OOPS! _unescape() failed!
                qDebug() << ex.errorMessage();
                throw;  //  TODO for now...
            }
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

int FileResourceFactory::_xdigit(const QChar & c)
{
    if (c >= '0' && c <= '9')
    {
        return c.unicode() - '0';
    }
    else if (c >= 'a' && c <= 'f')
    {
        return c.unicode() - 'a' + 10;
    }
    else if (c >= 'A' && c <= 'F')
    {
        return c.unicode() - 'A' + 10;
    }
    else
    {
        return -1;
    }
}

QString FileResourceFactory::_unescape(const QString & s)
{
    QString result;
    for (int i = 0; i < s.length(); )
    {
        if (s[i] != '\\')
        {   //  A literal character
            result += s[i];
            i++;
            continue;
        }
        //  We have ab escape sequence. Skip '\'
        i++;
        if (i >= s.length())
        {   //  OOPS! Missing!
            throw tt3::util::ParseException(s, i);
        }
        //  Special character ?
        if (s[i] == 'a')
        {   //  \a
            result += '\a';
            i++;
        }
        else if (s[i] == 'b')
        {   //  \b
            result += '\b';
            i++;
        }
        else if (s[i] == 'f')
        {   //  \f
            result += '\f';
            i++;
        }
        else if (s[i] == 'n')
        {   //  \n
            result += '\n';
            i++;
        }
        else if (s[i] == 'r')
        {   //  \r
            result += '\r';
            i++;
        }
        else if (s[i] == 't')
        {   //  \t
            result += '\t';
            i++;
        }
        else if (s[i] == 'v')
        {   //  \a
            result += '\v';
            i++;
        }
        //  Numeric escape ?
        else if (s[i] == 'x')
        {   //  \xXX
            i++;
            if (i + 1 < s.length() &&
                _xdigit(s[i]) != -1 && _xdigit(s[i + 1]) != -1)
            {
                result += QChar(_xdigit(s[i]) * 16 +
                                _xdigit(s[i + 1]));
                i += 2;
            }
            else
            {   //  OOPS! Invalid hex escape!
                throw tt3::util::ParseException(s, i);
            }
        }
        else if (s[i] == 'u')
        {   //  \uXXXX
            i++;
            if (i + 3 < s.length() &&
                _xdigit(s[i]) != -1 && _xdigit(s[i + 1]) != -1 &&
                _xdigit(s[i + 2]) != -1 && _xdigit(s[i + 3]) != -1)
            {
                result += QChar(_xdigit(s[i]) * 4096 +
                                _xdigit(s[i + 1]) * 256 +
                                _xdigit(s[i + 2]) * 16 +
                                _xdigit(s[i + 3]));
                i += 4;
            }
            else
            {   //  OOPS! Invalid hex escape!
                throw tt3::util::ParseException(s, i);
            }
        }
        //  Literal escape
        else
        {
            result += s[i++];
        }
    }
    return result;
}

//  End of tt3-util/FileResourceFactory.cpp
