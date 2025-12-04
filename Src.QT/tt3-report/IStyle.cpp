//
//  tt3-report/IStyle.cpp - tt3::report::IStyle class implementation
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
//  Imlementation helpers
void IStyle::_storeAttributes(QDomElement & element) const
{
    IReportTemplate::_setAttribute(
        element,
        "Name",
        this->name());
    IReportTemplate::_setAttribute(
        element,
        "FontSpecs",
        this->fontSpecs());
    IReportTemplate::_setAttribute(
        element,
        "FontSize",
        this->fontSize());
    IReportTemplate::_setAttribute(
        element,
        "FontStyle",
        this->fontStyle());
    IReportTemplate::_setAttribute(
        element,
        "TextColor",
        this->textColor());
    IReportTemplate::_setAttribute(
        element,
        "BackgroundColor",
        this->backgroundColor());
    IReportTemplate::_setAttribute(
        element,
        "UnderlineMode",
        this->underlineMode());
}

//  End of tt3-report/IStyle.cpp
