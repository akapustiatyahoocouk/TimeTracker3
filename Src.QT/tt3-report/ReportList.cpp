//
//  tt3-report/ReportList.cpp - tt3::report::ReportList class implementation
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
ReportList::ReportList(
        ReportFlowElement * parent,
        IListStyle * style
    ) : ReportBlockElement(parent),
        _style(style)
{
    Q_ASSERT(_style == nullptr ||
             _style->reportTemplate() == _report->reportTemplate());
}

ReportList::~ReportList()
{
    for (auto item : ReportListItems(_items))   //  shallow clone
    {
        delete item;    //  removes ListItem from List
    }
}

//////////
//  ReportElement
auto ReportList::resolveFontSpecs() const -> FontSpecs
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

auto ReportList::resolveFontSize() const -> TypographicSize
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

auto ReportList::resolveFontStyle() const -> FontStyle
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

auto ReportList::resolveTextColor() const -> ColorSpec
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

auto ReportList::resolveBackgroundColor() const -> ColorSpec
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
//  ReportBlockElement
TypographicSize ReportList::resolveLeftMargin() const
{
    //  Honor own style first
    if (_style != nullptr && _style->leftMargin().has_value())
    {
        return _style->leftMargin().value();
    }
    //  The rest is up to the base implementation
    return ReportBlockElement::resolveLeftMargin();
}

TypographicSize ReportList::resolveRightMargin() const
{
    //  Honour own style first
    if (_style != nullptr && _style->rightMargin().has_value())
    {
        return _style->rightMargin().value();
    }
    //  The rest is up to the base implementation
    return ReportBlockElement::resolveRightMargin();
}

TypographicSize ReportList::resolveGapAboven() const
{
    //  Honor own style first
    if (_style != nullptr && _style->gapAbove().has_value())
    {
        return _style->gapAbove().value();
    }
    //  The rest is up to the base implementation
    return ReportBlockElement::resolveGapAboven();
}

TypographicSize ReportList::resolveGapBelow() const
{
    //  Honor own style first
    if (_style != nullptr && _style->gapBelow().has_value())
    {
        return _style->gapBelow().value();
    }
    //  The rest is up to the base implementation
    return ReportBlockElement::resolveGapBelow();
}

//////////
//  Operations
TypographicSize ReportList::resolveIndent() const
{
    //  Honor own style first
    if (_style != nullptr && _style->indent().has_value())
    {
        return _style->indent().value();
    }
    //  Must go to the parent list
    for (ReportElement * parent = this->parent();
         parent != nullptr;
         parent = parent->parent())
    {
        if (auto parentList =
            dynamic_cast<ReportList*>(parent))
        {
            return parentList->resolveIndent();
        }
    }
    //  No ancestor list was any good
    return _report->_reportTemplate->defaultListIndent();
}

ReportListItem * ReportList::createItem(const QString & label)
{
#ifdef QT_DEBUG
    _report->_validate();
#endif
    auto result = new ReportListItem(this, label);
#ifdef QT_DEBUG
    _report->_validate();
#endif
    return result;
}

//  End of tt3-report/ReportList.cpp
