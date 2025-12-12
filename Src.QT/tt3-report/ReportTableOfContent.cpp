//
//  tt3-report/ReportTableOfContent.cpp - tt3::report::ReportTableOfContent class implementation
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
ReportTableOfContent::ReportTableOfContent(
        ReportFlowElement * parent
    ) : ReportBlockElement(parent)
{
}

ReportTableOfContent::~ReportTableOfContent()
{
}

//////////
//  ReportElement
auto ReportTableOfContent::resolveFontSpecs() const -> FontSpecs
{
    //  No own style - go to parent
    Q_ASSERT(_parent != nullptr);
    return _parent->resolveFontSpecs();
}

auto ReportTableOfContent::resolveFontSize() const -> TypographicSize
{
    //  No own style - go to parent
    Q_ASSERT(_parent != nullptr);
    return _parent->resolveFontSize();
}

auto ReportTableOfContent::resolveFontStyle() const -> FontStyle
{
    //  No own style - go to parent
    Q_ASSERT(_parent != nullptr);
    return _parent->resolveFontStyle();
}

auto ReportTableOfContent::resolveTextColor() const -> ColorSpec
{
    //  No own style - go to parent
    Q_ASSERT(_parent != nullptr);
    return _parent->resolveTextColor();
}

auto ReportTableOfContent::resolveBackgroundColor() const -> ColorSpec
{
    //  No own style - go to parent
    Q_ASSERT(_parent != nullptr);
    return _parent->resolveBackgroundColor();
}

//////////
//  ReportBlockElement
TypographicSize ReportTableOfContent::resolveLeftMargin() const
{   //  No own style - always use base implementation
    return ReportBlockElement::resolveLeftMargin();
}

TypographicSize ReportTableOfContent::resolveRightMargin() const
{   //  No own style - always use base implementation
    return ReportBlockElement::resolveRightMargin();
}

TypographicSize ReportTableOfContent::resolveGapAboven() const
{   //  No own style - always use base implementation
    return ReportBlockElement::resolveGapAboven();
}

TypographicSize ReportTableOfContent::resolveGapBelow() const
{   //  No own style - always use base implementation
    return ReportBlockElement::resolveGapBelow();
}

//////////
//  Serialization
void ReportTableOfContent::serialize(QDomElement & element) const
{
    ReportBlockElement::serialize(element);
}

//  End of tt3-report/ReportTableOfContent.cpp
