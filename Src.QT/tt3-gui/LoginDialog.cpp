//
//  tt3-gui/LoginDialog.cpp - tt3::gui::LoginDialog class implementation
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
#include "ui_LoginDialog.h"
using namespace tt3::gui;

//////////
//  Construction/destruction
LoginDialog::LoginDialog(QWidget * parent, const QString & login)
    :   QDialog(parent),
        _ui(new Ui::LoginDialog)
{
    tt3::util::ResourceReader rr(Component::Resources::instance(), RSID(LoginDialog));

    _ui->setupUi(this);
    setWindowTitle(rr.string(RID(Title)));

    _ui->loginLabel->setText(
        rr.string(RID(LoginLabel)));
    _ui->passwordLabel->setText(
        rr.string(RID(PasswordLabel)));

    _ui->buttonBox->button(QDialogButtonBox::StandardButton::Ok)->
        setText(rr.string(RID(OkPushButton)));
    _ui->buttonBox->button(QDialogButtonBox::StandardButton::Ok)->
        setIcon(QIcon(":/tt3-gui/Resources/Images/Actions/OkSmall.png"));
    _ui->buttonBox->button(QDialogButtonBox::StandardButton::Cancel)->
        setText(rr.string(RID(CancelPushButton)));
    _ui->buttonBox->button(QDialogButtonBox::StandardButton::Cancel)->
        setIcon(QIcon(":/tt3-gui/Resources/Images/Actions/CancelSmall.png"));

    _ui->loginLineEdit->setText(login.trimmed());

    //  We don't want a modal dialog so importnt to be obscured
    Qt::WindowFlags flags = this->windowFlags();
    flags |= Qt::WindowStaysOnTopHint;
    this->setWindowFlags(flags);

    //  Done
    _refresh();
    adjustSize();
    (login.trimmed().isEmpty() ?
        _ui->loginLineEdit :
        _ui->passwordLineEdit)->setFocus();
}

LoginDialog::~LoginDialog()
{
    delete _ui;
}

//////////
//  Operations
auto LoginDialog::doModal(
    ) -> Result
{
    return Result(this->exec());
}

//////////
//  Implementation helpers
void LoginDialog::_refresh()
{
    _ui->buttonBox->button(QDialogButtonBox::StandardButton::Ok)->
        setEnabled(_ui->loginLineEdit->text().trimmed().length() != 0);
}

//////////
//  Event handlers
void LoginDialog::_loginLineEditTextChanged(QString)
{
    _refresh();
}

void LoginDialog::_passwordLineEditTextChanged(QString)
{
    _refresh();
}

void LoginDialog::accept()
{
    _credentials =
        tt3::ws::Credentials(
            _ui->loginLineEdit->text().trimmed(),
            _ui->passwordLineEdit->text());
    Component::Settings::instance()->lastLogin = _ui->loginLineEdit->text();
    done(int(Result::Ok));
}

void LoginDialog::reject()
{
    done(int(Result::Cancel));
}

//  End of tt3-gui/LoginDialog.cpp
