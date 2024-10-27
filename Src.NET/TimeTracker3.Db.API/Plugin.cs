using System.Reflection;
using System;
using TimeTracker3.Util;

namespace TimeTracker3.Db.API
{
    /// <summary>
    ///     The DB API plugin.
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
            get => "Database API";
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
            get => "Defines abstract API for database access";
        }

        public void Initialize()
        {
        }
    }
}
