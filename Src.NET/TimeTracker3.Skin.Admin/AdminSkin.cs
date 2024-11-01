using System.Drawing;
using System.Windows.Forms;
using TimeTracker3.GUI;

namespace TimeTracker3.Skin.Admin
{
    /// <summary>
    ///     The "admin skin offers full management functionality.
    /// </summary>
    public sealed class AdminSkin : ISkin
    {
        //////////
        //  Singleton
        private static AdminSkin _Instance /*= null*/;

        private AdminSkin()
        {
        }

        /// <summary>
        ///     The one and only instance of this class
        /// </summary>
        public static AdminSkin Instance => _Instance ?? (_Instance = new AdminSkin());

        //////////
        //  ISkin - Properties
        public string Mnemonic => "Administrator";
        public string DisplayName => "Administrator";
        public Image SmallImage => Properties.Resources.Tt3Small;
        public Image LargeImage => Properties.Resources.Tt3Large;
        public bool IsActive => _MainFrame != null;

        //////////
        //  ISkin - Operations
        public void Activate()
        {
            if (_MainFrame == null)
            {   //  Create UI
                _MainFrame = new AdminSkinMainFrame();
                _MainFrame.Visible = true;
            }
            else
            {   //  De-iconify if necessary
                if (_MainFrame.WindowState == FormWindowState.Minimized)
                {
                    _MainFrame.WindowState = FormWindowState.Normal;
                }
            }
            _MainFrame.BringToFront();
        }

        public void Deactivate()
        {
            if (_MainFrame != null)
            {
                _MainFrame.Dispose();
                _MainFrame = null;
            }
        }

        //////////
        //  Implementation
        private AdminSkinMainFrame _MainFrame;  //  null when this skin is inactive
    }
}
