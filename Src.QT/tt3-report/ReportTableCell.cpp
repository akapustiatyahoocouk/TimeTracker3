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
        VerticalAlignment contentAlignment,
        const TypographicSizeOpt & preferredWidth
    ) : ReportFlowElement(table->_report),
        _table(table),
        _startColumn(std::max<qsizetype>(startColumn, 0)), //  be defensive
        _startRow(std::max<qsizetype>(startRow, 0)),       //  be defensive
        _columnSpan(std::max<qsizetype>(columnSpan, 1)),   //  be defensive
        _rowSpan(std::max<qsizetype>(rowSpan, 1)),         //  be defensive
        _contentAlignment(contentAlignment),
        _preferredWidth(preferredWidth)
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
auto ReportTableCell::resolveCellBorderType() const -> BorderType
{   //  No own style - must go to the parent
    Q_ASSERT(_table != nullptr);
    return _table->resolveCellBorderType();
}

auto ReportTableCell::resolveContentAlignment() const -> VerticalAlignment
{
    return _contentAlignment;
}

//////////
//  Serialization
void ReportTableCell::serialize(QDomElement & element) const
{
    ReportFlowElement::serialize(element);

    element.setAttribute("StartColumn", _startColumn);
    element.setAttribute("StartRow", _startRow);
    element.setAttribute("ColumnSpan", _columnSpan);
    element.setAttribute("RowSpan", _rowSpan);
    element.setAttribute("ContentAlignment", tt3::util::toString(_contentAlignment));
    if (_preferredWidth.has_value())
    {
        element.setAttribute("PreferredWidth", tt3::util::toString(_preferredWidth.value()));
    }
}

void ReportTableCell::deserialize(const QDomElement & element)
{
    ReportFlowElement::deserialize(element);

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
    _contentAlignment =
        tt3::util::fromString(
            element.attribute("ContentAlignment"),
            _contentAlignment);
    if (element.hasAttribute("PreferredWidth"))
    {
        _preferredWidth =
            tt3::util::fromString(
                element.attribute("PreferredWidth"),
                _preferredWidth.value());
    }
}

//  End of tt3-report/ReportTableCell.cpp
