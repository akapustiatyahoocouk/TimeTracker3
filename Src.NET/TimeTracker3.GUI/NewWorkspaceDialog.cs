using System;
using System.Linq;
using System.Windows.Forms;
using TimeTracker3.Workspace;

namespace TimeTracker3.GUI
{
    /// <summary>
    ///     The "new workspace" modal dialog.
    /// </summary>
    public sealed partial class NewWorkspaceDialog : Form
    {
        //////////
        //  Construction
        public NewWorkspaceDialog()
        {
            InitializeComponent();

            //  Populate the "workspace type" combo box
            foreach (WorkspaceType workspaceType in
                     WorkspaceType.All.OrderBy(t => t.DisplayName))
            {
                _WorkspaceTypeComboBox.Items.Add(new _WorkspaceTypeComboBoxItem(workspaceType));
            }

            //  Adjust controls
            if (_WorkspaceTypeComboBox.Items.Count > 0)
            {   //  TODO select the last workspace type used
                //  by this dialog to create a workspace
                _WorkspaceTypeComboBox.SelectedIndex = 0;
            }
            else
            {   //  OOPS! No workspace types!
                _WorkspaceTypeComboBox.Enabled = false;
                _WorkspaceAddressLabel.Enabled = false;
                _WorkspaceAddressTextBox.Enabled = false;
                _BrowseButton.Enabled = false;
            }

            //  Done
            Refresh();
        }

        //////////
        //  Control
        public override void Refresh()
        {
            base.Refresh();
            _WorkspaceAddressTextBox.Text =
                (_SelectedWorkspaceAddress != null) ? _SelectedWorkspaceAddress.DisplayForm : "";
            _LoginLabel.Enabled = _UseCustomCredentialsRadioButton.Checked;
            _LoginTextBox.Enabled = _UseCustomCredentialsRadioButton.Checked;
            _Password1Label.Enabled = _UseCustomCredentialsRadioButton.Checked;
            _Password1TextBox.Enabled = _UseCustomCredentialsRadioButton.Checked;
            _Password2Label.Enabled = _UseCustomCredentialsRadioButton.Checked;
            _Password2TextBox.Enabled = _UseCustomCredentialsRadioButton.Checked;
            _OkButton.Enabled =
                (_SelectedWorkspaceType != null) &&
                (_SelectedWorkspaceAddress != null) &&
                _UseCurrentCredentialsRadioButton.Checked ?
                    CurrentCredentialsProvider.Instance.Value != null :
                    (_LoginTextBox.Text.Length > 0 && _LoginTextBox.Text.Length <= 255 &&
                     _Password1TextBox.Text == _Password2TextBox.Text);
        }

        //////////
        //  Properties

        /// <summary>
        ///     The workspace created with this dialog or null
        ///     if the user has cancelled the workspace creation
        ///     process by cancelling this dialog.
        /// </summary>
        public Workspace.Workspace CreatedWorkspace => _CreatedWorkspace;

        /// <summary>
        ///     The administrator credentials for the newly
        ///     created workspace..
        /// </summary>
        public Credentials CreatedWorkspaceCredentials => _CreatedWorkspaceCredentials;

        //////////
        //  Implementation
        private WorkspaceType _SelectedWorkspaceType =>
            ((_WorkspaceTypeComboBoxItem)_WorkspaceTypeComboBox.SelectedItem)?._WorkspaceType;
        private WorkspaceAddress _SelectedWorkspaceAddress /* = null */;
        private Workspace.Workspace _CreatedWorkspace /* = null*/;
        private Credentials _CreatedWorkspaceCredentials /* = null*/;

        private sealed class _WorkspaceTypeComboBoxItem
        {
            //  Construction
            internal _WorkspaceTypeComboBoxItem(WorkspaceType workspaceType)
            {
                _WorkspaceType = workspaceType;
            }

            //  object
            public override string ToString()
            {
                return _WorkspaceType.DisplayName;
            }

            //  Properties
            internal readonly WorkspaceType _WorkspaceType;
        }

        //////////
        //  Event handlers
        private void _WorkspaceTypeComboBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (_SelectedWorkspaceAddress != null &&
                _SelectedWorkspaceType != _SelectedWorkspaceAddress.WorkspaceType)
            {
                _SelectedWorkspaceAddress = null;
            }
            Refresh();
        }

        private void _BrowseButton_Click(object sender, EventArgs e)
        {
            if (_SelectedWorkspaceType != null)
            {
                WorkspaceAddress workspaceAddress =
                    _SelectedWorkspaceType.EnterNewWorkspaceAddress(this);
                if (workspaceAddress != null)
                {
                    _SelectedWorkspaceAddress = workspaceAddress;
                    Refresh();
                }
            }
        }

        private void _OkButton_Click(object sender, EventArgs e)
        {
            if (_SelectedWorkspaceAddress != null)
            {   //  Try creating the workspace
                try
                {
                    _CreatedWorkspace = _SelectedWorkspaceAddress.WorkspaceType.CreateWorkspace(_SelectedWorkspaceAddress);
                    _CreatedWorkspaceCredentials =
                        _UseCurrentCredentialsRadioButton.Checked ?
                            CurrentCredentialsProvider.Instance.Value :
                            new Credentials(_LoginTextBox.Text, _Password1TextBox.Text);
                    DialogResult = DialogResult.OK;
                }
                catch (Exception ex)
                {   //  OOPS! Must report, keep dialog open
                    ErrorDialog.Show(this, ex);
                }
            }
        }

        private void _CancelButton_Click(object sender, EventArgs e)
        {
            DialogResult = DialogResult.Cancel;
        }

        private void _UseCurrentCredentialsRadioButton_CheckedChanged(object sender, EventArgs e)
        {
            Refresh();
        }

        private void _UseCustomCredentialsRadioButton_CheckedChanged(object sender, EventArgs e)
        {
            Refresh();
        }

        private void _LoginTextBox_TextChanged(object sender, EventArgs e)
        {
            Refresh();
        }

        private void _Password1TextBox_TextChanged(object sender, EventArgs e)
        {
            Refresh();
        }

        private void _Password2TextBox_TextChanged(object sender, EventArgs e)
        {
            Refresh();
        }
    }
}
