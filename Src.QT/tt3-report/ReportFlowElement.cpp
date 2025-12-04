//
//  tt3-report/ReportFlowElement.cpp - tt3::report::ReportFlowElement class implementation
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
ReportFlowElement::ReportFlowElement(
        Report * report
    ) : ReportElement(report)
{
}

ReportFlowElement::~ReportFlowElement()
{
    for (auto child : ReportBlockElements(_children))   //  shallow clone
    {
        delete child;   //  removes BlockElement from this FlowElement
    }
}

//////////
//  Operations
ReportParagraph * ReportFlowElement::createParagraph(IParagraphStyle * style)
{
    Q_ASSERT(style == nullptr ||
             style->reportTemplate() == _report->reportTemplate());

    return new ReportParagraph(this, style);
}

ReportTable * ReportFlowElement::createTable(ITableStyle * style)
{
    Q_ASSERT(style == nullptr ||
             style->reportTemplate() == _report->reportTemplate());

    return new ReportTable(this, style);
}

ReportList * ReportFlowElement::createList(IListStyle * style)
{
    Q_ASSERT(style == nullptr ||
             style->reportTemplate() == _report->reportTemplate());

    return new ReportList(this, style);
}

auto ReportFlowElement::createTableOfContent() -> ReportTableOfContent *
{
    return new ReportTableOfContent(this);
}

//  End of tt3-report/ReportFlowElement.cpp
