//
//  tt3-report/ReportSection.cpp - tt3::report::ReportSection class implementation
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
ReportSection::ReportSection(
        Report * report,
        const QString & name,
        ISectionStyle * style
    ) : ReportFlowElement(report),
        _name(name),
        _style(style)
{
    Q_ASSERT(_style == nullptr ||
             _style->reportTemplate() == report->reportTemplate());

    //  Add to parent(s)
    _report->_sections.append(this);
}

ReportSection::~ReportSection()
{
    //  Remove from parent(s)
    Q_ASSERT(_report->_sections.contains(this));
    _report->_sections.removeAll(this);
}

//////////
//  ReportElement
auto ReportSection::resolveFontSpecs() const -> FontSpecs
{
    if (_style != nullptr)
    {
        auto styleFontSpecs = _style->fontSpecs();
        //  empty list == inherit from parent, but
        //  There IS NO parent!
        if (styleFontSpecs.has_value() &&
            !styleFontSpecs.value().isEmpty())
        {
            return styleFontSpecs.value();
        }
    }
    //  Style not specified - go to parent or template defaults
    //  EXCEPT THERE IS NO PARENT!
    return _report->_reportTemplate->defaultFontSpecs();
}

auto ReportSection::resolveFontSize() const -> TypographicSize
{
    if (_style != nullptr)
    {
        auto styleFontSize = _style->fontSize();
        if (styleFontSize.has_value())
        {
            return styleFontSize.value();
        }
    }
    //  Style not specified - go to parent or template defaults
    //  EXCEPT THERE IS NO PARENT!
    return _report->_reportTemplate->defaultFontSize();
}

auto ReportSection::resolveFontStyle() const -> FontStyle
{
    if (_style != nullptr)
    {
        auto styleFontStyle = _style->fontStyle();
        if (styleFontStyle.has_value())
        {
            return styleFontStyle.value();
        }
    }
    //  Style not specified - go to parent or template defaults
    //  EXCEPT THERE IS NO PARENT!
    return _report->_reportTemplate->defaultFontStyle();
}

auto ReportSection::resolveTextColor() const -> ColorSpec
{
    if (_style != nullptr)
    {
        auto styleTextColor = _style->textColor();
        if (styleTextColor.has_value())
        {
            if (styleTextColor.value().colorClass() == ColorSpec::Default)
            {   //  Go directly to report template
                return _report->_reportTemplate->defaultTextColor();
            }
            return styleTextColor.value();
        }
    }
    //  Style not specified - go to parent or template defaults
    //  EXCEPT THERE IS NO PARENT!
    return _report->_reportTemplate->defaultTextColor();
}

auto ReportSection::resolveBackgroundColor() const -> ColorSpec
{
    if (_style != nullptr)
    {
        auto styleBackgroundColor = _style->backgroundColor();
        if (styleBackgroundColor.has_value())
        {
            if (styleBackgroundColor.value().colorClass() == ColorSpec::Default)
            {   //  Go directly to report template
                return _report->_reportTemplate->defaultBackgroundColor();
            }
            return styleBackgroundColor.value();
        }
    }
    //  Style not specified - go to parent or template defaults
    //  EXCEPT THERE IS NO PARENT!
    return _report->_reportTemplate->defaultBackgroundColor();
}

//////////
//  Operations
void ReportSection::setStyle(ISectionStyle * style)
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
void ReportSection::serialize(QDomElement & element) const
{
    ReportFlowElement::serialize(element);

    element.setAttribute("Name", _name);
    if (_style != nullptr)
    {
        element.setAttribute("Style", _style->name().toString());
    }
}

//  End of tt3-report/ReportSection.cpp
