//
//  tt3-report/CustomStyle.cpp - tt3::report::CustomStyle class implementation
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
CustomStyle::CustomStyle(
        CustomReportTemplate * reportTemplate,
        const Mnemonic & name,
        const FontSpecsOpt & fontSpecs,
        const TypographicSizeOpt & fontSize,
        const FontStyleOpt & fontStyle,
        const ColorSpecOpt & textColor,
        const ColorSpecOpt & backgroundColor,
        const UnderlineModeOpt & underlineMode
    ) : _reportTemplate(reportTemplate),
        _name(name),
        _fontSpecs(fontSpecs),
        _fontSize(fontSize),
        _fontStyle(fontStyle),
        _textColor(textColor),
        _backgroundColor(backgroundColor),
        _underlineMode(underlineMode)
{
}

CustomStyle::~CustomStyle()
{
}

//////////
//  IStyle
auto CustomStyle::reportTemplate() const -> IReportTemplate *
{
    return _reportTemplate;
}

auto CustomStyle::name() const -> Mnemonic
{
    return _name;
}

auto CustomStyle::fontSpecs() const -> FontSpecsOpt
{
    return _fontSpecs;
}

auto CustomStyle::fontSize() const -> TypographicSizeOpt
{
    return _fontSize;
}

auto CustomStyle::fontStyle() const -> FontStyleOpt
{
    return _fontStyle;
}

auto CustomStyle::textColor() const -> ColorSpecOpt
{
    return _textColor;
}

auto CustomStyle::backgroundColor() const -> ColorSpecOpt
{
    return _backgroundColor;
}

auto CustomStyle::underlineMode() const -> UnderlineModeOpt
{
    return _underlineMode;
}

//////////
//  Serialization
CustomStyle::CustomStyle(
        CustomReportTemplate * reportTemplate
    ) : _reportTemplate(reportTemplate),
        //  Use "no value" for all properties
        _name(),
        _fontSpecs(),
        _fontSize(),
        _fontStyle(),
        _textColor(),
        _backgroundColor(),
        _underlineMode()
{
}

void CustomStyle::_deserialize(const QDomElement & styleElement)
{
    _reportTemplate->_parseAttribute(
        styleElement,
        "Name",
        _name);
    _reportTemplate->_parseAttribute(
        styleElement,
        "FontSpecs",
        _fontSpecs);
    _reportTemplate->_parseAttribute(
        styleElement,
        "FontSize",
        _fontSize);
    _reportTemplate->_parseAttribute(
        styleElement,
        "FontStyle",
        _fontStyle);
    _reportTemplate->_parseAttribute(
        styleElement,
        "TextColor",
        _textColor);
    _reportTemplate->_parseAttribute(
        styleElement,
        "BackgroundColor",
        _backgroundColor);
    _reportTemplate->_parseAttribute(
        styleElement,
        "UnderlineMode",
        _underlineMode);
}


//  End of tt3-report/CustomStyle.cpp
