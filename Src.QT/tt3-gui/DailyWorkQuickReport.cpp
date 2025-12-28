//
//  tt3-gui/DailyWorkQuickReport.cpp - tt3::gui::DailyWorkQuickReport class implementation
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

//////////
//  DailyWorkQuickReport
TT3_IMPLEMENT_SINGLETON(DailyWorkQuickReport)
DailyWorkQuickReport::DailyWorkQuickReport() {}
DailyWorkQuickReport::~DailyWorkQuickReport() {}

DailyWorkQuickReport::Mnemonic DailyWorkQuickReport::mnemonic() const
{
    return M(DailyWork);
}

QString DailyWorkQuickReport::displayName() const
{
    return Component::Resources::instance()->string(
        RSID(DailyWorkQuickReport), RID(DisplayName));
}

QString DailyWorkQuickReport::description() const
{
    return Component::Resources::instance()->string(
        RSID(DailyWorkQuickReport), RID(Description));
}

QIcon DailyWorkQuickReport::smallIcon() const
{
    static const QIcon icon(":/tt3-gui/Resources/Images/Reports/DailyWorkLarge.png");
    return icon;
}

QIcon DailyWorkQuickReport::largeIcon() const
{
    static const QIcon icon(":/tt3-gui/Resources/Images/Reports/DailyWorkSmall.png");
    return icon;
}

auto DailyWorkQuickReport::createView(
        QWidget * parent
    ) -> QuickReportView *
{
    return new DailyWorkQuickReportView(parent);
}

//  End of tt3-gui/DailyWorkQuickReport.cpp
