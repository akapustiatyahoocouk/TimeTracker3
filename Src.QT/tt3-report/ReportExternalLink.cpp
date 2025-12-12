//
//  tt3-report/ReportExternalLink.cpp - tt3::report::ReportExternalLink class implementation
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
ReportExternalLink::ReportExternalLink(
        ReportSpanElement * spanElement,
        ILinkStyle * style,
        const QString & url
    ) : ReportLink(spanElement, style),
        _url(url)
{
}

ReportExternalLink::~ReportExternalLink()
{
}

//////////
//  Serialization
void ReportExternalLink::serialize(QDomElement & element) const
{
    ReportLink::serialize(element);

    element.setAttribute("URL", _url);
}

//  End of tt3-report/ReportExternalLink.cpp
