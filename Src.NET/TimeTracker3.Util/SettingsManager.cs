using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Xml;

namespace TimeTracker3.Util
{
    /// <summary>
    ///     The registry/manager of known settings.
    /// </summary>
    public static class SettingsManager
    {
        //////////
        //  Properties

        /// <summary>
        ///     An unordered list of all registered Settings.
        /// </summary>
        public static Settings[] AllSettings
        {
            get => _AllSettings.ToArray();
        }

        //////////
        //  Operations

        /// <summary>
        ///     Registers the specified Settings.
        /// </summary>
        /// <param name="settings">
        ///     The settings to register.
        /// </param>
        /// <returns>
        ///     True if the settings registration was successful,
        /// else false.
        /// </returns>
        public static bool RegisterSettings(Settings settings)
        {
            Debug.Assert(settings != null);

            Settings registeredSettings = FindSettings(settings.Mnemonic);
            if (registeredSettings == null)
            {
                _AllSettings.Add(settings);
                return true;
            }
            return registeredSettings == settings;
        }

        /// <summary>
        ///     Finds a registered Settings by mnemonic.
        /// </summary>
        /// <param name="mnemonic">
        ///     The mnemonic to look for.
        /// </param>
        /// <returns>
        ///     The Settings with the required mnemonic, null if not found.
        /// </returns>
        public static Settings FindSettings(string mnemonic)
        {
            Debug.Assert(mnemonic != null);

            foreach (Settings settings in _AllSettings)
            {
                if (settings.Mnemonic == mnemonic)
                {
                    return settings;
                }
            }
            return null;
        }

        /// <summary>
        ///     Loads all registered settings from an XML file
        /// </summary>
        public static void LoadSettings()
        {
            try
            {
                //  Load XML DOM...
                XmlDocument document = new XmlDocument();
                document.Load(_SettingsFileName);
                //  ...and analyze it
                if (document.DocumentElement == null ||
                    document.DocumentElement.Name != "TimeTracker" ||
                    document.DocumentElement.GetAttribute("Version") != "3")
                {   //  OOPS! Not a valid setting file!
                    return;
                }
                foreach (XmlElement settingsElement in
                         document.DocumentElement.ChildNodes
                                 .OfType<XmlElement>()
                                 .Where(e => e.Name == "Settings"))
                {
                    Settings settings = FindSettings(settingsElement.GetAttribute("Mnemonic"));
                    if (settings != null)
                    {
                        foreach (XmlElement settingElement in
                                 settingsElement.ChildNodes
                                         .OfType<XmlElement>()
                                         .Where(e => e.Name == "Setting"))
                        {
                            AbstractSetting setting =
                                settings.FindSetting(settingElement.GetAttribute("Mnemonic"));
                            if (setting != null)
                            {
                                setting.ValueString = settingElement.GetAttribute("Value");
                            }
                        }
                    }
                }
            }
            catch (Exception)
            {   //  TODO log ?
                //  Failing to load settings is not a showstopper - we'll
                //  just continue with setting' default values
            }
        }

        /// <summary>
        ///     Saves all registered settings to an XML file
        /// </summary>
        public static void SaveSettings()
        {
            //  Create XML DOM...
            XmlDocument document = new XmlDocument();
            XmlElement documentElement = document.CreateElement("TimeTracker");
            documentElement.SetAttribute("Version", "3");
            document.AppendChild(documentElement);
            Debug.Assert(document.DocumentElement != null);
            
            foreach (Settings settings in _AllSettings)
            {
                XmlElement settingsElement = document.CreateElement("Settings");
                settingsElement.SetAttribute("Mnemonic", settings.Mnemonic);
                document.DocumentElement.AppendChild(settingsElement);
                foreach (AbstractSetting setting in settings.AllSettings)
                {
                    XmlElement settingElement = document.CreateElement("Setting");
                    settingElement.SetAttribute("Mnemonic", setting.Mnemonic);
                    settingElement.SetAttribute("Value", setting.ValueString);
                    settingsElement.AppendChild(settingElement);
                }

                //  ...and save it to XML file
                try
                {
                    document.Save(_SettingsFileName);
                }
                catch (Exception)
                {   //  TODO log ?
                }
            }
        }

        //////////
        //  Implementation
        private static string _CachedSettingsFileName;  /* = null; */
        private static readonly IList<Settings> _AllSettings = new List<Settings>();

        //  Helpers
        private static string _SettingsFileName
        {
            get
            {
                if (_CachedSettingsFileName == null)
                {
                    string appDataFolder = Environment.GetFolderPath(Environment.SpecialFolder.ApplicationData);
                    _CachedSettingsFileName = Path.Combine(appDataFolder, ".tt3.cfg");
                }
                return _CachedSettingsFileName;
            }
        }
    }
}
