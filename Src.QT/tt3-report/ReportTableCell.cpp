//
//  tt3-report/ReportTableCell.cpp - tt3::report::ReportTableCell class implementation
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
#include "tt3-report/API.hpp"
using namespace tt3::report;

//////////
//  Construction/destruction
ReportTableCell::ReportTableCell(
        ReportTable * table,
        int startColumn,
        int startRow,
        int columnSpan,
        int rowSpan,
        VerticalAlignment contentAlignment,
        const TypographicSizeOpt & preferredWidth
    ) : ReportFlowElement(table->_report),
        _table(table),
        _startColumn(std::max(startColumn, 0)), //  be defensive
        _startRow(std::max(startRow, 0)),       //  be defensive
        _columnSpan(std::max(columnSpan, 1)),   //  be defensive
        _rowSpan(std::max(rowSpan, 1)),         //  be defensive
        _contentAlignment(contentAlignment),
        _preferredWidth(preferredWidth)
{
    Q_ASSERT(startColumn >= 0);
    Q_ASSERT(startRow >= 0);
    Q_ASSERT(columnSpan > 0);
    Q_ASSERT(rowSpan > 0);

    //  Add to parents
    _table->_cells.append(this);
}

ReportTableCell::~ReportTableCell()
{
    //  Remove from parents
    Q_ASSERT(_table->_cells.contains(this));
    _table->_cells.removeAll(this);
}

//  End of tt3-report/ReportTableCell.cpp
