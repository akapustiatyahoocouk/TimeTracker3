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
            this._HelpMenu = new System.Windows.Forms.ToolStripMenuItem();
            this._HelpAboutMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this._FileExitMenuItem = new System.Windows.Forms.ToolStripMenuItem();
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
            this._MenuStrip.Size = new System.Drawing.Size(352, 24);
            this._MenuStrip.TabIndex = 0;
            this._MenuStrip.Text = "menuStrip1";
            // 
            // _FileMenu
            // 
            this._FileMenu.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this._FileExitMenuItem});
            this._FileMenu.Name = "_FileMenu";
            this._FileMenu.Size = new System.Drawing.Size(37, 20);
            this._FileMenu.Text = "&File";
            // 
            // _HelpMenu
            // 
            this._HelpMenu.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this._HelpAboutMenuItem});
            this._HelpMenu.Name = "_HelpMenu";
            this._HelpMenu.Size = new System.Drawing.Size(44, 20);
            this._HelpMenu.Text = "&Help";
            // 
            // _HelpAboutMenuItem
            // 
            this._HelpAboutMenuItem.Image = ((System.Drawing.Image)(resources.GetObject("_HelpAboutMenuItem.Image")));
            this._HelpAboutMenuItem.Name = "_HelpAboutMenuItem";
            this._HelpAboutMenuItem.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.F1)));
            this._HelpAboutMenuItem.Size = new System.Drawing.Size(180, 22);
            this._HelpAboutMenuItem.Text = "A&bout";
            // 
            // _FileExitMenuItem
            // 
            this._FileExitMenuItem.Image = ((System.Drawing.Image)(resources.GetObject("_FileExitMenuItem.Image")));
            this._FileExitMenuItem.Name = "_FileExitMenuItem";
            this._FileExitMenuItem.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Alt | System.Windows.Forms.Keys.F4)));
            this._FileExitMenuItem.Size = new System.Drawing.Size(180, 22);
            this._FileExitMenuItem.Text = "E&xit";
            this._FileExitMenuItem.Click += new System.EventHandler(this._FileExitMenuItem_Click);
            // 
            // AdminSkinMainFrame
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(9F, 20F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(352, 190);
            this.Controls.Add(this._MenuStrip);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MainMenuStrip = this._MenuStrip;
            this.Name = "AdminSkinMainFrame";
            this.Text = "TimeTracker3";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.AdminSkinMainFrame_FormClosing);
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
    }
}