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
    const ResourceSectionId & sectionId, const ResourceId & resourceId)
    :   _resourceFactoryName(resourceFactoryName),
        _sectionId(sectionId),
        _resourceId(resourceId)
{
}

QString MissingResourceException::errorMessage() const
{
    return "Missing resource [" +
           _sectionId +
           "]" +
           _resourceId +
           " in " +
           _resourceFactoryName;
}

//////////
//  ParseException
ParseException::ParseException(const QString & string, int position)
    :   _string(string),
        _position(position)
{
}

QString ParseException::errorMessage() const
{
    QString s = (_string.length() < 129) ? _string : _string.left(128) + "...";
    return "Error parsing \"" +
           s +
           "\" at position " +
           toString(_position);
}

//  End of tt3-util/Exceptions.cpp
