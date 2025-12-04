//
//  tt3-report/ReportParagraph.cpp - tt3::report::ReportParagraph class implementation
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
//  Construction/destruction - from friends only
ReportParagraph::ReportParagraph(
        ReportFlowElement * parent,
        IParagraphStyle * style
    ) : ReportBlockElement(parent),
        _style(style)
{
    Q_ASSERT(_style == nullptr ||
             _style->reportTemplate() == _report->reportTemplate());
}

ReportParagraph::~ReportParagraph()
{
    for (auto child : ReportSpanElements(_children))    //  shallow clone
    {
        delete child;   //  removes SpanElement from Paragraph
    }
}

//////////
//  Operations
void ReportParagraph::setStyle(IParagraphStyle * style)
{
    Q_ASSERT(style == nullptr ||
             style->reportTemplate() == _report->reportTemplate());

    _style = style;
}

ReportText * ReportParagraph::createText(
        const QString & text,
        ICharacterStyle * style
    )
{
#ifdef QT_DEBUG
    _report->_validate();
#endif
    Q_ASSERT(style == nullptr ||
             style->reportTemplate() == _report->reportTemplate());

    auto result = new ReportText(this, text, style);
#ifdef QT_DEBUG
    _report->_validate();
#endif
    return result;
}

ReportPicture * ReportParagraph::createPicture(
        const TypographicSize & width,
        const TypographicSize & height,
        const QImage & image
    )
{
#ifdef QT_DEBUG
    _report->_validate();
#endif
    auto result = new ReportPicture(this, width, height, image);
#ifdef QT_DEBUG
    _report->_validate();
#endif
    return result;
}

//  End of tt3-report/ReportParagraph.cpp

