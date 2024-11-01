using System.Windows.Forms;

namespace TimeTracker3.Skin.Admin
{
    /// <summary>
    ///     The main frame of the "Admin" skin.
    /// </summary>
    public sealed partial class AdminSkinMainFrame : Form
    {
        //////////
        //  Construction

        /// <summary>
        ///     Constructs the frame.
        /// </summary>
        public AdminSkinMainFrame()
        {
            InitializeComponent();
        }

        // ////////
        //  Implementation

        //  Helpers
        private void _Exit()
        {
            Application.Exit();
        }

        //////////
        //  Event handlers
        private void AdminSkinMainFrame_FormClosing(object sender, FormClosingEventArgs e)
        {
            _Exit();
        }
    }
}
