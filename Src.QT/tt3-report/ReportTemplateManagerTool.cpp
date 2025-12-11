//
//  tt3-report/ReportTemplateManagerTool.cpp - ReportTemplateManagerTool class implementation
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
TT3_IMPLEMENT_SINGLETON(ReportTemplateManagerTool)
ReportTemplateManagerTool::ReportTemplateManagerTool() {}
ReportTemplateManagerTool::~ReportTemplateManagerTool() {}

//////////
//  tt3::uti::ITool
ReportTemplateManagerTool::Mnemonic ReportTemplateManagerTool::mnemonic() const
{
    return M(ReportTemplateManager);
}

QString ReportTemplateManagerTool::displayName() const
{
    static Component::Resources *const resources = Component::Resources::instance();   //  idempotent
    return resources->string(RSID(ReportTemplateManagerTool), RID(DisplayName));
}

QString ReportTemplateManagerTool::description() const
{
    static Component::Resources *const resources = Component::Resources::instance();   //  idempotent
    return resources->string(RSID(ReportTemplateManagerTool), RID(Description));
}

QIcon ReportTemplateManagerTool::smallIcon() const
{
    static const QIcon icon(":/tt3-report/Resources/Images/Reports/ReportTemplateManagerSmall.png");
    return icon;
}

QIcon ReportTemplateManagerTool::largeIcon() const
{
    static const QIcon icon(":/tt3-report/Resources/Images/Reports/ReportTemplateManagerLarge.png");
    return icon;
}

bool ReportTemplateManagerTool::enabled() const
{
    return true;
}

void ReportTemplateManagerTool::run(QWidget * parent)
{
    Q_ASSERT(QThread::currentThread()->eventDispatcher() != nullptr);

    //  Use wait cursor when creating the dialog - the
    //  QWebEngineView it uses for previews needs a long
    //  time to initialize the WebEngine for the 1st time
    QApplication::setOverrideCursor(Qt::WaitCursor);
    QApplication::processEvents();
    ManageReportTemplatesDialog dlg(parent);
    QApplication::restoreOverrideCursor();

    dlg.doModal();
}

//  End of tt3-report/ReportTemplateManagerTool.cpp
