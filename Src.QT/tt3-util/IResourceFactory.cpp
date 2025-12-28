//
//  tt3-util/IResourceFactory.cpp - tt3::util::IResourceFactory interface implementation
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
//  Operations
QString IResourceFactory::string(
        const ResourceSectionId & sectionId,
        const ResourceId & resourceId,
        const QStringList & params
    ) const
{
    Q_ASSERT(params.size() <= 10);  //  ...for now

    QString format = string(sectionId, resourceId);
    QString result;
    for (int i = 0; i < format.length(); )
    {
        if (format[i] == '{' &&
            i + 2 < format.length() &&
            format[i + 1] >= '0' && format[i + 1] <= '9' &&
            format[i + 2] == '}')
        {   //  Paramerter placeholder
            int n = format[i + 1].unicode() - '0';
            Q_ASSERT(n < params.size());
            if (n < params.size())
            {   //  Be defensive in release mode
                result += params[n];
            }
            i += 3;
        }
        else if (format[i] == '{' &&
                 i + 1 < format.length() && format[i + 1] == '{')
        {   //  {{ -> {
            result += '{';
            i += 2;
        }
        else if (format[i] == '}' &&
                 i + 1 < format.length() && format[i + 1] == '}')
        {   //  }} -> }
            result += '}';
            i += 2;
        }
        else
        {   //  Treat this character literally
            result += format[i++];
        }
    }
    return result;
}

QString IResourceFactory::string(
        const ResourceSectionId & sectionId,
        const ResourceId & resourceId,
        const QString & param0
    ) const
{
    return string(sectionId, resourceId,
                  QStringList{param0});
}

QString IResourceFactory::string(
        const ResourceSectionId & sectionId,
        const ResourceId & resourceId,
        const QString & param0,
        const QString & param1
    ) const
{
    return string(sectionId, resourceId,
                  QStringList{param0, param1});
}

QString IResourceFactory::string(
        const ResourceSectionId & sectionId,
        const ResourceId & resourceId,
        const QString & param0,
        const QString & param1,
        const QString & param2) const
{
    return string(sectionId, resourceId,
                  QStringList{param0, param1, param2});
}

QString IResourceFactory::string(
        const ResourceSectionId & sectionId,
        const ResourceId & resourceId,
        const QString & param0,
        const QString & param1,
        const QString & param2,
        const QString & param3
    ) const
{
    return string(sectionId, resourceId,
                  QStringList{param0, param1, param2,
                              param3});
}

QString IResourceFactory::string(
        const ResourceSectionId & sectionId,
        const ResourceId & resourceId,
        const QString & param0,
        const QString & param1,
        const QString & param2,
        const QString & param3,
        const QString & param4
    ) const
{
    return string(sectionId, resourceId,
                  QStringList{param0, param1, param2,
                              param3, param4});
}

QString IResourceFactory::string(
        const ResourceSectionId & sectionId,
        const ResourceId & resourceId,
        const QString & param0,
        const QString & param1,
        const QString & param2,
        const QString & param3,
        const QString & param4,
        const QString & param5
    ) const
{
    return string(sectionId, resourceId,
                  QStringList{param0, param1, param2,
                              param3, param4, param5});
}

//  End of tt3-util/IResourceFactory.cpp
