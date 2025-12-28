//
//  tt3-tools-restore/Exceptions.cpp - tt3 restore exceptions
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
#include "tt3-tools-restore/API.hpp"
using namespace tt3::tools::restore;

//////////
//  BackupFileCorruptException
BackupFileCorruptException::BackupFileCorruptException(
        const QString & fileName
    ) : _fileName(fileName)
{
}

QString BackupFileCorruptException::errorMessage() const
{
    static Component::Resources *const resources = Component::Resources::instance();   //  idempotent
    return resources->string(
        RSID(Errors),
        RID(BackupFileCorruptException),
        _fileName);
}

//  End of tt3-tools-restore/Exceptions.cpp
