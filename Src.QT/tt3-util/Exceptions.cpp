//
//  tt3-util/Exceptions.cpp - tt3::util exeptions
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
//  MissingResourceException
MissingResourceException::MissingResourceException(
        const QString & resourceFactoryName,
        const ResourceSectionId & sectionId, const ResourceId & resourceId
    )
    :   _resourceFactoryName(resourceFactoryName),
        _sectionId(sectionId),
        _resourceId(resourceId)
{
}

QString MissingResourceException::errorMessage() const
{
    static Component::Resources *const resources = Component::Resources::instance();   //  idempotent
    return resources->string(RSID(Errors), RID(MissingResourceException),
                             _resourceFactoryName, _sectionId, _resourceId);
}

//////////
//  ParseException
ParseException::ParseException(
        const QString & string,
        qsizetype position
    ) : _string(string),
        _position(position)
{
}

QString ParseException::errorMessage() const
{
    static Component::Resources *const resources = Component::Resources::instance();   //  idempotent
    QString s = (_string.length() < 129) ? _string : _string.left(128) + "...";
    return resources->string(RSID(Errors), RID(ParseException),
                             _string, _position);
}

//////////
//  NotImplementedError
NotImplementedError::NotImplementedError()
{
#ifndef Q_NODEBUG
    #if defined(Q_OS_WINDOWS)
        __debugbreak();
    #elif defined(Q_OS_LINUX) || defined(Q_OS_UNIX)
        raise(SIGTRAP)
    #else
        #error Unsupported host OS
    #endif
#endif
}

QString NotImplementedError::errorMessage() const
{
    static Component::Resources *const resources = Component::Resources::instance();   //  idempotent
    return resources->string(RSID(Errors), RID(NotImplementedError));
}

//  End of tt3-util/Exceptions.cpp
