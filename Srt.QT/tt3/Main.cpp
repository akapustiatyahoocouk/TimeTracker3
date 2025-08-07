//
//  tt3/Main.cpp - TT3 entry point
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
#include "tt3/API.hpp"

namespace
{
    void registerStandardComponents()
    {
        util::ComponentRegistry::registerComponent(ApplicationComponent::instance());
        util::ComponentRegistry::registerComponent(gui::GuiComponent::instance());
        util::ComponentRegistry::registerComponent(tt3::ws::WorkspaceComponent::instance());
        util::ComponentRegistry::registerComponent(db::api::DbApiComponent::instance());
        util::ComponentRegistry::registerComponent(util::UtilComponent::instance());
    }

    void loadSettings()
    {
        QDir home = QDir::home();
        QString iniFileName = home.filePath(".tt3");

        QFile iniFile(iniFileName);
        if (iniFile.open(QIODevice::ReadOnly))
        {
            util::Component * currentComponent = nullptr;
            QTextStream iniStream(&iniFile);
            while (!iniStream.atEnd())
            {
                QString line = iniStream.readLine();
                //  Section header ?
                line  = line .trimmed();
                if (line.startsWith("[") && line.endsWith("]"))
                {
                    int separatorIndex = static_cast<int>(line.indexOf(':'));
                    if (separatorIndex == -1)
                    {
                        currentComponent = nullptr;
                        continue;
                    }
                    try
                    {
                        QString componentMnemonic = line.mid(1, separatorIndex - 1);
                        QVersionNumber componentVersion = util::fromString<QVersionNumber>(line.mid(separatorIndex + 1, line.length() - separatorIndex - 2));
                        currentComponent = util::ComponentRegistry::findComponent(componentMnemonic, componentVersion);
                        continue;
                    }
                    catch (const util::ParseException &)
                    {
                        currentComponent = nullptr;
                        continue;
                    }
                }
                //  <name> = <value>?
                int eqIndex = static_cast<int>(line.indexOf('='));
                if (eqIndex == -1 || currentComponent == nullptr)
                {
                    continue;
                }
                QString settingName = line.mid(0, eqIndex).trimmed();
                QString settingValueString = line.mid(eqIndex + 1).trimmed();
                util::AbstractSetting * setting = currentComponent->settings().findSetting(settingName);
                if (setting == nullptr)
                {
                    continue;
                }
                setting->setValueString(settingValueString);
            }
        }
    }

    void saveSettings()
    {
        QDir home = QDir::home();
        QString iniFileName = home.filePath(".tt3");

        QFile iniFile(iniFileName);
        if (iniFile.open(QIODevice::WriteOnly))
        {
            QTextStream iniStream(&iniFile);
            for (util::Component * component : util::ComponentRegistry::allComponents())
            {
                iniStream << "["
                          << component->mnemonic()
                          << ":"
                          << util::toString(component->version())
                          << "]"
                          << Qt::endl;
                for (util::AbstractSetting * setting : component->settings().settings())
                {
                    iniStream << setting->mnemonic()
                              << "="
                              << setting->valueString()
                              << Qt::endl;
                }
                iniStream << Qt::endl;
            }
        }
    }

    void selectActiveSkin()
    {
        gui::ISkin * initialSkin =
            gui::SkinRegistry::findSkin(gui::GuiSettings::instance()->activeSkin);
        //  Use a default skin ?
        for (gui::ISkin * skin : gui::SkinRegistry::allSkins())
        {
            if (skin->isDefault() && initialSkin == nullptr)
            {
                initialSkin = skin;
                break;
            }
        }
        //  Use any available skin ?
        for (gui::ISkin * skin : gui::SkinRegistry::allSkins())
        {
            if (initialSkin == nullptr)
            {
                initialSkin = skin;
                break;
            }
        }
        //  A skin MUST be selected
        if (initialSkin == nullptr)
        {
            QMessageBox msgBox;
            msgBox.setText("No UI skins defined.");
            msgBox.setIcon(QMessageBox::Icon::Critical);
            msgBox.exec();
            QCoreApplication::quit();
        }
        gui::theCurrentSkin = initialSkin;
        initialSkin->activate();
    }
}

//////////
//  TT3 entry point
int main(int argc, char *argv[])
{
    /*  TODO kill off
    dbapi::Capabilities c1 = util::fromString<dbapi::Capabilities>("");
    dbapi::Capabilities c2 = util::fromString<dbapi::Capabilities>("a");
    dbapi::Capabilities c3 = util::fromString<dbapi::Capabilities>("Administrator");
    dbapi::Capabilities c4 = util::fromString<dbapi::Capabilities>("ManageUsers");
    dbapi::Capabilities c5 = util::fromString<dbapi::Capabilities>("ManageUsers+ManageModels");
    dbapi::Capabilities c6 = util::fromString<dbapi::Capabilities>("ManageUsers+ManageModels+");
    */

    QApplication a(argc, argv);

    QPixmap pm;
    pm.load(":/tt3/Resources/Images/Misc/Tt3Large.png");
    QIcon ic(pm);
    QGuiApplication::setWindowIcon(ic);
    QGuiApplication::setApplicationName("TimeTracker3");

    registerStandardComponents();
    util::PluginManager::loadPlugins();
    loadSettings();

    selectActiveSkin();

    int exitCode = a.exec();

    saveSettings();
    return exitCode;
}

//  End of tt3/Main.cpp
