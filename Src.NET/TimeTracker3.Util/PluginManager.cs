using System;
using System.Collections.Generic;
using System.IO;
using System.Reflection;

namespace TimeTracker3.Util
{
    /// <summary>
    ///     The plugin manager.
    /// </summary>
    public static class PluginManager
    {
        //////////
        //  Properties

        //////////
        //  Operations

        public static void LoadPlugins()
        {
            Assembly entryAssembly = Assembly.GetEntryAssembly();
            string startupDirectory = 
                (entryAssembly != null) ?
                    Directory.GetParent(entryAssembly.Location)?.FullName :
                    ".";
            //  Locate all DLLs in the startup directory
            foreach (string dllFile in Directory.GetFiles(startupDirectory ?? ".", "*.dll"))
            {
                _ProcessDllFile(dllFile);
            }
            //  .EXE can define plugins too
            _ProcessAssembly(Assembly.GetEntryAssembly());
            //  Now initialize plugins. Repeatedly, as there may be
            //  dependencies between them.
            for (bool keepGoing = true; keepGoing;)
            {
                keepGoing = false;
                foreach (IPlugin plugin in _DiscoveredPlugins)
                {
                    if (!_InitializedPlugins.Contains(plugin))
                    {
                        try
                        {
                            plugin.Initialize();
                            //  Plugin initialization successful
                            _InitializedPlugins.Add(plugin);
                            keepGoing = true;
                        }
                        catch
                        {   //  OOPS! Plugin initialization failed
                        }
                    }
                }
            }
        }

        //////////
        //  Implementation
        private static readonly ISet<string> _ProcessedDllFiles = new HashSet<string>();
        private static readonly ISet<Assembly> _ProcessedAssemblies = new HashSet<Assembly>();
        private static readonly ISet<IPlugin> _DiscoveredPlugins = new HashSet<IPlugin>();
        private static readonly ISet<IPlugin> _InitializedPlugins = new HashSet<IPlugin>();

        //  Helpers
        private static void _ProcessDllFile(string dllFile)
        {   //  Process each dll at most once
            if (_ProcessedDllFiles.Contains(dllFile))
            {
                return;
            }
            _ProcessedDllFiles.Add(dllFile);
            //  Can we load the DLL as an assembly ?
            try 
            {
                Assembly assembly = Assembly.LoadFile(dllFile);
                _ProcessAssembly(assembly);
            }
            catch
            {   //  OOPS! Ignore this DLL
            }
        }

        private static void _ProcessAssembly(Assembly assembly)
        {   //  Only process each assembly once
            if (_ProcessedAssemblies.Contains(assembly))
            {
                return;
            }
            _ProcessedAssemblies.Add(assembly);
            //  Detect plugin types & instantiate
            foreach (Type type in assembly.GetTypes())
            {   //  We need a public concrete class...
                if (!type.IsClass || !type.IsPublic || type.IsAbstract)
                {
                    continue;
                }
                //  ...that implements IPlugin...
                if (!typeof(IPlugin).IsAssignableFrom(type))
                {
                    continue;
                }
                //  ...with a public default constructor...
                ConstructorInfo constructorInfo = type.GetConstructor(Type.EmptyTypes);
                if (constructorInfo == null)
                {
                    continue;
                }
                //  ...that we need to call
                IPlugin plugin;
                try
                {
                    plugin = (IPlugin)constructorInfo.Invoke(new object[0]);
                }
                catch
                {   //  OOPS! Ignore this type.
                    continue;
                }
                _DiscoveredPlugins.Add(plugin);
            }
        }
    }
}
