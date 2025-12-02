//
//  tt3-tools-restore/Plugin.cpp - tt3::tools::restore::Plugin class implementation
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
#include "tt3-tools-restore/API.hpp"
using namespace tt3::tools::restore;

TT3_BEGIN_PLUGIN_TABLE()
    TT3_EXPORT_PLUGIN(Plugin)
TT3_END_PLUGIN_TABLE()

//////////
//  Singleton
TT3_IMPLEMENT_SINGLETON(Plugin)
Plugin::Plugin() {}
Plugin::~Plugin() {}

//////////
//  tt3::util::IPlugin
Plugin::Mnemonic Plugin::mnemonic() const
{
    return M(tt3-tools-restore);
}

QString Plugin::displayName() const
{
    static Component::Resources *const resources = Component::Resources::instance();   //  idempotent
    return resources->string(RSID(Plugin), RID(DisplayName));
}

QString Plugin::description() const
{
    static Component::Resources *const resources = Component::Resources::instance();   //  idempotent
    return resources->string(RSID(Plugin), RID(Description));
}

QString Plugin::copyright() const
{
    static Component::Resources *const resources = Component::Resources::instance();  //  idempotent
    return resources->string(RSID(Component), RID(Copyright), QString(TT3_BUILD_DATE).left(4));
}

QVersionNumber Plugin::version() const
{
    return tt3::util::fromString<QVersionNumber>(TT3_VERSION);
}

QString Plugin::buildNumber() const
{
    return TT3_BUILD_DATE "-" TT3_BUILD_TIME;
}

void Plugin::initialize()
{
    tt3::util::ToolManager::registerTool(RestoreTool::instance());
}

//  End of tt3-tools-restore/Plugin.cpp
