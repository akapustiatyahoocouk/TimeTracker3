using System;
using System.Net;
using System.Windows.Forms;
using TimeTracker3.GUI;
using TimeTracker3.Workspace;
using TimeTracker3.Workspace.Exceptions;

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
            Credentials currentCredentials = CurrentCredentialsProvider.Instance.Value;
            if (currentCredentials != null)
            {
                title += " [" + currentCredentials.Login + "]";
            }

            this.Text = title;
        }

        //////////
        //  Implementation
        private bool _TrackPosition /*= false */;

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

        private Credentials _ObtainCompatibleCredentials(Workspace.Workspace workspace, Credentials credentials)
        {   //  Returns null if the user has chosen to "cancel"
            if (workspace == null)
            {   //  Any Credentials, even empty ones, are compatible
                //  with "no workspace"
                return credentials;
            }
            while (credentials == null ||
                   workspace.TryLogin(credentials) == null)
            {   //  Ask user for new credentials
                using (var dlg = new LoginDialog(credentials?.Login))
                {
                    if (dlg.ShowDialog(this) != DialogResult.OK)
                    {   //  Give up
                        return null;
                    }
                    credentials = dlg.Credentials;  //  try these ones
                }
            }
            return credentials;
        }

        private bool _ChangeWorkspace(Workspace.Workspace newWorkspace, Credentials newCredentials)
        {   //  returns true if newWorkspace has become "current", else false
            if (newWorkspace == null)
            {   //  Just make sure there is no "current" workspace
                try
                {
                    CurrentWorkspaceProvider.Instance.Value?.Close();
                }
                finally
                {   //  Even in case Close() throws, the "current"
                    //  workspace must still become null
                    CurrentWorkspaceProvider.Instance.Value = null;
                }
                return true;
            }
            if (newCredentials == null)
            {   //  OOPS! Null credentials do not grant
                //  access to any workspace
                throw new AccessDeniedWorkspaceException();
            }
            //  General case - newWorkspace and newCredentials
            //  are both not null
            if (newWorkspace == CurrentWorkspaceProvider.Instance.Value)
            {   //  Workspace doesn't change - but do the credentials ?
                if (newCredentials != CurrentCredentialsProvider.Instance.Value)
                {   //  Yes - must replace "current" credentials.
                    //  Do newCredentials grant access to the "current" workspace?
                    newCredentials = _ObtainCompatibleCredentials(newWorkspace, newCredentials);
                    if (newCredentials == null)
                    {   //  OOPS! The user has refused to provide credentials
                        //  compatible with newWorkspace
                        return false;
                    }
                }
                //  Nothing to do
                return true;
            }
            //  The "current" workspace actually changes.
            //  Do newCredentials grant access to the newWorkspace?
            newCredentials = _ObtainCompatibleCredentials(newWorkspace, newCredentials);
            if (newCredentials == null)
            {   //  OOPS! The user has refused to provide credentials
                //  compatible with newWorkspace
                return false;
            }
            //  All OK - replace "current" Workspace and Credentials
            CurrentWorkspaceProvider.Instance.Value?.Close();   //  TODO catch...
            CurrentWorkspaceProvider.Instance.Value = newWorkspace;
            CurrentCredentialsProvider.Instance.Value = newCredentials;
            return true;
        }

        //////////
        //  Event handlers
        private void AdminSkinMainFrame_FormClosing(object sender, FormClosingEventArgs e)
        {
            _Exit();
        }

        private void _FileExitMenuItem_Click(object sender, EventArgs e)
        {
            _Exit();
        }

        private void AdminSkinMainFrame_LocationChanged(object sender, EventArgs e)
        {
            if (_TrackPosition)
            {
                _SavePosition();
            }
        }

        private void AdminSkinMainFrame_SizeChanged(object sender, EventArgs e)
        {
            if (_TrackPosition)
            {
                _SavePosition();
            }
        }

        private void _NewWorkspaceMenuItem_Click(object sender, EventArgs e)
        {
            using (NewWorkspaceDialog dlg = new NewWorkspaceDialog())
            {
                if (dlg.ShowDialog(this) == DialogResult.OK)
                {
                    if (!_ChangeWorkspace(dlg.CreatedWorkspace, dlg.CreatedWorkspaceCredentials))
                    {   //  OOPS! We won't be using the newly created workspace
                        try
                        {
                            dlg.CreatedWorkspace.Close();   //  TODO catch...
                        }
                        catch (Exception ex)
                        {
                            ErrorDialog.Show(this, ex);
                        }
                        try
                        {
                            dlg.CreatedWorkspace.Type.DestroyWorkspace(dlg.CreatedWorkspace.Address);   //  TODO catch...
                        }
                        catch (Exception ex)
                        {
                            ErrorDialog.Show(this, ex);
                        }
                    }
                }
            }
        }
    }
}
