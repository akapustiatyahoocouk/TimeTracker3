//
//  tt3-report/CustomSectionStyle.cpp - tt3::report::CustomSectionStyle class implementation
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
CustomSectionStyle::CustomSectionStyle(
        CustomReportTemplate * reportTemplate,
        const Mnemonic & name,
        const FontSpecsOpt & fontSpecs,
        const TypographicSizeOpt & fontSize,
        const FontStyleOpt & fontStyle,
        const ColorSpecOpt & textColor,
        const ColorSpecOpt & backgroundColor,
        const UnderlineModeOpt & underlineMode,
        const PageNumberPlacementOpt & pageNumberPlacement
    ) : CustomStyle(
            reportTemplate,
            name,
            fontSpecs,
            fontSize,
            fontStyle,
            textColor,
            backgroundColor,
            underlineMode),
        _pageNumberPlacement(pageNumberPlacement)
{
}

CustomSectionStyle::~CustomSectionStyle()
{
}

//////////
//  ISectionStyle
auto CustomSectionStyle::pageNumberPlacement() const -> PageNumberPlacementOpt
{
    return _pageNumberPlacement;
}

//////////
//  Serialization
CustomSectionStyle::CustomSectionStyle(
        CustomReportTemplate * reportTemplate
    ) : CustomStyle(reportTemplate),
        //  Use "no value" for all properties
        _pageNumberPlacement()
{
}

void CustomSectionStyle::_deserialize(const QDomElement & styleElement)
{
    CustomStyle::_deserialize(styleElement);
    _reportTemplate->_parseAttribute(
        styleElement,
        "PageNumberPlacement",
        _pageNumberPlacement);
}

//  End of tt3-report/CustomSectionStyle.cpp
