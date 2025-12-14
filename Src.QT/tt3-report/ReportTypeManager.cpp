//
//  tt3-report/ReportTypeManager.cpp - tt3::report::ReportTypeManager class implementation
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

struct ReportTypeManager::_Impl
{
    using Registry = QMap<tt3::util::Mnemonic, IReportType*>;

    tt3::util::Mutex    guard;
    Registry            registry;
};

//////////
//  Operations
ReportTypes ReportTypeManager::allReportTypes()
{
    _Impl * impl = _impl();
    tt3::util::Lock _(impl->guard);

    QList<IReportType*> values = impl->registry.values();
    return QSet<IReportType*>(values.cbegin(), values.cend());
}

bool ReportTypeManager::registerReportType(IReportType * reportType)
{
    Q_ASSERT(reportType != nullptr);

    _Impl * impl = _impl();
    tt3::util::Lock _(impl->guard);

    if (impl->registry.contains(reportType->mnemonic()))
    {
        return reportType == impl->registry[reportType->mnemonic()];
    }
    impl->registry[reportType->mnemonic()] = reportType;
    return true;
}

IReportType * ReportTypeManager::findReportType(const tt3::util::Mnemonic & mnemonic)
{
    _Impl * impl = _impl();
    tt3::util::Lock _(impl->guard);

    return impl->registry.contains(mnemonic) ? impl->registry[mnemonic] : nullptr;
}

//////////
//  Implementation helpers
ReportTypeManager::_Impl * ReportTypeManager::_impl()
{
    static _Impl impl;
    return &impl;
}

//  End of tt3-report/ReportTypeManager.cpp
