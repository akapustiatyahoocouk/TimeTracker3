//
//  tt3-gui/QuickReportManager.cpp - tt3::gui::QuickReportManager class implementation
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
#include "tt3-gui/API.hpp"
using namespace tt3::gui;

struct QuickReportManager::_Impl
{
    _Impl()
    {
#define REGISTER(QuickReport)                   \
        registry.insert(                        \
            QuickReport::instance()->mnemonic(),\
            QuickReport::instance())
        REGISTER(DailyWorkQuickReport);
    }

    using Registry = QMap<tt3::util::Mnemonic, IQuickReport*>;

    tt3::util::Mutex    guard;
    Registry            registry;
};

//////////
//  Operations
QSet<IQuickReport*> QuickReportManager::all()
{
    _Impl * impl = _impl();
    tt3::util::Lock _(impl->guard);

    QList<IQuickReport*> values = impl->registry.values();
    return QSet<IQuickReport*>(values.cbegin(), values.cend());
}

bool QuickReportManager::register(IQuickReport * quickReport)
{
    Q_ASSERT(quickReport != nullptr);

    _Impl * impl = _impl();
    tt3::util::Lock _(impl->guard);

    if (impl->registry.contains(quickReport->mnemonic()))
    {
        return quickReport == impl->registry[quickReport->mnemonic()];
    }
    impl->registry[quickReport->mnemonic()] = quickReport;
    return true;
}

IQuickReport * QuickReportManager::find(const tt3::util::Mnemonic & mnemonic)
{
    _Impl * impl = _impl();
    tt3::util::Lock _(impl->guard);

    return impl->registry.value(mnemonic, nullptr);
}

//////////
//  Implementation helpers
QuickReportManager::_Impl * QuickReportManager::_impl()
{
    static _Impl impl;
    return &impl;
}

//  End of tt3-gui/QuickReportManager.cpp
