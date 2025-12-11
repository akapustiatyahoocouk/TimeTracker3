//
//  tt3-report/HtmlReportFormat._CssBuilder.cpp - tt3::report::HtmlReportFormat::_CssBuilder class implementation
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
using HRG = HtmlReportFormat;

//////////
//  Construction/destruction
HRG::_CssBuilder::_CssBuilder()
{
    reset();
}

HRG::_CssBuilder::~_CssBuilder()
{
    reset();    //  to chean up
}

//////////
//  Operations
void HRG::_CssBuilder::reset()
{
#define CLEAN(set)          \
    for (auto item : set)   \
    {                       \
        delete item;        \
    }                       \
    set.clear();

    CLEAN(_bodyStyles)
    CLEAN(_paragraphStyles)
    CLEAN(_tableStyles)
    CLEAN(_tableCellStyles)
    CLEAN(_linkStyles)
    CLEAN(_listStyles)
#undef CLEAN
}

QString HRG::_CssBuilder::bodyStyle(IReportTemplate * reportTemplate)
{
    //  Prepare style properties
    QString fontFamilyString = _formatFontSpecs(reportTemplate->defaultFontSpecs());
    QString fontSizeString = _formatSize(reportTemplate->defaultFontSize());
    QString fontStyleString = _formatFontStyle(reportTemplate->defaultFontStyle());
    QString fontWeightString = _formatFontWeight(reportTemplate->defaultFontStyle());
    QString colorString =
        (reportTemplate->defaultTextColor().colorClass() == ColorSpec::Custom) ?
            _formatColor(reportTemplate->defaultTextColor()) :
            "initial";
    QString backgroundColorString =
        (reportTemplate->defaultBackgroundColor().colorClass() == ColorSpec::Custom) ?
            _formatColor(reportTemplate->defaultBackgroundColor()) :
            "initial";;

    float widthPt =
        (((reportTemplate->pageSetup().pageOrientation() == PageOrientation::Portrait) ?
            reportTemplate->pageSetup().pageWidth().pointSize() :
            reportTemplate->pageSetup().pageHeight().pointSize()) -
            reportTemplate->pageSetup().leftMargin().pointSize() -
            reportTemplate->pageSetup().rightMargin().pointSize());
    QString widthString = tt3::util::toString(widthPt) + "pt";

    //  Find/create the style
    for (_BodyStyle * bodyStyle : _bodyStyles)
    {
        if (bodyStyle->fontFamilyString == fontFamilyString &&
            bodyStyle->fontSizeString == fontSizeString &&
            bodyStyle->fontStyleString == fontStyleString &&
            bodyStyle->fontWeightString == fontWeightString &&
            bodyStyle->colorString == colorString &&
            bodyStyle->backgroundColorString == backgroundColorString &&
            bodyStyle->widthString == widthString)
        {
            return bodyStyle->className;
        }
    }
    _BodyStyle * bodyStyle =
        new _BodyStyle(
            _nextUnusedStyleNumber++,
            fontFamilyString,
            fontSizeString,
            fontStyleString,
            fontWeightString,
            colorString,
            backgroundColorString,
            widthString);
    _bodyStyles.insert(bodyStyle);
    return bodyStyle->className;
}

QString HRG::_CssBuilder::paragraphStyle(
        ReportParagraph * paragraph
    )
{
    QString fontFamilyString =
        _formatFontSpecs(
            paragraph->resolveFontSpecs());
    QString fontSizeString =
        _formatSize(
            paragraph->resolveFontSize());
    QString fontStyleString =
        _formatFontStyle(
            paragraph->resolveFontStyle());
    QString fontWeightString =
        _formatFontWeight(
            paragraph->resolveFontStyle());
    QString textDecorationString =
        _formatTextDecoration(
            paragraph->resolveFontStyle());
    QString colorString =
        _formatColor(
            paragraph->resolveTextColor());
    QString backgroundColorString =
        _formatColor(
            paragraph->resolveBackgroundColor());
    QString leftMarginString =
        _formatSize(
            paragraph->resolveLeftMargin());
    QString rightMarginString =
        _formatSize(
            paragraph->resolveRightMargin());
    QString gapAboveString =
        _formatSize(
            paragraph->resolveGapAboven());
    QString gapBelowString =
        _formatSize(
            paragraph->resolveGapBelow());
    QString textAlignmentString =
        _formatHorizontalAlignment(
            paragraph->resolveTextAlignment());
    QString borderTypeString =
        _formatBorderType(
            paragraph->resolveBorderType());
    return _paragraphStyle(
                fontFamilyString,
                fontSizeString,
                fontStyleString,
                fontWeightString,
                textDecorationString,
                colorString,
                backgroundColorString,
                leftMarginString,
                rightMarginString,
                gapAboveString,
                gapBelowString,
                textAlignmentString,
                borderTypeString);
}

QString HRG::_CssBuilder::paragraphStyle(
        IParagraphStyle * style
    )
{
    Q_ASSERT(style != nullptr);

    //  Prepare style properties
    QString fontFamilyString =
        _formatFontSpecs(
            style->fontSpecs().has_value() ?
                style->fontSpecs().value() :
                style->reportTemplate()->defaultFontSpecs());
    QString fontSizeString =
        _formatSize(
            style->fontSize().has_value() ?
                style->fontSize().value() :
                style->reportTemplate()->defaultFontSize());
    QString fontStyleString =
        _formatFontStyle(
            style->fontStyle().has_value() ?
                style->fontStyle().value() :
                style->reportTemplate()->defaultFontStyle());
    QString fontWeightString =
        _formatFontWeight(
            style->fontStyle().has_value() ?
                style->fontStyle().value() :
                style->reportTemplate()->defaultFontStyle());
    QString textDecorationString =
        _formatTextDecoration(
            style->fontStyle().has_value() ?
                style->fontStyle().value() :
                style->reportTemplate()->defaultFontStyle());
    QString colorString =
        _formatColor(
            style->textColor().has_value() ?
                style->textColor().value() :
                style->reportTemplate()->defaultTextColor());
    QString backgroundColorString =
        _formatColor(
            style->backgroundColor().has_value() ?
                style->backgroundColor().value() :
                style->reportTemplate()->defaultBackgroundColor());
    QString leftMarginString =
        style->leftMargin().has_value() ?
            _formatSize(style->leftMargin().value()) :
            "";
    QString rightMarginString =
        style->rightMargin().has_value() ?
            _formatSize(style->rightMargin().value()) :
            "";
    QString gapAboveString =
        style->gapAbove().has_value() ?
            _formatSize(style->gapAbove().value()) :
            "";
    QString gapBelowString =
        style->gapBelow().has_value() ?
            _formatSize(style->gapBelow().value()) :
            "";
    QString textAlignmentString =
        style->textAlignment().has_value() ?
            _formatHorizontalAlignment(style->textAlignment().value()) :
            "";
    QString borderTypeString =
        _formatBorderType(
            style->borderType().has_value() ?
                style->borderType().value() :
                BorderType::None);

    //  Find/create the style
    return _paragraphStyle(
                fontFamilyString,
                fontSizeString,
                fontStyleString,
                fontWeightString,
                textDecorationString,
                colorString,
                backgroundColorString,
                leftMarginString,
                rightMarginString,
                gapAboveString,
                gapBelowString,
                textAlignmentString,
                borderTypeString);
}

QString HRG::_CssBuilder::tableStyle(
        ReportTable * table
    )
{
    QString fontFamilyString =
        _formatFontSpecs(
            table->resolveFontSpecs());
    QString fontSizeString =
        _formatSize(
            table->resolveFontSize());
    QString fontStyleString =
        _formatFontStyle(
            table->resolveFontStyle());
    QString fontWeightString =
        _formatFontWeight(
            table->resolveFontStyle());
    QString textDecorationString =
        _formatTextDecoration(
            table->resolveFontStyle());
    QString colorString =
        _formatColor(
            table->resolveTextColor());
    QString backgroundColorString =
        _formatColor(
            table->resolveBackgroundColor());
    QString leftMarginString =
        _formatSize(
            table->resolveLeftMargin());
    QString rightMarginString =
        _formatSize(
            table->resolveRightMargin());
    QString gapAboveString =
        _formatSize(
            table->resolveGapAboven());
    QString gapBelowString =
        _formatSize(
            table->resolveGapBelow());
    QString tableBorderTypeString =
        _formatBorderType(
            table->resolveTableBorderType());
    QString cellBorderTypeString =
        _formatBorderType(
            table->resolveCellBorderType());
    return _tableStyle(
                fontFamilyString,
                fontSizeString,
                fontStyleString,
                fontWeightString,
                textDecorationString,
                colorString,
                backgroundColorString,
                leftMarginString,
                rightMarginString,
                gapAboveString,
                gapBelowString,
                tableBorderTypeString,
                cellBorderTypeString);
}

QString HRG::_CssBuilder::tableStyle(
        ITableStyle * style
    )
{
    Q_ASSERT(style != nullptr);

    //  Prepare style properties
    QString fontFamilyString =
        _formatFontSpecs(
            style->fontSpecs().has_value() ?
                style->fontSpecs().value() :
                style->reportTemplate()->defaultFontSpecs());
    QString fontSizeString =
        _formatSize(
            style->fontSize().has_value() ?
                style->fontSize().value() :
                style->reportTemplate()->defaultFontSize());
    QString fontStyleString =
        _formatFontStyle(
            style->fontStyle().has_value() ?
                style->fontStyle().value() :
                style->reportTemplate()->defaultFontStyle());
    QString fontWeightString =
        _formatFontWeight(
            style->fontStyle().has_value() ?
                style->fontStyle().value() :
                style->reportTemplate()->defaultFontStyle());
    QString textDecorationString =
        _formatTextDecoration(
            style->fontStyle().has_value() ?
                style->fontStyle().value() :
                style->reportTemplate()->defaultFontStyle());
    QString colorString =
        _formatColor(
            style->textColor().has_value() ?
                style->textColor().value() :
                style->reportTemplate()->defaultTextColor());
    QString backgroundColorString =
        _formatColor(
            style->backgroundColor().has_value() ?
                style->backgroundColor().value() :
                style->reportTemplate()->defaultBackgroundColor());
    QString leftMarginString =
        style->leftMargin().has_value() ?
            _formatSize(style->leftMargin().value()) :
            "";
    QString rightMarginString =
        style->rightMargin().has_value() ?
            _formatSize(style->rightMargin().value()) :
            "";
    QString gapAboveString =
        style->gapAbove().has_value() ?
            _formatSize(style->gapAbove().value()) :
            "";
    QString gapBelowString =
        style->gapBelow().has_value() ?
            _formatSize(style->gapBelow().value()) :
            "";
    QString tableBorderTypeString =
        style->tableBorderType().has_value() ?
            _formatBorderType(style->tableBorderType().value()) :
            "";
    QString cellBorderTypeString =
        _formatBorderType(
            style->cellBorderType().has_value() ?
                style->cellBorderType().value() :
                BorderType::None);

    //  Find/create the style
    return _tableStyle(
                fontFamilyString,
                fontSizeString,
                fontStyleString,
                fontWeightString,
                textDecorationString,
                colorString,
                backgroundColorString,
                leftMarginString,
                rightMarginString,
                gapAboveString,
                gapBelowString,
                tableBorderTypeString,
                cellBorderTypeString);
}

QString HRG::_CssBuilder::tableCellStyle(
        ReportTableCell * tableCell
    )
{   //  TODO reimplement using resolveXXX()
    QString fontFamilyString =
        _formatFontSpecs(
            tableCell->resolveFontSpecs());
    QString fontSizeString =
        _formatSize(
            tableCell->resolveFontSize());
    QString fontStyleString =
        _formatFontStyle(
            tableCell->resolveFontStyle());
    QString fontWeightString =
        _formatFontWeight(
            tableCell->resolveFontStyle());
    QString textDecorationString =
        _formatTextDecoration(
            tableCell->resolveFontStyle());
    QString colorString =
        _formatColor(
            tableCell->resolveTextColor());
    QString backgroundColorString =
        _formatColor(
            tableCell->resolveBackgroundColor());
    QString cellBorderTypeString =
        _formatBorderType(
            tableCell->resolveCellBorderType());
    QString verticalAlignString =
        _formatVerticalAlignment(
            tableCell->resolveContentAlignment());
    QString preferredWidthString =
        tableCell->preferredWidth().has_value() ?
            _formatPreferredWidth(tableCell->preferredWidth().value()) :
            "";

    auto tableCellStyle =
        new _TableCellStyle(
            _nextUnusedStyleNumber++,
            fontFamilyString,
            fontSizeString,
            fontStyleString,
            fontWeightString,
            textDecorationString,
            colorString,
            backgroundColorString,
            cellBorderTypeString,
            verticalAlignString,
            preferredWidthString);
    _tableCellStyles.insert(tableCellStyle);
    return tableCellStyle->className;
}

QString HRG::_CssBuilder::linkStyle(
        ReportLink * link
    )
{
    QString fontFamilyString =
        _formatFontSpecs(
            link->resolveFontSpecs());
    QString fontSizeString =
        _formatSize(
            link->resolveFontSize());
    QString fontStyleString =
        _formatFontStyle(
            link->resolveFontStyle());
    QString fontWeightString =
        _formatFontWeight(
            link->resolveFontStyle());
    QString textDecorationString =
        _formatTextDecoration(
            link->resolveFontStyle());
    QString colorString =
        _formatColor(
            link->resolveTextColor());
    QString backgroundColorString =
        _formatColor(
            link->resolveBackgroundColor());
    QString textDecorationStyleString; /* TODO kill off  =
        _formatSize(
            link->resolveTextDecorationStyle()); */
    return _linkStyle(
                fontFamilyString,
                fontSizeString,
                fontStyleString,
                fontWeightString,
                textDecorationString,
                colorString,
                backgroundColorString,
                textDecorationStyleString);
}

QString HRG::_CssBuilder::linkStyle(
        ILinkStyle * style
    )
{
    Q_ASSERT(style != nullptr);

    //  Prepare style properties
    QString fontFamilyString =
        _formatFontSpecs(
            style->fontSpecs().has_value() ?
                style->fontSpecs().value() :
                style->reportTemplate()->defaultFontSpecs());
    QString fontSizeString =
        _formatSize(
            style->fontSize().has_value() ?
                style->fontSize().value() :
                style->reportTemplate()->defaultFontSize());
    QString fontStyleString =
        _formatFontStyle(
            style->fontStyle().has_value() ?
                style->fontStyle().value() :
                style->reportTemplate()->defaultFontStyle());
    QString fontWeightString =
        _formatFontWeight(
            style->fontStyle().has_value() ?
                style->fontStyle().value() :
                style->reportTemplate()->defaultFontStyle());
    QString textDecorationString =
        _formatTextDecoration(
            style->fontStyle().has_value() ?
                style->fontStyle().value() :
                style->reportTemplate()->defaultFontStyle());
    QString colorString =
        _formatColor(
            style->textColor().has_value() ?
                style->textColor().value() :
                style->reportTemplate()->defaultTextColor());
    QString backgroundColorString =
        _formatColor(
            style->backgroundColor().has_value() ?
                style->backgroundColor().value() :
                style->reportTemplate()->defaultBackgroundColor());
    QString textDecorationStyleString =
        _formatUnderlineMode(
            style->underlineMode().has_value() ?
                style->underlineMode().value() :
                style->reportTemplate()->defaultLinkUnderlineMode());

    //  Find/create the style
    return _linkStyle(
                fontFamilyString,
                fontSizeString,
                fontStyleString,
                fontWeightString,
                textDecorationString,
                colorString,
                backgroundColorString,
                textDecorationStyleString);
}

QString HRG::_CssBuilder::listStyle(
        ReportList * list
    )
{
    QString fontFamilyString =
        _formatFontSpecs(
            list->resolveFontSpecs());
    QString fontSizeString =
        _formatSize(
            list->resolveFontSize());
    QString fontStyleString =
        _formatFontStyle(
            list->resolveFontStyle());
    QString fontWeightString =
        _formatFontWeight(
            list->resolveFontStyle());
    QString textDecorationString =
        _formatTextDecoration(
            list->resolveFontStyle());
    QString colorString =
        _formatColor(
            list->resolveTextColor());
    QString backgroundColorString =
        _formatColor(
            list->resolveBackgroundColor());
    QString leftMarginString =
        _formatSize(
            list->resolveLeftMargin());
    QString rightMarginString =
        _formatSize(
            list->resolveRightMargin());
    QString gapAboveString =
        _formatSize(
            list->resolveGapAboven());
    QString gapBelowString =
        _formatSize(
            list->resolveGapBelow());
    QString indentString =
        _formatSize(
            list->resolveIndent());
    return _listStyle(
                fontFamilyString,
                fontSizeString,
                fontStyleString,
                fontWeightString,
                textDecorationString,
                colorString,
                backgroundColorString,
                leftMarginString,
                rightMarginString,
                gapAboveString,
                gapBelowString,
                indentString);
}

QString HRG::_CssBuilder::listStyle(
        IListStyle * style
    )
{
    Q_ASSERT(style != nullptr);

    //  Prepare style properties
    QString fontFamilyString =
        _formatFontSpecs(
            style->fontSpecs().has_value() ?
                style->fontSpecs().value() :
                style->reportTemplate()->defaultFontSpecs());
    QString fontSizeString =
        _formatSize(
            style->fontSize().has_value() ?
                style->fontSize().value() :
                style->reportTemplate()->defaultFontSize());
    QString fontStyleString =
        _formatFontStyle(
            style->fontStyle().has_value() ?
                style->fontStyle().value() :
                style->reportTemplate()->defaultFontStyle());
    QString fontWeightString =
        _formatFontWeight(
            style->fontStyle().has_value() ?
                style->fontStyle().value() :
                style->reportTemplate()->defaultFontStyle());
    QString textDecorationString =
        _formatTextDecoration(
            style->fontStyle().has_value() ?
                style->fontStyle().value() :
                style->reportTemplate()->defaultFontStyle());
    QString colorString =
        _formatColor(
            style->textColor().has_value() ?
                style->textColor().value() :
                style->reportTemplate()->defaultTextColor());
    QString backgroundColorString =
        _formatColor(
            style->backgroundColor().has_value() ?
                style->backgroundColor().value() :
                style->reportTemplate()->defaultBackgroundColor());
    QString leftMarginString =
        style->leftMargin().has_value() ?
            _formatSize(style->leftMargin().value()) :
            "";
    QString rightMarginString =
        style->rightMargin().has_value() ?
            _formatSize(style->rightMargin().value()) :
            "";
    QString gapAboveString =
        style->gapAbove().has_value() ?
            _formatSize(style->gapAbove().value()) :
            "";
    QString gapBelowString =
        style->gapBelow().has_value() ?
            _formatSize(style->gapBelow().value()) :
            "";
    QString indentString =
        style->indent().has_value() ?
            _formatSize(style->indent().value()) :
            "";

    //  Find/create the style
    return _listStyle(
                fontFamilyString,
                fontSizeString,
                fontStyleString,
                fontWeightString,
                colorString,
                backgroundColorString,
                leftMarginString,
                textDecorationString,
                rightMarginString,
                gapAboveString,
                gapBelowString,
                indentString);
}

QString HRG::_CssBuilder::css()
{
    QString css;

#define FORMAT_STYLES(stylesSet)                                \
    {                                                           \
        using T = decltype(stylesSet)::value_type;              \
        QList<T> stylesList(stylesSet.cbegin(), stylesSet.cend()); \
        std::sort(                                              \
            stylesList.begin(),                                 \
            stylesList.end(),                                   \
            [](auto a, auto b)                                  \
            {                                                   \
                return a->className < b->className;             \
            });                                                 \
        for (auto style : stylesList)                           \
        {                                                       \
            css += style->css();                                \
            css += "\n";                                        \
        }                                                       \
    }

    FORMAT_STYLES(_bodyStyles)
    FORMAT_STYLES(_paragraphStyles)
    FORMAT_STYLES(_tableStyles)
    FORMAT_STYLES(_tableCellStyles)
    FORMAT_STYLES(_linkStyles)
    FORMAT_STYLES(_listStyles)
#undef FORMAT_STYLES

    return css;
}

//////////
//  Implementation helpers
QString HRG::_CssBuilder::_formatColor(
        const ColorSpec & c
    )
{
    switch (c.colorClass())
    {
        default:
        case ColorSpec::Default:
            return "";
        case ColorSpec::Transparent:
            return "rgba(0,0,0,0)";
        case ColorSpec::Custom:
            //  #rrggbb
            return tt3::util::toString(c.customColor());
    }
}

QString HRG::_CssBuilder::_formatFontSpecs(
        const FontSpecs & fontSpecs
    )
{
    return
        tt3::util::transform(
            fontSpecs,
            [](auto s)
            {
                QString name = s.name();
                //   Do we need to quote ?
                for (int i = 0; i < name.length(); i++)
                {
                    if (!((name[i] >= 'a' && name[i] <= 'z') ||
                          (name[i] >= 'A' && name[i] <= 'Z') ||
                          (name[i] >= '0' && name[i] <= '9') ||
                          name[i] == '-'))
                    {   //  Yes!
                        return '"' + s.name() + '"';
                    }
                }
                return name;
            }).join('+');
}

QString HRG::_CssBuilder::_formatSize(const TypographicSize & size)
{
    return tt3::util::toString(size.pointSize()) + "pt";
}

QString HRG::_CssBuilder::_formatFontStyle(
        const FontStyle & fontStyle
    )
{
    return (fontStyle & FontFlag::Italic).isEmpty() ? "normal" : "italic";
}

QString HRG::_CssBuilder::_formatFontWeight(const FontStyle & fontStyle)
{
    return (fontStyle & FontFlag::Bold).isEmpty() ? "normal" : "bold";
}

QString HRG::_CssBuilder::_formatTextDecoration(const FontStyle & fontStyle)
{
    if (fontStyle.contains(FontFlag::Underline) &&
        fontStyle.contains(FontFlag::StrikeThrough))
    {
        return "underline line-through";
    }
    else if (fontStyle.contains(FontFlag::Underline))
    {
        return "underline";
    }
    else if (fontStyle.contains(FontFlag::StrikeThrough))
    {
        return "line-through";
    }
    else
    {
        return "";
    }
}

QString HRG::_CssBuilder::_formatHorizontalAlignment(
        HorizontalAlignment alignment
    )
{
    switch (alignment)
    {
        case HorizontalAlignment::Default:
            return "";
        case HorizontalAlignment::Left:
            return "left";
        case HorizontalAlignment::Center:
            return "center";
        case HorizontalAlignment::Right:
            return "right";
        case HorizontalAlignment::Justify:
            return "justify";
        default:
            Q_ASSERT(false);
            return "left";
    }
}

QString HRG::_CssBuilder::_formatBorderType(
        BorderType borderType
    )
{
    switch (borderType)
    {
        case BorderType::Default:
            return "";
        case BorderType::None:
            return "none";
        case BorderType::Single:
            return "solid";
        case BorderType::Double:
            return "double";
        default:
            Q_ASSERT(false);
            return "none";
    }
}

QString HRG::_CssBuilder::_formatVerticalAlignment(
        VerticalAlignment alignment
    )
{
    switch (alignment)
    {
        case VerticalAlignment::Default:
            return "";
        case VerticalAlignment::Top:
            return "top";
        case VerticalAlignment::Middle:
            return "middle";
        case VerticalAlignment::Bottom:
            return "bottom";
        default:
            Q_ASSERT(false);
            return "top";
    }
}

QString HRG::_CssBuilder::_formatUnderlineMode(
        UnderlineMode underlineMode
    )
{
    switch (underlineMode)
    {
        case UnderlineMode::Default:
            return "inherit";
        case UnderlineMode::None:
            return "";
        case UnderlineMode::Single:
            return "solid";
        case UnderlineMode::Double:
            return "double";
        default:
            Q_ASSERT(false);
            return "solid";
    }
}

QString HRG::_CssBuilder::_formatPreferredWidth(
        const TypographicSizeOpt & preferredWidth
    )
{
    return preferredWidth.has_value() ?
                _formatSize(preferredWidth.value()) :
                "auto";
}

QString HRG::_CssBuilder::_paragraphStyle(
        const QString & fontFamilyString,
        const QString & fontSizeString,
        const QString & fontStyleString,
        const QString & fontWeightString,
        const QString & textDecorationString,
        const QString & colorString,
        const QString & backgroundColorString,
        const QString & leftMarginString,
        const QString & rightMarginString,
        const QString & gapAboveString,
        const QString & gapBelowString,
        const QString & textAlignmentString,
        const QString & borderTypeString
    )
{
    for (auto paragraphStyle : _paragraphStyles)
    {
        if (paragraphStyle->fontFamilyString == fontFamilyString &&
            paragraphStyle->fontSizeString == fontSizeString &&
            paragraphStyle->fontStyleString == fontStyleString &&
            paragraphStyle->fontWeightString == fontWeightString &&
            paragraphStyle->textDecorationString == textDecorationString &&
            paragraphStyle->colorString == colorString &&
            paragraphStyle->backgroundColorString == backgroundColorString &&
            paragraphStyle->leftMarginString == leftMarginString &&
            paragraphStyle->rightMarginString == rightMarginString &&
            paragraphStyle->gapAboveString == gapAboveString &&
            paragraphStyle->gapBelowString == gapBelowString &&
            paragraphStyle->textAlignmentString == textAlignmentString &&
            paragraphStyle->borderTypeString == borderTypeString)
        {
            return paragraphStyle->className;
        }
    }
    auto paragraphStyle =
        new _ParagraphStyle(
            _nextUnusedStyleNumber++,
            fontFamilyString,
            fontSizeString,
            fontStyleString,
            fontWeightString,
            textDecorationString,
            colorString,
            backgroundColorString,
            leftMarginString,
            rightMarginString,
            gapAboveString,
            gapBelowString,
            textAlignmentString,
            borderTypeString);
    _paragraphStyles.insert(paragraphStyle);
    return paragraphStyle->className;
}

QString HRG::_CssBuilder::_tableStyle(
        const QString & fontFamilyString,
        const QString & fontSizeString,
        const QString & fontStyleString,
        const QString & fontWeightString,
        const QString & textDecorationString,
        const QString & colorString,
        const QString & backgroundColorString,
        const QString & leftMarginString,
        const QString & rightMarginString,
        const QString & gapAboveString,
        const QString & gapBelowString,
        const QString & tableBorderTypeString,
        const QString & cellBorderTypeString
    )
{
    for (auto tableStyle : _tableStyles)
    {
        if (tableStyle->fontFamilyString == fontFamilyString &&
            tableStyle->fontSizeString == fontSizeString &&
            tableStyle->fontStyleString == fontStyleString &&
            tableStyle->fontWeightString == fontWeightString &&
            tableStyle->textDecorationString == textDecorationString &&
            tableStyle->colorString == colorString &&
            tableStyle->backgroundColorString == backgroundColorString &&
            tableStyle->leftMarginString == leftMarginString &&
            tableStyle->rightMarginString == rightMarginString &&
            tableStyle->gapAboveString == gapAboveString &&
            tableStyle->gapBelowString == gapBelowString &&
            tableStyle->tableBorderTypeString == tableBorderTypeString &&
            tableStyle->cellBorderTypeString == cellBorderTypeString)
        {
            return tableStyle->className;
        }
    }
    auto tableStyle =
        new _TableStyle(
            _nextUnusedStyleNumber++,
            fontFamilyString,
            fontSizeString,
            fontStyleString,
            fontWeightString,
            textDecorationString,
            colorString,
            backgroundColorString,
            leftMarginString,
            rightMarginString,
            gapAboveString,
            gapBelowString,
            tableBorderTypeString,
            cellBorderTypeString);
    _tableStyles.insert(tableStyle);
    return tableStyle->className;
}

QString HRG::_CssBuilder::_listStyle(
        const QString & fontFamilyString,
        const QString & fontSizeString,
        const QString & fontStyleString,
        const QString & fontWeightString,
        const QString & textDecorationString,
        const QString & colorString,
        const QString & backgroundColorString,
        const QString & leftMarginString,
        const QString & rightMarginString,
        const QString & gapAboveString,
        const QString & gapBelowString,
        const QString & indentString
    )
{
    for (auto listStyle : _listStyles)
    {
        if (listStyle->fontFamilyString == fontFamilyString &&
            listStyle->fontSizeString == fontSizeString &&
            listStyle->fontStyleString == fontStyleString &&
            listStyle->fontWeightString == fontWeightString &&
            listStyle->textDecorationString == textDecorationString &&
            listStyle->colorString == colorString &&
            listStyle->backgroundColorString == backgroundColorString &&
            listStyle->leftMarginString == leftMarginString &&
            listStyle->rightMarginString == rightMarginString &&
            listStyle->gapAboveString == gapAboveString &&
            listStyle->gapBelowString == gapBelowString &&
            listStyle->indentString == indentString)
        {
            return listStyle->className;
        }
    }
    auto listStyle =
        new _ListStyle(
            _nextUnusedStyleNumber++,
            fontFamilyString,
            fontSizeString,
            fontStyleString,
            fontWeightString,
            textDecorationString,
            colorString,
            backgroundColorString,
            leftMarginString,
            rightMarginString,
            gapAboveString,
            gapBelowString,
            indentString);
    _listStyles.insert(listStyle);
    return listStyle->className;
}

QString HRG::_CssBuilder::_linkStyle(
        const QString & fontFamilyString,
        const QString & fontSizeString,
        const QString & fontStyleString,
        const QString & fontWeightString,
        const QString & textDecorationString,
        const QString & colorString,
        const QString & backgroundColorString,
        const QString & textDecorationStyleString
    )
{
    for (auto linkStyle : _linkStyles)
    {
        if (linkStyle->fontFamilyString == fontFamilyString &&
            linkStyle->fontSizeString == fontSizeString &&
            linkStyle->fontStyleString == fontStyleString &&
            linkStyle->fontWeightString == fontWeightString &&
            linkStyle->textDecorationString == textDecorationString &&
            linkStyle->colorString == colorString &&
            linkStyle->backgroundColorString == backgroundColorString &&
            linkStyle->textDecorationStyleString == textDecorationStyleString)
        {
            return linkStyle->className;
        }
    }
    auto linkStyle =
        new _LinkStyle(
            _nextUnusedStyleNumber++,
            fontFamilyString,
            fontSizeString,
            fontStyleString,
            fontWeightString,
            textDecorationString,
            colorString,
            backgroundColorString,
            textDecorationStyleString);
    _linkStyles.insert(linkStyle);
    return linkStyle->className;
}

//////////
//  HRG::_CssBuilder::_BodyStyle
namespace
{
    QString cssProperty(const QString & name, const QString & value)
    {
        return
            "    " +
            (value.trimmed().isEmpty() ?
             "" :
             (name + ": " + value.trimmed() + ";\n"));
    }
    }

HRG::_CssBuilder::_BodyStyle::_BodyStyle(
        int sequenceNumber,
        const QString fontFamilyString_,
        const QString fontSizeString_,
        const QString fontStyleString_,
        const QString fontWeightString_,
        const QString colorString_,
        const QString backgroundColorString_,
        const QString widthString_
    ) : className("class" + tt3::util::toString(sequenceNumber)),
        fontFamilyString(fontFamilyString_),
        fontSizeString(fontSizeString_),
        fontStyleString(fontStyleString_),
        fontWeightString(fontWeightString_),
        colorString(colorString_),
        backgroundColorString(backgroundColorString_),
        widthString(widthString_)
{
}

QString HRG::_CssBuilder::_BodyStyle::css() const
{
    return "." + className + "\n" +
           "{\n" +
           cssProperty("font-family: ", fontFamilyString) +
           cssProperty("font-size", fontSizeString) +
           cssProperty("font-style", fontStyleString) +
           cssProperty("font-weight", fontWeightString) +
           cssProperty("text-decoration", textDecorationString) +
           cssProperty("color", colorString) +
           cssProperty("background-color", backgroundColorString) +
           cssProperty("width", widthString) +
           cssProperty("margin", "0 auto") +
           "}\n";
}

//////////
//  HRG::_CssBuilder::_ParagraphStyle
HRG::_CssBuilder::_ParagraphStyle::_ParagraphStyle(
        int sequenceNumber,
        const QString & fontFamilyString_,
        const QString & fontSizeString_,
        const QString & fontStyleString_,
        const QString & fontWeightString_,
        const QString & textDecorationString_,
        const QString & colorString_,
        const QString & backgroundColorString_,
        const QString & leftMarginString_,
        const QString & rightMarginString_,
        const QString & gapAboveString_,
        const QString & gapBelowString_,
        const QString & textAlignmentString_,
        const QString & borderTypeString_
    ) : className("class" + tt3::util::toString(sequenceNumber)),
        fontFamilyString(fontFamilyString_),
        fontSizeString(fontSizeString_),
        fontStyleString(fontStyleString_),
        fontWeightString(fontWeightString_),
        textDecorationString(textDecorationString_),
        colorString(colorString_),
        backgroundColorString(backgroundColorString_),
        leftMarginString(leftMarginString_),
        rightMarginString(rightMarginString_),
        gapAboveString(gapAboveString_),
        gapBelowString(gapBelowString_),
        textAlignmentString(textAlignmentString_),
        borderTypeString(borderTypeString_)
{
}

QString HRG::_CssBuilder::_ParagraphStyle::css() const
{
    return "." + className + "\n" +
           "{\n" +
           cssProperty("font-family", fontFamilyString) +
           cssProperty("font-size", fontSizeString) +
           cssProperty("font-style", fontStyleString) +
           cssProperty("font-weight", fontWeightString) +
           cssProperty("text-decoration", textDecorationString) +
           cssProperty("color", colorString) +
           cssProperty("background-color", backgroundColorString) +
           cssProperty("margin-left", leftMarginString) +
           cssProperty("margin-right", rightMarginString) +
           cssProperty("margin-top", gapAboveString) +
           cssProperty("margin-bottom", gapBelowString) +
           cssProperty("text-align", textAlignmentString) +
           cssProperty("border-style", borderTypeString) +
           cssProperty("border-color", colorString) +
           cssProperty("line-height", "100%") +
           "}\n";
}

//////////
//  HRG::_CssBuilder::_TableStyle
HRG::_CssBuilder::_TableStyle::_TableStyle(
        int sequenceNumber,
        const QString & fontFamilyString_,
        const QString & fontSizeString_,
        const QString & fontStyleString_,
        const QString & fontWeightString_,
        const QString & textDecorationString_,
        const QString & colorString_,
        const QString & backgroundColorString_,
        const QString & leftMarginString_,
        const QString & rightMarginString_,
        const QString & gapAboveString_,
        const QString & gapBelowString_,
        const QString & tableBorderTypeString_,
        const QString & cellBorderTypeString_
    ) : className("class" + tt3::util::toString(sequenceNumber)),
        fontFamilyString(fontFamilyString_),
        fontSizeString(fontSizeString_),
        fontStyleString(fontStyleString_),
        fontWeightString(fontWeightString_),
        textDecorationString(textDecorationString_),
        colorString(colorString_),
        backgroundColorString(backgroundColorString_),
        leftMarginString(leftMarginString_),
        rightMarginString(rightMarginString_),
        gapAboveString(gapAboveString_),
        gapBelowString(gapBelowString_),
        tableBorderTypeString(tableBorderTypeString_),
        cellBorderTypeString(cellBorderTypeString_)
{
}

QString HRG::_CssBuilder::_TableStyle::css() const
{
    return "." + className + "\n" +
           "{\n" +
           cssProperty("font-family", fontFamilyString) +
           cssProperty("font-size", fontSizeString) +
           cssProperty("font-style", fontStyleString) +
           cssProperty("font-weight", fontWeightString) +
           cssProperty("text-decoration", textDecorationString) +
           cssProperty("color", colorString) +
           cssProperty("background-color", backgroundColorString) +
           cssProperty("margin-left", leftMarginString) +
           cssProperty("margin-right", rightMarginString) +
           cssProperty("margin-top", gapAboveString) +
           cssProperty("margin-bottom", gapBelowString) +
           cssProperty("border-style", tableBorderTypeString) +
           cssProperty("border-color", colorString) +
           cssProperty("border-spacing", "0pt") +
           cssProperty("border-collapse", "collapse") +
           "}\n";
}

//////////
//  HRG::_CssBuilder::_TableCellStyle
HRG::_CssBuilder::_TableCellStyle::_TableCellStyle(
        int sequenceNumber,
        const QString & fontFamilyString_,
        const QString & fontSizeString_,
        const QString & fontStyleString_,
        const QString & fontWeightString_,
        const QString & textDecorationString_,
        const QString & colorString_,
        const QString & backgroundColorString_,
        const QString & cellBorderTypeString_,
        const QString & verticalAlignString_,
        const QString & preferredWidthString_
    ) : className("class" + tt3::util::toString(sequenceNumber)),
        fontFamilyString(fontFamilyString_),
        fontSizeString(fontSizeString_),
        fontStyleString(fontStyleString_),
        fontWeightString(fontWeightString_),
        textDecorationString(textDecorationString_),
        colorString(colorString_),
        backgroundColorString(backgroundColorString_),
        cellBorderTypeString(cellBorderTypeString_),
        verticalAlignString(verticalAlignString_),
        preferredWidthString(preferredWidthString_)
{
}

QString HRG::_CssBuilder::_TableCellStyle::css() const
{
    return "." + className + "\n" +
           "{\n" +
           cssProperty("font-family", fontFamilyString) +
           cssProperty("font-size", fontSizeString) +
           cssProperty("font-style", fontStyleString) +
           cssProperty("font-weight", fontWeightString) +
           cssProperty("text-decoration", textDecorationString) +
           cssProperty("color", colorString) +
           cssProperty("background-color", backgroundColorString) +
           cssProperty("border-style", cellBorderTypeString) +
           cssProperty("border-width", ((cellBorderTypeString == "solid") ? "1px" : "auto")) +
           cssProperty("border-color", colorString) +
           cssProperty("vertical-align", verticalAlignString) +
           cssProperty("width", preferredWidthString) +
           "}\n";
}

//////////
//  HRG::_CssBuilder::_LinkStyle
HRG::_CssBuilder::_LinkStyle::_LinkStyle(
        int sequenceNumber,
        const QString & fontFamilyString_,
        const QString & fontSizeString_,
        const QString & fontStyleString_,
        const QString & fontWeightString_,
        const QString & textDecorationString_,
        const QString & colorString_,
        const QString & backgroundColorString_,
        const QString & textDecorationStyleString_
    ) : className("class" + tt3::util::toString(sequenceNumber)),
        fontFamilyString(fontFamilyString_),
        fontSizeString(fontSizeString_),
        fontStyleString(fontStyleString_),
        fontWeightString(fontWeightString_),
        textDecorationString(textDecorationString_),
        colorString(colorString_),
        backgroundColorString(backgroundColorString_),
        textDecorationStyleString(textDecorationStyleString_)
{
}

QString HRG::_CssBuilder::_LinkStyle::css() const
{
    return "." + className + "\n" +
           "{\n" +
           cssProperty("font-family", fontFamilyString) +
           cssProperty("font-size", fontSizeString) +
           cssProperty("font-style", fontStyleString) +
           cssProperty("font-weight", fontWeightString) +
           cssProperty("text-decoration", textDecorationString) +
           cssProperty("color", colorString) +
           cssProperty("background-color", backgroundColorString) +
           (!textDecorationStyleString.isEmpty() ?
                (cssProperty("text-decoration-line", "underline") +
                 cssProperty("text-decoration-style", textDecorationStyleString) +
                 cssProperty("text-decoration-color", colorString)) :
                "") +
           "}\n";
}

//////////
//  HRG::_CssBuilder::_ListStyle
HRG::_CssBuilder::_ListStyle::_ListStyle(
        int sequenceNumber,
        const QString & fontFamilyString_,
        const QString & fontSizeString_,
        const QString & fontStyleString_,
        const QString & fontWeightString_,
        const QString & textDecorationString_,
        const QString & colorString_,
        const QString & backgroundColorString_,
        const QString & leftMarginString_,
        const QString & rightMarginString_,
        const QString & gapAboveString_,
        const QString & gapBelowString_,
        const QString & indentString_
    ) : className("class" + tt3::util::toString(sequenceNumber)),
        fontFamilyString(fontFamilyString_),
        fontSizeString(fontSizeString_),
        fontStyleString(fontStyleString_),
        fontWeightString(fontWeightString_),
        textDecorationString(textDecorationString_),
        colorString(colorString_),
        backgroundColorString(backgroundColorString_),
        leftMarginString(leftMarginString_),
        rightMarginString(rightMarginString_),
        gapAboveString(gapAboveString_),
        gapBelowString(gapBelowString_),
        indentString(indentString_)
{
}

QString HRG::_CssBuilder::_ListStyle::css() const
{
    return "." + className + "-table\n" +
           "{\n" +
           cssProperty("font-family: ", fontFamilyString) +
           cssProperty("font-size: ", fontSizeString) +
           cssProperty("font-style: ", fontStyleString) +
           cssProperty("font-weight: ", fontWeightString) +
           cssProperty("text-decoration", textDecorationString) +
           cssProperty("color: ", colorString) +
           cssProperty("background-color: ", backgroundColorString) +
           cssProperty("margin-left: ", leftMarginString) +
           cssProperty("margin-right: ", rightMarginString) +
           cssProperty("margin-top: ", gapAboveString) +
           cssProperty("margin-bottom: ", gapBelowString) +
           cssProperty("border-style", "none") +
           cssProperty("border-collapse", "collapse") +
           cssProperty("border-spacing", "0pt") +
           cssProperty("padding", "0pt") +
           "}\n" +
           "." + className + "-td1\n" +
           "{\n" +
           cssProperty("font-family: ", fontFamilyString) +
           cssProperty("font-size: ", fontSizeString) +
           cssProperty("font-style: ", fontStyleString) +
           cssProperty("font-weight: ", fontWeightString) +
           cssProperty("color: ", colorString) +
           cssProperty("background-color: ", backgroundColorString) +
           cssProperty("margin-left", "0pt") +
           cssProperty("margin-right", "0pt") +
           cssProperty("margin-top", "0pt") +
           cssProperty("margin-bottom", "0pt") +
           cssProperty("border-style", "none") +
           cssProperty("width: ", indentString) +
           cssProperty("border-spacing", "0pt") +
           cssProperty("padding-left", "0pt") +
           cssProperty("padding-right", "10pt") +
           cssProperty("padding-top", "0pt") +
           cssProperty("padding-botom", "0pt") +
           cssProperty("vertical-align", "top") +
           "}\n" +
           "." + className + "-td2\n" +
           "{\n" +
           cssProperty("font-family: ", fontFamilyString) +
           cssProperty("font-size: ", fontSizeString) +
           cssProperty("font-style: ", fontStyleString) +
           cssProperty("font-weight: ", fontWeightString) +
           cssProperty("color: ", colorString) +
           cssProperty("background-color: ", backgroundColorString) +
           cssProperty("margin-left", "0pt") +
           cssProperty("margin-right", "0pt") +
           cssProperty("margin-top", "0pt") +
           cssProperty("margin-bottom", "0pt") +
           cssProperty("border-style", "none") +
           cssProperty("border-spacing", "0pt") +
           cssProperty("padding", "0pt") +
           cssProperty("vertical-align", "top") +
           "}\n";
}

//  End of tt3-report/HtmlReportFormat._CssBuilder.cpp
