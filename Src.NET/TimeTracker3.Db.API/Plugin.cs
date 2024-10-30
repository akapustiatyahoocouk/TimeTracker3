using System.Reflection;
using System;
using TimeTracker3.Util;

namespace TimeTracker3.Db.API
{
    /// <summary>
    ///     The DB API plugin.
    /// </summary>
    public sealed class Plugin : IPlugin
    {   //  Public default constructor is generated automatically

        //////////
        //  IPlugin
        public string DisplayName => "Database API";
        public Version Version => Assembly.GetCallingAssembly().GetName().Version;
        public string Copyright => "Copyright (C) 2025, Andrey Kapustin";
        public string Summary => "Defines abstract API for database access";

        public void Initialize()
        {
        }
    }
}
