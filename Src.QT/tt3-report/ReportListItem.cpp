//
//  tt3-report/ReportListItem.cpp - tt3::report::ReportListItem class implementation
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
ReportListItem::ReportListItem(
        ReportList * list,
        const QString & label
    ) : ReportFlowElement(list->_report),
        _list(list),
        _label(label)
{
    //  Add to parents
    _list->_items.append(this);
}

ReportListItem::~ReportListItem()
{
    //  Remove from parents
    _list->_items.removeAll(this);
}

//////////
//  ReportElement
auto ReportListItem::resolveFontSpecs() const -> FontSpecs
{
    //  No own style - go to parent
    Q_ASSERT(_list != nullptr);
    return _list->resolveFontSpecs();
}

auto ReportListItem::resolveFontSize() const -> TypographicSize
{
    //  No own style - go to parent
    Q_ASSERT(_list != nullptr);
    return _list->resolveFontSize();
}

auto ReportListItem::resolveFontStyle() const -> FontStyle
{
    //  No own style - go to parent
    Q_ASSERT(_list != nullptr);
    return _list->resolveFontStyle();
}

auto ReportListItem::resolveTextColor() const -> ColorSpec
{
    //  No own style - go to parent
    Q_ASSERT(_list != nullptr);
    return _list->resolveTextColor();
}

auto ReportListItem::resolveBackgroundColor() const -> ColorSpec
{
    //  No own style - go to parent
    Q_ASSERT(_list != nullptr);
    return _list->resolveBackgroundColor();
}

//  End of tt3-report/ReportListItem.cpp
