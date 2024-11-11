namespace TimeTracker3.GUI
{
    partial class LicenceDialog
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(LicenceDialog));
            this._LicenceTextBox = new System.Windows.Forms.TextBox();
            this._OkButton = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // _LicenceTextBox
            // 
            this._LicenceTextBox.Location = new System.Drawing.Point(8, 8);
            this._LicenceTextBox.Multiline = true;
            this._LicenceTextBox.Name = "_LicenceTextBox";
            this._LicenceTextBox.ReadOnly = true;
            this._LicenceTextBox.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
            this._LicenceTextBox.Size = new System.Drawing.Size(416, 264);
            this._LicenceTextBox.TabIndex = 0;
            // 
            // _OkButton
            // 
            this._OkButton.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this._OkButton.Location = new System.Drawing.Point(344, 288);
            this._OkButton.Name = "_OkButton";
            this._OkButton.Size = new System.Drawing.Size(75, 23);
            this._OkButton.TabIndex = 1;
            this._OkButton.Text = "OK";
            this._OkButton.UseVisualStyleBackColor = true;
            // 
            // LicenceDialog
            // 
            this.AcceptButton = this._OkButton;
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.CancelButton = this._OkButton;
            this.ClientSize = new System.Drawing.Size(434, 317);
            this.Controls.Add(this._OkButton);
            this.Controls.Add(this._LicenceTextBox);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "LicenceDialog";
            this.ShowInTaskbar = false;
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "GNU GPL v3";
            this.Shown += new System.EventHandler(this.LicenceDialog_Shown);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TextBox _LicenceTextBox;
        private System.Windows.Forms.Button _OkButton;
    }
}