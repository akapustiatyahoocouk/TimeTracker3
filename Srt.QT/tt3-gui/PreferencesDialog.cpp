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

    _noPropertiesLabel = new QLabel("No properties to edit");
    _noPropertiesLabel->setAlignment(Qt::AlignmentFlag::AlignCenter);
    _editorsFrameLayout = new QStackedLayout();
    _ui->editorsFrame->setLayout(_editorsFrameLayout);

    //  Populate the preferences tree, creating
    //  editors for each Preferences node
    QList<Preferences*> rootNodes = PreferencesRegistry::rootPreferences().values();
    std::sort(rootNodes.begin(), rootNodes.end(), _compare);
    for (Preferences * rootNode : rootNodes)
    {
        QTreeWidgetItem * rootItem = new QTreeWidgetItem();
        _ui->preferencesTreeWidget->addTopLevelItem(rootItem);
        rootItem->setText(0, rootNode->displayName());
        rootItem->setData(0, Qt::ItemDataRole::UserRole, QVariant::fromValue(rootNode));
        _createEditor(rootItem);
        _createChildItems(rootItem);
    }

    _editorsFrameLayout->addWidget(_noPropertiesLabel);
    _noPropertiesLabel->setParent(_ui->editorsFrame);
    _editorsFrameLayout->setCurrentWidget(_noPropertiesLabel);

    //  TODO selest last "current" item

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

void PreferencesDialog::_createChildItems(QTreeWidgetItem * parentItem)
{
    Preferences * parentNode =
        parentItem->data(0, Qt::ItemDataRole::UserRole).value<Preferences*>();;
    QList<Preferences*> childNodes = parentNode->children().values();
    std::sort(childNodes.begin(), childNodes.end(), _compare);
    for (Preferences * childNode : childNodes)
    {
        QTreeWidgetItem * childItem = new QTreeWidgetItem();
        parentItem->addChild(childItem);
        childItem->setText(0, childNode->displayName());
        childItem->setData(0, Qt::ItemDataRole::UserRole, QVariant::fromValue(childNode));
        _createEditor(childItem);
        _createChildItems(childItem);
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

void PreferencesDialog::_accept()
{
    //  TODO save last "current" item
    accept();
}

void PreferencesDialog::_reject()
{
    //  TODO save last "current" item
    reject();
}

//  End of tt3-gui/PreferencesDialog.cpp
