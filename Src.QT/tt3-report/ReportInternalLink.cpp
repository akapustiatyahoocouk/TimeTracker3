//
//  tt3-report/ReportInternalLink.cpp - tt3::report::ReportInternalLink class implementation
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
ReportInternalLink::ReportInternalLink(
        ReportSpanElement * spanElement,
        ILinkStyle * style,
        ReportAnchor * anchor
    ) : ReportLink(spanElement, style),
        _anchor(anchor)
{
    Q_ASSERT(_anchor != nullptr &&
             _anchor->_report == this->_report);
}

ReportInternalLink::~ReportInternalLink()
{   //  Destrouing the link doea NOT destroy the nchor!
}

//////////
//  Serialization
void ReportInternalLink::serialize(QDomElement & element) const
{
    ReportLink::serialize(element);

    element.setAttribute(
        "AnchorID",
        tt3::util::toString(static_cast<const void *>(_anchor)));
}

//  End of tt3-report/ReportInternalLink.cpp
