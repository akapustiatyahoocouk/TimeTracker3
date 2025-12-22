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

        REGISTER(BasicReportTemplate);

        for (const auto & kcrt : Component::Settings::instance()->knownCustomReportTemplates.value())
        {
            //  Load file as text (assuming it's XML)...
            QFile file(kcrt.location);
            if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
            {   //  OOPS! Ignore, though
                continue;
            }
            QTextStream in(&file);
            QString xml = in.readAll();
            file.close();
            //  Create report template from XML
            CustomReportTemplate * reportTemplate;
            try
            {
                reportTemplate = new CustomReportTemplate(xml);    //  may throw
            }
            catch (const tt3::util::Exception & ex)
            {   //  OOPS! Recover & continue
                qCritical() << ex;
                continue;
            }
            catch (...)
            {   //  OOPS! Recover & continue
                continue;
            }
            //  No duplication!
            if (registry.contains(reportTemplate->mnemonic()))
            {   //  OOPS! Ignore, though
                delete reportTemplate;
                continue;
            }
            //  Register (cannot fail at this point)
            registry.insert(reportTemplate->mnemonic(), reportTemplate);
        }
    }

    using Registry = QMap<tt3::util::Mnemonic, IReportTemplate*>;

    tt3::util::Mutex    guard;
    Registry            registry;
};

//////////
//  Operations
ReportTemplates ReportTemplateManager::all()
{
    _Impl * impl = _impl();
    tt3::util::Lock _(impl->guard);

    QList<IReportTemplate*> values = impl->registry.values();
    return QSet<IReportTemplate*>(values.cbegin(), values.cend());
}

bool ReportTemplateManager::register(IReportTemplate * reportTemplate)
{
    Q_ASSERT(reportTemplate != nullptr);

    _Impl * impl = _impl();
    tt3::util::Lock _(impl->guard);

    auto key = reportTemplate->mnemonic();
    if (impl->registry.contains(key))
    {
        return reportTemplate == impl->registry[reportTemplate->mnemonic()];
    }
    impl->registry[key] = reportTemplate;
    return true;
}

bool ReportTemplateManager::unregister(IReportTemplate * reportTemplate)
{
    Q_ASSERT(reportTemplate != nullptr);

    _Impl * impl = _impl();
    tt3::util::Lock _(impl->guard);

    auto key = reportTemplate->mnemonic();
    if (impl->registry.contains(key) &&
        impl->registry[key] == reportTemplate)
    {   //  Guard against an impersonator
        impl->registry.remove(key);
        return true;
    }
    return false;
}

IReportTemplate * ReportTemplateManager::find(const tt3::util::Mnemonic & mnemonic)
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
