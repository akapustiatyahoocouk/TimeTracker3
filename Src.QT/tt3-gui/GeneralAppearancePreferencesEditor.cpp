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

namespace tt3::gui
{
    extern CurrentTheme theCurrentTheme;
}

//////////
//  Construction/destruction
GeneralAppearancePreferencesEditor::GeneralAppearancePreferencesEditor(QWidget *parent)
    :   PreferencesEditor(parent),
        _ui(new Ui::GeneralAppearancePreferencesEditor)
{
    tt3::util::ResourceReader rr(Component::Resources::instance(), RSID(GeneralAppearancePreferencesEditor));

    _ui->setupUi(this);

    _ui->languageLabel->setText(
        rr.string(RID(LanguageLabel)));
    _ui->skinLabel->setText(
        rr.string(RID(SkinLabel)));
    _ui->themeLabel->setText(
        rr.string(RID(ThemeLabel)));

    //  Fill the language combo box with available locales
    //  from the set of locales supported by components and
    //  sorted by display name
    for (QLocale locale : tt3::util::ComponentManager::supportedLocales())
    {
        _locales.append(locale);
    }
    std::sort(
        _locales.begin(),
        _locales.end(),
        [](auto a, auto b)
        {
            return tt3::util::LocaleManager::displayName(a) < tt3::util::LocaleManager::displayName(b);
        });

    for (QLocale locale : _locales)
    {
        _ui->languageComboBox->addItem(
            tt3::util::LocaleManager::smallIcon(locale),
            tt3::util::LocaleManager::displayName(locale));
    }

    //  Fill the skin combo box with available skins
    //  sorted by display name
    _skins.append(SkinManager::allSkins().values());
    std::sort(
        _skins.begin(),
        _skins.end(),
        [](auto a, auto b) { return a->displayName() < b->displayName(); });

    for (ISkin * skin : _skins)
    {
        _ui->skinComboBox->addItem(skin->smallIcon(), skin->displayName());
    }

    //  Fill the theme combo box with available themes
    //  sorted by display name
    _themes.append(ThemeManager::allThemes().values());
    std::sort(
        _themes.begin(),
        _themes.end(),
        [](auto a, auto b) { return a->displayName() < b->displayName(); });

    for (ITheme * theme : _themes)
    {
        _ui->themeComboBox->addItem(theme->smallIcon(), theme->displayName());
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
    _setSelectedTheme(ThemeManager::findTheme(Component::Settings::instance()->activeTheme));
}

void GeneralAppearancePreferencesEditor::saveControlValues()
{
    Component::Settings::instance()->uiLocale = _selectedLocale();
    Component::Settings::instance()->activeSkin = _selectedSkin()->mnemonic();
    Component::Settings::instance()->activeTheme = _selectedTheme()->mnemonic();
    tt3::util::theCurrentLocale = _selectedLocale();
    theCurrentTheme = _selectedTheme();
}

void GeneralAppearancePreferencesEditor::resetControlValues()
{
    _setSelectedLocale(Component::Settings::instance()->uiLocale.defaultValue());
    _setSelectedSkin(SkinManager::findSkin(Component::Settings::instance()->activeSkin.defaultValue()));
    _setSelectedTheme(ThemeManager::findTheme(Component::Settings::instance()->activeTheme.defaultValue()));
}

bool GeneralAppearancePreferencesEditor::isValid() const
{
    return _ui->languageComboBox->currentIndex() != -1 &&
           _ui->skinComboBox->currentIndex() != -1 &&
           _ui->themeComboBox->currentIndex() != -1;
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

tt3::gui::ITheme * GeneralAppearancePreferencesEditor::_selectedTheme()
{
    qsizetype n = _ui->themeComboBox->currentIndex();
    Q_ASSERT(n >= 0 && n < _themes.size());
    return _themes[n];
}

void GeneralAppearancePreferencesEditor::_setSelectedTheme(tt3::gui::ITheme * theme)
{
    for (int n = 0; n < _themes.size(); n++)
    {
        if (_themes[n] == theme)
        {
            _ui->themeComboBox->setCurrentIndex(n);
            break;
        }
    }
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

void GeneralAppearancePreferencesEditor::_themeComboBoxCurrentIndexChanged(int)
{
    emit controlValueChanged();
}

//  End of tt3-gui/GeneralAppearancePreferencesEditor.cpp
