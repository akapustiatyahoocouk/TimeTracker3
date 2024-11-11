using System.Windows.Forms;

namespace TimeTracker3.GUI
{
    /// <summary>
    ///     The modal "licence" dialog
    /// </summary>
    public sealed partial class LicenceDialog : Form
    {
        //////////
        //  Construction

        /// <summary>
        ///     Constructs the dialog.
        /// </summary>
        public LicenceDialog()
        {
            InitializeComponent();

            _LicenceTextBox.Text = Properties.Resources.GPLv3;
        }

        //////////
        //  Event handlers
        private void LicenceDialog_Shown(object sender, System.EventArgs e)
        {
            _OkButton.Focus();
        }
    }
}
