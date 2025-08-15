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
    //  Initialize the application
    QApplication a(argc, argv);

    QPixmap pm;
    pm.load(":/tt3/Resources/Images/Misc/Tt3Large.png");
    QIcon ic(pm);
    QGuiApplication::setWindowIcon(ic);
    QGuiApplication::setApplicationName("TimeTracker3");

    registerStandardComponents();
    tt3::util::PluginManager::loadPlugins();
    loadSettings();
    selectActiveSkin();

    //  Go!
    int exitCode = a.exec();

    //  Cleanup
    //  TODO if there's a "current" activity, record & syop it
    //  If there's a "current" workspace, close it
    tt3::ws::WorkspacePtr currentWorkspace;
    tt3::ws::theCurrentWorkspace.swap(currentWorkspace);
    if (currentWorkspace != nullptr)
    {
        currentWorkspace->close();
        //  TODO handle "close" exceptions
    }

    //  Done
    saveSettings();
    return exitCode;
}

//  End of tt3/Main.cpp
