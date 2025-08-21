//
//  tt3-gui/PreferencesDialog.cpp - tt3::gui::PreferencesDialog class implementation
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
#include "ui_PreferencesDialog.h"
using namespace tt3::gui;

//////////
//  Construction/destruction
PreferencesDialog::PreferencesDialog(QWidget * parent) throw(int)
    :   QDialog(parent),
        _ui(new Ui::PreferencesDialog)
{
    _ui->setupUi(this);

    _ui->buttonBox->button(QDialogButtonBox::StandardButton::Ok)->
        setIcon(QIcon(":/tt3-gui/Resources/Images/Actions/OkSmall.png"));
    _ui->buttonBox->button(QDialogButtonBox::StandardButton::Cancel)->
        setIcon(QIcon(":/tt3-gui/Resources/Images/Actions/CancelSmall.png"));

    _noPropertiesLabel = new QLabel("No properties to edit");
    _noPropertiesLabel->setAlignment(Qt::AlignmentFlag::AlignCenter);
    _editorsFrameLayout = new QStackedLayout();
    _ui->editorsFrame->setLayout(_editorsFrameLayout);

    //  Populate the preferences tree, creating
    //  editors for each Preferences node
    QList<Preferences*> rootNodes = PreferencesRegistry::rootPreferences().values();
    std::sort(rootNodes.begin(), rootNodes.end(), _compare);
    QMap<Preferences*,QTreeWidgetItem*> itemsForPreferences;
    for (Preferences * rootNode : rootNodes)
    {
        QTreeWidgetItem * rootItem = new QTreeWidgetItem();
        itemsForPreferences[rootNode] = rootItem;
        _ui->preferencesTreeWidget->addTopLevelItem(rootItem);
        rootItem->setText(0, rootNode->displayName());
        rootItem->setData(0, Qt::ItemDataRole::UserRole, QVariant::fromValue(rootNode));
        _createEditor(rootItem);
        _createChildItems(rootItem, itemsForPreferences);
    }

    _editorsFrameLayout->addWidget(_noPropertiesLabel);
    _noPropertiesLabel->setParent(_ui->editorsFrame);
    _editorsFrameLayout->setCurrentWidget(_noPropertiesLabel);

    //  Selest last "current" item
    _loadCurrentPreferences(itemsForPreferences);

    //  Start listening to component settings changes
    for (tt3::util::IComponent * component : tt3::util::ComponentManager::allComponents())
    {
        for (tt3::util::AbstractSetting * setting : component->settings().settings())
        {
            connect(setting,
                    &tt3::util::AbstractSetting::valueChanged,
                    this,
                    &PreferencesDialog::_settingValueChanged);

        }
    }

    //  Done
    _refresh();
}

PreferencesDialog::~PreferencesDialog()
{
    delete _editorsFrameLayout;
    delete _ui;
}

//////////
//  Implementation helpers
bool PreferencesDialog::_compare(const Preferences * a, const Preferences * b)
{
    if (a->order() < b->order())
    {
        return true;
    }
    if (a->order() > b->order())
    {
        return false;
    }
    return a->displayName() < b->displayName();
}

void PreferencesDialog::_createChildItems(QTreeWidgetItem * parentItem,
                                          QMap<Preferences*,QTreeWidgetItem*> & itemsForPreferences)
{
    Preferences * parentNode =
        parentItem->data(0, Qt::ItemDataRole::UserRole).value<Preferences*>();;
    QList<Preferences*> childNodes = parentNode->children().values();
    std::sort(childNodes.begin(), childNodes.end(), _compare);
    for (Preferences * childNode : childNodes)
    {
        QTreeWidgetItem * childItem = new QTreeWidgetItem();
        itemsForPreferences[childNode] = childItem;
        parentItem->addChild(childItem);
        childItem->setText(0, childNode->displayName());
        childItem->setData(0, Qt::ItemDataRole::UserRole, QVariant::fromValue(childNode));
        _createEditor(childItem);
        _createChildItems(childItem, itemsForPreferences);
    }
}

void PreferencesDialog::_createEditor(QTreeWidgetItem * item)
{
    Preferences * node =
        item->data(0, Qt::ItemDataRole::UserRole).value<Preferences*>();;
    PreferencesEditor * editor = node->createEditor();
    if (editor != nullptr)
    {
        _editorsFrameLayout->addWidget(editor);
        editor->setParent(_ui->editorsFrame);
        _editorsForItems[item] = editor;
        //  Set up signal listeners
        connect(editor,
                &PreferencesEditor::controlValueChanged,
                this,
                &PreferencesDialog::_refresh);
    }
}

void PreferencesDialog::_loadCurrentPreferences(const QMap<Preferences*,QTreeWidgetItem*> & itemsForPreferences)
{
    if (Preferences * node =
            PreferencesRegistry::findPreferences(
                Component::Settings::instance()->currentPreferences))
    {
        if (itemsForPreferences.contains(node))
        {
            _ui->preferencesTreeWidget->setCurrentItem(itemsForPreferences[node]);
        }
    }
}

void PreferencesDialog::_saveCurrentPreferences()
{
    QTreeWidgetItem * item =
        _ui->preferencesTreeWidget->currentItem();
    if (item != nullptr)
    {   //  Save!
        Preferences * node =
            item->data(0, Qt::ItemDataRole::UserRole).value<Preferences*>();;
        Component::Settings::instance()->currentPreferences = node->mnemonic();
    }
}

//////////
//  Signal handlers
void PreferencesDialog::_preferencesTreeWidgetCurrentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)
{
    QTreeWidgetItem * item = _ui->preferencesTreeWidget->currentItem();
    if (_editorsForItems.contains(item))
    {
        _editorsFrameLayout->setCurrentWidget(_editorsForItems[item]);
    }
    else
    {
        _editorsFrameLayout->setCurrentWidget(_noPropertiesLabel);
    }
}

void PreferencesDialog::_refresh()
{
    bool okEnabled = true;
    for (PreferencesEditor * editor : _editorsForItems.values())
    {
        if (!editor->isValid())
        {
            okEnabled = false;
        }
    }
    _ui->buttonBox->button(QDialogButtonBox::StandardButton::Ok)->setEnabled(okEnabled);
}

void PreferencesDialog::_resetPushNuttonClocked()
{
    //  TODO encapsulate the "mb" stuff into tt3::gui::AskYesNoDialog
    QMessageBox mb(this);
    mb.setWindowTitle("Reset all settings");
    mb.setIcon(QMessageBox::Icon::Question);
    mb.setText("Are you sure you want to reset all settings to their default values ?"
               "\nThe effects will not be permanent until you press 'OK' to close the dialog.");
    mb.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    mb.button(QMessageBox::StandardButton::Yes)->setIcon(
        QIcon(":/tt3-gui/Resources/Images/Actions/OkSmall.png"));
    mb.button(QMessageBox::StandardButton::No)->setIcon(
        QIcon(":/tt3-gui/Resources/Images/Actions/CancelSmall.png"));
    if (/*QMessageBox::question(
            this,
            "Reset all settings",
            "Are you sure you want to reset all settings to their default values ?"
                "\nThe effects will not be permanent until you press 'OK' to close the dialog.",
            QMessageBox::Yes | QMessageBox::No)*/
        mb.exec() == QMessageBox::StandardButton::Yes)
    {   //  Do it!
        for (PreferencesEditor * editor : _editorsForItems.values())
        {
            editor->resetControlValues();
        }
        _refresh();
    }
}

void PreferencesDialog::_accept()
{
    //  Apply all changes to all settings
    for (PreferencesEditor * editor : _editorsForItems.values())
    {
        editor->saveControlValues();
    }
    tt3::util::ComponentManager::saveComponentSettings();
    _saveCurrentPreferences();
    if (_restartRequired)
    {
        if (QMessageBox::question(
                this,
                "Restart required",
                "One of the changes you have made to application settings"
                    "\nwill take place only after the application is restarted."
                    "\nDo you want to restart TimeTracker3 now ?",
                QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
        {
            QApplication::exit(-1);
        }
        else
        {
            accept();
        }
    }
    else
    {
        accept();
    }
}

void PreferencesDialog::_reject()
{
    _saveCurrentPreferences();
    reject();
}

void PreferencesDialog::_settingValueChanged(tt3::util::AbstractSetting * setting)
{
    if (setting->changeRequiresRestart())
    {
        _restartRequired = true;
    }
}

//  End of tt3-gui/PreferencesDialog.cpp
