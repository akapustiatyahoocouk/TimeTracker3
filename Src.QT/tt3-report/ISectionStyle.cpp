//
//  tt3-report/ISectionStyle.cpp - tt3::report::ISectionStyle class implementation
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
//  Serialization
void ISectionStyle::serialize(QDomElement & element) const
{
    IStyle::serialize(element);

    IReportTemplate::_setAttribute(
        element,
        "PageNumberPlacement",
        this->pageNumberPlacement());
}

//  End of tt3-report/ISectionStyle.cpp
