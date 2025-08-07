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
    public:
        IPlugin() = default;
    protected:
        virtual ~IPlugin() = default;

        //////////
        //  Operations
    public:
        //  TODO document
        virtual QString         mnemonic() const = 0;
        virtual QString         displayName() const = 0;
        virtual QString         desciption() const = 0;
        virtual QString         copyright() const = 0;
        virtual QVersionNumber  version() const = 0;
        virtual QString         buildNumber() const { return __DATE__; }

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
        //  TODO document
        static void             loadPlugins();
        static QSet<IPlugin*>   discoveredPlugins();
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
