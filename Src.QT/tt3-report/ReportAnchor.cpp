//
//  tt3-report/ReportAnchor.cpp - tt3::report::ReportAnchor class implementation
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
ReportAnchor::ReportAnchor(
        ReportElement * anchoredElement
    ) : ReportElement(anchoredElement->_report),
        _anchoredElement(anchoredElement)
{
    //  Add to parents
    _report->_anchors.append(this);
    _anchoredElement->_anchors.append(this);
}

ReportAnchor::~ReportAnchor()
{
    //  Destroy all internal links referring to this Anchor
    for (auto link : ReportLinks(_report->_links))  //  shallow clone
    {
        if (auto internalLink =
            dynamic_cast<ReportInternalLink*>(link))
        {
            if (internalLink->_anchor == this)
            {
                delete internalLink;
            }
        }
    }

    //  Remove from parents
    Q_ASSERT(_report->_anchors.contains(this));
    _report->_anchors.removeAll(this);
    Q_ASSERT(_anchoredElement->_anchors.contains(this));
    _anchoredElement->_anchors.removeAll(this);
}

//  End of tt3-report/ReportAnchor.cpp
