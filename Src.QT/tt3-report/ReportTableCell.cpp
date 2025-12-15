//
//  tt3-report/ReportTableCell.cpp - tt3::report::ReportTableCell class implementation
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
ReportTableCell::ReportTableCell(
        ReportTable * table,
        qsizetype startColumn,
        qsizetype startRow,
        qsizetype columnSpan,
        qsizetype rowSpan,
        const TypographicSizeOpt & preferredWidth,
        ITableCellStyle * style
    ) : ReportFlowElement(table->_report),
        _table(table),
        _startColumn(std::max<qsizetype>(startColumn, 0)), //  be defensive
        _startRow(std::max<qsizetype>(startRow, 0)),       //  be defensive
        _columnSpan(std::max<qsizetype>(columnSpan, 1)),   //  be defensive
        _rowSpan(std::max<qsizetype>(rowSpan, 1)),         //  be defensive
        _preferredWidth(preferredWidth),
        _style(style)
{
    Q_ASSERT(startColumn >= 0);
    Q_ASSERT(startRow >= 0);
    Q_ASSERT(columnSpan > 0);
    Q_ASSERT(rowSpan > 0);

    //  Add to parents
    _table->_cells.append(this);
}

ReportTableCell::~ReportTableCell()
{
    //  Remove from parents
    Q_ASSERT(_table->_cells.contains(this));
    _table->_cells.removeAll(this);
}

//////////
//  ReportElement
auto ReportTableCell::resolveFontSpecs() const -> FontSpecs
{   //  No own style - must go to the parent
    Q_ASSERT(_table != nullptr);
    return _table->resolveFontSpecs();
}

auto ReportTableCell::resolveFontSize() const -> TypographicSize
{   //  No own style - must go to the parent
    Q_ASSERT(_table != nullptr);
    return _table->resolveFontSize();
}

auto ReportTableCell::resolveFontStyle() const -> FontStyle
{   //  No own style - must go to the parent
    Q_ASSERT(_table != nullptr);
    return _table->resolveFontStyle();
}

auto ReportTableCell::resolveTextColor() const -> ColorSpec
{   //  No own style - must go to the parent
    Q_ASSERT(_table != nullptr);
    return _table->resolveTextColor();
}

auto ReportTableCell::resolveBackgroundColor() const -> ColorSpec
{   //  No own style - must go to the parent
    Q_ASSERT(_table != nullptr);
    return _table->resolveBackgroundColor();
}

//////////
//  Operations
void ReportTableCell::setStyle(ITableCellStyle * style)
{
    Q_ASSERT(style == nullptr ||
             style->reportTemplate() == _report->reportTemplate());

    if (style == nullptr ||
        style->reportTemplate() == _report->reportTemplate())
    {   //  Be defensive in Release mode
        _style = style;
    }
}

auto ReportTableCell::resolveCellBorderType() const -> BorderType
{   //  No own style - must go to the parent
    Q_ASSERT(_table != nullptr);
    return _table->resolveCellBorderType();
}

auto ReportTableCell::resolveHorizontalAlignment() const -> HorizontalAlignment
{
    //  Honor own style first
    if (_style != nullptr && _style->horizontalAlignment().has_value())
    {
        return _style->horizontalAlignment().value();
    }
    //  Must go to the parent TableCell (usually there will be none)
    for (const ReportElement * parent = this->parent();
         parent != nullptr;
         parent = parent->parent())
    {
        if (auto parentCell =
            dynamic_cast<const ReportTableCell*>(parent))
        {
            return parentCell->resolveHorizontalAlignment();
        }
    }
    //  No ancestor cell was any good
    return HorizontalAlignment::Default;
}

auto ReportTableCell::resolveVerticalAlignment() const -> VerticalAlignment
{
    //  Honor own style first
    if (_style != nullptr && _style->verticalAlignment().has_value())
    {
        return _style->verticalAlignment().value();
    }
    //  Must go to the parent TableCell (usually there will be none)
    for (const ReportElement * parent = this->parent();
         parent != nullptr;
         parent = parent->parent())
    {
        if (auto parentCell =
            dynamic_cast<const ReportTableCell*>(parent))
        {
            return parentCell->resolveVerticalAlignment();
        }
    }
    //  No ancestor cell was any good
    return VerticalAlignment::Default;
}

//////////
//  Serialization
void ReportTableCell::serialize(QDomElement & element) const
{
    ReportFlowElement::serialize(element);

    if (_style != nullptr)
    {
        element.setAttribute("Style", _style->name().toString());
    }
    element.setAttribute("StartColumn", _startColumn);
    element.setAttribute("StartRow", _startRow);
    element.setAttribute("ColumnSpan", _columnSpan);
    element.setAttribute("RowSpan", _rowSpan);
    if (_preferredWidth.has_value())
    {
        element.setAttribute("PreferredWidth", tt3::util::toString(_preferredWidth.value()));
    }
}

void ReportTableCell::deserialize(const QDomElement & element)
{
    ReportFlowElement::deserialize(element);

    if (element.hasAttribute("Style"))
    {
        _style =
            _report->_reportTemplate->tableCellStyle(
                IStyle::Name(element.attribute("Style")));
    }
    _startColumn =
        tt3::util::fromString(
            element.attribute("StartColumn"),
            _startColumn);
    _startRow =
        tt3::util::fromString(
            element.attribute("StartRow"),
            _startRow);
    _columnSpan =
        tt3::util::fromString(
            element.attribute("ColumnSpan"),
            _columnSpan);
    _rowSpan =
        tt3::util::fromString(
            element.attribute("RowSpan"),
            _rowSpan);
    if (element.hasAttribute("PreferredWidth"))
    {
        _preferredWidth =
            tt3::util::fromString(
                element.attribute("PreferredWidth"),
                _preferredWidth.value());
    }
}

//  End of tt3-report/ReportTableCell.cpp
