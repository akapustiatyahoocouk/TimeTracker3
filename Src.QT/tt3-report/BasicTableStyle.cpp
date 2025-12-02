//
//  tt3-report/BasicTableStyle.cpp - tt3::report::BasicTableStyle class implementation
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
BasicTableStyle::BasicTableStyle(
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
    ) : BasicBlockStyle(
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

BasicTableStyle::~BasicTableStyle()
{
}

//////////
//  ITableStyle
auto BasicTableStyle::tableBorderType() const -> BorderTypeOpt
{
    return _tableBorderType;
}

auto BasicTableStyle::cellBorderType() const -> BorderTypeOpt
{
    return _cellBorderType;
}

//  End of tt3-report/BasicTableStyle.cpp
