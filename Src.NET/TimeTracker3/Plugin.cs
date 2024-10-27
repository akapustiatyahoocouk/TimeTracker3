using System;
using System.Reflection;
using TimeTracker3.Util;

namespace TimeTracker3
{
    /// <summary>
    ///     The TT3 app plugin.
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
            get => "Application";
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
            get => "Defines startup and termination logic";
        }

        public void Initialize()
        {
        }
    }
}
