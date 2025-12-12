//
//  tt3-report/ReportLink.cpp - tt3::report::ReportLink class implementation
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
//  Construction/destruction
ReportLink::ReportLink(
        ReportSpanElement * spanElement,
        ILinkStyle * style
    ) : ReportElement(spanElement->_report),
        _spanElement(spanElement),
        _style(style)
{
    Q_ASSERT(_style == nullptr ||
             _style->reportTemplate() == _report->reportTemplate());

    //  Add to parents
    _report->_links.append(this);
    Q_ASSERT(_spanElement->_link == nullptr);
    _spanElement->_link = this;
}

ReportLink::~ReportLink()
{
    //  Remove from parents
    Q_ASSERT(_report->_links.contains(this));
    _report->_links.removeAll(this);
    Q_ASSERT(_spanElement->_link == this);
    _spanElement->_link = nullptr;
}

//////////
//  ReportElement
auto ReportLink::resolveFontSpecs() const -> FontSpecs
{
    if (_style != nullptr)
    {
        auto styleFontSpecs = _style->fontSpecs();
        if (!styleFontSpecs.has_value())
        {   //  Inherit from parent
            Q_ASSERT(_spanElement != nullptr);
            return _spanElement->resolveFontSpecs();
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
    Q_ASSERT(_spanElement != nullptr);
    return _spanElement->resolveFontSpecs();
}

auto ReportLink::resolveFontSize() const -> TypographicSize
{
    if (_style != nullptr)
    {
        auto styleFontSize = _style->fontSize();
        if (!styleFontSize.has_value())
        {   //  Inherit from parent
            Q_ASSERT(_spanElement != nullptr);
            return _spanElement->resolveFontSize();
        }
        else
        {   //  Use value from style
            return styleFontSize.value();
        }
    }
    //  Style not specified - go to parent
    Q_ASSERT(_spanElement != nullptr);
    return _spanElement->resolveFontSize();
}

auto ReportLink::resolveFontStyle() const -> FontStyle
{
    if (_style != nullptr)
    {
        auto styleFontStyle = _style->fontStyle();
        if (!styleFontStyle.has_value())
        {   //  Inherit from parent
            Q_ASSERT(_spanElement != nullptr);
            return _spanElement->resolveFontStyle();
        }
        else
        {   //  Use value from style
            return styleFontStyle.value();
        }
    }
    //  Style not specified - go to parent
    Q_ASSERT(_spanElement != nullptr);
    return _spanElement->resolveFontStyle();
}

auto ReportLink::resolveTextColor() const -> ColorSpec
{
    if (_style != nullptr)
    {
        auto styleTextColor = _style->textColor();
        if (!styleTextColor.has_value())
        {   //  Inherit from parent
            Q_ASSERT(_spanElement != nullptr);
            return _spanElement->resolveTextColor();
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
    Q_ASSERT(_spanElement != nullptr);
    return _spanElement->resolveTextColor();
}

auto ReportLink::resolveBackgroundColor() const -> ColorSpec
{
    if (_style != nullptr)
    {
        auto styleBackgroundColor = _style->backgroundColor();
        if (!styleBackgroundColor.has_value())
        {   //  Inherit from parent
            Q_ASSERT(_spanElement != nullptr);
            return _spanElement->resolveBackgroundColor();
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
    Q_ASSERT(_spanElement != nullptr);
    return _spanElement->resolveBackgroundColor();
}

//////////
//  Operations
void ReportLink::setStyle(ILinkStyle * style)
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
void ReportLink::serialize(QDomElement & element) const
{
    ReportElement::serialize(element);

    if (_style != nullptr)
    {
        element.setAttribute("Style", _style->name().toString());
    }
}

//  End of tt3-report/ReportLink.cpp
