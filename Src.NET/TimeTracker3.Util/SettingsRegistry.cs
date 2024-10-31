using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;

namespace TimeTracker3.Util
{
    /// <summary>
    ///     The registry of known settings.
    /// </summary>
    public static class SettingsRegistry
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
        ///     The Settingswith the required mnemonic, null if not found.
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

        //////////
        //  Implementation
        private static readonly IList<Settings> _AllSettings = new List<Settings>();
    }
}
