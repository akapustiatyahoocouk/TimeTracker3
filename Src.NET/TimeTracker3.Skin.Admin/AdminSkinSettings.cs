using System.Drawing;
using TimeTracker3.Util;

namespace TimeTracker3.Skin.Admin
{
    /// <summary>
    ///     The Admin skin settings.
    /// </summary>
    public sealed class AdminSkinSettings : Settings
    {
        //////////
        //  Singleton
        private static AdminSkinSettings _Instance /*= null*/;

        private AdminSkinSettings()
        : base(typeof(AdminSkinSettings).FullName)
        {
            Add(MainFrameBounds);
            Add(MainFrameMaximized);
        }

        /// <summary>
        ///     The one and only instance of this class
        /// </summary>
        public static AdminSkinSettings Instance => _Instance ?? (_Instance = new AdminSkinSettings());

        //////////
        //  Properties

        /// <summary>
        ///     The normal (when not minimized or maximized) bounds
        ///     of the Admin skin main UI frame.
        /// </summary>
        public readonly Setting<Rectangle> MainFrameBounds = new Setting<Rectangle>("MainFrameBounds", new Rectangle(64, 64, 480, 320));

        /// <summary>
        ///     True if the Admin skin main frame is maximized, else false.
        /// </summary>
        public readonly Setting<bool> MainFrameMaximized = new Setting<bool>("MainFrameMaximized", false);
    }
}
