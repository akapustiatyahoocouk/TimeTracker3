//
//  tt3-report/ReportLink.cpp - tt3::report::ReportLink class implementation
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
ReportLink::ReportLink(
        ReportSpanElement * spanElement,
        ILinkStyle * style
    ) : ReportElement(spanElement->_report),
        _spanElement(spanElement),
        _style(style)
{
    Q_ASSERT(_style == nullptr ||
             _style->reportTemplate() == _report->reportTemplate());

    //  Add to parents
    _report->_links.append(this);
    Q_ASSERT(_spanElement->_link == nullptr);
    _spanElement->_link = this;
}

ReportLink::~ReportLink()
{
    //  Remove from parents
    Q_ASSERT(_report->_links.contains(this));
    _report->_links.removeAll(this);
    Q_ASSERT(_spanElement->_link == this);
    _spanElement->_link = nullptr;
}

//////////
//  Operations
void ReportLink::setStyle(ILinkStyle * style)
{
    Q_ASSERT(style == nullptr ||
             style->reportTemplate() == _report->reportTemplate());

    _style = style;
}

//  End of tt3-report/ReportLink.cpp
