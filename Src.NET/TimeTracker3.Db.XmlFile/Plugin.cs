using System.Reflection;
using System;
using TimeTracker3.Db.API;
using TimeTracker3.Util;

namespace TimeTracker3.Db.XmlFile
{
    /// <summary>
    ///     The XML file DB plugin.
    /// </summary>
    public sealed class Plugin : IPlugin
    {   //  Public default constructor is generated automatically

        //////////
        //  IPlugin
        public string DisplayName => "XML file database";
        public Version Version => Assembly.GetCallingAssembly().GetName().Version;
        public string Copyright => "Copyright (C) 2025, Andrey Kapustin";
        public string Summary => "Allows storing database in a local XML file (single-user)";

        public void Initialize()
        {
            DatabaseTypeManager.RegisterDatabaseType(XmlFileDatabaseType.Instance);
        }
    }
}
