//
//  tt3-gui/AskYesNoDialog.cpp - tt3::gui::AskYesNoDialog class implementation
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
#include "ui_AskYesNoDialog.h"
using namespace tt3::gui;

//////////
//  Construction/destruction
AskYesNoDialog::AskYesNoDialog(
        QWidget * parent,
        const QIcon & icon,
        const QString & title,
        const QString & prompt,
        tt3::util::Setting<bool> * confirmActionSetting
    ) : QDialog(parent),
        _confirmActionSetting(confirmActionSetting),
        _ui(new Ui::AskYesNoDialog)
{
    tt3::util::ResourceReader rr(Component::Resources::instance(), RSID(AskYesNoDialog));

    _ui->setupUi(this);
    this->setWindowIcon(icon);
    this->setWindowTitle(title);

    //  Set initial control values
    _ui->promptLabel->setText(prompt);

    _ui->assumeYesCheckBox->setText(
        rr.string(RID(AssumeYesCheckBox)));

    _ui->buttonBox->button(QDialogButtonBox::StandardButton::Yes)->
        setText(rr.string(RID(YesPushButton)));
    _ui->buttonBox->button(QDialogButtonBox::StandardButton::Yes)->
        setIcon(QIcon(":/tt3-gui/Resources/Images/Actions/OkSmall.png"));
    _ui->buttonBox->button(QDialogButtonBox::StandardButton::No)->
        setText(rr.string(RID(NoPushButton)));
    _ui->buttonBox->button(QDialogButtonBox::StandardButton::No)->
        setIcon(QIcon(":/tt3-gui/Resources/Images/Actions/CancelSmall.png"));

    //  Adjust control states
    if (_confirmActionSetting != nullptr)
    {
        _ui->assumeYesCheckBox->setChecked(!_confirmActionSetting->value());
    }
    else
    {
        _ui->assumeYesCheckBox->setVisible(false);
    }

    //  Done
    adjustSize();
}

AskYesNoDialog::~AskYesNoDialog()
{
    delete _ui;
}

//////////
//  Operations
auto AskYesNoDialog::doModal() -> Result
{
    return Result(this->exec());
}

//////////
//  Signal handlers
void AskYesNoDialog::accept()
{
    if (_confirmActionSetting != nullptr)
    {
        _confirmActionSetting->setValue(!_ui->assumeYesCheckBox->isChecked());
    }
    done(int(Result::Yes));
}

void AskYesNoDialog::reject()
{
    done(int(Result::No));
}

//  End of tt3-gui/AskYesNoDialog.cpp
