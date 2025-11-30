//
//  tt3-report/ReportTemplateManager.cpp - tt3::report::ReportTemplateManager class implementation
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

struct ReportTemplateManager::_Impl
{
    _Impl()
    {
#define REGISTER(ReportTemplate)                    \
        registry.insert(                            \
            ReportTemplate::instance()->mnemonic(), \
            ReportTemplate::instance())

        //  TODO standard report templates REGISTER(StandardReportTemplates::Dark);
    }

    tt3::util::Mutex                    guard;
    QMap<tt3::util::Mnemonic, IReportTemplate*> registry;
};

//////////
//  Operations
QSet<IReportTemplate*> ReportTemplateManager::allReportTemplates()
{
    _Impl * impl = _impl();
    tt3::util::Lock _(impl->guard);

    QList<IReportTemplate*> values = impl->registry.values();
    return QSet<IReportTemplate*>(values.cbegin(), values.cend());
}

bool ReportTemplateManager::registerReportTemplate(IReportTemplate * reportTemplate)
{
    Q_ASSERT(reportTemplate != nullptr);

    _Impl * impl = _impl();
    tt3::util::Lock _(impl->guard);

    if (impl->registry.contains(reportTemplate->mnemonic()))
    {
        return reportTemplate == impl->registry[reportTemplate->mnemonic()];
    }
    impl->registry[reportTemplate->mnemonic()] = reportTemplate;
    return true;
}

IReportTemplate * ReportTemplateManager::findReportTemplate(const tt3::util::Mnemonic & mnemonic)
{
    _Impl * impl = _impl();
    tt3::util::Lock _(impl->guard);

    return impl->registry.contains(mnemonic) ? impl->registry[mnemonic] : nullptr;
}

//////////
//  Implementation helpers
ReportTemplateManager::_Impl * ReportTemplateManager::_impl()
{
    static _Impl impl;
    return &impl;
}

//  End of tt3-report/ReportTemplateManager.cpp
