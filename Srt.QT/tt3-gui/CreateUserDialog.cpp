//
//  tt3-gui/CreateUserDialog.cpp - tt3::gui::CreateUserDialog class implementation
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
#include "ui_CreateUserDialog.h"
using namespace tt3::gui;

//////////
//  Construction/destruction
CreateUserDialog::CreateUserDialog(QWidget * parent,
                                   tt3::ws::Workspace workspace, const tt3::ws::Credentials & credentials )
    :   QDialog(parent),
        _workspace(workspace),
        _credentials(credentials),
        _ui(new Ui::CreateUserDialog)
{
    _ui->setupUi(this);

    _ui->buttonBox->button(QDialogButtonBox::StandardButton::Ok)->
        setIcon(QIcon(":/tt3-gui/Resources/Images/Actions/OkSmall.png"));
    _ui->buttonBox->button(QDialogButtonBox::StandardButton::Cancel)->
        setIcon(QIcon(":/tt3-gui/Resources/Images/Actions/CancelSmall.png"));

    //  Fill "hours" amd "minutes" combo boxes
    for (int h = 0; h < 12; h++)
    {
        _ui->hoursComboBox->addItem(tt3::util::toString(h) + " hrs", QVariant::fromValue(h));
    }
    for (int m = 0; m < 60; m += 15)
    {
        _ui->minutesComboBox->addItem(tt3::util::toString(m) + " min", QVariant::fromValue(m));
    }

    //  Fill "UI locale" combo box
    _ui->uiLocaleComboBox->addItem("- (system default)");
    for (QLocale locale : tt3::util::ComponentManager::supportedLocales())
    {
        _locales.append(locale);
    }
    std::sort(_locales.begin(),
              _locales.end(),
              [](auto a, auto b) { return _displayName(a) < _displayName(b); });

    for (QLocale locale : _locales)
    {
        _ui->uiLocaleComboBox->addItem(
            tt3::util::LocaleManager::smallIcon(locale),
            tt3::util::LocaleManager::displayName(locale));
    }
    _locales.insert(0, QLocale());  //  ...to make combo box and QList indexes match

    //  Done
    _refresh();
}

CreateUserDialog::~CreateUserDialog()
{
    delete _ui;
}

//////////
//  Operations
CreateUserDialog::Result CreateUserDialog::doModal()
{
    return Result(this->exec());
}

//////////
//  Implementation helpers
QString CreateUserDialog::_displayName(const QLocale & locale)
{
    return QLocale::languageToString(locale.language()) +
           " (" +
           QLocale::territoryToString(locale.territory()) +
           ")";
}

void CreateUserDialog::_refresh()
{
}

//////////
//  Signal handlers
void CreateUserDialog::_accept()
{
    done(int(Result::Ok));
}

void CreateUserDialog::_reject()
{
    done(int(Result::Cancel));
}

//  End of tt3-gui/CreateUserDialog.cpp


//  End of tt3-gui/CreateUserDialog.cpp
