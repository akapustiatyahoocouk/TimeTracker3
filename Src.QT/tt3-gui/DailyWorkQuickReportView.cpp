//
//  tt3-gui/DailyWorkQuickReportView.cpp - tt3::gui::DailyWorkQuickReportView class implementation
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
#include "ui_DailyWorkQuickReportView.h"
using namespace tt3::gui;

namespace tt3::gui
{
    extern CurrentCredentials theCurrentCredentials;
    extern CurrentWorkspace theCurrentWorkspace;
}

//////////
//  Construction/destruction
DailyWorkQuickReportView::DailyWorkQuickReportView(QWidget *parent)
    :   QuickReportView(parent),
        _ui(new Ui::DailyWorkQuickReportView)
{
    _ui->setupUi(this);
}

DailyWorkQuickReportView::~DailyWorkQuickReportView()
{
    delete _ui;
}

//////////
//  QuickReportView
IQuickReport * DailyWorkQuickReportView::quickReport() const
{
    return StandardQuickReports::DailyWork::instance();
}

//  End of tt3-gui/DailyWorkQuickReportView.cpp
