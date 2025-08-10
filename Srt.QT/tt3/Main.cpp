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
using namespace tt3;

namespace
{
    void registerStandardComponents()
    {   //  Sone components form the TT3 skeleton
        //  and, therefore, are NOT registered by plugins
        tt3::util::ComponentRegistry::registerComponent(tt3::Component::instance());
        tt3::util::ComponentRegistry::registerComponent(tt3::gui::Component::instance());
        tt3::util::ComponentRegistry::registerComponent(tt3::ws::Component::instance());
        tt3::util::ComponentRegistry::registerComponent(tt3::db::api::Component::instance());
        tt3::util::ComponentRegistry::registerComponent(tt3::util::Component::instance());
    }

    void loadSettings()
    {
        QDir home = QDir::home();
        QString iniFileName = home.filePath(".tt3");

        QFile iniFile(iniFileName);
        if (iniFile.open(QIODevice::ReadOnly))
        {
            tt3::util::IComponent * currentComponent = nullptr;
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
                        QVersionNumber componentVersion = tt3::util::fromString<QVersionNumber>(line.mid(separatorIndex + 1, line.length() - separatorIndex - 2));
                        currentComponent = tt3::util::ComponentRegistry::findComponent(componentMnemonic, componentVersion);
                        continue;
                    }
                    catch (const tt3::util::ParseException &)
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
                tt3::util::AbstractSetting * setting = currentComponent->settings().findSetting(settingName);
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
            for (tt3::util::IComponent * component : tt3::util::ComponentRegistry::allComponents())
            {
                iniStream << "["
                          << component->mnemonic()
                          << ":"
                          << tt3::util::toString(component->version())
                          << "]"
                          << Qt::endl;
                for (tt3::util::AbstractSetting * setting : component->settings().settings())
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
        tt3::gui::ISkin * initialSkin =
            tt3::gui::SkinRegistry::findSkin(tt3::gui::Component::Settings::instance()->activeSkin);
        //  Use a default skin ?
        for (tt3::gui::ISkin * skin : tt3::gui::SkinRegistry::allSkins())
        {
            if (skin->isDefault() && initialSkin == nullptr)
            {
                initialSkin = skin;
                break;
            }
        }
        //  Use any available skin ?
        for (tt3::gui::ISkin * skin : tt3::gui::SkinRegistry::allSkins())
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
            tt3::gui::ErrorDialog::show("No UI skins defined.");
            QCoreApplication::quit();
        }
        tt3::gui::theCurrentSkin = initialSkin;
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
    tt3::util::PluginManager::loadPlugins();
    loadSettings();

    //  TODO define & use "synchronized (lock)" statement - like macro
    tt3::util::Mutex m;
    if (tt3::util::Lock(m), true)
    {
        selectActiveSkin();
    }

    int exitCode = a.exec();

    saveSettings();
    return exitCode;
}

//  End of tt3/Main.cpp
