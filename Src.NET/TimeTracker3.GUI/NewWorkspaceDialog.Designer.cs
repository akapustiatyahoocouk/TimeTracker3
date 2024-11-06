namespace TimeTracker3.GUI
{
    partial class NewWorkspaceDialog
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(NewWorkspaceDialog));
            this._CancelButton = new System.Windows.Forms.Button();
            this._OkButton = new System.Windows.Forms.Button();
            this._BrowseButton = new System.Windows.Forms.Button();
            this._WorkspaceAddressTextBox = new System.Windows.Forms.TextBox();
            this._WorkspaceAddressLabel = new System.Windows.Forms.Label();
            this._WorkspaceTypeComboBox = new System.Windows.Forms.ComboBox();
            this._WorkspaceTypeLabel = new System.Windows.Forms.Label();
            this._WorkspaceAdministratorGroupBox = new System.Windows.Forms.GroupBox();
            this._UseCurrentCredentialsRadioButton = new System.Windows.Forms.RadioButton();
            this._UseCustomCredentialsRadioButton = new System.Windows.Forms.RadioButton();
            this._Password1TextBox = new System.Windows.Forms.TextBox();
            this._Password1Label = new System.Windows.Forms.Label();
            this._LoginTextBox = new System.Windows.Forms.TextBox();
            this._LoginLabel = new System.Windows.Forms.Label();
            this._Password2TextBox = new System.Windows.Forms.TextBox();
            this._Password2Label = new System.Windows.Forms.Label();
            this._WorkspaceAdministratorGroupBox.SuspendLayout();
            this.SuspendLayout();
            // 
            // _CancelButton
            // 
            this._CancelButton.AutoSize = true;
            this._CancelButton.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this._CancelButton.Image = ((System.Drawing.Image)(resources.GetObject("_CancelButton.Image")));
            this._CancelButton.Location = new System.Drawing.Point(256, 208);
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
            this._OkButton.Location = new System.Drawing.Point(192, 208);
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
            // _WorkspaceAdministratorGroupBox
            // 
            this._WorkspaceAdministratorGroupBox.Controls.Add(this._Password2TextBox);
            this._WorkspaceAdministratorGroupBox.Controls.Add(this._Password2Label);
            this._WorkspaceAdministratorGroupBox.Controls.Add(this._Password1TextBox);
            this._WorkspaceAdministratorGroupBox.Controls.Add(this._Password1Label);
            this._WorkspaceAdministratorGroupBox.Controls.Add(this._LoginTextBox);
            this._WorkspaceAdministratorGroupBox.Controls.Add(this._LoginLabel);
            this._WorkspaceAdministratorGroupBox.Controls.Add(this._UseCustomCredentialsRadioButton);
            this._WorkspaceAdministratorGroupBox.Controls.Add(this._UseCurrentCredentialsRadioButton);
            this._WorkspaceAdministratorGroupBox.Location = new System.Drawing.Point(8, 56);
            this._WorkspaceAdministratorGroupBox.Name = "_WorkspaceAdministratorGroupBox";
            this._WorkspaceAdministratorGroupBox.Size = new System.Drawing.Size(320, 144);
            this._WorkspaceAdministratorGroupBox.TabIndex = 7;
            this._WorkspaceAdministratorGroupBox.TabStop = false;
            this._WorkspaceAdministratorGroupBox.Text = "Workspace administrator";
            // 
            // _UseCurrentCredentialsRadioButton
            // 
            this._UseCurrentCredentialsRadioButton.Checked = true;
            this._UseCurrentCredentialsRadioButton.Location = new System.Drawing.Point(8, 24);
            this._UseCurrentCredentialsRadioButton.Name = "_UseCurrentCredentialsRadioButton";
            this._UseCurrentCredentialsRadioButton.Size = new System.Drawing.Size(288, 19);
            this._UseCurrentCredentialsRadioButton.TabIndex = 0;
            this._UseCurrentCredentialsRadioButton.TabStop = true;
            this._UseCurrentCredentialsRadioButton.Text = "Use current login credentials";
            this._UseCurrentCredentialsRadioButton.UseVisualStyleBackColor = true;
            this._UseCurrentCredentialsRadioButton.CheckedChanged += new System.EventHandler(this._UseCurrentCredentialsRadioButton_CheckedChanged);
            // 
            // _UseCustomCredentialsRadioButton
            // 
            this._UseCustomCredentialsRadioButton.Location = new System.Drawing.Point(8, 48);
            this._UseCustomCredentialsRadioButton.Name = "_UseCustomCredentialsRadioButton";
            this._UseCustomCredentialsRadioButton.Size = new System.Drawing.Size(288, 19);
            this._UseCustomCredentialsRadioButton.TabIndex = 1;
            this._UseCustomCredentialsRadioButton.Text = "Use custom login credentials";
            this._UseCustomCredentialsRadioButton.UseVisualStyleBackColor = true;
            this._UseCustomCredentialsRadioButton.CheckedChanged += new System.EventHandler(this._UseCustomCredentialsRadioButton_CheckedChanged);
            // 
            // _Password1TextBox
            // 
            this._Password1TextBox.Location = new System.Drawing.Point(120, 96);
            this._Password1TextBox.Margin = new System.Windows.Forms.Padding(2);
            this._Password1TextBox.Name = "_Password1TextBox";
            this._Password1TextBox.PasswordChar = '*';
            this._Password1TextBox.Size = new System.Drawing.Size(128, 20);
            this._Password1TextBox.TabIndex = 7;
            this._Password1TextBox.TextChanged += new System.EventHandler(this._Password1TextBox_TextChanged);
            // 
            // _Password1Label
            // 
            this._Password1Label.Location = new System.Drawing.Point(24, 97);
            this._Password1Label.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this._Password1Label.Name = "_Password1Label";
            this._Password1Label.Size = new System.Drawing.Size(96, 16);
            this._Password1Label.TabIndex = 6;
            this._Password1Label.Text = "Password:";
            this._Password1Label.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // _LoginTextBox
            // 
            this._LoginTextBox.Location = new System.Drawing.Point(120, 74);
            this._LoginTextBox.Margin = new System.Windows.Forms.Padding(2);
            this._LoginTextBox.Name = "_LoginTextBox";
            this._LoginTextBox.Size = new System.Drawing.Size(128, 20);
            this._LoginTextBox.TabIndex = 5;
            this._LoginTextBox.TextChanged += new System.EventHandler(this._LoginTextBox_TextChanged);
            // 
            // _LoginLabel
            // 
            this._LoginLabel.Location = new System.Drawing.Point(24, 75);
            this._LoginLabel.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this._LoginLabel.Name = "_LoginLabel";
            this._LoginLabel.Size = new System.Drawing.Size(96, 15);
            this._LoginLabel.TabIndex = 4;
            this._LoginLabel.Text = "Login:";
            this._LoginLabel.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // _Password2TextBox
            // 
            this._Password2TextBox.Location = new System.Drawing.Point(120, 120);
            this._Password2TextBox.Margin = new System.Windows.Forms.Padding(2);
            this._Password2TextBox.Name = "_Password2TextBox";
            this._Password2TextBox.PasswordChar = '*';
            this._Password2TextBox.Size = new System.Drawing.Size(128, 20);
            this._Password2TextBox.TabIndex = 9;
            this._Password2TextBox.TextChanged += new System.EventHandler(this._Password2TextBox_TextChanged);
            // 
            // _Password2Label
            // 
            this._Password2Label.Location = new System.Drawing.Point(24, 121);
            this._Password2Label.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this._Password2Label.Name = "_Password2Label";
            this._Password2Label.Size = new System.Drawing.Size(96, 16);
            this._Password2Label.TabIndex = 8;
            this._Password2Label.Text = "Confirm password:";
            this._Password2Label.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // NewWorkspaceDialog
            // 
            this.AcceptButton = this._OkButton;
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.CancelButton = this._CancelButton;
            this.ClientSize = new System.Drawing.Size(339, 241);
            this.Controls.Add(this._WorkspaceAdministratorGroupBox);
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
            this.Name = "NewWorkspaceDialog";
            this.ShowInTaskbar = false;
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "New workspace";
            this._WorkspaceAdministratorGroupBox.ResumeLayout(false);
            this._WorkspaceAdministratorGroupBox.PerformLayout();
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
        private System.Windows.Forms.GroupBox _WorkspaceAdministratorGroupBox;
        private System.Windows.Forms.RadioButton _UseCustomCredentialsRadioButton;
        private System.Windows.Forms.RadioButton _UseCurrentCredentialsRadioButton;
        private System.Windows.Forms.TextBox _Password1TextBox;
        private System.Windows.Forms.Label _Password1Label;
        private System.Windows.Forms.TextBox _LoginTextBox;
        private System.Windows.Forms.Label _LoginLabel;
        private System.Windows.Forms.TextBox _Password2TextBox;
        private System.Windows.Forms.Label _Password2Label;
    }
}