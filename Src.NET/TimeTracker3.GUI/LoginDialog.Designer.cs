namespace TimeTracker3.GUI
{
    /// <summary>
    ///     The "Login" modal dialog.
    /// </summary>
    public sealed partial class LoginDialog
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(LoginDialog));
            this._LoginLabel = new System.Windows.Forms.Label();
            this._LoginTextBox = new System.Windows.Forms.TextBox();
            this._PasswordLabel = new System.Windows.Forms.Label();
            this._PasswordTextBox = new System.Windows.Forms.TextBox();
            this._OkButton = new System.Windows.Forms.Button();
            this._CancelButton = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // _LoginLabel
            // 
            this._LoginLabel.Anchor = System.Windows.Forms.AnchorStyles.Left;
            this._LoginLabel.Location = new System.Drawing.Point(5, 10);
            this._LoginLabel.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this._LoginLabel.Name = "_LoginLabel";
            this._LoginLabel.Size = new System.Drawing.Size(59, 15);
            this._LoginLabel.TabIndex = 0;
            this._LoginLabel.Text = "Login:";
            // 
            // _LoginTextBox
            // 
            this._LoginTextBox.Location = new System.Drawing.Point(69, 9);
            this._LoginTextBox.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this._LoginTextBox.Name = "_LoginTextBox";
            this._LoginTextBox.Size = new System.Drawing.Size(129, 20);
            this._LoginTextBox.TabIndex = 1;
            this._LoginTextBox.TextChanged += new System.EventHandler(this._LoginTextBox_TextChanged);
            // 
            // _PasswordLabel
            // 
            this._PasswordLabel.Anchor = System.Windows.Forms.AnchorStyles.Left;
            this._PasswordLabel.Location = new System.Drawing.Point(5, 32);
            this._PasswordLabel.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this._PasswordLabel.Name = "_PasswordLabel";
            this._PasswordLabel.Size = new System.Drawing.Size(59, 16);
            this._PasswordLabel.TabIndex = 2;
            this._PasswordLabel.Text = "Password:";
            // 
            // _PasswordTextBox
            // 
            this._PasswordTextBox.Location = new System.Drawing.Point(69, 31);
            this._PasswordTextBox.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this._PasswordTextBox.Name = "_PasswordTextBox";
            this._PasswordTextBox.PasswordChar = '*';
            this._PasswordTextBox.Size = new System.Drawing.Size(129, 20);
            this._PasswordTextBox.TabIndex = 3;
            // 
            // _OkButton
            // 
            this._OkButton.Image = ((System.Drawing.Image)(resources.GetObject("_OkButton.Image")));
            this._OkButton.Location = new System.Drawing.Point(75, 57);
            this._OkButton.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this._OkButton.Name = "_OkButton";
            this._OkButton.Size = new System.Drawing.Size(61, 26);
            this._OkButton.TabIndex = 4;
            this._OkButton.Text = "OK";
            this._OkButton.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageBeforeText;
            this._OkButton.UseVisualStyleBackColor = true;
            this._OkButton.Click += new System.EventHandler(this._OkButton_Click);
            // 
            // _CancelButton
            // 
            this._CancelButton.AutoSize = true;
            this._CancelButton.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this._CancelButton.Image = ((System.Drawing.Image)(resources.GetObject("_CancelButton.Image")));
            this._CancelButton.Location = new System.Drawing.Point(139, 57);
            this._CancelButton.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this._CancelButton.Name = "_CancelButton";
            this._CancelButton.Size = new System.Drawing.Size(66, 26);
            this._CancelButton.TabIndex = 5;
            this._CancelButton.Text = "Cancel";
            this._CancelButton.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageBeforeText;
            this._CancelButton.UseVisualStyleBackColor = true;
            this._CancelButton.Click += new System.EventHandler(this._CancelButton_Click);
            // 
            // LoginDialog
            // 
            this.AcceptButton = this._OkButton;
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.SystemColors.Control;
            this.CancelButton = this._CancelButton;
            this.ClientSize = new System.Drawing.Size(205, 88);
            this.Controls.Add(this._CancelButton);
            this.Controls.Add(this._OkButton);
            this.Controls.Add(this._PasswordTextBox);
            this.Controls.Add(this._PasswordLabel);
            this.Controls.Add(this._LoginTextBox);
            this.Controls.Add(this._LoginLabel);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "LoginDialog";
            this.ShowInTaskbar = false;
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Login to TimeTracker3";
            this.TopMost = true;
            this.Shown += new System.EventHandler(this.LoginDialog_Shown);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label _LoginLabel;
        private System.Windows.Forms.TextBox _LoginTextBox;
        private System.Windows.Forms.Label _PasswordLabel;
        private System.Windows.Forms.TextBox _PasswordTextBox;
        private System.Windows.Forms.Button _OkButton;
        private System.Windows.Forms.Button _CancelButton;
    }
}