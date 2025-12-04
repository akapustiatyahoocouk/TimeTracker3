//
//  tt3-report/ReportTable.cpp - tt3::report::ReportTable class implementation
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
ReportTable::ReportTable(
        ReportFlowElement * parent,
        ITableStyle * style
    ) : ReportBlockElement(parent),
        _style(style)
{
    Q_ASSERT(_style == nullptr ||
             _style->reportTemplate() == _report->reportTemplate());
}

ReportTable::~ReportTable()
{
    for (auto cell : ReportTableCells(_cells))   //  shallow clone
    {
        delete cell;    //  removes ListItem from List
    }
}

//////////
//  Operations
ReportTableCell * ReportTable::createCell(
        int startColumn,
        int startRow,
        int columnSpan,
        int rowSpan,
        VerticalAlignment contentAlignment,
        TypographicSizeOpt preferredWidth
    )
{
    Q_ASSERT(startColumn >= 0);
    Q_ASSERT(startRow >= 0);
    Q_ASSERT(columnSpan > 0);
    Q_ASSERT(rowSpan > 0);

    return new ReportTableCell(
        this,
        std::max(startColumn, 0),   //  be defensive
        std::max(startRow, 0),      //  be defensive
        std::max(columnSpan, 1),    //  be defensive
        std::max(rowSpan, 1),       //  be defensive
        contentAlignment,
        preferredWidth);
}

//  End of tt3-report/ReportTable.cpp
