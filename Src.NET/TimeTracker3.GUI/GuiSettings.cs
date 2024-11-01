using TimeTracker3.Util;

namespace TimeTracker3.GUI
{
    /// <summary>
    ///     The GUI settings.
    /// </summary>
    public sealed class GuiSettings : Settings
    {
        //////////
        //  Singleton
        private static GuiSettings _Instance /*= null*/;

        private GuiSettings()
        : base(typeof(GuiSettings).FullName)
        {
            Add(LastLogin);
            Add(CurrentSkin);
        }

        /// <summary>
        ///     The one and only instance of this class
        /// </summary>
        public static GuiSettings Instance => _Instance ?? (_Instance = new GuiSettings());

        //////////
        //  Properties

        /// <summary>
        ///     The last successfully used login; empty
        ///     string if not known.
        /// </summary>
        public readonly Setting<string> LastLogin = new Setting<string>("LastLogin", "");

        /// <summary>
        ///     The mnemonic identifier of the "current" skin.
        /// </summary>
        public readonly Setting<string> CurrentSkin = new Setting<string>("CurrentSkin", "");
    }
}
