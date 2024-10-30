using System.Reflection;
using System;
using TimeTracker3.Util;

namespace TimeTracker3.GUI
{
    /// <summary>
    ///     The GUI plugin.
    /// </summary>
    public sealed class Plugin : IPlugin
    {   //  Public default constructor is generated automatically

        //////////
        //  IPlugin
        public string DisplayName => "GUI";
        public Version Version => Assembly.GetCallingAssembly().GetName().Version;
        public string Copyright => "Copyright (C) 2025, Andrey Kapustin";
        public string Summary => "Defines common GUI facilities";

        public void Initialize()
        {
        }
    }
}
