//
//  tt3-help/Exceptions.cpp - tt3-help exceptions
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
//  HelpSiteDoesNotExistException
HelpSiteDoesNotExistException::HelpSiteDoesNotExistException(
        const QString & location
    ) : _location(location)
{
}

QString HelpSiteDoesNotExistException::errorMessage() const
{
    static Component::Resources *const resources = Component::Resources::instance();   //  idempotent
    return resources->string(
        RSID(Errors),
        RID(HelpSiteDoesNotExistException),
        _location);
}

//////////
//  CannotCreateDirectoryException
CannotCreateDirectoryException::CannotCreateDirectoryException(
        const QString & path
    ) : _path(path)
{
}

QString CannotCreateDirectoryException::errorMessage() const
{
    static Component::Resources *const resources = Component::Resources::instance();   //  idempotent
    return resources->string(
        RSID(Errors),
        RID(CannotCreateDirectoryException),
        _path);
}

//////////
//  FileAlreadyExistsException
FileAlreadyExistsException::FileAlreadyExistsException(
        const QString & path
    ) : _path(path)
{
}

QString FileAlreadyExistsException::errorMessage() const
{
    static Component::Resources *const resources = Component::Resources::instance();   //  idempotent
    return resources->string(
        RSID(Errors),
        RID(FileAlreadyExistsException),
        _path);
}

//////////
//  UnsupportedProtocolException
UnsupportedProtocolException::UnsupportedProtocolException(
        const QString & url
    ) : _url(url)
{
}

QString UnsupportedProtocolException::errorMessage() const
{
    static Component::Resources *const resources = Component::Resources::instance();   //  idempotent
    return resources->string(
        RSID(Errors),
        RID(UnsupportedProtocolException),
        _url);
}

//  End of tt3-help/Exceptions.cpp
