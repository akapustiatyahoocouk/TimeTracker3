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
    QString s = _rows[_currentRow][columnIndex];
    return s.compare("NULL", Qt::CaseInsensitive) == 0;
}

bool ResultSet::isNull(const QString & columnName) const
{
    return isNull(_columnIndex(columnName));
}

qint64 ResultSet::value(int columnIndex, qint64 defaultValue) const
{
    if (_currentRow < 0 || _currentRow >= _rows.size())
    {   //  OOPS! Be defensive in release mode
        Q_ASSERT(false);
        return defaultValue;
    }
    if (columnIndex < 0 || columnIndex >= _columns.size())
    {   //  OOPS! Be defensive in release mode
        Q_ASSERT(false);
        return false;
    }
    QString s = _rows[_currentRow][columnIndex];
    return tt3::util::fromString<qint64>(s);
}

qint64 ResultSet::value(const QString & columnName, qint64 defaultValue) const
{
    return value(_columnIndex(columnName), defaultValue);
}

//////////
//  Implementation helpers
int ResultSet::_columnIndex(const QString & columnName) const
{   //  We need columns -> indexes map
    if (_columnIndices.isEmpty() && !_columnIndices.isEmpty())
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
