//
//  tt3-report/CustomParagraphStyle.cpp - tt3::report::CustomParagraphStyle class implementation
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
CustomParagraphStyle::CustomParagraphStyle(
        CustomReportTemplate * reportTemplate,
        const Name & name,
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
        const HorizontalAlignmentOpt & textAlignment,
        const BorderTypeOpt & borderType
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
        _textAlignment(textAlignment),
        _borderType(borderType)
{
}

CustomParagraphStyle::~CustomParagraphStyle()
{
}

//////////
//  IParagraphStyle
auto CustomParagraphStyle::textAlignment() const -> HorizontalAlignmentOpt
{
    return _textAlignment;
}

auto CustomParagraphStyle::borderType() const -> BorderTypeOpt
{
    return _borderType;
}

//////////
//  Serialization
CustomParagraphStyle::CustomParagraphStyle(
        CustomReportTemplate * reportTemplate
    ) : CustomBlockStyle(reportTemplate),
        //  Use "no value" for all properties
        _textAlignment(),
        _borderType()
{
}

void CustomParagraphStyle::_deserialize(const QDomElement & styleElement)
{
    CustomBlockStyle::_deserialize(styleElement);
    _reportTemplate->_parseAttribute(
        styleElement,
        "TextAlignment",
        _textAlignment);
    _reportTemplate->_parseAttribute(
        styleElement,
        "BorderType",
        _borderType);
}

//  End of tt3-report/CustomParagraphStyle.cpp
