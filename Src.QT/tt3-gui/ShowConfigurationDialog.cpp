//
//  tt3-gui/ShowConfigurationDialog.cpp - tt3::gui::ShowConfigurationDialog class implementation
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
#include "ui_ShowConfigurationDialog.h"
using namespace tt3::gui;

//////////
//  Construction/destruction
ShowConfigurationDialog::ShowConfigurationDialog(QWidget * parent)
    :   QDialog(parent),
        _ui(new Ui::ShowConfigurationDialog)
{
    tt3::util::ResourceReader rr(Component::Resources::instance(), RSID(ShowConfigurationDialog));

    _ui->setupUi(this);
    setWindowTitle(rr.string(RID(Title)));

    _ui->tabWidget->setTabText(
        0, rr.string(RID(ComponentsTab)));
    _ui->displayNameLabel->setText(
        rr.string(RID(DisplayNameLabel)));
    _ui->descriptionLabel->setText(
        rr.string(RID(DescriptionLabel)));
    _ui->versionLabel->setText(
        rr.string(RID(VersionLabel)));
    _ui->copyrightLabel->setText(
        rr.string(RID(CopyrightLabel)));
    _ui->licenseLabel->setText(
        rr.string(RID(LicenseLabel)));
    _ui->showLicensePushButton->setText(
        rr.string(RID(ShowLicensePushButton)));

    _ui->buttonBox->button(QDialogButtonBox::StandardButton::Ok)->
        setText(rr.string(RID(OkPushButton)));
    _ui->buttonBox->button(QDialogButtonBox::StandardButton::Ok)->
        setIcon(QIcon(":/tt3-gui/Resources/Images/Actions/OkSmall.png"));

    //  Fill the "components" tree
    QList<tt3::util::ISubsystem*> subsystems =
        tt3::util::SubsystemManager::allSubsystems().values();
    std::sort(
        subsystems.begin(),
        subsystems.end(),
        [](auto a, auto b) { return a->displayName() < b->displayName(); });
    for (tt3::util::ISubsystem * subsystem : subsystems)
    {
        QTreeWidgetItem * subsystemItem = new QTreeWidgetItem();
        subsystemItem->setText(0, subsystem->displayName());
        subsystemItem->setIcon(0, subsystem->smallIcon());
        subsystemItem->setData(0, Qt::ItemDataRole::UserRole, QVariant::fromValue(subsystem));
        _ui->componentsTreeWidget->addTopLevelItem(subsystemItem);
        //  Do the components
        QList<tt3::util::IComponent*> components = subsystem->components().values();
        std::sort(
            components.begin(),
            components.end(),
            [](auto a, auto b) { return a->mnemonic() < b->mnemonic(); });
        for (tt3::util::IComponent * component : components)
        {
            QTreeWidgetItem * componentItem = new QTreeWidgetItem();
            componentItem->setText(0, component->displayName());
            componentItem->setIcon(0, component->smallIcon());
            componentItem->setData(0, Qt::ItemDataRole::UserRole, QVariant::fromValue(component));
            subsystemItem->addChild(componentItem);
        }
    }
    _ui->componentsTreeWidget->expandAll();

    //  Done
    adjustSize();
    _refresh();
}

ShowConfigurationDialog::~ShowConfigurationDialog()
{
    delete _ui;
}

//////////
//  Operations
void ShowConfigurationDialog::doModal()
{
    exec();
}

//////////
//  Implementation helpers
void ShowConfigurationDialog::_refresh()
{
    tt3::util::ResourceReader rr(Component::Resources::instance(), RSID(ShowConfigurationDialog));

    QTreeWidgetItem * item = _ui->componentsTreeWidget->currentItem();
    if (item == nullptr)
    {   //  No selection
        _ui->displayNameLabel->setEnabled(false);
        _ui->displayNameValue->setEnabled(false);
        _ui->descriptionLabel->setEnabled(false);
        _ui->descriptionValue->setEnabled(false);
        _ui->versionLabel->setEnabled(false);
        _ui->versionValue->setEnabled(false);;
        _ui->copyrightLabel->setEnabled(false);
        _ui->copyrightValue->setEnabled(false);;
        _ui->licenseLabel->setEnabled(false);
        _ui->licenseValue->setEnabled(false);
        _ui->showLicensePushButton->setEnabled(false);

        _ui->displayNameValue->setText("");
        _ui->descriptionValue->setText("");
        _ui->versionValue->setText("");
        _ui->copyrightValue->setText("");
        _ui->licenseValue->setText("");
    }
    else if (item->parent() == nullptr)
    {   //  Subsystem is selected
        tt3::util::ISubsystem * subsystem =
            item->data(0, Qt::ItemDataRole::UserRole).value<tt3::util::ISubsystem*>();

        _ui->displayNameLabel->setEnabled(true);
        _ui->displayNameValue->setEnabled(true);
        _ui->descriptionLabel->setEnabled(true);
        _ui->descriptionValue->setEnabled(true);
        _ui->versionLabel->setEnabled(false);
        _ui->versionValue->setEnabled(false);;
        _ui->copyrightLabel->setEnabled(false);
        _ui->copyrightValue->setEnabled(false);;
        _ui->licenseLabel->setEnabled(false);
        _ui->licenseValue->setEnabled(false);
        _ui->showLicensePushButton->setEnabled(false);

        _ui->displayNameValue->setText(subsystem->displayName());
        _ui->descriptionValue->setText(subsystem->description());
        _ui->versionValue->setText("");
        _ui->copyrightValue->setText("");
        _ui->licenseValue->setText("");
    }
    else
    {   //  Component is selected
        tt3::util::IComponent * component =
            item->data(0, Qt::ItemDataRole::UserRole).value<tt3::util::IComponent*>();

        _ui->displayNameLabel->setEnabled(true);
        _ui->displayNameValue->setEnabled(true);
        _ui->descriptionLabel->setEnabled(true);
        _ui->descriptionValue->setEnabled(true);
        _ui->versionLabel->setEnabled(true);
        _ui->versionValue->setEnabled(true);;
        _ui->copyrightLabel->setEnabled(true);
        _ui->copyrightValue->setEnabled(true);;
        _ui->licenseLabel->setEnabled(true);
        _ui->licenseValue->setEnabled(true);
        _ui->showLicensePushButton->setEnabled(true);

        _ui->displayNameValue->setText(component->displayName());
        _ui->descriptionValue->setText(component->description());
        _ui->versionValue->setText(
            rr.string(
                RID(VersionAndBuild),
                component->version().toString(),
                component->buildNumber()));
        _ui->copyrightValue->setText(component->copyright());
        _ui->licenseValue->setText(component->license()->description());
    }
}

//////////
//  Signal handlers
void ShowConfigurationDialog::_configurationTreeWidgetCurrentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)
{
    _refresh();
}

void ShowConfigurationDialog::_showLicensePushButtonClicked()
{
    QTreeWidgetItem * item = _ui->componentsTreeWidget->currentItem();
    if (item != nullptr && item->parent() != nullptr)
    {
        tt3::util::IComponent * component =
            item->data(0, Qt::ItemDataRole::UserRole).value<tt3::util::IComponent*>();
        ShowLicenseDialog dlg(this, component->license());
        dlg.doModal();
    }
}

//  End of tt3-gui/ShowConfigurationDialog.cpp
