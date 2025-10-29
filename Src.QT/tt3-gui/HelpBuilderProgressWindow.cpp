//
//  tt3-gui/HelpBuilderProgressWindow.cpp - tt3::gui::HelpBuilderProgressWindow class implementation
//  //  TODO localize via Resources
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
#include "ui_HelpBuilderProgressWindow.h"
using namespace tt3::gui;

//////////
//  Construction/destruction
HelpBuilderProgressWindow::HelpBuilderProgressWindow()
    :   QDialog(QApplication::activeWindow()),
        _ui(new Ui::HelpBuilderProgressWindow)
{
    _ui->setupUi(this);
    Qt::WindowFlags flags = windowFlags();
    flags |= Qt::CustomizeWindowHint;
    flags |= Qt::WindowStaysOnTopHint;
    flags &= ~Qt::WindowSystemMenuHint;
    flags &= ~Qt::WindowTitleHint;
    flags &= ~Qt::WindowCloseButtonHint;
    setWindowFlags(flags);

    _ui->contextLabel->setText("");
    _ui->actionLabel->setText("");
}

HelpBuilderProgressWindow::~HelpBuilderProgressWindow()
{
    delete _ui;
}

//////////
//  Operations
void HelpBuilderProgressWindow::reportProgress(const QString & context, const QString & action)
{
    _ui->contextLabel->setText(context);
    _ui->actionLabel->setText(action);
    QApplication::processEvents();
}

//  End of tt3-gui/HelpBuilderProgressWindow.cpp
