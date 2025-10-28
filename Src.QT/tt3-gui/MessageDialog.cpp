//
//  tt3-gui/MessageDialog.cpp - tt3::gui::MessageDialog class implementation (+specializations)
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
#include "ui_MessageDialog.h"
using namespace tt3::gui;

//////////
//  Construction/destruction
MessageDialog::MessageDialog(
        QWidget * parent,
        const QString & title,
        const QString & message
    ) : QDialog(parent),
        _ui(new Ui::MessageDialog)
{
    tt3::util::ResourceReader rr(Component::Resources::instance(), RSID(MessageDialog));

    _ui->setupUi(this);
    setWindowTitle(title);

    _ui->buttonBox->button(QDialogButtonBox::StandardButton::Ok)->
        setText(rr.string(RID(OkPushButton)));
    _ui->buttonBox->button(QDialogButtonBox::StandardButton::Ok)->
        setIcon(QIcon(":/tt3-gui/Resources/Images/Actions/OkSmall.png"));

    _ui->messageLabel->setText(message);

    //  Done
    adjustSize();
}

MessageDialog::~MessageDialog()
{
    delete _ui;
}

//////////
//  Operations
void MessageDialog::doModal()
{
    this->exec();
}

void MessageDialog::show(
        const QString & title,
        const QString & message
    )
{
    MessageDialog dlg(nullptr, title, message);
    dlg.doModal();
}

void MessageDialog::show(
        QWidget * parent,
        const QString & title,
        const QString & message
    )
{
    MessageDialog dlg(parent, title, message);
    dlg.doModal();
}

//  End of tt3-gui/MessageDialog.cpp
