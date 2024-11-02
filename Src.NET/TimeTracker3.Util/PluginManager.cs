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

        /// <summary>
        ///     Discovers, loads and initializes available plugins.
        ///     IMPORTANT: NOT REENTRANT!
        /// </summary>
        /// <param name="progressTracker">
        ///     The progress tracker to be notified of the plugin
        ///     loading progress, null == none.
        /// </param>
        public static void LoadPlugins(IProgressTracker progressTracker)
        {
            _ProgressTracker = progressTracker ?? new _DummyProgressTracker();
            Assembly entryAssembly = Assembly.GetEntryAssembly();
            string startupDirectory = 
                (entryAssembly != null) ?
                    Directory.GetParent(entryAssembly.Location)?.FullName :
                    ".";
            //  Locate all DLLs in the startup directory
            string[] dllFiles = Directory.GetFiles(startupDirectory ?? ".", "*.dll");
            if (dllFiles.Length > 0)
            {
                for (int i = 0; i < dllFiles.Length; i++)
                {
                    _ProgressTracker.OnProgressReached(
                        "Loading",
                        dllFiles[i],
                        (double)i / dllFiles.Length);
                    _ProcessDllFile(dllFiles[i]);
                }
            }
            //  .EXE can define plugins too
            if (entryAssembly != null)
            {
                _ProcessAssembly(Assembly.GetEntryAssembly());
                _ProgressTracker.OnProgressReached(
                    "Loading",
                    entryAssembly.Location,
                    1.0);
            }
            //  Now initialize plugins. Repeatedly, as there may be
            //  dependencies between them.
            for (bool keepGoing = true; keepGoing;)
            {
                keepGoing = false;
                foreach (IPlugin plugin in _DiscoveredPlugins)
                {
                    if (_InitializedPlugins.Contains(plugin))
                    {
                        continue;
                    }
                    try
                    {
                        plugin.Initialize();
                        //  Plugin initialization successful
                        _InitializedPlugins.Add(plugin);
                        _ProgressTracker.OnProgressReached(
                            "Initializing", 
                            plugin.DisplayName,
                            (double)_InitializedPlugins.Count / _DiscoveredPlugins.Count);
                        keepGoing = true;
                    }
                    catch
                    {   //  OOPS! Plugin initialization failed
                    }
                }
            }
        }

        //////////
        //  Implementation
        private static IProgressTracker _ProgressTracker;   //  never null
        private static readonly ISet<string> _ProcessedDllFiles = new HashSet<string>();
        private static readonly ISet<Assembly> _ProcessedAssemblies = new HashSet<Assembly>();
        private static readonly ISet<IPlugin> _DiscoveredPlugins = new HashSet<IPlugin>();
        private static readonly ISet<IPlugin> _InitializedPlugins = new HashSet<IPlugin>();

        private sealed class _DummyProgressTracker : IProgressTracker
        {
            //////////
            //  IProgressTracker
            public void OnProgressReached(string activity, string context, double? completed) {}
        }

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
                try
                {
                    IPlugin plugin = (IPlugin)constructorInfo.Invoke(new object[0]);
                    _DiscoveredPlugins.Add(plugin);
                }
                catch
                {   //  OOPS! Ignore this type.
                }
            }
        }
    }
}
