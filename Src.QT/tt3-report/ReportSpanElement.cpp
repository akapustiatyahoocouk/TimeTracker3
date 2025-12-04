//
//  tt3-report/ReportSpanElement.cpp - tt3::report::ReportSpanElement class implementation
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
ReportSpanElement::ReportSpanElement(
        ReportParagraph * paragraph
    ) : ReportElement(paragraph->_report),
        _paragraph()
{
    //  Add to parent
    _paragraph->_children.append(this);
}

ReportSpanElement::~ReportSpanElement()
{
    delete _link;   //  "delete nullptr" is safe

    //  Remove from parent
    Q_ASSERT(_paragraph->_children.contains(this));
    _paragraph->_children.removeAll(this);
}

//////////
//  Operations
auto ReportSpanElement::createInternalLink(
        ReportAnchor * anchor,
        ILinkStyle * style
    ) -> ReportInternalLink *
{
#ifdef QT_DEBUG
    _report->_validate();
#endif
    auto result = new ReportInternalLink(this, style, anchor);
#ifdef QT_DEBUG
    _report->_validate();
#endif
    return result;
}

auto ReportSpanElement::createExternalLink(
        const QString & url,
        ILinkStyle * style
    ) -> ReportExternalLink *
{
#ifdef QT_DEBUG
    _report->_validate();
#endif
    auto result = new ReportExternalLink(this, style, url);
#ifdef QT_DEBUG
    _report->_validate();
#endif
    return result;
}

//  End of tt3-report/ReportSpanElement.cpp

