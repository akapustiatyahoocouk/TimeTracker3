//
//  tt3-db-sql/CountResultSet.cpp - tt3::db::sql::CountResultSet class implementation
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
#include "tt3-db-sql/API.hpp"
using namespace tt3::db::sql;

//////////
//  Construction/destruction
CountResultSet::CountResultSet(qint64 count)
    :   _count(count),
        _currentRow(-1)
{
}

CountResultSet::~CountResultSet()
{
}

//////////
//  ResultSet
qint64 CountResultSet::size() const
{
    return 1;
}

bool CountResultSet::next()
{
    if (_currentRow < 1)
    {
        _currentRow++;
    }
    return _currentRow == 0;
}

bool CountResultSet::isNull(int /*columnIndex*/) const
{
    return false;
}

bool CountResultSet::isNull(const QString & /*columnName*/) const
{
    return false;
}

bool CountResultSet::boolValue(int /*columnIndex*/, bool defaultValue) const
{
    return defaultValue;
}

bool CountResultSet::boolValue(const QString & /*columnName*/, bool defaultValue) const
{
    return defaultValue;
}

qint64 CountResultSet::intValue(int columnIndex, qint64 defaultValue) const
{
    return (_currentRow == 0 && columnIndex == 0) ? _count : defaultValue;
}

qint64 CountResultSet::intValue(const QString & columnName, qint64 defaultValue) const
{
    return (_currentRow == 0 && columnName.compare(CountColumnName, Qt::CaseInsensitive) == 0) ? _count : defaultValue;
}

QString CountResultSet::stringValue(int /*columnIndex*/, const QString & defaultValue) const
{
    return defaultValue;
}

QString CountResultSet::stringValue(const QString & /*columnName*/, const QString & defaultValue) const
{
    return defaultValue;
}

auto CountResultSet::timeSpanValue(int /*columnIndex*/, const tt3::util::TimeSpan & defaultValue) const -> tt3::util::TimeSpan
{
    return defaultValue;
}

auto CountResultSet::timeSpanValue(const QString & /*columnName*/, const tt3::util::TimeSpan & defaultValue) const -> tt3::util::TimeSpan
{
    return defaultValue;
}

tt3::db::api::Oid CountResultSet::oidValue(int /*columnIndex*/, const Oid & defaultValue) const
{
    return defaultValue;
}

tt3::db::api::Oid CountResultSet::oidValue(const QString & /*columnName*/, const Oid & defaultValue) const
{
    return defaultValue;
}

//  End of tt3-db-sql/CountResultSet.cpp
