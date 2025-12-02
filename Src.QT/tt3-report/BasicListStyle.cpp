//
//  tt3-report/BasicListStyle.cpp - tt3::report::BasicListStyle class implementation
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
BasicListStyle::BasicListStyle(
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
        const TypographicSizeOpt & gapBelow,
        const TypographicSizeOpt & indent
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
        _indent(indent)
{
}

BasicListStyle::~BasicListStyle()
{
}

//////////
//  IListStyle
auto BasicListStyle::indent() const -> TypographicSizeOpt
{
    return _indent;
}

//  End of tt3-report/BasicListStyle.cpp
