//
//  tt3/Application.cpp - tt3::Application class implementation
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

//////////
//  Construction/destruction
Application::Application(int &argc, char **argv)
    :   QApplication(argc, argv)
{
}

Application::~Application()
{
}

//////////
//  QApplication
int Application::exec()
{
    _initialize();
    int result = QApplication::exec();
    _cleanup();
    return result;
}

//////////
//  Implementation helpers
void Application::_registerStandardComponents()
{   //  Some components form the TT3 skeleton
    //  and, therefore, are NOT registered by plugins
    tt3::util::ComponentManager::registerComponent(tt3::Component::instance());
    tt3::util::ComponentManager::registerComponent(tt3::gui::Component::instance());
    tt3::util::ComponentManager::registerComponent(tt3::ws::Component::instance());
    tt3::util::ComponentManager::registerComponent(tt3::db::api::Component::instance());
    tt3::util::ComponentManager::registerComponent(tt3::util::Component::instance());
    //  Same for standard Preferences
    tt3::gui::PreferencesManager::registerPreferences(tt3::gui::GeneralPreferences::instance());
    tt3::gui::PreferencesManager::registerPreferences(tt3::gui::GeneralAppearancePreferences::instance());
    tt3::gui::PreferencesManager::registerPreferences(tt3::gui::GeneralStartupPreferences::instance());
}

void Application::_selectActiveSkin()
{   //  TODO move to tt3::util::SkinManager ?
    tt3::gui::ISkin * initialSkin =
        tt3::gui::SkinManager::findSkin(tt3::gui::Component::Settings::instance()->activeSkin);
    //  Use a default skin ?
    for (tt3::gui::ISkin * skin : tt3::gui::SkinManager::allSkins())
    {
        if (skin->isDefault() && initialSkin == nullptr)
        {
            initialSkin = skin;
            break;
        }
    }
    //  Use any available skin ?
    for (tt3::gui::ISkin * skin : tt3::gui::SkinManager::allSkins())
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

void Application::_initialize()
{
    QPixmap pm;
    pm.load(":/tt3/Resources/Images/Misc/Tt3Large.png");
    QIcon ic(pm);
    QGuiApplication::setWindowIcon(ic);
    QGuiApplication::setApplicationName("TimeTracker3");
    QGuiApplication::setApplicationDisplayName("TimeTracker3");
    QGuiApplication::setApplicationVersion(Component::instance()->version().toString());

    _registerStandardComponents();
    tt3::util::PluginManager::loadPlugins();
    tt3::util::ComponentManager::loadComponentSettings();
    QLocale::setDefault(tt3::gui::Component::Settings::instance()->uiLocale);
    _selectActiveSkin();

    //  Perform initial login
    tt3::gui::LoginDialog loginDialog(
        tt3::gui::theCurrentSkin->mainWindow(),
        tt3::gui::Component::Settings::instance()->rememberLastLogin ?
            tt3::gui::Component::Settings::instance()->lastLogin.value() :
            "");
    if (loginDialog.exec() != QDialog::DialogCode::Accepted)
    {   //  No need to cleanup - nothing has
        //  chnaged in application's settings
        ::exit(0);
    }
    tt3::ws::theCurrentCredentials = loginDialog.credentials();

    //  Can we open the last workspace now ?
    if (tt3::gui::Component::Settings::instance()->loadLastWorkspaceAtStartup)
    {
        tt3::ws::WorkspaceAddressesList mru =
            tt3::ws::Component::Settings::instance()->recentWorkspaces;
        if (mru.size() != 0)
        {
            try
            {
                tt3::ws::Workspace workspace =
                    mru[0]->workspaceType()->openWorkspace(mru[0]);
                if (workspace->canAccess(tt3::ws::theCurrentCredentials))
                {
                    tt3::ws::theCurrentWorkspace.swap(workspace);
                }
            }
            catch (const tt3::util::Exception & ex)
            {   //  OOPS! Report
                tt3::gui::ErrorDialog::show(tt3::gui::theCurrentSkin->mainWindow(), ex);
            }
        }
    }
}

void Application::_cleanup()
{
    //  TODO if there is a "current activity", record & stop it

    //  If there's a "current" workspace, close it
    tt3::ws::Workspace currentWorkspace;
    tt3::ws::theCurrentWorkspace.swap(currentWorkspace);
    if (currentWorkspace != nullptr)
    {
        try
        {
            currentWorkspace->close();
        }
        catch (const tt3::util::Exception & ex)
        {
            tt3::gui::ErrorDialog::show(ex);
        }
    }

    //  If there's a "current" skin, deactivate it
    tt3::gui::ISkin * currentSkin = nullptr;
    tt3::gui::theCurrentSkin.swap(currentSkin);
    Q_ASSERT(currentSkin != nullptr);
    currentSkin->deactivate();

    //  Done
    tt3::util::ComponentManager::saveComponentSettings();
}

//  End of tt3/Application.cpp
