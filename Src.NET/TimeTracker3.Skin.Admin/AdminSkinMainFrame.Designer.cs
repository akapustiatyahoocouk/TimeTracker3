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
            this._NewWorkspaceMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this._OpenWorkspaceMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItem1 = new System.Windows.Forms.ToolStripSeparator();
            this._CloseWorkspaceMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItem2 = new System.Windows.Forms.ToolStripSeparator();
            this._DestroyWorkspaceMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItem3 = new System.Windows.Forms.ToolStripSeparator();
            this._FileExitMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this._HelpMenu = new System.Windows.Forms.ToolStripMenuItem();
            this._HelpAboutMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this._MenuStrip.SuspendLayout();
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
            this._MenuStrip.Size = new System.Drawing.Size(235, 24);
            this._MenuStrip.TabIndex = 0;
            this._MenuStrip.Text = "menuStrip1";
            // 
            // _FileMenu
            // 
            this._FileMenu.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this._NewWorkspaceMenuItem,
            this._OpenWorkspaceMenuItem,
            this.toolStripMenuItem1,
            this._CloseWorkspaceMenuItem,
            this.toolStripMenuItem2,
            this._DestroyWorkspaceMenuItem,
            this.toolStripMenuItem3,
            this._FileExitMenuItem});
            this._FileMenu.Name = "_FileMenu";
            this._FileMenu.Size = new System.Drawing.Size(37, 22);
            this._FileMenu.Text = "&File";
            // 
            // _NewWorkspaceMenuItem
            // 
            this._NewWorkspaceMenuItem.Image = ((System.Drawing.Image)(resources.GetObject("_NewWorkspaceMenuItem.Image")));
            this._NewWorkspaceMenuItem.Name = "_NewWorkspaceMenuItem";
            this._NewWorkspaceMenuItem.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.N)));
            this._NewWorkspaceMenuItem.Size = new System.Drawing.Size(208, 22);
            this._NewWorkspaceMenuItem.Text = "&New workspace";
            this._NewWorkspaceMenuItem.Click += new System.EventHandler(this._NewWorkspaceMenuItem_Click);
            // 
            // _OpenWorkspaceMenuItem
            // 
            this._OpenWorkspaceMenuItem.Image = ((System.Drawing.Image)(resources.GetObject("_OpenWorkspaceMenuItem.Image")));
            this._OpenWorkspaceMenuItem.Name = "_OpenWorkspaceMenuItem";
            this._OpenWorkspaceMenuItem.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.O)));
            this._OpenWorkspaceMenuItem.Size = new System.Drawing.Size(208, 22);
            this._OpenWorkspaceMenuItem.Text = "&Open workspace";
            // 
            // toolStripMenuItem1
            // 
            this.toolStripMenuItem1.Name = "toolStripMenuItem1";
            this.toolStripMenuItem1.Size = new System.Drawing.Size(205, 6);
            // 
            // _CloseWorkspaceMenuItem
            // 
            this._CloseWorkspaceMenuItem.Image = ((System.Drawing.Image)(resources.GetObject("_CloseWorkspaceMenuItem.Image")));
            this._CloseWorkspaceMenuItem.Name = "_CloseWorkspaceMenuItem";
            this._CloseWorkspaceMenuItem.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.F4)));
            this._CloseWorkspaceMenuItem.Size = new System.Drawing.Size(208, 22);
            this._CloseWorkspaceMenuItem.Text = "C&lose workspace";
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
            this._HelpAboutMenuItem.Size = new System.Drawing.Size(153, 22);
            this._HelpAboutMenuItem.Text = "A&bout";
            // 
            // AdminSkinMainFrame
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(235, 123);
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
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.MenuStrip _MenuStrip;
        private System.Windows.Forms.ToolStripMenuItem _FileMenu;
        private System.Windows.Forms.ToolStripMenuItem _FileExitMenuItem;
        private System.Windows.Forms.ToolStripMenuItem _HelpMenu;
        private System.Windows.Forms.ToolStripMenuItem _HelpAboutMenuItem;
        private System.Windows.Forms.ToolStripMenuItem _NewWorkspaceMenuItem;
        private System.Windows.Forms.ToolStripMenuItem _OpenWorkspaceMenuItem;
        private System.Windows.Forms.ToolStripSeparator toolStripMenuItem1;
        private System.Windows.Forms.ToolStripMenuItem _CloseWorkspaceMenuItem;
        private System.Windows.Forms.ToolStripSeparator toolStripMenuItem2;
        private System.Windows.Forms.ToolStripMenuItem _DestroyWorkspaceMenuItem;
        private System.Windows.Forms.ToolStripSeparator toolStripMenuItem3;
    }
}