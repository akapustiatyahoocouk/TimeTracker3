//
//  tt3-report/Report.cpp - tt3::report::Report class implementation
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
Report::Report(
        const QString & name,
        IReportTemplate * reportTemplate
    ) : _name(name),
        _reportTemplate(reportTemplate)
{
    Q_ASSERT(_reportTemplate != nullptr);

    _reportTemplate->_referenceCount++;
}

Report::~Report()
{
    for (auto link : ReportLinks(_links))  //  shallow clone
    {
        delete link; //  removes Links from Report ans SpanElements
    }
    for (auto anchor : ReportAnchors(_anchors))  //  shallow clone
    {
        delete anchor; //  removes Anchors from Report ans Elements
    }
    for (auto section : ReportSections(_sections))  //  shallow clone
    {
        delete section; //  removes Sections from Report
    }
    _reportTemplate->_referenceCount--;
}

//////////
//  Operations
ReportSection * Report::createSection(
        const QString & name,
        ISectionStyle * style
    )
{
    return new ReportSection(this, name, style);
}

//  End of tt3-report/Report.cpp
