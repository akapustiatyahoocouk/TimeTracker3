//
//  tt3-report/ReportParagraph.cpp - tt3::report::ReportParagraph class implementation
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
ReportParagraph::ReportParagraph(
        ReportFlowElement * parent,
        IParagraphStyle * style
    ) : ReportBlockElement(parent),
        _style(style)
{
    Q_ASSERT(_style == nullptr ||
             _style->reportTemplate() == _report->reportTemplate());
}

ReportParagraph::~ReportParagraph()
{
    for (auto child : ReportSpanElements(_children))    //  shallow clone
    {
        delete child;   //  removes SpanElement from Paragraph
    }
}

//////////
//  ReportElement
auto ReportParagraph::resolveFontSpecs() const -> FontSpecs
{
    if (_style != nullptr)
    {
        auto styleFontSpecs = _style->fontSpecs();
        if (!styleFontSpecs.has_value())
        {   //  Inherit from parent
            Q_ASSERT(_parent != nullptr);
            return _parent->resolveFontSpecs();
        }
        else if (styleFontSpecs.value().isEmpty())
        {   //  Go directly to template
            return _report->_reportTemplate->defaultFontSpecs();
        }
        else
        {   //  Use value from style
            return styleFontSpecs.value();
        }
    }
    //  Style not specified - go to parent
    Q_ASSERT(_parent != nullptr);
    return _parent->resolveFontSpecs();
}

auto ReportParagraph::resolveFontSize() const -> TypographicSize
{
    if (_style != nullptr)
    {
        auto styleFontSize = _style->fontSize();
        if (!styleFontSize.has_value())
        {   //  Inherit from parent
            Q_ASSERT(_parent != nullptr);
            return _parent->resolveFontSize();
        }
        else
        {   //  Use value from style
            return styleFontSize.value();
        }
    }
    //  Style not specified - go to parent
    Q_ASSERT(_parent != nullptr);
    return _parent->resolveFontSize();
}

auto ReportParagraph::resolveFontStyle() const -> FontStyle
{
    if (_style != nullptr)
    {
        auto styleFontStyle = _style->fontStyle();
        if (!styleFontStyle.has_value())
        {   //  Inherit from parent
            Q_ASSERT(_parent != nullptr);
            return _parent->resolveFontStyle();
        }
        else
        {   //  Use value from style
            return styleFontStyle.value();
        }
    }
    //  Style not specified - go to parent
    Q_ASSERT(_parent != nullptr);
    return _parent->resolveFontStyle();
}

auto ReportParagraph::resolveTextColor() const -> ColorSpec
{
    if (_style != nullptr)
    {
        auto styleTextColor = _style->textColor();
        if (!styleTextColor.has_value())
        {   //  Inherit from parent
            Q_ASSERT(_parent != nullptr);
            return _parent->resolveTextColor();
        }
        else if (styleTextColor.value() == ColorSpec::Default)
        {   //  Go directly to report template
            return _report->_reportTemplate->defaultTextColor();
        }
        else
        {   //  Use value from style
            return styleTextColor.value();
        }
    }
    //  Style not specified - go to parent
    Q_ASSERT(_parent != nullptr);
    return _parent->resolveTextColor();
}

auto ReportParagraph::resolveBackgroundColor() const -> ColorSpec
{
    if (_style != nullptr)
    {
        auto styleBackgroundColor = _style->backgroundColor();
        if (!styleBackgroundColor.has_value())
        {   //  Inherit from parent
            Q_ASSERT(_parent != nullptr);
            return _parent->resolveBackgroundColor();
        }
        else if (styleBackgroundColor.value() == ColorSpec::Default)
        {   //  Go directly to report template
            return _report->_reportTemplate->defaultBackgroundColor();
        }
        else
        {   //  Use value from style
            return styleBackgroundColor.value();
        }
    }
    //  Style not specified - go to parent
    Q_ASSERT(_parent != nullptr);
    return _parent->resolveBackgroundColor();
}

//////////
//  Operations
void ReportParagraph::setStyle(IParagraphStyle * style)
{
    Q_ASSERT(style == nullptr ||
             style->reportTemplate() == _report->reportTemplate());

    _style = style;
}

ReportText * ReportParagraph::createText(
        const QString & text,
        ICharacterStyle * style
    )
{
#ifdef QT_DEBUG
    _report->_validate();
#endif
    Q_ASSERT(style == nullptr ||
             style->reportTemplate() == _report->reportTemplate());

    auto result = new ReportText(this, text, style);
#ifdef QT_DEBUG
    _report->_validate();
#endif
    return result;
}

ReportPicture * ReportParagraph::createPicture(
        const TypographicSize & width,
        const TypographicSize & height,
        const QImage & image
    )
{
#ifdef QT_DEBUG
    _report->_validate();
#endif
    auto result = new ReportPicture(this, width, height, image);
#ifdef QT_DEBUG
    _report->_validate();
#endif
    return result;
}

//  End of tt3-report/ReportParagraph.cpp

