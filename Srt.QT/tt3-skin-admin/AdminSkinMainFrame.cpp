//
//  tt3-skin-admin/AdminSkinMainFrame.cpp - AdminSkinMainFrame class implementation
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
#include "tt3-skin-admin/API.hpp"
#include "ui_AdminSkinMainFrame.h"
using namespace skin::admin;

AdminSkinMainFrame::AdminSkinMainFrame(QWidget * parent)
    :   QMainWindow(parent),
        _ui(new Ui::AdminSkinMainFrame)
{
    _ui->setupUi(this);
}

AdminSkinMainFrame::~AdminSkinMainFrame()
{
    delete _ui;
}

//  End of tt3-skin-admin/AdminSkinMainFrame.cpp
