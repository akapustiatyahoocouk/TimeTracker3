//
//  tt3-gui/StandardQuickReports.cpp - tt3::gui::StandardQuickReports class implementation
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
//  StandardQuickReports::DailyWork
IMPLEMENT_SINGLETON(StandardQuickReports::DailyWork)
StandardQuickReports::DailyWork::DailyWork() {}
StandardQuickReports::DailyWork::~DailyWork() {}

StandardQuickReports::DailyWork::Mnemonic StandardQuickReports::DailyWork::mnemonic() const
{
    return M(DailyWork);
}

QString StandardQuickReports::DailyWork::displayName() const
{
    return Component::Resources::instance()->string(
        RSID(StandardQuickReports), RID(DailyWork.DisplayName));
}

QString StandardQuickReports::DailyWork::description() const
{
    return Component::Resources::instance()->string(
        RSID(StandardQuickReports), RID(DailyWork.Description));
}

QIcon StandardQuickReports::DailyWork::smallIcon() const
{
    static const QIcon icon(":/tt3-gui/Resources/Images/Reports/DailyWorkLarge.png");
    return icon;
}

QIcon StandardQuickReports::DailyWork::largeIcon() const
{
    static const QIcon icon(":/tt3-gui/Resources/Images/Reports/DailyWorkSmall.png");
    return icon;
}

auto StandardQuickReports::DailyWork::createView(
        QWidget * parent
    ) -> QuickReportView *
{
    return new DailyWorkQuickReportView(parent);
}

//  End of tt3-gui/StandardQuickReports.cpp
