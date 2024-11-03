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
            _OkButton.Enabled =
                (_SelectedWorkspaceType != null) &&
                (_SelectedWorkspaceAddress != null);
        }

        //////////
        //  Implementation
        private WorkspaceType _SelectedWorkspaceType => 
            ((_WorkspaceTypeComboBoxItem)_WorkspaceTypeComboBox.SelectedItem)?._WorkspaceType;
        private WorkspaceAddress _SelectedWorkspaceAddress /* = null */;
        private Workspace.Workspace _CreateWorkspace /* = null*/;

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
        private void _WorkspaceTypeComboBox_SelectedIndexChanged(object sender, System.EventArgs e)
        {
            if (_SelectedWorkspaceAddress != null &&
                _SelectedWorkspaceType != _SelectedWorkspaceAddress.WorkspaceType)
            {
                _SelectedWorkspaceAddress = null;
            }
            Refresh();
        }

        private void _BrowseButton_Click(object sender, System.EventArgs e)
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

        private void _OkButton_Click(object sender, System.EventArgs e)
        {
            if (_SelectedWorkspaceAddress != null)
            {   //  Try creating the workspace
                try
                {
                    _CreateWorkspace = _SelectedWorkspaceAddress.WorkspaceType.CreateWorkspace(_SelectedWorkspaceAddress);
                    DialogResult = DialogResult.OK;
                }
                catch (Exception ex)
                {   //  OOPS! Must report, keep dialog open
                    ErrorDialog.Show(this, ex);
                }
            }
        }

        private void _CancelButton_Click(object sender, System.EventArgs e)
        {
            DialogResult = DialogResult.Cancel;
        }
    }
}
