//
//  tt3-gui/SelectPublicTaskParentDialog.cpp - tt3::gui::SelectPublicTaskParentDialog class implementation
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
#include "ui_SelectPublicTaskParentDialog.h"
using namespace tt3::gui;

//////////
//  Construction/destruction
SelectPublicTaskParentDialog::SelectPublicTaskParentDialog(
        QWidget * parent,
        tt3::ws::PublicTask publicTask,
        const tt3::ws::Credentials & credentials
    ) : QDialog(parent),
        //  Implementation
        _publicTask(publicTask),
        _credentials(credentials),
        _selectedParentTask(publicTask->parent(credentials)),
        //  Controls
        _ui(new Ui::SelectPublicTaskParentDialog)
{
    _ui->setupUi(this);

    _ui->buttonBox->button(QDialogButtonBox::StandardButton::Ok)->
        setIcon(QIcon(":/tt3-gui/Resources/Images/Actions/OkSmall.png"));
    _ui->buttonBox->button(QDialogButtonBox::StandardButton::Cancel)->
        setIcon(QIcon(":/tt3-gui/Resources/Images/Actions/CancelSmall.png"));

    //  Set initial control values
    _ui->showCompletedTasksCheckBox->setChecked(
        _publicTask->completed(_credentials));

    //  Done
    adjustSize();
    _refresh();
}

SelectPublicTaskParentDialog::~SelectPublicTaskParentDialog()
{
    delete _ui;
}

//////////
//  Operations
auto SelectPublicTaskParentDialog::doModal(
    ) -> SelectPublicTaskParentDialog::Result
{
    return Result(this->exec());
}

//////////
//  Implementation helpers
void SelectPublicTaskParentDialog::_refresh()
{
}

//////////
//  Signal handlers
void SelectPublicTaskParentDialog::accept()
{
    done(int(Result::Ok));
}

void SelectPublicTaskParentDialog::reject()
{
    done(int(Result::Cancel));
}

//  End of tt3-gui/SelectPublicTaskParentDialog.cpp
