//
//  tt3-report/BasicTableCellStyle.cpp - tt3::report::BasicTableCellStyle class implementation
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
BasicTableCellStyle::BasicTableCellStyle(
        const Name & name,
        const FontSpecsOpt & fontSpecs,
        const TypographicSizeOpt & fontSize,
        const FontStyleOpt & fontStyle,
        const ColorSpecOpt & textColor,
        const ColorSpecOpt & backgroundColor,
        const UnderlineModeOpt & underlineMode,
        const HorizontalAlignmentOpt & horizontalAlignment,
        const VerticalAlignmentOpt & verticalAlignment
    ) : BasicStyle(
            name,
            fontSpecs,
            fontSize,
            fontStyle,
            textColor,
            backgroundColor,
            underlineMode),
        _horizontalAlignment(horizontalAlignment),
        _verticalAlignment(verticalAlignment)
{
}

BasicTableCellStyle::~BasicTableCellStyle()
{
}

//////////
//  ITableCellStyle
auto BasicTableCellStyle::horizontalAlignment() const -> HorizontalAlignmentOpt
{
    return _horizontalAlignment;
}

auto BasicTableCellStyle::verticalAlignment() const -> VerticalAlignmentOpt
{
    return _verticalAlignment;
}

//  End of tt3-report/BasicTableCellStyle.cpp
