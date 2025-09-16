//
//  tt3-util/PluginManager.cpp - tt3::util::PluginManager class implementation
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
#include "tt3-util/API.hpp"
using namespace tt3::util;

struct PluginManager::_Impl
{
    QSet<QString>   processedDlls;
    Plugins         discoveredPlugins;
    Plugins         initializedPlugins;
};

//////////
//  Operations
void PluginManager::loadPlugins()
{
    _Impl * impl = _impl();

    QString startupDirectory = QCoreApplication::applicationDirPath();
    QString exeFile = QCoreApplication::applicationFilePath();
    //  Discover plugins
    for (const auto & ei : QDir(startupDirectory).entryInfoList())
    {
        if (ei.isFile() && !ei.isSymbolicLink() &&
            ei.baseName().startsWith("tt3-") && ei.absoluteFilePath() != exeFile)
        {
            qDebug() << ei;
            _loadPluginsFromLibrary(ei.absoluteFilePath());
        }
    }
    //  Initialize plugins
    for (bool keepGoing = true; keepGoing; )
    {
        keepGoing = false;
        for (auto plugin : impl->discoveredPlugins)
        {
            if (!impl->initializedPlugins.contains(plugin))
            {
                try
                {
                    plugin->initialize();
                    impl->initializedPlugins.insert(plugin);
                    keepGoing = true;
                }
                catch (const tt3::util::Exception &)
                {   //  OOPS! Plugin initialization failed. Maybe, next round ?
                }
            }
        }
    }
}

Plugins PluginManager::discoveredPlugins()
{
    return _impl()->discoveredPlugins;
}

Plugins PluginManager::initializedPlugins()
{
    return _impl()->initializedPlugins;
}

//////////
//  Implementation helpers
PluginManager::_Impl * PluginManager::_impl()
{
    static _Impl impl;
    return &impl;
}

void PluginManager::_loadPluginsFromLibrary(const QString & fileName)
{
    _Impl * impl = _impl();

    if (impl->processedDlls.contains(fileName))
    {   //  Already processed
        return;
    }
    impl->processedDlls.insert(fileName);
    //  Load file as a library
    QLibrary library(fileName);
    if (!library.load())
    {   // OOPS! Can't!
        return;
    }
    //  Is there a plugin provider ?
    PluginExportProc pluginExportProc =
        reinterpret_cast<PluginExportProc>(library.resolve("PluginProvider"));
    if (pluginExportProc == nullptr)
    {   //  No point in keeping this library loaded
        library.unload();
        return;
    }
    //  Get and register all plugis
    Plugins plugins;
    (*pluginExportProc)(plugins);
    for (auto plugin : plugins)
    {
        impl->discoveredPlugins.insert(plugin);
    }
}

//////////
//  MUST BE IN THIS FILE!!!
bool IPlugin::isInitialized() const
{
    return PluginManager::_impl()->initializedPlugins.contains(const_cast<IPlugin*>(this));
}

//  End of tt3-util/PluginManager.cpp
