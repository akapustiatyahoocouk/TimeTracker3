using System;
using System.Diagnostics;
using System.Windows.Forms;
using TimeTracker3.Workspace;

namespace TimeTracker3.GUI
{
    /// <summary>
    ///     The modal "Login" dialog.
    /// </summary>
    public partial class LoginDialog : Form
    {
        //////////
        //  Construction

        /// <summary>
        ///     Constructs the dialog with empty fields.
        /// </summary>
        public LoginDialog()
        {
            InitializeComponent();
        }

        /// <summary>
        ///     Constructs the dialog with a known login.
        /// </summary>
        public LoginDialog(string login)
        {
            Debug.Assert(login != null);

            InitializeComponent();
            _LoginTextBox.Text = login;
        }

        //////////
        //  Control
        public override void Refresh()
        {
            base.Refresh();

            _OkButton.Enabled =
                _LoginTextBox.Text.Length > 0 &&
                _LoginTextBox.Text.Length <= 255 &&
                _LoginTextBox.Text.Length == _LoginTextBox.Text.Trim().Length;
        }

        //////////
        //  Properties

        /// <summary>
        ///     The credentials specified by the user; null
        ///     if the user has cancelled the dialog.
        /// </summary>
        public Credentials Credentials => _Credentials;

        //////////
        //  Implementation
        private Credentials _Credentials /*= null*/;

        //////////
        //  Event handlers
        private void LoginDialog_Shown(object sender, EventArgs e)
        {
            if (_LoginTextBox.Text.Length > 0)
            {
                _PasswordTextBox.Focus();
            }
        }

        private void _LoginTextBox_TextChanged(object sender, EventArgs e)
        {
            Refresh();
        }

        private void _OkButton_Click(object sender, EventArgs e)
        {
            _Credentials = new Credentials(_LoginTextBox.Text, _PasswordTextBox.Text);
            DialogResult = DialogResult.OK;
        }

        private void _CancelButton_Click(object sender, EventArgs e)
        {
            _Credentials = null;
            DialogResult = DialogResult.Cancel;
        }
    }
}
