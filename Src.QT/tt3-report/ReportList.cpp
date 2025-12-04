//
//  tt3-report/ReportList.cpp - tt3::report::ReportList class implementation
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
ReportList::ReportList(
        ReportFlowElement * parent,
        IListStyle * style
    ) : ReportBlockElement(parent),
        _style(style)
{
    Q_ASSERT(_style == nullptr ||
             _style->reportTemplate() == _report->reportTemplate());
}

ReportList::~ReportList()
{
    for (auto item : ReportListItems(_items))   //  shallow clone
    {
        delete item;    //  removes ListItem from List
    }
}

//////////
//  Operations
ReportListItem * ReportList::createItem(const QString & label)
{
#ifdef QT_DEBUG
    _report->_validate();
#endif
    auto result = new ReportListItem(this, label);
#ifdef QT_DEBUG
    _report->_validate();
#endif
    return result;
}

//  End of tt3-report/ReportList.cpp
