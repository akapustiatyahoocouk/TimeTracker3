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
PreferencesDialog::PreferencesDialog(
        QWidget * parent
    ) : QDialog(parent),
        _ui(new Ui::PreferencesDialog)
{
    tt3::util::ResourceReader rr(Component::Resources::instance(), RSID(PreferencesDialog));

    _ui->setupUi(this);
    setWindowTitle(rr.string(RID(Title)));

    //  Set static control values
    _ui->resetPushButton->setText(
        rr.string(RID(ResetPushButton)));
    _ui->resetAllPushButton->setText(
        rr.string(RID(ResetAllPushButton)));

    _ui->buttonBox->button(QDialogButtonBox::StandardButton::Ok)->
        setText(rr.string(RID(OkPushButton)));
    _ui->buttonBox->button(QDialogButtonBox::StandardButton::Ok)->
        setIcon(QIcon(":/tt3-gui/Resources/Images/Actions/OkSmall.png"));
    _ui->buttonBox->button(QDialogButtonBox::StandardButton::Cancel)->
        setText(rr.string(RID(CancelPushButton)));
    _ui->buttonBox->button(QDialogButtonBox::StandardButton::Cancel)->
        setIcon(QIcon(":/tt3-gui/Resources/Images/Actions/CancelSmall.png"));

    _noPropertiesLabel =
        new QLabel(
            rr.string(RID(NoPropertiesLabel)));
    _noPropertiesLabel->setAlignment(Qt::AlignmentFlag::AlignCenter);
    _editorsFrameLayout = new QStackedLayout();
    _ui->editorsFrame->setLayout(_editorsFrameLayout);

    //  Populate the preferences tree, creating
    //  editors for each Preferences node
    QList<Preferences*> rootNodes = PreferencesManager::rootPreferences().values();
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
        for (tt3::util::AbstractSetting * setting : component->settings()->settings())
        {
            connect(setting,
                    SIGNAL(valueChanged()),
                    this,
                    SLOT(_settingValueChanged()));
        }
    }

    //  Done
    adjustSize();
    _refresh();
}

PreferencesDialog::~PreferencesDialog()
{
    delete _editorsFrameLayout;
    delete _ui;
}

//////////
//  Operations
auto PreferencesDialog::doModal(
    ) -> Result
{
    return Result(this->exec());
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
    if (auto editor = node->createEditor())
    {
        _editorsFrameLayout->addWidget(editor);
        editor->setParent(_ui->editorsFrame);
        _editorsForItems[item] = editor;
        //  Set up signal listeners
        //  We need to connect() by name (old-style)
        //  because we privately inherit from QDialog
        connect(editor,
                SIGNAL(controlValueChanged()),
                this,
                SLOT(_refresh()));
    }
}

void PreferencesDialog::_loadCurrentPreferences(const QMap<Preferences*,QTreeWidgetItem*> & itemsForPreferences)
{
    if (Preferences * node =
            PreferencesManager::findPreferences(
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
    if (auto item = _ui->preferencesTreeWidget->currentItem())
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
        _ui->resetPushButton->setEnabled(true);
    }
    else
    {
        _editorsFrameLayout->setCurrentWidget(_noPropertiesLabel);
        _ui->resetPushButton->setEnabled(false);
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

void PreferencesDialog::_resetPushNuttonClicked()
{
    ConfirmResetPageSettingsDialog dlg(this);
    if (dlg.doModal() == ConfirmResetPageSettingsDialog::Result::Yes)
    {   //  Do it!
        QTreeWidgetItem * item = _ui->preferencesTreeWidget->currentItem();
        if (_editorsForItems.contains(item))
        {
            _editorsForItems[item]->resetControlValues();
        }
        _refresh();
    }
}

void PreferencesDialog::_resetAllPushNuttonClicked()
{
    ConfirmResetAllSettingsDialog dlg(this);
    if (dlg.doModal() == ConfirmResetAllSettingsDialog::Result::Yes)
    {   //  Do it!
        for (PreferencesEditor * editor : _editorsForItems.values())
        {
            editor->resetControlValues();
        }
        _refresh();
    }
}

void PreferencesDialog::_settingValueChanged()
{
    QObject * sender = this->sender();
    if (tt3::util::AbstractSetting * setting =
        qobject_cast<tt3::util::AbstractSetting*>(sender))
    {
        if (setting->changeRequiresRestart())
        {
            _restartRequired = true;
        }
    }
}

void PreferencesDialog::accept()
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
        RestartRequiredDialog dlg(this);
        if (dlg.doModal() == RestartRequiredDialog::Result::Yes)
        {
            QApplication::exit(-1);
        }
        else
        {
            done(int(Result::Ok));
        }
    }
    else
    {
        done(int(Result::Ok));
    }
}

void PreferencesDialog::reject()
{
    _saveCurrentPreferences();
    done(int(Result::Cancel));
}

//  End of tt3-gui/PreferencesDialog.cpp
