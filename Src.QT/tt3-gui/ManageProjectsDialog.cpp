//
//  tt3-gui/ManageProjectsDialog.cpp - tt3::gui::ManageProjectsDialog class implementation
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
#include "ui_ManageProjectsDialog.h"
using namespace tt3::gui;

//////////
//  Construction/destruction
ManageProjectsDialog::ManageProjectsDialog(
        QWidget * parent,
        tt3::ws::Workspace workspace,
        const tt3::ws::Credentials & credentials
    ) : QDialog(parent),
        _ui(new Ui::ManageProjectsDialog)
{
    tt3::util::ResourceReader rr(Component::Resources::instance(), RSID(ManageProjectsDialog));

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
    _projectManager = new tt3::gui::ProjectManager(_ui->managerPanel);
    _managerPanelLayout->addWidget(_projectManager);
    _ui->managerPanel->setLayout(_managerPanelLayout);

    //  Adjust controls
    _projectManager->setWorkspace(workspace);
    _projectManager->setCredentials(credentials);

    //  Done
    _projectManager->refresh();
    adjustSize();
}

ManageProjectsDialog::~ManageProjectsDialog()
{
    delete _ui;
}

//////////
//  Operations
void ManageProjectsDialog::doModal()
{
    exec();
}

//  End of tt3-gui/ManagePsDialog.cpp
