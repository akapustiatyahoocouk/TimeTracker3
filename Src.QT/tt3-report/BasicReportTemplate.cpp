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

TT3_IMPLEMENT_SINGLETON(BasicReportTemplate)

BasicReportTemplate::BasicReportTemplate()
    :   _pageSetup(),   //  default, A4
        _defaultFontSpecs{FontSpec::SansSerif},
        _defaultFontSize(TypographicSize::pt(12)),
        _defaultFontStyle(FontStyle::Plain),
        _defaultTextColor(ColorSpec::Default),
        _defaultBackgroundColor(ColorSpec::Default),
        _defaultListIndent(TypographicSize::in(0.25)),
        _defaultTableBorderType(BorderType::Single),
        _defaultLinkUnderlineMode(UnderlineMode::Single),
        _defaultPageNumberPlacement(PageNumberPlacement::Default)
{
    _addStyle(
        new BasicSectionStyle(
            ISectionStyle::DefaultStyleName,
            _defaultFontSpecs,
            _defaultFontSize,
            FontStyle::Bold,
            _defaultTextColor,
            _defaultBackgroundColor,
            UnderlineMode::None,
            PageNumberPlacement::BottomRight));

    _addStyle(
        new BasicParagraphStyle(
            IParagraphStyle::TitleStyleName,
            _defaultFontSpecs,
            _defaultFontSize.scaled(4),
            FontStyle::Bold,
            _defaultTextColor,
            _defaultBackgroundColor,
            UnderlineMode::None,
            TypographicSize::pt(0),
            TypographicSize::pt(0),
            TypographicSize::pt(2),
            TypographicSize::pt(2),
            HorizontalAlignment::Center,
            BorderType::None));
    _addStyle(
        new BasicParagraphStyle(
            IParagraphStyle::SubtitleStyleName,
            _defaultFontSpecs,
            _defaultFontSize.scaled(2),
            _defaultFontStyle,
            _defaultTextColor,
            _defaultBackgroundColor,
            UnderlineMode::None,
            TypographicSize::pt(0),
            TypographicSize::pt(0),
            TypographicSize::pt(2),
            TypographicSize::pt(2),
            HorizontalAlignment::Center,
            BorderType::None));
    _addStyle(
        new BasicParagraphStyle(
            IParagraphStyle::DefaultStyleName,
            _defaultFontSpecs,
            _defaultFontSize,
            _defaultFontStyle,
            _defaultTextColor,
            _defaultBackgroundColor,
            UnderlineMode::None,
            TypographicSize::pt(0),
            TypographicSize::pt(0),
            TypographicSize::pt(2),
            TypographicSize::pt(2),
            HorizontalAlignment::Left,
            BorderType::None));
    _addStyle(
        new BasicParagraphStyle(
            IParagraphStyle::Heading1StyleName,
            _defaultFontSpecs,
            _defaultFontSize.scaled(2.4f),
            FontStyle::Bold,
            _defaultTextColor,
            _defaultBackgroundColor,
            UnderlineMode::None,
            TypographicSize::pt(0),
            TypographicSize::pt(0),
            TypographicSize::pt(2),
            TypographicSize::pt(2),
            HorizontalAlignment::Left,
            BorderType::None));
    _addStyle(
        new BasicParagraphStyle(
            IParagraphStyle::Heading2StyleName,
            _defaultFontSpecs,
            _defaultFontSize.scaled(2.2f),
            FontStyle::Bold,
            _defaultTextColor,
            _defaultBackgroundColor,
            UnderlineMode::None,
            TypographicSize::pt(0),
            TypographicSize::pt(0),
            TypographicSize::pt(2),
            TypographicSize::pt(2),
            HorizontalAlignment::Left,
            BorderType::None));
    _addStyle(
        new BasicParagraphStyle(
            IParagraphStyle::Heading3StyleName,
            _defaultFontSpecs,
            _defaultFontSize.scaled(2.0f),
            FontStyle::Bold,
            _defaultTextColor,
            _defaultBackgroundColor,
            UnderlineMode::None,
            TypographicSize::pt(0),
            TypographicSize::pt(0),
            TypographicSize::pt(2),
            TypographicSize::pt(2),
            HorizontalAlignment::Left,
            BorderType::None));
    _addStyle(
        new BasicParagraphStyle(
            IParagraphStyle::Heading4StyleName,
            _defaultFontSpecs,
            _defaultFontSize.scaled(1.8f),
            FontStyle::Bold,
            _defaultTextColor,
            _defaultBackgroundColor,
            UnderlineMode::None,
            TypographicSize::pt(0),
            TypographicSize::pt(0),
            TypographicSize::pt(2),
            TypographicSize::pt(2),
            HorizontalAlignment::Left,
            BorderType::None));
    _addStyle(
        new BasicParagraphStyle(
            IParagraphStyle::Heading5StyleName,
            _defaultFontSpecs,
            _defaultFontSize.scaled(1.6f),
            FontStyle::Bold,
            _defaultTextColor,
            _defaultBackgroundColor,
            UnderlineMode::None,
            TypographicSize::pt(0),
            TypographicSize::pt(0),
            TypographicSize::pt(2),
            TypographicSize::pt(2),
            HorizontalAlignment::Left,
            BorderType::None));
    _addStyle(
        new BasicParagraphStyle(
            IParagraphStyle::Heading6StyleName,
            _defaultFontSpecs,
            _defaultFontSize.scaled(1.5f),
            FontStyle::Bold,
            _defaultTextColor,
            _defaultBackgroundColor,
            UnderlineMode::None,
            TypographicSize::pt(0),
            TypographicSize::pt(0),
            TypographicSize::pt(2),
            TypographicSize::pt(2),
            HorizontalAlignment::Left,
            BorderType::None));
    _addStyle(
        new BasicParagraphStyle(
            IParagraphStyle::Heading7StyleName,
            _defaultFontSpecs,
            _defaultFontSize.scaled(1.4f),
            FontStyle::Bold,
            _defaultTextColor,
            _defaultBackgroundColor,
            UnderlineMode::None,
            TypographicSize::pt(0),
            TypographicSize::pt(0),
            TypographicSize::pt(2),
            TypographicSize::pt(2),
            HorizontalAlignment::Left,
            BorderType::None));
    _addStyle(
        new BasicParagraphStyle(
            IParagraphStyle::Heading8StyleName,
            _defaultFontSpecs,
            _defaultFontSize.scaled(1.3f),
            FontStyle::Bold,
            _defaultTextColor,
            _defaultBackgroundColor,
            UnderlineMode::None,
            TypographicSize::pt(0),
            TypographicSize::pt(0),
            TypographicSize::pt(2),
            TypographicSize::pt(2),
            HorizontalAlignment::Left,
            BorderType::None));
    _addStyle(
        new BasicParagraphStyle(
            IParagraphStyle::Heading9StyleName,
            _defaultFontSpecs,
            _defaultFontSize.scaled(1.2f),
            FontStyle::Bold,
            _defaultTextColor,
            _defaultBackgroundColor,
            UnderlineMode::None,
            TypographicSize::pt(0),
            TypographicSize::pt(0),
            TypographicSize::pt(2),
            TypographicSize::pt(2),
            HorizontalAlignment::Left,
            BorderType::None));
    _addStyle(
        new BasicParagraphStyle(
            IParagraphStyle::Toc1StyleName,
            _defaultFontSpecs,
            _defaultFontSize,
            FontStyle::Bold,
            _defaultTextColor,
            _defaultBackgroundColor,
            UnderlineMode::None,
            TypographicSize::pt(0),
            TypographicSize::pt(0),
            TypographicSize::pt(6),
            TypographicSize::pt(0),
            HorizontalAlignment::Left,
            BorderType::None));
    _addStyle(
        new BasicParagraphStyle(
            IParagraphStyle::Toc2StyleName,
            _defaultFontSpecs,
            _defaultFontSize,
            _defaultFontStyle,
            _defaultTextColor,
            _defaultBackgroundColor,
            UnderlineMode::None,
            TypographicSize::pt(12),
            TypographicSize::pt(0),
            TypographicSize::pt(0),
            TypographicSize::pt(0),
            HorizontalAlignment::Justify,
            BorderType::None));
    _addStyle(
        new BasicParagraphStyle(
            IParagraphStyle::Toc3StyleName,
            _defaultFontSpecs,
            _defaultFontSize,
            _defaultFontStyle,
            _defaultTextColor,
            _defaultBackgroundColor,
            UnderlineMode::None,
            TypographicSize::pt(24),
            TypographicSize::pt(0),
            TypographicSize::pt(0),
            TypographicSize::pt(0),
            HorizontalAlignment::Justify,
            BorderType::None));
    _addStyle(
        new BasicParagraphStyle(
            IParagraphStyle::Toc4StyleName,
            _defaultFontSpecs,
            _defaultFontSize,
            _defaultFontStyle,
            _defaultTextColor,
            _defaultBackgroundColor,
            UnderlineMode::None,
            TypographicSize::pt(36),
            TypographicSize::pt(0),
            TypographicSize::pt(0),
            TypographicSize::pt(0),
            HorizontalAlignment::Justify,
            BorderType::None));
    _addStyle(
        new BasicParagraphStyle(
            IParagraphStyle::Toc5StyleName,
            _defaultFontSpecs,
            _defaultFontSize,
            _defaultFontStyle,
            _defaultTextColor,
            _defaultBackgroundColor,
            UnderlineMode::None,
            TypographicSize::pt(48),
            TypographicSize::pt(0),
            TypographicSize::pt(0),
            TypographicSize::pt(0),
            HorizontalAlignment::Justify,
            BorderType::None));
    _addStyle(
        new BasicParagraphStyle(
            IParagraphStyle::Toc6StyleName,
            _defaultFontSpecs,
            _defaultFontSize,
            _defaultFontStyle,
            _defaultTextColor,
            _defaultBackgroundColor,
            UnderlineMode::None,
            TypographicSize::pt(60),
            TypographicSize::pt(0),
            TypographicSize::pt(0),
            TypographicSize::pt(0),
            HorizontalAlignment::Justify,
            BorderType::None));
    _addStyle(
        new BasicParagraphStyle(
            IParagraphStyle::Toc7StyleName,
            _defaultFontSpecs,
            _defaultFontSize,
            _defaultFontStyle,
            _defaultTextColor,
            _defaultBackgroundColor,
            UnderlineMode::None,
            TypographicSize::pt(72),
            TypographicSize::pt(0),
            TypographicSize::pt(0),
            TypographicSize::pt(0),
            HorizontalAlignment::Justify,
            BorderType::None));
    _addStyle(
        new BasicParagraphStyle(
            IParagraphStyle::Toc8StyleName,
            _defaultFontSpecs,
            _defaultFontSize,
            _defaultFontStyle,
            _defaultTextColor,
            _defaultBackgroundColor,
            UnderlineMode::None,
            TypographicSize::pt(84),
            TypographicSize::pt(0),
            TypographicSize::pt(0),
            TypographicSize::pt(0),
            HorizontalAlignment::Justify,
            BorderType::None));
    _addStyle(
        new BasicParagraphStyle(
            IParagraphStyle::Toc9StyleName,
            _defaultFontSpecs,
            _defaultFontSize,
            _defaultFontStyle,
            _defaultTextColor,
            _defaultBackgroundColor,
            UnderlineMode::None,
            TypographicSize::pt(96),
            TypographicSize::pt(0),
            TypographicSize::pt(0),
            TypographicSize::pt(0),
            HorizontalAlignment::Justify,
            BorderType::None));

    _addStyle(
        new BasicTableStyle(
            ITableStyle::DefaultStyleName,
            _defaultFontSpecs,
            _defaultFontSize,
            _defaultFontStyle,
            _defaultTextColor,
            _defaultBackgroundColor,
            UnderlineMode::None,
            TypographicSize::pt(0),
            TypographicSize::pt(0),
            TypographicSize::pt(2),
            TypographicSize::pt(2),
            _defaultTableBorderType,
            _defaultTableBorderType));

    _addStyle(
        new BasicListStyle(
            IListStyle::DefaultStyleName,
            _defaultFontSpecs,
            _defaultFontSize,
            _defaultFontStyle,
            _defaultTextColor,
            _defaultBackgroundColor,
            UnderlineMode::None,
            TypographicSize::in(0.25),
            TypographicSize::pt(0),
            TypographicSize::pt(2),
            TypographicSize::pt(2),
            _defaultListIndent));

    _addStyle(
        new BasicLinkStyle(
            ILinkStyle::DefaultStyleName,
            _defaultFontSpecs,
            _defaultFontSize,
            _defaultFontStyle,
            QColor(0, 128, 0),
            _defaultBackgroundColor,
            _defaultLinkUnderlineMode));
}

BasicReportTemplate::~BasicReportTemplate()
{
    for (auto style : _styles.values())
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
    QList<BasicStyle*> basicStyles = _styles.values();
    return Styles(basicStyles.cbegin(), basicStyles.cend());
}

auto BasicReportTemplate::findStyleByMnemonic(
        const Mnemonic & mnemonic
    ) const -> IStyle *
{
    return _styles.contains(mnemonic) ?
                _styles[mnemonic] :
                nullptr;
}

//////////
//  Implementation helpers
void BasicReportTemplate::_addStyle(BasicStyle * style)
{
    Q_ASSERT(style != nullptr);
    Q_ASSERT(!_styles.contains(style->name()));

    _styles[style->name()] = style;
}

//  End of tt3-report/BasicReportTemplate.cpp
