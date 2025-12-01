//
//  tt3-report/BasicReportTemplate.cpp - tt3::report::BasicReportTemplate class implementation
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

IMPLEMENT_SINGLETON(BasicReportTemplate)

BasicReportTemplate::BasicReportTemplate()
    :   _pageSetup(),   //  default, A4
        _defaultFontSpecs{FontSpec::SansSerif},
        _defaultFontSize(TypographicSize::pt(12)),
        _defaultFontStyle(FontStyle::Plain),
        _defaultTextColor(ColorSpec::ColorClass::Default),
        _defaultBackgroundColor(ColorSpec::ColorClass::Default),
        _defaultListIndent(TypographicSize::in(0.25)),
        _defaultTableBorderType(BorderType::Single),
        _defaultLinkUnderlineMode(UnderlineMode::Single),
        _defaultPageNumberPlacement(PageNumberPlacement::Default)
{
}

BasicReportTemplate::~BasicReportTemplate()
{
    for (auto style : _mapNamesToStyles.values())
    {
        delete style;
    }
}

//////////
//  IReportTemplate
auto BasicReportTemplate::mnemonic() const -> Mnemonic
{
    return M(Basic);
}

auto BasicReportTemplate::displayName() const -> QString
{
    static Component::Resources *const resources = Component::Resources::instance();   //  idempotent
    return resources->string(RSID(BasicReportTemplate), RID(DisplayName));
}

auto BasicReportTemplate::pageSetup() const -> PageSetup
{
    return _pageSetup;
}

auto BasicReportTemplate::defaultFontSpecs() const -> FontSpecs
{
    return _defaultFontSpecs;
}

auto BasicReportTemplate::defaultFontSize() const -> TypographicSize
{
    return _defaultFontSize;
}

auto BasicReportTemplate::defaultFontStyle() const -> FontStyle
{
    return _defaultFontStyle;
}

auto BasicReportTemplate::defaultTextColor() const -> ColorSpec
{
    return _defaultTextColor;
}

auto BasicReportTemplate::defaultBackgroundColor() const -> ColorSpec
{
    return _defaultBackgroundColor;
}

auto BasicReportTemplate::defaultListIndent() const -> TypographicSize
{
    return _defaultListIndent;
}

auto BasicReportTemplate::defaultTableBorderType() const -> BorderType
{
    return _defaultTableBorderType;
}

auto BasicReportTemplate::defaultLinkUnderlineMode() const -> UnderlineMode
{
    return _defaultLinkUnderlineMode;
}

auto BasicReportTemplate::defaultPageNumberPlacement() const -> PageNumberPlacement
{
    return _defaultPageNumberPlacement;
}

auto BasicReportTemplate::styles() const -> Styles
{
    QList<BasicStyle*> basicStyles = _mapNamesToStyles.values();
    return Styles(basicStyles.cbegin(), basicStyles.cend());
}
auto BasicReportTemplate::findStyleByMnemonic(
        const Mnemonic & mnemonic
    ) const -> IStyle *
{
    return _mapNamesToStyles.contains(mnemonic) ?
                _mapNamesToStyles[mnemonic] :
                nullptr;
}

//  End of tt3-report/BasicReportTemplate.cpp
