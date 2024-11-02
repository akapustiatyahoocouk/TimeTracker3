using System.Net;
using System.Windows.Forms;
using TimeTracker3.Workspace;

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
            _LoadPosition();
            _TrackPosition = true;
            Refresh();
        }

        //////////
        //  object
        public override void Refresh()
        {
            base.Refresh();

            //  Title
            string title = "TimeTracker3";
            Credentials currentCredentials = Credentials.Current;
            if (currentCredentials != null)
            {
                title += " [" + currentCredentials.Login + "]";
            }

            this.Text = title;
        }

        //////////
        //  Implementation
        private bool _TrackPosition = false;

        //  Helpers
        private void _Exit()
        {
            Application.Exit();
        }

        private void _LoadPosition()
        {
            WindowState = FormWindowState.Normal;
            Bounds = AdminSkinSettings.Instance.MainFrameBounds.Value;
            if (AdminSkinSettings.Instance.MainFrameMaximized.Value)
            {
                WindowState = FormWindowState.Maximized;
            }
        }

        private void _SavePosition()
        {
            if (WindowState == FormWindowState.Normal)
            {
                AdminSkinSettings.Instance.MainFrameBounds.Value = Bounds;
                AdminSkinSettings.Instance.MainFrameMaximized.Value = false;
            }
            else if (WindowState == FormWindowState.Maximized)
            {
                AdminSkinSettings.Instance.MainFrameMaximized.Value = true;
            }
        }

        //////////
        //  Event handlers
        private void AdminSkinMainFrame_FormClosing(object sender, FormClosingEventArgs e)
        {
            _Exit();
        }

        private void _FileExitMenuItem_Click(object sender, System.EventArgs e)
        {
            _Exit();
        }

        private void AdminSkinMainFrame_LocationChanged(object sender, System.EventArgs e)
        {
            if (_TrackPosition)
            {
                _SavePosition();
            }
        }

        private void AdminSkinMainFrame_SizeChanged(object sender, System.EventArgs e)
        {
            if (_TrackPosition)
            {
                _SavePosition();
            }
        }
}
}
