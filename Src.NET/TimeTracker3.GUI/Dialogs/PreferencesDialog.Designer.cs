namespace TimeTracker3.GUI.Dialogs
{
    partial class PreferencesDialog
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(PreferencesDialog));
            this._PreferencesTtreeView = new System.Windows.Forms.TreeView();
            this._PreferencesPanel = new System.Windows.Forms.Panel();
            this._OkButton = new System.Windows.Forms.Button();
            this._CancelButton = new System.Windows.Forms.Button();
            this.SuspendLayout();
            //
            // _PreferencesTtreeView
            //
            this._PreferencesTtreeView.Location = new System.Drawing.Point(8, 8);
            this._PreferencesTtreeView.Name = "_PreferencesTtreeView";
            this._PreferencesTtreeView.Size = new System.Drawing.Size(224, 248);
            this._PreferencesTtreeView.TabIndex = 0;
            //
            // _PreferencesPanel
            //
            this._PreferencesPanel.Location = new System.Drawing.Point(240, 8);
            this._PreferencesPanel.Name = "_PreferencesPanel";
            this._PreferencesPanel.Size = new System.Drawing.Size(256, 248);
            this._PreferencesPanel.TabIndex = 1;
            //
            // _OkButton
            //
            this._OkButton.Image = ((System.Drawing.Image)(resources.GetObject("_OkButton.Image")));
            this._OkButton.Location = new System.Drawing.Point(336, 272);
            this._OkButton.Name = "_OkButton";
            this._OkButton.RightToLeft = System.Windows.Forms.RightToLeft.No;
            this._OkButton.Size = new System.Drawing.Size(75, 23);
            this._OkButton.TabIndex = 2;
            this._OkButton.Text = "OK";
            this._OkButton.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageBeforeText;
            this._OkButton.UseVisualStyleBackColor = true;
            //
            // _CancelButton
            //
            this._CancelButton.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this._CancelButton.Image = ((System.Drawing.Image)(resources.GetObject("_CancelButton.Image")));
            this._CancelButton.Location = new System.Drawing.Point(416, 272);
            this._CancelButton.Name = "_CancelButton";
            this._CancelButton.Size = new System.Drawing.Size(75, 23);
            this._CancelButton.TabIndex = 3;
            this._CancelButton.Text = "Cancel";
            this._CancelButton.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageBeforeText;
            this._CancelButton.UseVisualStyleBackColor = true;
            //
            // PreferencesDialog
            //
            this.AcceptButton = this._OkButton;
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.CancelButton = this._CancelButton;
            this.ClientSize = new System.Drawing.Size(505, 302);
            this.Controls.Add(this._CancelButton);
            this.Controls.Add(this._OkButton);
            this.Controls.Add(this._PreferencesPanel);
            this.Controls.Add(this._PreferencesTtreeView);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "PreferencesDialog";
            this.ShowInTaskbar = false;
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "Preferences";
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.TreeView _PreferencesTtreeView;
        private System.Windows.Forms.Panel _PreferencesPanel;
        private System.Windows.Forms.Button _OkButton;
        private System.Windows.Forms.Button _CancelButton;
    }
}