//
//  tt3-db-sqlite3/ResultSet.cpp - tt3::db::sqlite3::ResultSet class implementation
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
#include "tt3-db-sqlite3/API.hpp"
using namespace tt3::db::sqlite3;

//////////
//  tt3::db::sql::ResultSet
bool ResultSet::next()
{
    if (_rows.size() == 0)
    {   //  Can't advance
        return false;
    }
    else if (_currentRow >= _rows.size())
    {   //  Already at end
        return false;
    }
    else
    {   //  Advance & check
        _currentRow++;
        return _currentRow < _rows.size();
    }
}

bool ResultSet::isNull(int columnIndex) const
{
    if (_currentRow < 0 || _currentRow >= _rows.size())
    {   //  OOPS! Be defensive in release mode
        Q_ASSERT(false);
        return false;
    }
    if (columnIndex < 0 || columnIndex >= _columns.size())
    {   //  OOPS! Be defensive in release mode
        Q_ASSERT(false);
        return false;
    }
    const auto & cell = _rows[_currentRow][columnIndex];
    return std::get<1>(cell);
}

bool ResultSet::isNull(const QString & columnName) const
{
    return isNull(_columnIndex(columnName));
}

bool ResultSet::boolValue(int columnIndex, bool defaultValue) const
{
    if (_currentRow < 0 || _currentRow >= _rows.size())
    {   //  OOPS! Be defensive in release mode
        Q_ASSERT(false);
        return defaultValue;
    }
    if (columnIndex < 0 || columnIndex >= _columns.size())
    {   //  OOPS! Be defensive in release mode
        Q_ASSERT(false);
        return defaultValue;
    }
    const auto & cell = _rows[_currentRow][columnIndex];
    if (std::get<1>(cell))
    {   //  NULL
        return defaultValue;
    }
    else if (std::get<0>(cell) == "Y")
    {
        return true;
    }
    else if (std::get<0>(cell) == "N")
    {
        return false;
    }
    else
    {
        return defaultValue;
    }
}

bool ResultSet::boolValue(const QString & columnName, bool defaultValue) const
{
    return boolValue(_columnIndex(columnName), defaultValue);
}

qint64 ResultSet::intValue(int columnIndex, qint64 defaultValue) const
{
    if (_currentRow < 0 || _currentRow >= _rows.size())
    {   //  OOPS! Be defensive in release mode
        Q_ASSERT(false);
        return defaultValue;
    }
    if (columnIndex < 0 || columnIndex >= _columns.size())
    {   //  OOPS! Be defensive in release mode
        Q_ASSERT(false);
        return defaultValue;
    }
    const auto & cell = _rows[_currentRow][columnIndex];
    if (std::get<1>(cell))
    {   //  NULL
        return defaultValue;
    }
    else
    {   //  Go via quint64 - otherwise fromString call will be ambiguous
        return tt3::util::fromString<quint64>(std::get<0>(cell), defaultValue);
    }
}

qint64 ResultSet::intValue(const QString & columnName, qint64 defaultValue) const
{
    return intValue(_columnIndex(columnName), defaultValue);
}

QString ResultSet::stringValue(int columnIndex, const QString & defaultValue) const
{
    if (_currentRow < 0 || _currentRow >= _rows.size())
    {   //  OOPS! Be defensive in release mode
        Q_ASSERT(false);
        return defaultValue;
    }
    if (columnIndex < 0 || columnIndex >= _columns.size())
    {   //  OOPS! Be defensive in release mode
        Q_ASSERT(false);
        return defaultValue;
    }
    const auto & cell = _rows[_currentRow][columnIndex];
    if (std::get<1>(cell))
    {   //  NULL
        return defaultValue;
    }
    else
    {
        return std::get<0>(cell);
    }
}

QString ResultSet::stringValue(const QString & columnName, const QString & defaultValue) const
{
    return stringValue(_columnIndex(columnName), defaultValue);
}

auto ResultSet::timeSpanValue(int columnIndex, const tt3::util::TimeSpan & defaultValue) const -> tt3::util::TimeSpan
{
    if (_currentRow < 0 || _currentRow >= _rows.size())
    {   //  OOPS! Be defensive in release mode
        Q_ASSERT(false);
        return defaultValue;
    }
    if (columnIndex < 0 || columnIndex >= _columns.size())
    {   //  OOPS! Be defensive in release mode
        Q_ASSERT(false);
        return defaultValue;
    }
    const auto & cell = _rows[_currentRow][columnIndex];
    if (std::get<1>(cell))
    {   //  NULL
        return defaultValue;
    }
    else
    {
        return tt3::util::fromString(std::get<0>(cell), defaultValue);
    }
}

auto ResultSet::timeSpanValue(const QString & columnName, const tt3::util::TimeSpan & defaultValue) const -> tt3::util::TimeSpan
{
    return timeSpanValue(_columnIndex(columnName), defaultValue);
}

ResultSet::Oid ResultSet::oidValue(int columnIndex, const Oid & defaultValue) const
{
    if (_currentRow < 0 || _currentRow >= _rows.size())
    {   //  OOPS! Be defensive in release mode
        Q_ASSERT(false);
        return defaultValue;
    }
    if (columnIndex < 0 || columnIndex >= _columns.size())
    {   //  OOPS! Be defensive in release mode
        Q_ASSERT(false);
        return defaultValue;
    }
    const auto & cell = _rows[_currentRow][columnIndex];
    if (std::get<1>(cell))
    {   //  NULL
        return defaultValue;
    }
    else
    {
        return tt3::util::fromString(std::get<0>(cell), defaultValue);
    }
}

ResultSet::Oid ResultSet::oidValue(const QString & columnName, const Oid & defaultValue) const
{
    return oidValue(_columnIndex(columnName), defaultValue);
}

//////////
//  Implementation helpers
int ResultSet::_columnIndex(const QString & columnName) const
{   //  We need columns -> indexes map
    if (_columnIndices.isEmpty() && !_columns.isEmpty())
    {
        for (int i = 0; i < _columns.size(); i++)
        {
            _columnIndices[_columns[i]] = i;
        }
    }
    //  Translate the column name to index
    return _columnIndices.value(columnName.toUpper(), -1);
}

//  End of tt3-db-sqlite3/ResultSet.cpp
