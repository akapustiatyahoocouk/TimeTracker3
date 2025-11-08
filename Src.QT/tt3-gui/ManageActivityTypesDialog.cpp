//
//  tt3-gui/ManageActivityTypesDialog.cpp - tt3::gui::ManageActivityTypesDialog class implementation
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
#include "ui_ManageActivityTypesDialog.h"
using namespace tt3::gui;

//////////
//  Construction/destruction
ManageActivityTypesDialog::ManageActivityTypesDialog(
        QWidget * parent,
        tt3::ws::Workspace workspace,
        const tt3::ws::Credentials & credentials
    ) : QDialog(parent),
        _ui(new Ui::ManageActivityTypesDialog)
{
    tt3::util::ResourceReader rr(Component::Resources::instance(), RSID(ManageActivityTypesDialog));

    Q_ASSERT(workspace != nullptr);
    Q_ASSERT(credentials.isValid());

    _ui->setupUi(this);
    setWindowTitle(rr.string(RID(Title)));

    _ui->buttonBox->button(QDialogButtonBox::StandardButton::Ok)->
        setText(rr.string(RID(OkPushButton)));
    _ui->buttonBox->button(QDialogButtonBox::StandardButton::Ok)->
        setIcon(QIcon(":/tt3-gui/Resources/Images/Actions/OkSmall.png"));

    //  Create dynamic controls
    _managerPanelLayout = new QStackedLayout();
    _activityTypeManager = new tt3::gui::ActivityTypeManager(_ui->managerPanel);
    _managerPanelLayout->addWidget(_activityTypeManager);
    _ui->managerPanel->setLayout(_managerPanelLayout);

    //  Adjust controls
    _activityTypeManager->setWorkspace(workspace);
    _activityTypeManager->setCredentials(credentials);

    //  Done
    _activityTypeManager->refresh();
    adjustSize();
}

ManageActivityTypesDialog::~ManageActivityTypesDialog()
{
    delete _ui;
}

//////////
//  Operations
void ManageActivityTypesDialog::doModal()
{
    exec();
}

//  End of tt3-gui/ManageActivityTypesDialog.cpp
