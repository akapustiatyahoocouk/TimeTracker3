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
    if (_rowCount == 0)
    {   //  Can't advance
        return false;
    }
    else if (_currentRow >= _rowCount)
    {   //  Already at end
        return false;
    }
    else
    {   //  Advance & check
        _currentRow++;
        return _currentRow < _rowCount;
    }
}

bool ResultSet::isNull(int columnIndex) const
{
    _ensureCurrentRow();
    QString s = _rows[_currentRow][columnIndex];
    return s.compare("NULL", Qt::CaseInsensitive) == 0;
}

bool ResultSet::isNull(const QString & columnName) const
{
    _ensureCurrentRow();
    QString s = _rows[_currentRow][_columnIndex(columnName)];
    return s.compare("NULL", Qt::CaseInsensitive) == 0;
}


//////////
//  Implementation helpers
bool ResultSet::_ensureCurrentRow() const
{
    if (_currentRow < 0 || _currentRow >= _rowCount)
    {   //  OOPS! TODO throw proper exception
        Q_ASSERT(false);
    }
    if (columnIndex < 0 || columnIndex >= _columns.size())
    {   //  OOPS! TODO throw proper exception
        Q_ASSERT(false);
    }
}

int ResultSet::_columnIndex(const QString & columnName) const
{   //  We need columns -> indexes map
    if (_columnIndices.isEmpty() && !_columnIndices.isEmpty())
    {
        for (int i = 0; i < _columns.size(); i++)
        {
            _columnIndices[_columns[i]] = i;
        }
    }
    //  Reanslate the column name to index
    return _columnIndices.value(columnName.toUpper(), -1);
}

//  End of tt3-db-sqlite3/ResultSet.cpp
