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
    /// \class IPlugin tt3-util/API.hpp
    /// \brief
    ///     An abstract interface to a "plugin" - an agent discovered
    ///     and instantiated at runtime.
    /// \details
    ///     Any implementing concrete class P must be a SINGLETTON!
    class TT3_UTIL_PUBLIC  IPlugin
    {
        //////////
        //  Types
    public:
        /// \brief A type alias to improve code readability.
        using Mnemonic = tt3::util::Mnemonic;

        //////////
        //  This is an interface
    protected:
        /// \brief
        ///     The empty [interface] constructor.
        IPlugin() = default;

        /// \brief
        ///     The empty [interface] destructor.
        virtual ~IPlugin() = default;

        //////////
        //  Operations
    public:
        /// \brief
        ///     Returns the mnemonic identifier of this plugin.
        /// \return
        ///     The mnemonic identifier of this plugin.
        virtual Mnemonic        mnemonic() const = 0;

        /// \brief
        ///     Returns the user-readanle display name of this
        ///     plugin.
        /// \return
        ///     The user-readanle display name of this plugin
        ///     for the current default locale.
        virtual QString         displayName() const = 0;

        /// \brief
        ///     Returns the short (1 line) user-readable
        ///     description of this plugin.
        /// \return
        ///     The short (1 line) user-readable description of
        ///     this plugin for the current default locale.
        virtual QString         description() const = 0;

        /// \brief
        ///     Returns the copyright message of this plugin.
        /// \return
        ///     The copyright message of this plugin for the
        ///     current default locale.
        virtual QString         copyright() const = 0;

        /// \brief
        ///     Returns the version of this plugin.
        /// \return
        ///     The version of this plugin.
        virtual QVersionNumber  version() const = 0;

        /// \brief
        ///     Returns the build number of this plugin.
        /// \return
        ///     The build number of this plugin.
        virtual QString         buildNumber() const = 0;

        /// \brief
        ///     Returns the small (16x16) icon representing this plugin.
        /// \return
        ///     The small (16x16) icon representing this plugin.
        virtual QIcon           smallIcon() const;

        /// \brief
        ///     Returns the large (32x32) icon representing this plugin.
        /// \return
        ///     The large (32x32) icon representing this plugin.
        virtual QIcon           largeIcon() const;

        /// \brief
        ///     Called by plugin manager to initialize this plugin.
        /// \details
        ///     On success, returns normally.
        /// \exception Exception
        ///     (Or a subclass thereof) if an error occurs.
        virtual void            initialize() = 0;

        /// \brief
        ///     Checks whether this plugin was successfully
        ///     initialized.
        /// \return
        ///     True if plugin was successfully initialized,
        ///     else false.
        bool                    isInitialized() const;
    };

    /// \class PluginManager tt3-util/API.hpp
    /// \brief A locator and manager of available plugins.
    class TT3_UTIL_PUBLIC PluginManager
    {
        TT3_UTILITY_CLASS(PluginManager)

        friend class IPlugin;

        //////////
        //  Operations
    public:
        /// \brief
        ///     Loads available Plugins and initializes them.
        /// \details
        ///     Plugins are loaded from tje .DLLs/.SOs that reside
        ///     in the same directpry as the application executavle.
        static void         loadPlugins();

        /// \brief
        ///     Returns the set of all discovered Plugins (whether
        ///     successfully iniyialized or not)
        /// \return
        ///     The set of all discovered Plugins (whether
        ///     successfully iniyialized or not)
        static Plugins      discoveredPlugins();

        /// \brief
        ///     Returns the set of all discovered AND successfully
        ///     initialized Plugins.
        /// \return
        ///     The set of all discovered AND successfully
        ///     initialized Plugins.
        static Plugins      initializedPlugins();

        //////////
        //  Implementation
    private:
        struct _Impl;

        //  Helpers
        static _Impl *          _impl();
        static void             _loadPluginsFromLibrary(const QString & fileName);
    };

    //////////
    //  Plugin export helpers
    typedef void (*PluginExportProc)(Plugins & plugins);
}

#define TT3_BEGIN_PLUGIN_TABLE()                                            \
    extern "C" /* GCC -Weffc++ requires a prototype! */                     \
    Q_DECL_EXPORT void PluginProvider(QSet<tt3::util::IPlugin*> & plugins); \
    extern "C"                                                              \
    Q_DECL_EXPORT void PluginProvider(QSet<tt3::util::IPlugin*> & plugins)  \
    {

#define TT3_EXPORT_PLUGIN(PluginClazz)              \
        plugins.insert(PluginClazz::instance());

#define TT3_END_PLUGIN_TABLE()  \
    }

//  End of tt3-util/Plugin.cpp
