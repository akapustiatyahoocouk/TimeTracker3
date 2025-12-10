//
//  tt3-report/CustomTableStyle.cpp - tt3::report::CustomTableStyle class implementation
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
CustomTableStyle::CustomTableStyle(
        CustomReportTemplate * reportTemplate,
        const Mnemonic & name,
        const FontSpecsOpt & fontSpecs,
        const TypographicSizeOpt & fontSize,
        const FontStyleOpt & fontStyle,
        const ColorSpecOpt & textColor,
        const ColorSpecOpt & backgroundColor,
        const UnderlineModeOpt & underlineMode,
        const TypographicSizeOpt & leftMargin,
        const TypographicSizeOpt & rightMargin,
        const TypographicSizeOpt & gapAbove,
        const TypographicSizeOpt & gapBelow,
        const BorderTypeOpt & tableBorderType,
        const BorderTypeOpt & cellBorderType
    ) : CustomBlockStyle(
            reportTemplate,
            name,
            fontSpecs,
            fontSize,
            fontStyle,
            textColor,
            backgroundColor,
            underlineMode,
            leftMargin,
            rightMargin,
            gapAbove,
            gapBelow),
        _tableBorderType(tableBorderType),
        _cellBorderType(cellBorderType)
{
}

CustomTableStyle::~CustomTableStyle()
{
}

//////////
//  ITableStyle
auto CustomTableStyle::tableBorderType() const -> BorderTypeOpt
{
    return _tableBorderType;
}

auto CustomTableStyle::cellBorderType() const -> BorderTypeOpt
{
    return _cellBorderType;
}

//////////
//  Serialization
CustomTableStyle::CustomTableStyle(
        CustomReportTemplate * reportTemplate
    ) : CustomBlockStyle(reportTemplate),
        //  Use "no value" for all propertoies
        _tableBorderType(),
        _cellBorderType()
{
}

void CustomTableStyle::_deserialize(const QDomElement & styleElement)
{
    CustomBlockStyle::_deserialize(styleElement);
    _reportTemplate->_parseAttribute(
        styleElement,
        "TableBorderType",
        _tableBorderType);
    _reportTemplate->_parseAttribute(
        styleElement,
        "CellBorderType",
        _cellBorderType);
}

//  End of tt3-report/CustomTableStyle.cpp
