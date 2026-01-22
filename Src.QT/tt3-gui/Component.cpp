//
//  tt3-gui/Component.cpp - tt3::gui::Component class implementation
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
using namespace tt3::gui;

//////////
//  Registration
TT3_IMPLEMENT_COMPONENT(Component)

//////////
//  IComponent
Component::Mnemonic Component::mnemonic() const
{
    return M(tt3-gui);
}

QString Component::displayName() const
{
    static Resources *const resources = Resources::instance();   //  idempotent
    return resources->string(RSID(Component), RID(DisplayName));
}

QString Component::description() const
{
    static Resources *const resources = Resources::instance();   //  idempotent
    return resources->string(RSID(Component), RID(Description));
}

QString Component::copyright() const
{
    static Resources *const resources = Resources::instance();   //  idempotent
    return resources->string(RSID(Component), RID(Copyright), QString(TT3_BUILD_DATE).left(4));
}

QVersionNumber Component::version() const
{
    return tt3::util::fromString<QVersionNumber>(TT3_VERSION);
}

QString Component::stage() const
{
    return TT3_STAGE;
}

QString Component::buildNumber() const
{
    return TT3_BUILD_DATE "-" TT3_BUILD_TIME;
}

auto Component::license() const -> tt3::util::ILicense *
{
    return tt3::util::StandardLicenses::Gpl3::instance();
}

Component::ISubsystem * Component::subsystem() const
{
    return tt3::util::StandardSubsystems::Gui::instance();
}

Component::Resources * Component::resources() const
{
    return Resources::instance();
}

Component::Settings * Component::settings()
{
    return Settings::instance();
}

const Component::Settings * Component::settings() const
{
    return Settings::instance();
}

void Component::initialize()
{
}

void Component::deinitialize()
{
}

//////////
//  Component::Resources
TT3_IMPLEMENT_SINGLETON(Component::Resources)
Component::Resources::Resources()
    :   FileResourceFactory(":/tt3-gui/Resources/tt3-gui.txt") {}
Component::Resources::~Resources() {}

//////////
//  Component::Settings
TT3_IMPLEMENT_SINGLETON(Component::Settings)

Component::Settings::Settings()
    :   activeSkin(this, M(ActiveSkin), M(-), true),
        activeStyle(this, M(ActiveStyle), M(-), true),
        activeTheme(this, M(ActiveTheme), StandardThemes::System::instance()->mnemonic(), true),
        lastLogin(this, M(LastLogin), ""),
        uiLocale(this, M(UiLocale), QLocale(QLocale::English, QLocale::UnitedKingdom)),
        loadLastWorkspaceAtStartup(this, M(LoadLastWorkspaceAtStartup), false),
        lastUsedWorkspaceType(this, M(LastUsedWorkspaceType), M(-)),
        rememberLastLogin(this, M(RememberLastLogin), true),
        currentPreferences(this, M(CurrentPreferences), M(-)),
        useSplashScreen(this, M(UseSplashScreen), true),
        //  Properties (user confirmations)
        confirmCloseWorkspace(this, M(ConfirmCloseWorkspace), true),
        confirmRestart(this, M(ConfirmRestart), true),
        confirmExit(this, M(ConfirmExit), true),
        //  Properties (view configurations)
        showDisabledUsersAndAccounts(this, M(ShowDisabledUsersAndAccounts), true),
        showCompletedPublicTasks(this, M(ShowCompletedPublicTasks), true),
        showCompletedPrivateTasks(this, M(ShowCompletedPrivateTasks), true),
        showCompletedProjects(this, M(ShowCompletedProjects), true),
        myDayLogDepth(this, M(MyDayLogDepth), 1),
        //  Properties (dialogs)
        manageQuickPicksListDialogTab(this, M(ManageQuickPicksListDialogTab), 0),
        selectWorkloadsDialogTab(this, M(SelectWorkloadsDialogTab), 0),
        //  Properties (quick reports)
        quickReport(this, M(QuickReport), M(-)),
        quickReportRefreshInterval(this, M(QuickReportRefreshInterval), 0),
        dailyWorkQuickReportScale(this, M(DailyWorkQuickReportScale), 70),
        dailyWorkQuickReportShowCustomDate(this, M(DailyWorkQuickReportShowCustomDate), false),
        dailyWorkQuickReportCustomDate(this, M(DailyWorkQuickReportCustomDate), QDate::currentDate()),
        //  Properties (help)
        cacheHelpCollection(this, M(CacheHelpCollection), false),
        helpAgent(this, M(HelpAgent), HelpAgent::SystemBrowser)
{
}

Component::Settings::~Settings()
{
}

//  Formatting/parsing
namespace
{
    struct HelpAgentName
    {
        HelpAgent       helpAgent;
        const char *    name;
    };
    const HelpAgentName theHelpAgentNames[] =
    {
        { HelpAgent::SystemBrowser, "SystemBrowser" },
        { HelpAgent::HelpViewer,    "HelpViewer "   }
    };
}

template <> TT3_GUI_PUBLIC
QString tt3::util::toString<tt3::gui::HelpAgent>(const tt3::gui::HelpAgent & value)
{
    for (size_t i = 0; i < sizeof(theHelpAgentNames) / sizeof(theHelpAgentNames[0]); i++)
    {
        if (theHelpAgentNames[i].helpAgent == value)
        {
            return theHelpAgentNames[i].name;
        }
    }
    //  Else give up
    return theHelpAgentNames[0].name;
}

template <> TT3_GUI_PUBLIC
tt3::gui::HelpAgent tt3::util::fromString<tt3::gui::HelpAgent>(const QString & s, qsizetype & scan)
{
    if (scan < 0 || scan >= s.length())
    {   //  OOPS!
        throw tt3::util::ParseException(s, scan);
    }
    QString fragment = s.mid(scan, 16);
    for (size_t i = 0; i < sizeof(theHelpAgentNames) / sizeof(theHelpAgentNames[0]); i++)
    {
        if (fragment.startsWith(theHelpAgentNames[i].name))
        {
            scan += strlen(theHelpAgentNames[i].name);
            return theHelpAgentNames[i].helpAgent;
        }
    }
    //  OOPS! Give up
    throw tt3::util::ParseException(s, scan);
}

//  End of tt3-gui/Component.cpp
