//
//  tt3-report/CustomLinkStyle.cpp - tt3::report::CustomLinkStyle class implementation
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
CustomLinkStyle::CustomLinkStyle(
        CustomReportTemplate * reportTemplate,
        const Name & name,
        const FontSpecsOpt & fontSpecs,
        const TypographicSizeOpt & fontSize,
        const FontStyleOpt & fontStyle,
        const ColorSpecOpt & textColor,
        const ColorSpecOpt & backgroundColor,
        const UnderlineModeOpt & underlineMode
    ) : CustomStyle(
            reportTemplate,
            name,
            fontSpecs,
            fontSize,
            fontStyle,
            textColor,
            backgroundColor,
            underlineMode)
{
}

CustomLinkStyle::~CustomLinkStyle()
{
}

//////////
//  Serialization
CustomLinkStyle::CustomLinkStyle(
        CustomReportTemplate * reportTemplate
    ) : CustomStyle(reportTemplate)
{
}

void CustomLinkStyle::_deserialize(const QDomElement & styleElement)
{
    CustomStyle::_deserialize(styleElement);
}

//  End of tt3-report/CustomLinkStyle.cpp
