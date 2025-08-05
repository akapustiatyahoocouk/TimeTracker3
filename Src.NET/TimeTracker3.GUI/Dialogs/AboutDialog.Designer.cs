namespace TimeTracker3.GUI.Dialogs
{
    partial class AboutDialog
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(AboutDialog));
            this._InfoLabel1 = new System.Windows.Forms.Label();
            this._InfoLabel2 = new System.Windows.Forms.Label();
            this._InfoLabel3 = new System.Windows.Forms.Label();
            this._PictureBox1 = new System.Windows.Forms.PictureBox();
            this._PictureBox2 = new System.Windows.Forms.PictureBox();
            this._OkButton = new System.Windows.Forms.Button();
            this._LicenceButton = new System.Windows.Forms.Button();
            this._ConfigurationButton = new System.Windows.Forms.Button();
            ((System.ComponentModel.ISupportInitialize)(this._PictureBox1)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this._PictureBox2)).BeginInit();
            this.SuspendLayout();
            //
            // _InfoLabel1
            //
            this._InfoLabel1.Location = new System.Drawing.Point(56, 8);
            this._InfoLabel1.Name = "_InfoLabel1";
            this._InfoLabel1.Size = new System.Drawing.Size(216, 16);
            this._InfoLabel1.TabIndex = 0;
            this._InfoLabel1.Text = "TimeTracker3 (build XXXX)";
            this._InfoLabel1.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            //
            // _InfoLabel2
            //
            this._InfoLabel2.Location = new System.Drawing.Point(56, 24);
            this._InfoLabel2.Name = "_InfoLabel2";
            this._InfoLabel2.Size = new System.Drawing.Size(216, 16);
            this._InfoLabel2.TabIndex = 1;
            this._InfoLabel2.Text = "Copyright (C) 2025, Andrey Kapustin";
            this._InfoLabel2.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            //
            // _InfoLabel3
            //
            this._InfoLabel3.Location = new System.Drawing.Point(56, 40);
            this._InfoLabel3.Name = "_InfoLabel3";
            this._InfoLabel3.Size = new System.Drawing.Size(216, 16);
            this._InfoLabel3.TabIndex = 2;
            this._InfoLabel3.Text = "All rights reserved";
            this._InfoLabel3.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            //
            // _PictureBox1
            //
            this._PictureBox1.Image = ((System.Drawing.Image)(resources.GetObject("_PictureBox1.Image")));
            this._PictureBox1.Location = new System.Drawing.Point(8, 16);
            this._PictureBox1.Name = "_PictureBox1";
            this._PictureBox1.Size = new System.Drawing.Size(32, 32);
            this._PictureBox1.TabIndex = 3;
            this._PictureBox1.TabStop = false;
            //
            // _PictureBox2
            //
            this._PictureBox2.Image = ((System.Drawing.Image)(resources.GetObject("_PictureBox2.Image")));
            this._PictureBox2.Location = new System.Drawing.Point(280, 16);
            this._PictureBox2.Name = "_PictureBox2";
            this._PictureBox2.Size = new System.Drawing.Size(32, 32);
            this._PictureBox2.TabIndex = 4;
            this._PictureBox2.TabStop = false;
            //
            // _OkButton
            //
            this._OkButton.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this._OkButton.Image = ((System.Drawing.Image)(resources.GetObject("_OkButton.Image")));
            this._OkButton.Location = new System.Drawing.Point(232, 96);
            this._OkButton.Name = "_OkButton";
            this._OkButton.Size = new System.Drawing.Size(75, 23);
            this._OkButton.TabIndex = 5;
            this._OkButton.Text = "OK";
            this._OkButton.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageBeforeText;
            this._OkButton.UseVisualStyleBackColor = true;
            //
            // _LicenceButton
            //
            this._LicenceButton.Image = ((System.Drawing.Image)(resources.GetObject("_LicenceButton.Image")));
            this._LicenceButton.Location = new System.Drawing.Point(8, 64);
            this._LicenceButton.Name = "_LicenceButton";
            this._LicenceButton.Size = new System.Drawing.Size(96, 23);
            this._LicenceButton.TabIndex = 3;
            this._LicenceButton.Text = "Licence";
            this._LicenceButton.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageBeforeText;
            this._LicenceButton.UseVisualStyleBackColor = true;
            this._LicenceButton.Click += new System.EventHandler(this._LicenceButton_Click);
            //
            // _ConfigurationButton
            //
            this._ConfigurationButton.Enabled = false;
            this._ConfigurationButton.Image = ((System.Drawing.Image)(resources.GetObject("_ConfigurationButton.Image")));
            this._ConfigurationButton.Location = new System.Drawing.Point(112, 64);
            this._ConfigurationButton.Name = "_ConfigurationButton";
            this._ConfigurationButton.Size = new System.Drawing.Size(96, 23);
            this._ConfigurationButton.TabIndex = 4;
            this._ConfigurationButton.Text = "Configuration";
            this._ConfigurationButton.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageBeforeText;
            this._ConfigurationButton.UseVisualStyleBackColor = true;
            //
            // AboutDialog
            //
            this.AcceptButton = this._OkButton;
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.CancelButton = this._OkButton;
            this.ClientSize = new System.Drawing.Size(322, 128);
            this.Controls.Add(this._ConfigurationButton);
            this.Controls.Add(this._LicenceButton);
            this.Controls.Add(this._OkButton);
            this.Controls.Add(this._PictureBox2);
            this.Controls.Add(this._PictureBox1);
            this.Controls.Add(this._InfoLabel3);
            this.Controls.Add(this._InfoLabel2);
            this.Controls.Add(this._InfoLabel1);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "AboutDialog";
            this.ShowInTaskbar = false;
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "About TimeTracker3";
            this.Shown += new System.EventHandler(this.AboutDialog_Shown);
            ((System.ComponentModel.ISupportInitialize)(this._PictureBox1)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this._PictureBox2)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Label _InfoLabel1;
        private System.Windows.Forms.Label _InfoLabel2;
        private System.Windows.Forms.Label _InfoLabel3;
        private System.Windows.Forms.PictureBox _PictureBox1;
        private System.Windows.Forms.PictureBox _PictureBox2;
        private System.Windows.Forms.Button _OkButton;
        private System.Windows.Forms.Button _LicenceButton;
        private System.Windows.Forms.Button _ConfigurationButton;
    }
}