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
#ifdef QT_DEBUG
    _report->_validate();
#endif
    Q_ASSERT(style == nullptr ||
             style->reportTemplate() == _report->reportTemplate());

    auto result = new ReportParagraph(this, style);
#ifdef QT_DEBUG
    _report->_validate();
#endif
    return result;
}

ReportTable * ReportFlowElement::createTable(ITableStyle * style)
{
#ifdef QT_DEBUG
    _report->_validate();
#endif
    Q_ASSERT(style == nullptr ||
             style->reportTemplate() == _report->reportTemplate());

    auto result = new ReportTable(this, style);
#ifdef QT_DEBUG
    _report->_validate();
#endif
    return result;
}

ReportList * ReportFlowElement::createList(IListStyle * style)
{
#ifdef QT_DEBUG
    _report->_validate();
#endif
    Q_ASSERT(style == nullptr ||
             style->reportTemplate() == _report->reportTemplate());

    auto result = new ReportList(this, style);
#ifdef QT_DEBUG
    _report->_validate();
#endif
    return result;
}

auto ReportFlowElement::createTableOfContent() -> ReportTableOfContent *
{
#ifdef QT_DEBUG
    _report->_validate();
#endif
    auto result = new ReportTableOfContent(this);
#ifdef QT_DEBUG
    _report->_validate();
#endif
    return result;
}

//  End of tt3-report/ReportFlowElement.cpp
