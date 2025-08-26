//
//  tt3-gui/AboutDialog.cpp - tt3::gui::AboutDialog class implementation
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
#include "ui_AboutDialog.h"
using namespace tt3::gui;

//////////
//  Construction/destruction
AboutDialog::AboutDialog(QWidget * parent)
    :   QDialog(parent),
        _ui(new Ui::AboutDialog)
{
    _ui->setupUi(this);

    _ui->buttonBox->button(QDialogButtonBox::StandardButton::Ok)->
        setIcon(QIcon(":/tt3-gui/Resources/Images/Actions/OkSmall.png"));

    _ui->versionLabel->setText(
        "Version " + Component::instance()->version().toString() +
        " (build " + Component::instance()->buildNumber() +
        ")");
}

AboutDialog::~AboutDialog()
{
    delete _licensesPopupMenu;  //  delete nullptr is safe
    delete _ui;
}

//////////
//  Signal handlers
void AboutDialog::_showLicensePushButtonClicked()
{
    //  Build the list of all applicable licenses - ONCE
    if (_licenses.isEmpty())
    {   //  as good a check for "one time" as any - the tt3-gui
        //  component itself WILL have a license
        for (tt3::util::IComponent * component : tt3::util::ComponentManager::allComponents())
        {
            if (!_licenses.contains(component->license()))
            {
                _licenses.append(component->license());
            }
        }
        std::sort(
            _licenses.begin(),
            _licenses.end(),
            [](auto a, auto b) { return a->displayName() < b->displayName(); });
    }

    //  If there is only ONE license, there's no need to choose
    if (_licenses.size() == 1)
    {
        ShowLicenseDialog dlg(this, _licenses[0]);
        dlg.exec();
        return;
    }

    //  Prepare the popup menu containing all applicable licenses
    delete _licensesPopupMenu;  //  delete nullptr is safe
    _licensesPopupMenu = new QMenu(this);
    for (tt3::util::ILicense * license : _licenses)
    {
        QAction * action = new QAction(license->smallIcon(), license->displayName());
        _licensesPopupMenu->addAction(action);
        connect(action,
                &QAction::triggered,
                this,
                &AboutDialog::_onShowLicense);
    }

    //  Go!
    _licensesPopupMenu->exec(
        _ui->showLicensePushButton->mapToGlobal(
            _ui->showLicensePushButton->rect().bottomLeft()));
}

void AboutDialog::_showConfigurationPushButtonClicked()
{
    ErrorDialog::show(this, "Not yet implemented");
}

void AboutDialog::_onShowLicense()
{
    QObject * sender = QObject::sender();
    QList<QAction*> actions = _licensesPopupMenu->actions();
    for (int i = 0; i < actions.size(); i++)
    {
        if (sender == actions[i])
        {   //  This one!
            ShowLicenseDialog dlg(this, _licenses[i]);
            dlg.exec();
            return;
        }
    }
    //  Fall back to GPLv3
    ShowLicenseDialog dlg(this, tt3::util::Gpl3Licene::instance());
    dlg.exec();
}

//  End of tt3-gui/AboutDialog.cpp
