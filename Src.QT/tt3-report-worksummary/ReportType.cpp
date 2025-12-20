//
//  tt3-report-worksummary/ReportType.cpp - ReportType class implementation
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
#include "tt3-report-worksummary/API.hpp"
using namespace tt3::report::worksummary;

//////////
//  Registration
TT3_IMPLEMENT_SINGLETON(ReportType)
ReportType::ReportType() {}
ReportType::~ReportType() {}

//////////
//  IReportType
ReportType::Mnemonic ReportType::mnemonic() const
{
    return M(WorkSummary);
}

QString ReportType::displayName() const
{
    static Component::Resources *const resources = Component::Resources::instance();   //  idempotent
    return resources->string(RSID(ReportType), RID(DisplayName));
}

QString ReportType::description() const
{
    static Component::Resources *const resources = Component::Resources::instance();   //  idempotent
    return resources->string(RSID(ReportType), RID(Description));
}

QIcon ReportType::smallIcon() const
{
    static const QIcon icon(":/tt3-report-worksummary/Resources/Images/Reports/WorkSummarySmall.png");
    return icon;
}

QIcon ReportType::largeIcon() const
{
    static const QIcon icon(":/tt3-report-worksummary/Resources/Images/Reports/WorkSummaryLarge.png");
    return icon;
}

auto ReportType::createConfigurationEditor(
        QWidget * parent
    ) -> ReportConfigurationEditor *
{
    return new ReportConfigurationEditor(parent);
}

auto ReportType::generateReport(
        const tt3::ws::Workspace & /*workspace*/,
        const tt3::ws::ReportCredentials & /*credentials*/,
        const tt3::report::IReportConfiguration * /*configuration*/,
        const tt3::report::IReportTemplate * reportTemplate,
        ProgressListener progressListener
    ) -> tt3::report::Report *
{
    for (int i = 0; i <= 100; i++)
    {
        if (progressListener != nullptr)
        {
            progressListener(float(i / 100.0));
        }
    }
    return new Report("TODO", reportTemplate);
}

//  End of tt3-report-worksummary/ReportType.cpp
