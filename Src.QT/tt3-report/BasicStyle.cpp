//
//  tt3-report/BasicStyle.cpp - tt3::report::BasicStyle class implementation
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
BasicStyle::BasicStyle(
        const Mnemonic & name,
        const FontSpecsOpt & fontSpecs,
        const TypographicSizeOpt & fontSize,
        const FontStyleOpt & fontStyle,
        const ColorSpecOpt & textColor,
        const ColorSpecOpt & backgroundColor,
        const UnderlineModeOpt & underlineMode
    ) : _name(name),
        _fontSpecs(fontSpecs),
        _fontSize(fontSize),
        _fontStyle(fontStyle),
        _textColor(textColor),
        _backgroundColor(backgroundColor),
        _underlineMode(underlineMode)
{
}

BasicStyle::~BasicStyle()
{
}

//////////
//  IStyle
auto BasicStyle::reportTemplate() const -> IReportTemplate *
{
    return BasicReportTemplate::instance();
}

auto BasicStyle::name() const -> Mnemonic
{
    return _name;
}

auto BasicStyle::fontSpecs() const -> FontSpecsOpt
{
    return _fontSpecs;
}

auto BasicStyle::fontSize() const -> TypographicSizeOpt
{
    return _fontSize;
}

auto BasicStyle::fontStyle() const -> FontStyleOpt
{
    return _fontStyle;
}

auto BasicStyle::textColor() const -> ColorSpecOpt
{
    return _textColor;
}

auto BasicStyle::backgroundColor() const -> ColorSpecOpt
{
    return _backgroundColor;
}

auto BasicStyle::underlineMode() const -> UnderlineModeOpt
{
    return _underlineMode;
}

//  End of tt3-report/BasicStyle.cpp
