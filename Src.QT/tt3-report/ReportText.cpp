//
//  tt3-report/ReportText.cpp - tt3::report::ReportText class implementation
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
ReportText::ReportText(
        ReportParagraph * paragraph,
        const QString & text,
        ICharacterStyle * style
    ) : ReportSpanElement(paragraph),
        _text(text),
        _style(style)
{
    Q_ASSERT(_style == nullptr ||
             _style->reportTemplate() == _report->reportTemplate());
}

ReportText::~ReportText()
{
}

//////////
//  ReportElement
auto ReportText::resolveFontSpecs() const -> FontSpecs
{
    if (_style != nullptr)
    {
        auto styleFontSpecs = _style->fontSpecs();
        if (!styleFontSpecs.has_value())
        {   //  Inherit from parent
            Q_ASSERT(_paragraph != nullptr);
            return _paragraph->resolveFontSpecs();
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
    Q_ASSERT(_paragraph != nullptr);
    return _paragraph->resolveFontSpecs();
}

auto ReportText::resolveFontSize() const -> TypographicSize
{
    if (_style != nullptr)
    {
        auto styleFontSize = _style->fontSize();
        if (!styleFontSize.has_value())
        {   //  Inherit from parent
            Q_ASSERT(_paragraph != nullptr);
            return _paragraph->resolveFontSize();
        }
        else
        {   //  Use value from style
            return styleFontSize.value();
        }
    }
    //  Style not specified - go to parent
    Q_ASSERT(_paragraph != nullptr);
    return _paragraph->resolveFontSize();
}

auto ReportText::resolveFontStyle() const -> FontStyle
{
    if (_style != nullptr)
    {
        auto styleFontStyle = _style->fontStyle();
        if (!styleFontStyle.has_value())
        {   //  Inherit from parent
            Q_ASSERT(_paragraph != nullptr);
            return _paragraph->resolveFontStyle();
        }
        else
        {   //  Use value from style
            return styleFontStyle.value();
        }
    }
    //  Style not specified - go to parent
    Q_ASSERT(_paragraph != nullptr);
    return _paragraph->resolveFontStyle();
}

auto ReportText::resolveTextColor() const -> ColorSpec
{
    if (_style != nullptr)
    {
        auto styleTextColor = _style->textColor();
        if (!styleTextColor.has_value())
        {   //  Inherit from parent
            Q_ASSERT(_paragraph != nullptr);
            return _paragraph->resolveTextColor();
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
    Q_ASSERT(_paragraph != nullptr);
    return _paragraph->resolveTextColor();
}

auto ReportText::resolveBackgroundColor() const -> ColorSpec
{
    if (_style != nullptr)
    {
        auto styleBackgroundColor = _style->backgroundColor();
        if (!styleBackgroundColor.has_value())
        {   //  Inherit from parent
            Q_ASSERT(_paragraph != nullptr);
            return _paragraph->resolveBackgroundColor();
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
    Q_ASSERT(_paragraph != nullptr);
    return _paragraph->resolveBackgroundColor();
}

//////////
//  Operations
void ReportText::setStyle(ICharacterStyle * style)
{
    Q_ASSERT(style == nullptr ||
             style->reportTemplate() == _report->reportTemplate());

    if (style == nullptr ||
        style->reportTemplate() == _report->reportTemplate())
    {   //  Be defensive in Release mode
        _style = style;
    }
}

//////////
//  Serialization
void ReportText::serialize(QDomElement & element) const
{
    ReportSpanElement::serialize(element);

    element.setAttribute("Text", _text);
    if (_style != nullptr)
    {
        element.setAttribute("Style", _style->name().toString());
    }
}

//  End of tt3-report/ReportText.cpp
