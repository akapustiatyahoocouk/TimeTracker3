namespace TimeTracker3.Skin.Admin
{
    partial class AdminSkinMainFrame
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(AdminSkinMainFrame));
            this._MenuStrip = new System.Windows.Forms.MenuStrip();
            this._FileMenu = new System.Windows.Forms.ToolStripMenuItem();
            this._FileNewWorkspaceMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this._FileOpenWorkspaceMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItem1 = new System.Windows.Forms.ToolStripSeparator();
            this._FileCloseWorkspaceMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItem2 = new System.Windows.Forms.ToolStripSeparator();
            this._DestroyWorkspaceMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItem3 = new System.Windows.Forms.ToolStripSeparator();
            this._RecentWorkspacesMenu = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItem4 = new System.Windows.Forms.ToolStripSeparator();
            this._FileExitMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this._HelpMenu = new System.Windows.Forms.ToolStripMenuItem();
            this._HelpAboutMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this._ToolStrip = new System.Windows.Forms.ToolStrip();
            this._NewWorkspaceButton = new System.Windows.Forms.ToolStripButton();
            this._OpenWorkspsceButton = new System.Windows.Forms.ToolStripButton();
            this.toolStripSeparator1 = new System.Windows.Forms.ToolStripSeparator();
            this._CloseWorkspaceButton = new System.Windows.Forms.ToolStripButton();
            this.toolStripSeparator2 = new System.Windows.Forms.ToolStripSeparator();
            this._AboutButton = new System.Windows.Forms.ToolStripButton();
            this._MenuStrip.SuspendLayout();
            this._ToolStrip.SuspendLayout();
            this.SuspendLayout();
            //
            // _MenuStrip
            //
            this._MenuStrip.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this._FileMenu,
            this._HelpMenu});
            this._MenuStrip.Location = new System.Drawing.Point(0, 0);
            this._MenuStrip.Name = "_MenuStrip";
            this._MenuStrip.Padding = new System.Windows.Forms.Padding(4, 1, 0, 1);
            this._MenuStrip.Size = new System.Drawing.Size(537, 24);
            this._MenuStrip.TabIndex = 0;
            this._MenuStrip.Text = "menuStrip1";
            //
            // _FileMenu
            //
            this._FileMenu.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this._FileNewWorkspaceMenuItem,
            this._FileOpenWorkspaceMenuItem,
            this.toolStripMenuItem1,
            this._FileCloseWorkspaceMenuItem,
            this.toolStripMenuItem2,
            this._DestroyWorkspaceMenuItem,
            this.toolStripMenuItem3,
            this._RecentWorkspacesMenu,
            this.toolStripMenuItem4,
            this._FileExitMenuItem});
            this._FileMenu.Name = "_FileMenu";
            this._FileMenu.Size = new System.Drawing.Size(37, 22);
            this._FileMenu.Text = "&File";
            //
            // _FileNewWorkspaceMenuItem
            //
            this._FileNewWorkspaceMenuItem.Image = ((System.Drawing.Image)(resources.GetObject("_FileNewWorkspaceMenuItem.Image")));
            this._FileNewWorkspaceMenuItem.Name = "_FileNewWorkspaceMenuItem";
            this._FileNewWorkspaceMenuItem.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.N)));
            this._FileNewWorkspaceMenuItem.Size = new System.Drawing.Size(208, 22);
            this._FileNewWorkspaceMenuItem.Text = "&New workspace";
            this._FileNewWorkspaceMenuItem.Click += new System.EventHandler(this._FileNewWorkspaceMenuItem_Click);
            //
            // _FileOpenWorkspaceMenuItem
            //
            this._FileOpenWorkspaceMenuItem.Image = ((System.Drawing.Image)(resources.GetObject("_FileOpenWorkspaceMenuItem.Image")));
            this._FileOpenWorkspaceMenuItem.Name = "_FileOpenWorkspaceMenuItem";
            this._FileOpenWorkspaceMenuItem.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.O)));
            this._FileOpenWorkspaceMenuItem.Size = new System.Drawing.Size(208, 22);
            this._FileOpenWorkspaceMenuItem.Text = "&Open workspace";
            this._FileOpenWorkspaceMenuItem.Click += new System.EventHandler(this._FileOpenWorkspaceMenuItem_Click);
            //
            // toolStripMenuItem1
            //
            this.toolStripMenuItem1.Name = "toolStripMenuItem1";
            this.toolStripMenuItem1.Size = new System.Drawing.Size(205, 6);
            //
            // _FileCloseWorkspaceMenuItem
            //
            this._FileCloseWorkspaceMenuItem.Image = ((System.Drawing.Image)(resources.GetObject("_FileCloseWorkspaceMenuItem.Image")));
            this._FileCloseWorkspaceMenuItem.Name = "_FileCloseWorkspaceMenuItem";
            this._FileCloseWorkspaceMenuItem.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.F4)));
            this._FileCloseWorkspaceMenuItem.Size = new System.Drawing.Size(208, 22);
            this._FileCloseWorkspaceMenuItem.Text = "C&lose workspace";
            this._FileCloseWorkspaceMenuItem.Click += new System.EventHandler(this._FileCloseWorkspaceMenuItem_Click);
            //
            // toolStripMenuItem2
            //
            this.toolStripMenuItem2.Name = "toolStripMenuItem2";
            this.toolStripMenuItem2.Size = new System.Drawing.Size(205, 6);
            //
            // _DestroyWorkspaceMenuItem
            //
            this._DestroyWorkspaceMenuItem.Image = ((System.Drawing.Image)(resources.GetObject("_DestroyWorkspaceMenuItem.Image")));
            this._DestroyWorkspaceMenuItem.Name = "_DestroyWorkspaceMenuItem";
            this._DestroyWorkspaceMenuItem.Size = new System.Drawing.Size(208, 22);
            this._DestroyWorkspaceMenuItem.Text = "&Destroy workspace";
            //
            // toolStripMenuItem3
            //
            this.toolStripMenuItem3.Name = "toolStripMenuItem3";
            this.toolStripMenuItem3.Size = new System.Drawing.Size(205, 6);
            //
            // _RecentWorkspacesMenu
            //
            this._RecentWorkspacesMenu.Image = ((System.Drawing.Image)(resources.GetObject("_RecentWorkspacesMenu.Image")));
            this._RecentWorkspacesMenu.Name = "_RecentWorkspacesMenu";
            this._RecentWorkspacesMenu.Size = new System.Drawing.Size(208, 22);
            this._RecentWorkspacesMenu.Text = "&Recent workspaces";
            //
            // toolStripMenuItem4
            //
            this.toolStripMenuItem4.Name = "toolStripMenuItem4";
            this.toolStripMenuItem4.Size = new System.Drawing.Size(205, 6);
            //
            // _FileExitMenuItem
            //
            this._FileExitMenuItem.Image = ((System.Drawing.Image)(resources.GetObject("_FileExitMenuItem.Image")));
            this._FileExitMenuItem.Name = "_FileExitMenuItem";
            this._FileExitMenuItem.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Alt | System.Windows.Forms.Keys.F4)));
            this._FileExitMenuItem.Size = new System.Drawing.Size(208, 22);
            this._FileExitMenuItem.Text = "E&xit";
            this._FileExitMenuItem.Click += new System.EventHandler(this._FileExitMenuItem_Click);
            //
            // _HelpMenu
            //
            this._HelpMenu.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this._HelpAboutMenuItem});
            this._HelpMenu.Name = "_HelpMenu";
            this._HelpMenu.Size = new System.Drawing.Size(44, 22);
            this._HelpMenu.Text = "&Help";
            //
            // _HelpAboutMenuItem
            //
            this._HelpAboutMenuItem.Image = ((System.Drawing.Image)(resources.GetObject("_HelpAboutMenuItem.Image")));
            this._HelpAboutMenuItem.Name = "_HelpAboutMenuItem";
            this._HelpAboutMenuItem.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.F1)));
            this._HelpAboutMenuItem.Size = new System.Drawing.Size(180, 22);
            this._HelpAboutMenuItem.Text = "A&bout";
            this._HelpAboutMenuItem.Click += new System.EventHandler(this._HelpAboutMenuItem_Click);
            //
            // _ToolStrip
            //
            this._ToolStrip.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this._NewWorkspaceButton,
            this._OpenWorkspsceButton,
            this.toolStripSeparator1,
            this._CloseWorkspaceButton,
            this.toolStripSeparator2,
            this._AboutButton});
            this._ToolStrip.Location = new System.Drawing.Point(0, 24);
            this._ToolStrip.Name = "_ToolStrip";
            this._ToolStrip.Size = new System.Drawing.Size(537, 25);
            this._ToolStrip.TabIndex = 1;
            this._ToolStrip.Text = "toolStrip1";
            //
            // _NewWorkspaceButton
            //
            this._NewWorkspaceButton.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this._NewWorkspaceButton.Image = ((System.Drawing.Image)(resources.GetObject("_NewWorkspaceButton.Image")));
            this._NewWorkspaceButton.ImageTransparentColor = System.Drawing.Color.Magenta;
            this._NewWorkspaceButton.Name = "_NewWorkspaceButton";
            this._NewWorkspaceButton.Size = new System.Drawing.Size(23, 22);
            this._NewWorkspaceButton.Text = "Creates a new workspace";
            this._NewWorkspaceButton.Click += new System.EventHandler(this._FileNewWorkspaceMenuItem_Click);
            //
            // _OpenWorkspsceButton
            //
            this._OpenWorkspsceButton.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this._OpenWorkspsceButton.Image = ((System.Drawing.Image)(resources.GetObject("_OpenWorkspsceButton.Image")));
            this._OpenWorkspsceButton.ImageTransparentColor = System.Drawing.Color.Magenta;
            this._OpenWorkspsceButton.Name = "_OpenWorkspsceButton";
            this._OpenWorkspsceButton.Size = new System.Drawing.Size(23, 22);
            this._OpenWorkspsceButton.Text = "Opend an existing workspace";
            this._OpenWorkspsceButton.Click += new System.EventHandler(this._FileOpenWorkspaceMenuItem_Click);
            //
            // toolStripSeparator1
            //
            this.toolStripSeparator1.Name = "toolStripSeparator1";
            this.toolStripSeparator1.Size = new System.Drawing.Size(6, 25);
            //
            // _CloseWorkspaceButton
            //
            this._CloseWorkspaceButton.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this._CloseWorkspaceButton.Image = ((System.Drawing.Image)(resources.GetObject("_CloseWorkspaceButton.Image")));
            this._CloseWorkspaceButton.ImageTransparentColor = System.Drawing.Color.Magenta;
            this._CloseWorkspaceButton.Name = "_CloseWorkspaceButton";
            this._CloseWorkspaceButton.Size = new System.Drawing.Size(23, 22);
            this._CloseWorkspaceButton.Text = "Closes the current workspace";
            this._CloseWorkspaceButton.Click += new System.EventHandler(this._FileCloseWorkspaceMenuItem_Click);
            //
            // toolStripSeparator2
            //
            this.toolStripSeparator2.Name = "toolStripSeparator2";
            this.toolStripSeparator2.Size = new System.Drawing.Size(6, 25);
            //
            // _AboutButton
            //
            this._AboutButton.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this._AboutButton.Image = ((System.Drawing.Image)(resources.GetObject("_AboutButton.Image")));
            this._AboutButton.ImageTransparentColor = System.Drawing.Color.Magenta;
            this._AboutButton.Name = "_AboutButton";
            this._AboutButton.Size = new System.Drawing.Size(23, 22);
            this._AboutButton.Text = "Shows TimeTracker3 version and copyright information";
            this._AboutButton.Click += new System.EventHandler(this._HelpAboutMenuItem_Click);
            //
            // AdminSkinMainFrame
            //
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(537, 123);
            this.Controls.Add(this._ToolStrip);
            this.Controls.Add(this._MenuStrip);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MainMenuStrip = this._MenuStrip;
            this.Margin = new System.Windows.Forms.Padding(2);
            this.Name = "AdminSkinMainFrame";
            this.StartPosition = System.Windows.Forms.FormStartPosition.Manual;
            this.Text = "TimeTracker3";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.AdminSkinMainFrame_FormClosing);
            this.LocationChanged += new System.EventHandler(this.AdminSkinMainFrame_LocationChanged);
            this.SizeChanged += new System.EventHandler(this.AdminSkinMainFrame_SizeChanged);
            this._MenuStrip.ResumeLayout(false);
            this._MenuStrip.PerformLayout();
            this._ToolStrip.ResumeLayout(false);
            this._ToolStrip.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.MenuStrip _MenuStrip;
        private System.Windows.Forms.ToolStripMenuItem _FileMenu;
        private System.Windows.Forms.ToolStripMenuItem _FileExitMenuItem;
        private System.Windows.Forms.ToolStripMenuItem _HelpMenu;
        private System.Windows.Forms.ToolStripMenuItem _HelpAboutMenuItem;
        private System.Windows.Forms.ToolStripMenuItem _FileNewWorkspaceMenuItem;
        private System.Windows.Forms.ToolStripMenuItem _FileOpenWorkspaceMenuItem;
        private System.Windows.Forms.ToolStripSeparator toolStripMenuItem1;
        private System.Windows.Forms.ToolStripMenuItem _FileCloseWorkspaceMenuItem;
        private System.Windows.Forms.ToolStripSeparator toolStripMenuItem2;
        private System.Windows.Forms.ToolStripMenuItem _DestroyWorkspaceMenuItem;
        private System.Windows.Forms.ToolStripSeparator toolStripMenuItem3;
        private System.Windows.Forms.ToolStripMenuItem _RecentWorkspacesMenu;
        private System.Windows.Forms.ToolStripSeparator toolStripMenuItem4;
        private System.Windows.Forms.ToolStrip _ToolStrip;
        private System.Windows.Forms.ToolStripButton _NewWorkspaceButton;
        private System.Windows.Forms.ToolStripButton _OpenWorkspsceButton;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator1;
        private System.Windows.Forms.ToolStripButton _CloseWorkspaceButton;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator2;
        private System.Windows.Forms.ToolStripButton _AboutButton;
    }
}