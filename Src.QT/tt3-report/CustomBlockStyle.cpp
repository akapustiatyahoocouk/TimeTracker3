//
//  tt3-report/CustomBlockStyle.cpp - tt3::report::CustomBlockStyle class implementation
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
CustomBlockStyle::CustomBlockStyle(
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
        const TypographicSizeOpt & gapBelow
    ) : CustomStyle(
            reportTemplate,
            name,
            fontSpecs,
            fontSize,
            fontStyle,
            textColor,
            backgroundColor,
            underlineMode),
        _leftMargin(leftMargin),
        _rightMargin(rightMargin),
        _gapAbove(gapAbove),
        _gapBelow(gapBelow)
{
}

CustomBlockStyle::~CustomBlockStyle()
{
}

//////////
//  IBlockStyle
auto CustomBlockStyle::leftMargin() const -> TypographicSizeOpt
{
    return _leftMargin;
}

auto CustomBlockStyle::rightMargin() const -> TypographicSizeOpt
{
    return _rightMargin;
}

auto CustomBlockStyle::gapAbove() const -> TypographicSizeOpt
{
    return _gapAbove;
}

auto CustomBlockStyle::gapBelow() const -> TypographicSizeOpt
{
    return _gapBelow;
}

//////////
//  Serialization
CustomBlockStyle::CustomBlockStyle(
        CustomReportTemplate * reportTemplate
    ) : CustomStyle(reportTemplate),
    //  Use "no value" for all properties
    _leftMargin(),
    _rightMargin(),
    _gapAbove(),
    _gapBelow()
{
}

void CustomBlockStyle::_deserialize(const QDomElement & styleElement)
{
    CustomStyle::_deserialize(styleElement);

    _reportTemplate->_parseAttribute(
        styleElement,
        "LeftMargin",
        _leftMargin);
    _reportTemplate->_parseAttribute(
        styleElement,
        "RightMargin",
        _rightMargin);
    _reportTemplate->_parseAttribute(
        styleElement,
        "GapAbove",
        _gapAbove);
    _reportTemplate->_parseAttribute(
        styleElement,
        "GapBelow",
        _gapBelow);
}

//  End of tt3-report/CustomBlockStyle.cpp
