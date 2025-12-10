//
//  tt3-report/ReportTable.cpp - tt3::report::ReportTable class implementation
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
ReportTable::ReportTable(
        ReportFlowElement * parent,
        ITableStyle * style
    ) : ReportBlockElement(parent),
        _style(style)
{
    Q_ASSERT(_style == nullptr ||
             _style->reportTemplate() == _report->reportTemplate());
}

ReportTable::~ReportTable()
{
    for (auto cell : ReportTableCells(_cells))   //  shallow clone
    {
        delete cell;    //  removes ListItem from List
    }
}

//////////
//  ReportElement
auto ReportTable::resolveFontSpecs() const -> FontSpecs
{
    if (_style != nullptr)
    {
        auto styleFontSpecs = _style->fontSpecs();
        if (!styleFontSpecs.has_value())
        {   //  Inherit from parent
            Q_ASSERT(_parent != nullptr);
            return _parent->resolveFontSpecs();
        }
        else if (styleFontSpecs.value().isEmpty())
        {   //  Go directly to template
            return _report->_reportTemplate->defaultFontSpecs();
        }
        else
        {   //  Use value from style
            return styleFontSpecs.value();
        }
    }
    //  Style not specified - go to parent
    Q_ASSERT(_parent != nullptr);
    return _parent->resolveFontSpecs();
}

auto ReportTable::resolveFontSize() const -> TypographicSize
{
    if (_style != nullptr)
    {
        auto styleFontSize = _style->fontSize();
        if (!styleFontSize.has_value())
        {   //  Inherit from parent
            Q_ASSERT(_parent != nullptr);
            return _parent->resolveFontSize();
        }
        else
        {   //  Use value from style
            return styleFontSize.value();
        }
    }
    //  Style not specified - go to parent
    Q_ASSERT(_parent != nullptr);
    return _parent->resolveFontSize();
}

auto ReportTable::resolveFontStyle() const -> FontStyle
{
    if (_style != nullptr)
    {
        auto styleFontStyle = _style->fontStyle();
        if (!styleFontStyle.has_value())
        {   //  Inherit from parent
            Q_ASSERT(_parent != nullptr);
            return _parent->resolveFontStyle();
        }
        else
        {   //  Use value from style
            return styleFontStyle.value();
        }
    }
    //  Style not specified - go to parent
    Q_ASSERT(_parent != nullptr);
    return _parent->resolveFontStyle();
}

auto ReportTable::resolveTextColor() const -> ColorSpec
{
    if (_style != nullptr)
    {
        auto styleTextColor = _style->textColor();
        if (!styleTextColor.has_value())
        {   //  Inherit from parent
            Q_ASSERT(_parent != nullptr);
            return _parent->resolveTextColor();
        }
        else if (styleTextColor.value() == ColorSpec::Default)
        {   //  Go directly to report template
            return _report->_reportTemplate->defaultTextColor();
        }
        else
        {   //  Use value from style
            return styleTextColor.value();
        }
    }
    //  Style not specified - go to parent
    Q_ASSERT(_parent != nullptr);
    return _parent->resolveTextColor();
}

auto ReportTable::resolveBackgroundColor() const -> ColorSpec
{
    if (_style != nullptr)
    {
        auto styleBackgroundColor = _style->backgroundColor();
        if (!styleBackgroundColor.has_value())
        {   //  Inherit from parent
            Q_ASSERT(_parent != nullptr);
            return _parent->resolveBackgroundColor();
        }
        else if (styleBackgroundColor.value() == ColorSpec::Default)
        {   //  Go directly to report template
            return _report->_reportTemplate->defaultBackgroundColor();
        }
        else
        {   //  Use value from style
            return styleBackgroundColor.value();
        }
    }
    //  Style not specified - go to parent
    Q_ASSERT(_parent != nullptr);
    return _parent->resolveBackgroundColor();
}

//////////
//  ReportBlockElement
TypographicSize ReportTable::resolveLeftMargin() const
{
    //  Honor own style first
    if (_style != nullptr && _style->leftMargin().has_value())
    {
        return _style->leftMargin().value();
    }
    //  The rest is up to the base implementation
    return ReportBlockElement::resolveLeftMargin();
}

TypographicSize ReportTable::resolveRightMargin() const
{
    //  Honor own style first
    if (_style != nullptr && _style->rightMargin().has_value())
    {
        return _style->rightMargin().value();
    }
    //  The rest is up to the base implementation
    return ReportBlockElement::resolveRightMargin();
}

TypographicSize ReportTable::resolveGapAboven() const
{
    //  Honor own style first
    if (_style != nullptr && _style->gapAbove().has_value())
    {
        return _style->gapAbove().value();
    }
    //  The rest is up to the base implementation
    return ReportBlockElement::resolveGapAboven();
}

TypographicSize ReportTable::resolveGapBelow() const
{
    //  Honor own style first
    if (_style != nullptr && _style->gapBelow().has_value())
    {
        return _style->gapBelow().value();
    }
    //  The rest is up to the base implementation
    return ReportBlockElement::resolveGapBelow();
}

//////////
//  Operations
ReportTableCell * ReportTable::createCell(
        int startColumn,
        int startRow,
        int columnSpan,
        int rowSpan,
        VerticalAlignment contentAlignment,
        TypographicSizeOpt preferredWidth
    )
{
    Q_ASSERT(startColumn >= 0);
    Q_ASSERT(startRow >= 0);
    Q_ASSERT(columnSpan > 0);
    Q_ASSERT(rowSpan > 0);

#ifdef QT_DEBUG
    _report->_validate();
#endif
    auto result = new ReportTableCell(
        this,
        std::max(startColumn, 0),   //  be defensive
        std::max(startRow, 0),      //  be defensive
        std::max(columnSpan, 1),    //  be defensive
        std::max(rowSpan, 1),       //  be defensive
        contentAlignment,
        preferredWidth);
    //  Must update the table geometry
    _columnCount = std::max(_columnCount, result->_startColumn + result->_columnSpan);
    _rowCount = std::max(_rowCount, result->_startRow + result->_rowSpan);
#ifdef QT_DEBUG
    _report->_validate();
#endif
    return result;
}

ReportTableCell * ReportTable::findTableCellAt(int column, int row)
{
    for (auto cell : _cells)
    {
        if (row >= cell->_startRow &&
            row < cell->_startRow + cell->_rowSpan &&
            column >= cell->_startColumn &&
            column < cell->_startColumn + cell->_columnSpan)
        {
            return cell;
        }
    }
    return nullptr;
}

auto ReportTable::resolveTableBorderType() const -> BorderType
{
    if (_style != nullptr)
    {
        auto styleTableBorder = _style->tableBorderType();
        if (!styleTableBorder.has_value())
        {   //  Inherit from parent
            for (ReportElement * parent = _parent; parent != nullptr; parent = parent->parent())
            {
                if (auto parentTable =
                    dynamic_cast<ReportTable*>(parent))
                {
                    return parentTable->resolveTableBorderType();
                }
            }
            //  Give up and go to template
            return _report->_reportTemplate->defaultTableBorderType();
        }
        else if (styleTableBorder.value() == BorderType::Default)
        {   //  Go directly to report template
            return _report->_reportTemplate->defaultTableBorderType();
        }
        else
        {   //  Use value from style
            return styleTableBorder.value();
        }
    }
    //  Style not specified - go to parent
    for (ReportElement * parent = _parent; parent != nullptr; parent = parent->parent())
    {
        if (auto parentTable =
            dynamic_cast<ReportTable*>(parent))
        {
            return parentTable->resolveTableBorderType();
        }
    }
    //  Give up and go to template
    return _report->_reportTemplate->defaultTableBorderType();
}

auto ReportTable::resolveCellBorderType() const -> BorderType
{
    if (_style != nullptr)
    {
        auto styleCellBorder = _style->cellBorderType();
        if (!styleCellBorder.has_value())
        {   //  Inherit from parent
            for (ReportElement * parent = _parent; parent != nullptr; parent = parent->parent())
            {
                if (auto parentTable =
                    dynamic_cast<ReportTable*>(parent))
                {
                    return parentTable->resolveCellBorderType();
                }
            }
            //  Give up and go to template
            return _report->_reportTemplate->defaultCellBorderType();
        }
        else if (styleCellBorder.value() == BorderType::Default)
        {   //  Go directly to report template
            return _report->_reportTemplate->defaultCellBorderType();
        }
        else
        {   //  Use value from style
            return styleCellBorder.value();
        }
    }
    //  Style not specified - go to parent
    for (ReportElement * parent = _parent; parent != nullptr; parent = parent->parent())
    {
        if (auto parentTable =
            dynamic_cast<ReportTable*>(parent))
        {
            return parentTable->resolveCellBorderType();
        }
    }
    //  Give up and go to template
    return _report->_reportTemplate->defaultCellBorderType();
}

//  End of tt3-report/ReportTable.cpp
