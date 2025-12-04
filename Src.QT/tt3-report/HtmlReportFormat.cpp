//
//  tt3-report/HtmlReportFormat.cpp - tt3::report::HtmlReportFormat class implementation
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
//  Singleton
TT3_IMPLEMENT_SINGLETON(HtmlReportFormat)
HtmlReportFormat::HtmlReportFormat() {}
HtmlReportFormat::~HtmlReportFormat() {}

//////////
//  IReportFormat
auto HtmlReportFormat::mnemonic() const -> Mnemonic
{
    return M(HTML);
}

auto HtmlReportFormat::displayName() const -> QString
{
    static Component::Resources *const resources = Component::Resources::instance();   //  idempotent
    return resources->string(RSID(HtmlReportFormat), RID(DisplayName));
}

QIcon HtmlReportFormat::smallIcon() const
{
    static const QIcon icon(":/tt3-report/Resources/Images/Reports/HtmlReportFormatSmall.png");
    return icon;
}

QIcon HtmlReportFormat::largeIcon() const
{
    static const QIcon icon(":/tt3-report/Resources/Images/Reports/HtmlReportFormatLarge.png");
    return icon;
}

QString HtmlReportFormat::preferredExtension() const
{
    return ".html";
}

//  End of tt3-report/HtmlReportFormat.cpp
