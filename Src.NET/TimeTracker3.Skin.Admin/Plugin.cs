using System.Reflection;
using System;
using TimeTracker3.GUI;
using TimeTracker3.Util;

namespace TimeTracker3.Skin.Admin
{
    /// <summary>
    ///     The XML file DB plugin.
    /// </summary>
    public sealed class Plugin : IPlugin
    {   //  Public default constructor is generated automatically

        //////////
        //  IPlugin
        public string DisplayName => "Administrator skin";
        public Version Version => Assembly.GetCallingAssembly().GetName().Version;
        public string Copyright => "Copyright (C) 2025, Andrey Kapustin";
        public string Summary => "Provides a skin that allows full control over TimeTracker3 facilities";

        public void Initialize()
        {
            SkinManager.RegisterSkin(AdminSkin.Instance);
        }
    }
}
