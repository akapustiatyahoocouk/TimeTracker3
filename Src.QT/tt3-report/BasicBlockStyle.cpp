//
//  tt3-report/BasicBlockStyle.cpp - tt3::report::BasicBlockStyle class implementation
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
BasicBlockStyle::BasicBlockStyle(
        const QString & name,
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
    ) : BasicStyle(
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

BasicBlockStyle::~BasicBlockStyle()
{
}

//////////
//  IBlockStyle
auto BasicBlockStyle::leftMargin() const -> TypographicSizeOpt
{
    return _leftMargin;
}

auto BasicBlockStyle::rightMargin() const -> TypographicSizeOpt
{
    return _rightMargin;
}

auto BasicBlockStyle::gapAbove() const -> TypographicSizeOpt
{
    return _gapAbove;
}

auto BasicBlockStyle::gapBelow() const -> TypographicSizeOpt
{
    return _gapBelow;
}

//  End of tt3-report/BasicBlockStyle.cpp
