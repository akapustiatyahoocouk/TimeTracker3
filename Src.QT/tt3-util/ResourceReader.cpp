//
//  tt3-util/ResourceReader.cpp - tt3::util::ResourceReader class implementation
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
ResourceReader::ResourceReader(
        IResourceFactory * resourceFactory,
        const ResourceSectionId & sectionId
    ) : _resourceFactory(resourceFactory),
        _sectionId(sectionId)
{
    Q_ASSERT(_resourceFactory != nullptr);
}

//////////
//  Operations
QString ResourceReader::string(
        const ResourceId & resourceId
    ) const
{
    return _resourceFactory->string(_sectionId, resourceId);
}

QString ResourceReader::string(
        const ResourceId & resourceId,
        const QStringList & params
    ) const
{
    return _resourceFactory->string(_sectionId, resourceId, params);
}

QString ResourceReader::string(
        const ResourceId & resourceId,
        const QString & param0
    ) const
{
    return _resourceFactory->string(_sectionId, resourceId, param0);
}

QString ResourceReader::string(
        const ResourceId & resourceId,
        const QString & param0,
        const QString & param1
    ) const
{
    return _resourceFactory->string(_sectionId, resourceId, param0, param1);
}

QString ResourceReader::string(
        const ResourceId & resourceId,
        const QString & param0,
        const QString & param1,
        const QString & param2
    ) const
{
    return _resourceFactory->string(_sectionId, resourceId, param0, param1, param2);
}

//  End of tt3-util/ResourceReader.cpp
