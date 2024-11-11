using System.Reflection;
using System;

namespace TimeTracker3.Util
{
    /// <summary>
    ///     The Util plugin.
    /// </summary>
    public sealed class Plugin : IPlugin
    {   //  Public default constructor is generated automatically

        //////////
        //  IPlugin
        public string DisplayName => "Utility services";
        public Version Version => Assembly.GetCallingAssembly().GetName().Version;
        public string Copyright => "Copyright (C) 2025, Andrey Kapustin";
        public string Summary => "Defines utility services used by all other components";

        public void Initialize()
        {
        }
    }
}
