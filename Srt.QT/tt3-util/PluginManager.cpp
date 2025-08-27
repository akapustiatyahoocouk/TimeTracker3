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

QSet<QString>  PluginManager::_processedDlls;
Plugins PluginManager::_discoveredPlugins;
Plugins PluginManager::_initializedPlugins;

//////////
//  Operations
void PluginManager::loadPlugins()
{
    QString startupDirectory = QCoreApplication::applicationDirPath();
    QString exeFile = QCoreApplication::applicationFilePath();
    qDebug() << startupDirectory;
    qDebug() << exeFile;
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
        for (auto plugin : _discoveredPlugins)
        {
            if (!_initializedPlugins.contains(plugin))
            {
                try
                {
                    plugin->initialize();
                    _initializedPlugins.insert(plugin);
                    keepGoing = true;
                }
                catch (...)
                {   //  OOPS! Plugin initialization failed. Maybe, next round ?
                }
            }
        }
    }
}

Plugins PluginManager::discoveredPlugins()
{
    return _discoveredPlugins;
}

Plugins PluginManager::initializedPlugins()
{
    return _initializedPlugins;
}

//////////
//  Implementation helpers
void PluginManager::_loadPluginsFromLibrary(const QString & fileName)
{
    if (_processedDlls.contains(fileName))
    {   //  Already processed
        return;
    }
    _processedDlls.insert(fileName);
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
    {
        return;
    }
    //  Get and register all plugis
    Plugins plugins;
    (*pluginExportProc)(plugins);
    for (auto plugin : plugins)
    {
        _discoveredPlugins.insert(plugin);
    }
}

//  End of tt3-util/PluginManager.cpp
