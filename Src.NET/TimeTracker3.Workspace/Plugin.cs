using System.Reflection;
using System;
using TimeTracker3.Util;

namespace TimeTracker3.Workspace
{
    /// <summary>
    ///     The Workspace plugin.
    /// </summary>
    public sealed class Plugin : IPlugin
    {   //  Public default constructor is generated automatically

        //////////
        //  IPlugin
        public string DisplayName => "Workspace";
        public Version Version => Assembly.GetCallingAssembly().GetName().Version;
        public string Copyright => "Copyright (C) 2025, Andrey Kapustin";
        public string Summary => "Defines business workspace storage services";

        public void Initialize()
        {
        }
    }
}
