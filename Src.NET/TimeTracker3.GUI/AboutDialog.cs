using System;
using System.Windows.Forms;

namespace TimeTracker3.GUI
{
    /// <summary>
    ///     The modal "about" dialog.
    /// </summary>
    public partial class AboutDialog : Form
    {
        //////////
        //  Construction

        /// <summary>
        ///     Constructs the dialog.
        /// </summary>
        public AboutDialog()
        {
            InitializeComponent();
        }

        //////////
        //  Event handlers
        private void _LicenceButton_Click(object sender, EventArgs e)
        {
            using (LicenceDialog dlg = new LicenceDialog())
            {
                dlg.ShowDialog(this);
            }

        }

        private void AboutDialog_Shown(object sender, EventArgs e)
        {
            _OkButton.Focus();
        }
    }
}
