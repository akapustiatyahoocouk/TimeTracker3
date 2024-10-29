using System.Reflection;
using System;

namespace TimeTracker3.Util
{
    /// <summary>
    ///     The Util plugin.
    /// </summary>
    public sealed class Plugin : IPlugin
    {
        //////////
        //  Construction
        public Plugin()
        {
        }

        //////////
        //  IPlugin
        public string DisplayName
        {
            get => "Utility services";
        }

        public Version Version
        {
            get => Assembly.GetCallingAssembly().GetName().Version;
        }

        public string Copyright
        {
            get => "Copyright (C) 2025, Andrey Kapustin";
        }

        public string Summary
        {
            get => "Defines utility services used by all other components";
        }

        public void Initialize()
        {
        }
    }
}
