//
//  tt3-report/HtmlReportFormat._HtmlGenerator.cpp - tt3::report::HtmlReportFormat::_HtmlGenerator class implementation
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
using HRG = HtmlReportFormat;

//////////
//  Construction/destruction
HRG::_HtmlGenerator::_HtmlGenerator()
{
}

HRG::_HtmlGenerator::~_HtmlGenerator()
{
}

//////////
//  Operations
QString HRG::_HtmlGenerator::generateHtml(Report * report)
{
    Q_ASSERT(report != nullptr);

    //  The number of "save" steps will be:
    //  *   1 for every "Paragraph" in the "report"
    _totalSteps = _countParagraps(report);
    _completedSteps = 0;
    //  TODO _progressListener.onProgressReached(0.0);

    //  We'll be embedding the CSS into the HTML - we need a "token" that will be
    //  inserted into the proper place of HTML document that we'll later replace
    //  with the CSS (because the complete CSS is only available after HTML document
    //  generation is completed)
    QString cssToken =
        QUuid::createUuid().toString() +
        "-" +
        QUuid::createUuid().toString() +
        "-" +
        QUuid::createUuid().toString();

    _htmlBuilder.reset();
    _cssBuilder.reset();

    _nextUnusedId = 1;
    _mapElementsToIds.clear();
    _assignIdsToElements(report);

    _htmlBuilder.openTag("html");

    _htmlBuilder.openTag("head");
    _htmlBuilder.writeTag("meta", "charset", "UTF-8");
    _htmlBuilder.openTag("title");
    _htmlBuilder.writeText(report->name());
    _htmlBuilder.closeTag("title");
    _htmlBuilder.openTag("style");
    _htmlBuilder.writeText(cssToken);
    _htmlBuilder.closeTag("style");
    _htmlBuilder.closeTag("head");

    _htmlBuilder.openTag("body", "class", _cssBuilder.bodyStyle(report->reportTemplate()));
    _htmlBuilder.openTag("div", "class", _cssBuilder.bodyStyle(report->reportTemplate()));
    for (auto section : report->sections())
    {
        _generateFlowElement(section);
        _htmlBuilder.writeTag("hr");
    }
    _htmlBuilder.closeTag("div");
    _htmlBuilder.closeTag("body");

    _htmlBuilder.closeTag("html");

    //  Done
    QString html = _htmlBuilder.html();
    html = html.replace(cssToken, _cssBuilder.css());
    //  TODO _progressListener.onProgressReached(1.0);
    return html;
}

//  End of tt3-report/HtmlReportFormat._HtmlGenerator.cpp
