//
//  tt3-help/LocalFileContentLoader.cpp - tt3::help::LocalFileContentLoader class implementation
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
//  Construction/destruction
LocalFileContentLoader::LocalFileContentLoader(const QString & path)
    :   _path(QFileInfo(path).absoluteFilePath())
{
}

LocalFileContentLoader::~LocalFileContentLoader()
{
}

//////////
//  IContentLoader
QUrl LocalFileContentLoader::contentUrl() const
{
    return QUrl::fromLocalFile(_path);
}

QByteArray LocalFileContentLoader::loadContent()
{
    QFile file(_path);
    if (!file.open(QIODevice::ReadOnly))
    {   //  OOPS!
        throw CustomHelpException(_path + ": " + file.errorString());
    }
    QByteArray content = file.readAll();
    file.close();
    if (file.error() != QFileDevice::NoError)
    {   //  OOPS! Maybe, read/close error
        throw CustomHelpException(_path + ": " + file.errorString());
    }
    return content;
}

//  End of tt3-help/LocalFileContentLoader.cpp
