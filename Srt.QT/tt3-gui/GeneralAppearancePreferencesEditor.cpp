//
//  tt3-gui/GeneralStartupPreferencesEditor.cpp - tt3::gui::GeneralStartupPreferencesEditor class implementation
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
#include "ui_GeneralAppearancePreferencesEditor.h"
using namespace tt3::gui;

//////////
//  Construction/destruction
GeneralAppearancePreferencesEditor::GeneralAppearancePreferencesEditor(QWidget *parent)
    :   PreferencesEditor(parent),
        _ui(new Ui::GeneralAppearancePreferencesEditor)
{
    _ui->setupUi(this);

    //  Fill the language combo box with available locales
    //  from the set of locales supported by components and
    //  sorted by display name
    for (QLocale locale : tt3::util::ComponentManager::supportedLocales())
    {
        _locales.append(locale);
    }
    std::sort(_locales.begin(),
              _locales.end(),
              [](auto a, auto b) { return _displayName(a) < _displayName(b); });

    for (QLocale locale : _locales)
    {
        _ui->languageComboBox->addItem(
            tt3::util::LocaleManager::smallIcon(locale),
            _displayName(locale));
    }

    //  Fill the skin combo box with available skins
    //  sorted by display name
    _skins.append(SkinManager::allSkins().values());
    std::sort(_skins.begin(),
              _skins.end(),
              [](auto a, auto b) { return a->displayName() < b->displayName(); });

    for (ISkin * skin : _skins)
    {
        _ui->skinComboBox->addItem(skin->smallIcon(), skin->displayName());
    }

    //  Start off with current values from Settings
    loadControlValues();
}

GeneralAppearancePreferencesEditor::~GeneralAppearancePreferencesEditor()
{
    delete _ui;
}

//////////
//  PreferencesEditor
Preferences * GeneralAppearancePreferencesEditor::preferences() const
{
    return GeneralAppearancePreferences::instance();
}

void GeneralAppearancePreferencesEditor::loadControlValues()
{
    _setSelectedLocale(Component::Settings::instance()->uiLocale);
    _setSelectedSkin(SkinManager::findSkin(Component::Settings::instance()->activeSkin));
}

void GeneralAppearancePreferencesEditor::saveControlValues()
{
    Component::Settings::instance()->uiLocale = _selectedLocale();
    Component::Settings::instance()->activeSkin = _selectedSkin()->mnemonic().toString();
}

void GeneralAppearancePreferencesEditor::resetControlValues()
{
    _setSelectedLocale(Component::Settings::instance()->uiLocale.defaultValue());
    _setSelectedSkin(SkinManager::findSkin(Component::Settings::instance()->activeSkin.defaultValue()));
}

bool GeneralAppearancePreferencesEditor::isValid() const
{
    return _ui->languageComboBox->currentIndex() != -1 &&
           _ui->skinComboBox->currentIndex() != -1;
}

//////////
//  Implementation helpers
QLocale GeneralAppearancePreferencesEditor::_selectedLocale()
{
    qsizetype n = _ui->languageComboBox->currentIndex();
    Q_ASSERT(n >= 0 && n < _locales.size());
    return _locales[n];
}

void GeneralAppearancePreferencesEditor::_setSelectedLocale(const QLocale & locale)
{
    for (int n = 0; n < _locales.size(); n++)
    {
        if (_locales[n] == locale)
        {
            _ui->languageComboBox->setCurrentIndex(n);
            break;
        }
    }
}

tt3::gui::ISkin * GeneralAppearancePreferencesEditor::_selectedSkin()
{
    qsizetype n = _ui->skinComboBox->currentIndex();
    Q_ASSERT(n >= 0 && n < _skins.size());
    return _skins[n];
}

void GeneralAppearancePreferencesEditor::_setSelectedSkin(tt3::gui::ISkin * skin)
{
    for (int n = 0; n < _skins.size(); n++)
    {
        if (_skins[n] == skin)
        {
            _ui->skinComboBox->setCurrentIndex(n);
            break;
        }
    }
}

QString GeneralAppearancePreferencesEditor::_displayName(const QLocale & locale)
{
    return QLocale::languageToString(locale.language()) +
           " (" +
           QLocale::territoryToString(locale.territory()) +
           ")";
}

//////////
//  Signal handlers
void GeneralAppearancePreferencesEditor::_languageComboBoxCurrentIndexChanged(int)
{
    emit controlValueChanged();
}

void GeneralAppearancePreferencesEditor::_skinComboBoxCurrentIndexChanged(int)
{
    emit controlValueChanged();
}

//  End of tt3-gui/GeneralAppearancePreferencesEditor.cpp
