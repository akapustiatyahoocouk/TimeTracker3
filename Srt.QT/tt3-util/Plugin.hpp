//
//  tt3-util/Plugin.cpp - Plugin framework
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

namespace tt3::util
{
    //////////
    //  An abstract interface to a "plugin" - an agent discovered
    //  and instantiated at runtime
    class TT3_UTIL_PUBLIC IPlugin
    {
        //////////
        //  This is an interface
    protected:
        IPlugin() = default;
        virtual ~IPlugin() = default;

        //////////
        //  Operations
    public:
        //  The mnemonic identifier of this plugin.
        virtual QString         mnemonic() const = 0;

        //  The user-readanle display name of this plugin
        //  for the current default locale.
        virtual QString         displayName() const = 0;

        //  The short (1 line) user-readable description of
        //  this plugin for the current default locale.
        virtual QString         description() const = 0;

        //  The copyright message of this plugin.
        virtual QString         copyright() const = 0;

        //  The version of this plugin.
        virtual QVersionNumber  version() const = 0;

        //  The build number of this plugin.
        virtual QString         buildNumber() const = 0;

        //  The small (16x16) icon representing this plugin.
        virtual QIcon           smallIcon() const;

        //  The large (32x32) icon representing this plugin.
        virtual QIcon           largeIcon() const;

        //  Called by plugin manager to initialize this plugin.
        //  On success, returns normally. On failure throws.
        virtual void            initialize() throws(QException) = 0;

        //  True if plugin was successfully initialized, else false
        bool                    isInitialized() const;
    };

    //////////
    //  A locator and manager of kavailable plugins
    class TT3_UTIL_PUBLIC PluginManager
    {
        UTILITY_CLASS(PluginManager)

        friend class IPlugin;

        //////////
        //  Operations
    public:
        //  Loads available Plugins and initializes them.
        //  Plugins are loaded from tje .DLLs/.SOs that reside
        //  in the same directpry as the application executavle.
        static void             loadPlugins();

        //  The set of all discovered Plugins (whether
        //  successfully iniyialized or not)
        static QSet<IPlugin*>   discoveredPlugins();

        //  The set of all discovered AND successfully
        //  initialized Plugins.
        static QSet<IPlugin*>   initializedPlugins();

        //////////
        //  Implementation
    private:
        static QSet<QString>    _processedDlls;
        static QSet<IPlugin*>   _discoveredPlugins;
        static QSet<IPlugin*>   _initializedPlugins;

        //  Helpers
        static void             _loadPluginsFromLibrary(const QString & fileName);
    };

    //////////
    //  Plugin export halpers
    typedef void (*PluginExportProc)(QSet<IPlugin*> & plugins);
}

#define BEGIN_PLUGIN_TABLE()                                                \
    extern "C"                                                              \
    Q_DECL_EXPORT void PluginProvider(QSet<tt3::util::IPlugin*> & plugins); \
    extern "C"                                                              \
    Q_DECL_EXPORT void PluginProvider(QSet<tt3::util::IPlugin*> & plugins)  \
    {

#define EXPORT_PLUGIN(PluginClazz)                                      \
        static PluginClazz plugin##PluginClazz;                         \
        plugins.insert(&plugin##PluginClazz);

#define END_PLUGIN_TABLE()                                              \
    }

//  End of tt3-util/Plugin.cpp
