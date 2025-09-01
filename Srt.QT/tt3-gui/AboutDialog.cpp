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

    _ui->productLabel->setText(tt3::util::ProductInformation::applicationDisplayName());
    _ui->versionLabel->setText(
        "Version " + tt3::util::ProductInformation::applicationVersion().toString() +
        " (build " + tt3::util::ProductInformation::applicationBuildNumber() +
        ")");
    _ui->copyrightLabel->setText(tt3::util::ProductInformation::applicationCopyright());
    _ui->creditsLabel->setText(tt3::util::ProductInformation::credits());

    //  Done
    adjustSize();
}

AboutDialog::~AboutDialog()
{
    delete _licensesPopupMenu;  //  delete nullptr is safe
    delete _ui;
}

//////////
//  Operations
void AboutDialog::doModal()
{
    exec();
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
        dlg.doModal();
        return;
    }

    //  Prepare the popup menu containing all applicable licenses
    delete _licensesPopupMenu;  //  delete nullptr is safe
    _licensesPopupMenu = new QMenu(this);
    for (tt3::util::ILicense * license : _licenses)
    {
        QAction * action = new QAction(license->smallIcon(), license->displayName());
        _licensesPopupMenu->addAction(action);
        //  We need to connect() by name (old-style)
        //  because we privately inherit from QDialog
        connect(action,
                SIGNAL(triggered()),
                this,
                SLOT(_onShowLicense()));
    }

    //  Go!
    _licensesPopupMenu->exec(
        _ui->showLicensePushButton->mapToGlobal(
            _ui->showLicensePushButton->rect().bottomLeft()));
}

void AboutDialog::_showConfigurationPushButtonClicked()
{
    ShowConfigurationDialog dlg(this);
    dlg.doModal();
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
            dlg.doModal();
            return;
        }
    }
    //  Fall back to GPLv3
    ShowLicenseDialog dlg(this, tt3::util::StandardLicenses::Gpl3::instance());
    dlg.doModal();
}

//  End of tt3-gui/AboutDialog.cpp
