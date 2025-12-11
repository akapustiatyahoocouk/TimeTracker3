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

//////////
//  Implementation helpers
void HRG::_HtmlGenerator::_completeStep()
{
    _completedSteps++;
    /*  TODO uncomment
    if (_progressListener != null)
    {
        _progressListener.onProgressReached((float)_completedSteps / (float)_totalSteps);
        if (_totalSteps > 0 && _completedSteps >= 0 && _completedSteps <= _totalSteps)
        {
            try
            {
                Thread.sleep(5000 / _totalSteps);
            }
            catch (@SuppressWarnings("unused") InterruptedException ex)
            {   //  Ignore wake-ups
            }
        }
    }
    */
}

int HRG::_HtmlGenerator::_countParagraps(
        Report * report
    )
{
    int result = 0;
    for (auto section : report->sections())
    {
        result += _countParagraps(section);
    }
    return result;
}

int HRG::_HtmlGenerator::_countParagraps(
        ReportFlowElement * flowElement
    )
{
    int result = 0;
    for (auto blockElement :flowElement->children())
    {
        result += _countParagraps(blockElement);
    }
    return result;
}

int HRG::_HtmlGenerator::_countParagraps(
        ReportBlockElement * blockElement
    )
{
    if (dynamic_cast<ReportParagraph*>(blockElement))
    {
        return 1;
    }
    else if (auto list =
             dynamic_cast<ReportList*>(blockElement))
    {
        return _countParagraps(list);
    }
    else if (auto table =
             dynamic_cast<ReportTable*>(blockElement))
    {
        return _countParagraps(table);
    }
    else if (dynamic_cast<ReportTableOfContent*>(blockElement))
    {
        return 1;
    }
    else
    {   //  OOPS! Can't be!
        Q_ASSERT(false);
        return 0;
    }
}

int HRG::_HtmlGenerator::_countParagraps(
        ReportList * list
    )
{
    int result = 0;
    for (auto item : list->items())
    {
        result += _countParagraps(item);
    }
    return result;
}

int HRG::_HtmlGenerator::_countParagraps(
        ReportTable * table
    )
{
    int result = 0;
    for (auto cell : table->cells())
    {
        result += _countParagraps(cell);
    }
    return result;
}

void HRG::_HtmlGenerator::_assignIdsToElements(
        Report * report
    )
{
    for (auto section : report->sections())
    {
        _assignIdsToElements(section);
    }
}

void HRG::_HtmlGenerator::_assignIdsToElements(
        ReportFlowElement * flowElement
    )
{
    QString id = "Flow" + tt3::util::toString(_nextUnusedId++);
    _mapElementsToIds[flowElement] = id;

    for (auto blockElement :flowElement->children())
    {
        _assignIdsToElements(blockElement);
    }
}

void HRG::_HtmlGenerator::_assignIdsToElements(
        ReportBlockElement * blockElement
    )
{
    QString id = "Block" + tt3::util::toString(_nextUnusedId++);
    _mapElementsToIds[blockElement] = id;

    if (auto paragraph =
        dynamic_cast<ReportParagraph*>(blockElement))
    {
        for (auto spanElement : paragraph->children())
        {
            id = "Span" + tt3::util::toString(_nextUnusedId++);
            _mapElementsToIds[spanElement] = id;
        }
    }
    else if (auto list =
             dynamic_cast<ReportList*>(blockElement))
    {
        for (auto item : list->items())
        {
            _assignIdsToElements(item);
        }
    }
    else if (auto table =
             dynamic_cast<ReportTable*>(blockElement))
    {
        for (auto cell : table->cells())
        {
            _assignIdsToElements(cell);
        }
    }
    else if (dynamic_cast<ReportTableOfContent*>(blockElement))
    {   //  Ignore
    }
    else
    {   //  OOPS! Can't be!
        Q_ASSERT(false);
    }
}

void HRG::_HtmlGenerator::_generateFlowElement(
        ReportFlowElement * flowElement
    )
{
    for (auto blockElement : flowElement->children())
    {
        if (auto paragraph =
            dynamic_cast<ReportParagraph*>(blockElement))
        {
            _generateParagraph(paragraph);
        }
        else if (auto table =
                 dynamic_cast<ReportTable*>(blockElement))
        {
            _generateTable(table);
        }
        else if (auto list =
                 dynamic_cast<ReportList*>(blockElement))
        {
            _generateList(list);
        }
        else if (auto toc =
                 dynamic_cast<ReportTableOfContent*>(blockElement))
        {
            _generateTableOfContent(toc);
        }
        else
        {   //  OOPS! Should never happen!
            Q_ASSERT(false);
        }
    }
}

void HRG::_HtmlGenerator::_generateParagraph(
        ReportParagraph * paragraph
    )
{
    _htmlBuilder.openTag(
        "p",
        "class", _cssBuilder.paragraphStyle(paragraph),
        "id", _mapElementsToIds[paragraph]);

    if (!paragraph->children().isEmpty())
    {
        for (auto spanElement : paragraph->children())
        {
            auto link = spanElement->link();
            if (auto internalLink =
                dynamic_cast<ReportInternalLink*>(link))
            {
                QString linkClass = _cssBuilder.linkStyle(link);
                auto linkTargetAnchor = internalLink->anchor();
                auto anchoredElement = linkTargetAnchor->anchoredElement();
                QString targetId = _mapElementsToIds[anchoredElement];
                _htmlBuilder.openTag(
                    "a",
                    "class", linkClass,
                    "href", "#" + targetId);
            }
            else if (auto externalLink =
                     dynamic_cast<ReportExternalLink*>(link))
            {
                QString linkClass = _cssBuilder.linkStyle(link);
                _htmlBuilder.openTag(
                    "a",
                    "class", linkClass,
                    "href", externalLink->url());
            }

            if (auto text =
                dynamic_cast<ReportText*>(spanElement))
            {
                _generateText(text);
            }
            else if (auto picture =
                     dynamic_cast<ReportPicture*>(spanElement))
            {
                throw tt3::util::NotImplementedError();
            }
            else
            {   //  OOPS! Should never happen!
                Q_ASSERT(false);
            }

            if (link != nullptr)
            {
                _htmlBuilder.closeTag("a");
            }
        }
    }
    else
    {
        _htmlBuilder.writeText("\u00A0");   //  NON-BREAKABLE SPACE
    }

    _htmlBuilder.closeTag("p");

    _completeStep();
}

void HRG::_HtmlGenerator::_generateTableOfContent(
        ReportTableOfContent * tableOfContent
    )
{
    tt3::util::ResourceReader rr(Component::Resources::instance(), RSID(HtmlReportFormat));

    IParagraphStyle * heading1Style = tableOfContent->report()->reportTemplate()->findParagraphStyle(IParagraphStyle::Heading1StyleName);    _htmlBuilder.openTag(
        "p",
        "class", _cssBuilder.paragraphStyle(heading1Style));
    _htmlBuilder.writeText(
        rr.string(RID(TableOfContent)));
    _htmlBuilder.closeTag("p");

    //  TOC data
    _generateTableOfContentForReport(tableOfContent->report());
}

void HRG::_HtmlGenerator::_generateTableOfContentForReport(
        Report * report
    )
{
    for (auto section : report->sections())
    {
        _generateTableOfContentForSection(section);
    }
}

void HRG::_HtmlGenerator::_generateTableOfContentForSection(
        ReportSection * section
    )
{
    for (auto blockElement : section->children())
    {
        if (auto paragraph =
            dynamic_cast<ReportParagraph*>(blockElement))
        {
            _generateTableOfContentForParagraph(paragraph);
        }
        else if (dynamic_cast<ReportTable*>(blockElement))
        {   //  Ignore
        }
        else if (dynamic_cast<ReportList*>(blockElement))
        {   //  Ignore
        }
        else if (dynamic_cast<ReportTableOfContent*>(blockElement))
        {   //  Ignore
        }
        else
        {   //  OOPS! Should never happen!
            Q_ASSERT(false);
        }
    }
}

void HRG::_HtmlGenerator::_generateTableOfContentForParagraph(
        ReportParagraph * paragraph
    )
{
    if (paragraph->style() == nullptr)
    {   //  Not a heading
        return;
    }
    //  Is this a heading paragraph ?
    //  If yes, locate the corresponding "TOC" style
    IParagraphStyle * tocStyle;
    tt3::util::Mnemonic paragraphStyleName = paragraph->style()->name();
    if (paragraphStyleName == IParagraphStyle::Heading1StyleName)
    {
        tocStyle = paragraph->report()->reportTemplate()->findParagraphStyle(IParagraphStyle::Toc1StyleName);
    }
    else if (paragraphStyleName == IParagraphStyle::Heading2StyleName)
    {
        tocStyle = paragraph->report()->reportTemplate()->findParagraphStyle(IParagraphStyle::Toc2StyleName);
    }
    else if (paragraphStyleName == IParagraphStyle::Heading3StyleName)
    {
        tocStyle = paragraph->report()->reportTemplate()->findParagraphStyle(IParagraphStyle::Toc3StyleName);
    }
    else if (paragraphStyleName == IParagraphStyle::Heading4StyleName)
    {
        tocStyle = paragraph->report()->reportTemplate()->findParagraphStyle(IParagraphStyle::Toc4StyleName);
    }
    else if (paragraphStyleName == IParagraphStyle::Heading5StyleName)
    {
        tocStyle = paragraph->report()->reportTemplate()->findParagraphStyle(IParagraphStyle::Toc5StyleName);
    }
    else if (paragraphStyleName == IParagraphStyle::Heading6StyleName)
    {
        tocStyle = paragraph->report()->reportTemplate()->findParagraphStyle(IParagraphStyle::Toc6StyleName);
    }
    else if (paragraphStyleName == IParagraphStyle::Heading7StyleName)
    {
        tocStyle = paragraph->report()->reportTemplate()->findParagraphStyle(IParagraphStyle::Toc7StyleName);
    }
    else if (paragraphStyleName == IParagraphStyle::Heading8StyleName)
    {
        tocStyle = paragraph->report()->reportTemplate()->findParagraphStyle(IParagraphStyle::Toc8StyleName);
    }
    else if (paragraphStyleName == IParagraphStyle::Heading9StyleName)
    {
        tocStyle = paragraph->report()->reportTemplate()->findParagraphStyle(IParagraphStyle::Toc9StyleName);
    }
    else
    {
        return;
    }

    //  Generate ToC paragraph
    _htmlBuilder.openTag(
        "p",
        "class", _cssBuilder.paragraphStyle(tocStyle));
    for (auto spanElement : paragraph->children())
    {
        QString linkClass = _cssBuilder.linkStyle(paragraph->report()->reportTemplate()->findLinkStyle(ILinkStyle::DefaultStyleName));
        _htmlBuilder.openTag("a",
                             "class", linkClass,
                             "href", "#" + _mapElementsToIds[paragraph]);
        if (auto text =
            dynamic_cast<ReportText*>(spanElement))
        {
            _htmlBuilder.writeText(text->text());
        }
        else if (auto picture =
                 dynamic_cast<ReportPicture*>(spanElement))

        {
            throw tt3::util::NotImplementedError();
        }
        else
        {   //  OOPS! Should never happen!
            Q_ASSERT(false);
        }
        _htmlBuilder.closeTag("a");
    }

    _htmlBuilder.closeTag("p");
}

void HRG::_HtmlGenerator::_generateText(
        ReportText * text
    )
{
    if (text->style() != nullptr)
    {
        Q_ASSERT(false);
        /*  TODO uncomment & implement
        _htmlBuilder.openTag(
            "span",
            "id", _mapElementsToIds[text]);
            "class", _cssBuilder.characterStyle(text->style()));
        */
    }
    else
    {
        _htmlBuilder.openTag(
            "span",
            "id", _mapElementsToIds[text]);
    }
    _htmlBuilder.writeText(text->text());
    _htmlBuilder.closeTag("span");
}

void HRG::_HtmlGenerator::_generateTable(
        ReportTable * table
    )
{
    _htmlBuilder.openTag(
        "table",
        "class", _cssBuilder.tableStyle(table),
        "width", "100%");

    //  TODO add support for table cells spanning more than 1 column.row
    for (int row = 0; row < table->rowCount(); row++)
    {
        _htmlBuilder.openTag("tr");
        for (int column = 0; column < table->columnCount(); column++)
        {
            ReportTableCell * cell = table->findTableCellAt(column, row);
            if (cell != nullptr)
            {   //  Cell found...
                if (cell->startColumn() == column && cell->startRow() == row)
                {   //  ...which starts here - generate HTML
                    _htmlBuilder.openTag(
                        "td",
                        "class", _cssBuilder.tableCellStyle(cell),
                        "id", _mapElementsToIds[cell],
                        "colspan", tt3::util::toString(cell->columnSpan()),
                        "rowspan", tt3::util::toString(cell->rowSpan()));
                    _generateFlowElement(cell);
                    _htmlBuilder.closeTag("td");
                }
            }
            else
            {   //  No cell here
                _htmlBuilder.writeTag("td");
            }
        }
        _htmlBuilder.closeTag("tr");
    }

    _htmlBuilder.closeTag("table");
}

void HRG::_HtmlGenerator::_generateList(
        ReportList * list
    )
{   //  We generate lists as borderless HTML tables containing 2 columns:
    //  *   1st column is the "label" of list items (e.g. bullets, numbers, etc.)
    //  *   2nd column is the contents of the list items

    //  We need:
    //  *   a CSS <table> style that has proper fonts, colors, no border, etc.
    //  *   two <td> styles - one for 1st column (fixed width equal to the list's
    //      "indent") and another for 2nd column (no explicit width)
    //  In the CSS all styles will have the same base name but different suffixes:
    //  *   "-table" for <table>
    //  *   "-td1" for <td> in column 1
    //  *   "-td2" for <td> in column 2
    QString listStyleClass = _cssBuilder.listStyle(list);
    _htmlBuilder.openTag(
        "table",
        "class", listStyleClass + "-table");

    for (auto listItem : list->items())
    {
        _htmlBuilder.openTag("tr");

        _htmlBuilder.openTag(
            "td",
            "class", listStyleClass + "-td1");
        QString label = listItem->label().trimmed();
        if (label.isEmpty() || label == "*")
        {   //  Use HTML bullet
            label = "\u2022";   //  Unicode bullet
        }
        _htmlBuilder.writeText(label);
        _htmlBuilder.closeTag("td");

        _htmlBuilder.openTag(
            "td",
            "class", listStyleClass + "-td2");
        _generateFlowElement(listItem);
        _htmlBuilder.closeTag("td");

        _htmlBuilder.closeTag("tr");
    }

    _htmlBuilder.closeTag("table");
}

//  End of tt3-report/HtmlReportFormat._HtmlGenerator.cpp
