//
//  tt3-report/ReportFormatManager.cpp - tt3::report::ReportFormatManager class implementation
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

struct ReportFormatManager::_Impl
{
    _Impl()
    {
#define REGISTER(ReportFormat)                      \
        registry.insert(                            \
            ReportFormat::instance()->mnemonic(),   \
            ReportFormat::instance())

            REGISTER(HtmlReportFormat);
    }

    using Registry = QMap<tt3::util::Mnemonic, IReportFormat*>;

    tt3::util::Mutex    guard;
    Registry            registry;
};

//////////
//  Operations
ReportFormats ReportFormatManager::allReportFormats()
{
    _Impl * impl = _impl();
    tt3::util::Lock _(impl->guard);

    QList<IReportFormat*> values = impl->registry.values();
    return QSet<IReportFormat*>(values.cbegin(), values.cend());
}

bool ReportFormatManager::registerReportFormat(IReportFormat * reportFormat)
{
    Q_ASSERT(reportFormat != nullptr);

    _Impl * impl = _impl();
    tt3::util::Lock _(impl->guard);

    if (impl->registry.contains(reportFormat->mnemonic()))
    {
        return reportFormat == impl->registry[reportFormat->mnemonic()];
    }
    impl->registry[reportFormat->mnemonic()] = reportFormat;
    return true;
}

bool ReportFormatManager::unregisterReportFormat(IReportFormat * reportFormat)
{
    Q_ASSERT(reportFormat != nullptr);

    _Impl * impl = _impl();
    tt3::util::Lock _(impl->guard);

    auto key = reportFormat->mnemonic();
    if (impl->registry.contains(key) &&
        impl->registry[key] == reportFormat)
    {
        impl->registry.remove(key);
        return true;
    }
    return false;
}

IReportFormat * ReportFormatManager::findReportFormat(const tt3::util::Mnemonic & mnemonic)
{
    _Impl * impl = _impl();
    tt3::util::Lock _(impl->guard);

    return impl->registry.contains(mnemonic) ? impl->registry[mnemonic] : nullptr;
}

//////////
//  Implementation helpers
ReportFormatManager::_Impl * ReportFormatManager::_impl()
{
    static _Impl impl;
    return &impl;
}

//  End of tt3-report/ReportFormatManager.cpp
