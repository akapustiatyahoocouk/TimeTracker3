using System.Reflection;
using System;
using TimeTracker3.Util;

namespace TimeTracker3.Db.XmlFile
{
    /// <summary>
    ///     The XML file DB plugin.
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
            get => "XML file database";
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
            get => "Allows storing database in a local XML file (single-user)";
        }

        public void Initialize()
        {
        }
    }
}
