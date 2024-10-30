using System;
using System.Reflection;
using TimeTracker3.Util;

namespace TimeTracker3
{
    /// <summary>
    ///     The TT3 app plugin.
    /// </summary>
    public sealed class Plugin : IPlugin
    {   //  Public default constructor is generated automatically

        //////////
        //  IPlugin
        public string DisplayName => "Application";
        public Version Version => Assembly.GetCallingAssembly().GetName().Version;
        public string Copyright => "Copyright (C) 2025, Andrey Kapustin";
        public string Summary => "Defines startup and termination logic";

        public void Initialize()
        {
        }
    }
}
