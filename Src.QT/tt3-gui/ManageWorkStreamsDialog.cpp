//
//  tt3-gui/ManageWorkStreamsDialog.cpp - tt3::gui::ManageWorkStreamsDialog class implementation
//  TODO translate UI via Resources
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
#include "ui_ManageWorkStreamsDialog.h"
using namespace tt3::gui;

//////////
//  Construction/destruction
ManageWorkStreamsDialog::ManageWorkStreamsDialog(
        QWidget * parent,
        tt3::ws::Workspace workspace,
        const tt3::ws::Credentials & credentials
    ) : QDialog(parent),
        _ui(new Ui::ManageWorkStreamsDialog)
{
    Q_ASSERT(workspace != nullptr);
    Q_ASSERT(credentials.isValid());

    _ui->setupUi(this);

    _ui->buttonBox->button(QDialogButtonBox::StandardButton::Ok)->
        setIcon(QIcon(":/tt3-gui/Resources/Images/Actions/OkSmall.png"));

    //  Create dynamic controls
    _managerPanelLayout = new QStackedLayout();
    _workStreamManager = new tt3::gui::WorkStreamManager(_ui->managerPanel);
    _managerPanelLayout->addWidget(_workStreamManager);
    _ui->managerPanel->setLayout(_managerPanelLayout);

    //  Adjust controls
    _workStreamManager->setWorkspace(workspace);
    _workStreamManager->setCredentials(credentials);

    //  Done
    adjustSize();
    _workStreamManager->refresh();
}

ManageWorkStreamsDialog::~ManageWorkStreamsDialog()
{
    delete _ui;
}

//////////
//  Operations
void ManageWorkStreamsDialog::doModal()
{
    exec();
}

//  End of tt3-gui/ManageWorkStreamsDialog.cpp
