namespace TimeTracker3.GUI
{
    partial class OpenWorkspaceDialog
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(OpenWorkspaceDialog));
            this._CancelButton = new System.Windows.Forms.Button();
            this._OkButton = new System.Windows.Forms.Button();
            this._BrowseButton = new System.Windows.Forms.Button();
            this._WorkspaceAddressTextBox = new System.Windows.Forms.TextBox();
            this._WorkspaceAddressLabel = new System.Windows.Forms.Label();
            this._WorkspaceTypeComboBox = new System.Windows.Forms.ComboBox();
            this._WorkspaceTypeLabel = new System.Windows.Forms.Label();
            this.SuspendLayout();
            //
            // _CancelButton
            //
            this._CancelButton.AutoSize = true;
            this._CancelButton.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this._CancelButton.Image = ((System.Drawing.Image)(resources.GetObject("_CancelButton.Image")));
            this._CancelButton.Location = new System.Drawing.Point(256, 64);
            this._CancelButton.Margin = new System.Windows.Forms.Padding(2);
            this._CancelButton.Name = "_CancelButton";
            this._CancelButton.Size = new System.Drawing.Size(66, 26);
            this._CancelButton.TabIndex = 6;
            this._CancelButton.Text = "Cancel";
            this._CancelButton.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageBeforeText;
            this._CancelButton.UseVisualStyleBackColor = true;
            this._CancelButton.Click += new System.EventHandler(this._CancelButton_Click);
            //
            // _OkButton
            //
            this._OkButton.Image = ((System.Drawing.Image)(resources.GetObject("_OkButton.Image")));
            this._OkButton.Location = new System.Drawing.Point(192, 64);
            this._OkButton.Margin = new System.Windows.Forms.Padding(2);
            this._OkButton.Name = "_OkButton";
            this._OkButton.Size = new System.Drawing.Size(61, 26);
            this._OkButton.TabIndex = 5;
            this._OkButton.Text = "OK";
            this._OkButton.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageBeforeText;
            this._OkButton.UseVisualStyleBackColor = true;
            this._OkButton.Click += new System.EventHandler(this._OkButton_Click);
            //
            // _BrowseButton
            //
            this._BrowseButton.Image = ((System.Drawing.Image)(resources.GetObject("_BrowseButton.Image")));
            this._BrowseButton.Location = new System.Drawing.Point(304, 31);
            this._BrowseButton.Name = "_BrowseButton";
            this._BrowseButton.Size = new System.Drawing.Size(24, 23);
            this._BrowseButton.TabIndex = 4;
            this._BrowseButton.UseVisualStyleBackColor = true;
            this._BrowseButton.Click += new System.EventHandler(this._BrowseButton_Click);
            //
            // _WorkspaceAddressTextBox
            //
            this._WorkspaceAddressTextBox.Location = new System.Drawing.Point(120, 32);
            this._WorkspaceAddressTextBox.Name = "_WorkspaceAddressTextBox";
            this._WorkspaceAddressTextBox.ReadOnly = true;
            this._WorkspaceAddressTextBox.Size = new System.Drawing.Size(184, 20);
            this._WorkspaceAddressTextBox.TabIndex = 3;
            //
            // _WorkspaceAddressLabel
            //
            this._WorkspaceAddressLabel.Location = new System.Drawing.Point(8, 33);
            this._WorkspaceAddressLabel.Name = "_WorkspaceAddressLabel";
            this._WorkspaceAddressLabel.Size = new System.Drawing.Size(112, 19);
            this._WorkspaceAddressLabel.TabIndex = 2;
            this._WorkspaceAddressLabel.Text = "Workspace address:";
            this._WorkspaceAddressLabel.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            //
            // _WorkspaceTypeComboBox
            //
            this._WorkspaceTypeComboBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this._WorkspaceTypeComboBox.FormattingEnabled = true;
            this._WorkspaceTypeComboBox.Location = new System.Drawing.Point(120, 8);
            this._WorkspaceTypeComboBox.Name = "_WorkspaceTypeComboBox";
            this._WorkspaceTypeComboBox.Size = new System.Drawing.Size(208, 21);
            this._WorkspaceTypeComboBox.TabIndex = 1;
            this._WorkspaceTypeComboBox.SelectedIndexChanged += new System.EventHandler(this._WorkspaceTypeComboBox_SelectedIndexChanged);
            //
            // _WorkspaceTypeLabel
            //
            this._WorkspaceTypeLabel.Location = new System.Drawing.Point(8, 9);
            this._WorkspaceTypeLabel.Name = "_WorkspaceTypeLabel";
            this._WorkspaceTypeLabel.Size = new System.Drawing.Size(112, 19);
            this._WorkspaceTypeLabel.TabIndex = 0;
            this._WorkspaceTypeLabel.Text = "Workspace type:";
            this._WorkspaceTypeLabel.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            //
            // OpenWorkspaceDialog
            //
            this.AcceptButton = this._OkButton;
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.CancelButton = this._CancelButton;
            this.ClientSize = new System.Drawing.Size(339, 101);
            this.Controls.Add(this._CancelButton);
            this.Controls.Add(this._OkButton);
            this.Controls.Add(this._BrowseButton);
            this.Controls.Add(this._WorkspaceAddressTextBox);
            this.Controls.Add(this._WorkspaceAddressLabel);
            this.Controls.Add(this._WorkspaceTypeComboBox);
            this.Controls.Add(this._WorkspaceTypeLabel);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "OpenWorkspaceDialog";
            this.ShowInTaskbar = false;
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "Open workspace";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button _CancelButton;
        private System.Windows.Forms.Button _OkButton;
        private System.Windows.Forms.Button _BrowseButton;
        private System.Windows.Forms.TextBox _WorkspaceAddressTextBox;
        private System.Windows.Forms.Label _WorkspaceAddressLabel;
        private System.Windows.Forms.ComboBox _WorkspaceTypeComboBox;
        private System.Windows.Forms.Label _WorkspaceTypeLabel;
    }
}