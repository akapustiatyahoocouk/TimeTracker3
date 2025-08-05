namespace TimeTracker3.GUI.Dialogs
{
    partial class ErrorDialog
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(ErrorDialog));
            this._StackTraceListBox = new System.Windows.Forms.ListBox();
            this._DetailsTextBox = new System.Windows.Forms.TextBox();
            this._SaveButton = new System.Windows.Forms.Button();
            this._CloseButton = new System.Windows.Forms.Button();
            this.SuspendLayout();
            //
            // _StackTraceListBox
            //
            this._StackTraceListBox.FormattingEnabled = true;
            this._StackTraceListBox.HorizontalScrollbar = true;
            this._StackTraceListBox.IntegralHeight = false;
            this._StackTraceListBox.Location = new System.Drawing.Point(8, 8);
            this._StackTraceListBox.Name = "_StackTraceListBox";
            this._StackTraceListBox.Size = new System.Drawing.Size(192, 120);
            this._StackTraceListBox.TabIndex = 0;
            this._StackTraceListBox.SelectedIndexChanged += new System.EventHandler(this._StackTraceListBox_SelectedIndexChanged);
            //
            // _DetailsTextBox
            //
            this._DetailsTextBox.Location = new System.Drawing.Point(208, 8);
            this._DetailsTextBox.Multiline = true;
            this._DetailsTextBox.Name = "_DetailsTextBox";
            this._DetailsTextBox.ReadOnly = true;
            this._DetailsTextBox.ScrollBars = System.Windows.Forms.ScrollBars.Both;
            this._DetailsTextBox.Size = new System.Drawing.Size(272, 120);
            this._DetailsTextBox.TabIndex = 1;
            //
            // _SaveButton
            //
            this._SaveButton.Enabled = false;
            this._SaveButton.Image = ((System.Drawing.Image)(resources.GetObject("_SaveButton.Image")));
            this._SaveButton.Location = new System.Drawing.Point(8, 144);
            this._SaveButton.Name = "_SaveButton";
            this._SaveButton.Size = new System.Drawing.Size(75, 23);
            this._SaveButton.TabIndex = 2;
            this._SaveButton.Text = "Save";
            this._SaveButton.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageBeforeText;
            this._SaveButton.UseVisualStyleBackColor = true;
            this._SaveButton.Click += new System.EventHandler(this._SaveButton_Click);
            //
            // _CloseButton
            //
            this._CloseButton.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this._CloseButton.Image = ((System.Drawing.Image)(resources.GetObject("_CloseButton.Image")));
            this._CloseButton.Location = new System.Drawing.Point(400, 144);
            this._CloseButton.Name = "_CloseButton";
            this._CloseButton.Size = new System.Drawing.Size(75, 23);
            this._CloseButton.TabIndex = 3;
            this._CloseButton.Text = "Close";
            this._CloseButton.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageBeforeText;
            this._CloseButton.UseVisualStyleBackColor = true;
            //
            // ErrorDialog
            //
            this.AcceptButton = this._CloseButton;
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.CancelButton = this._CloseButton;
            this.ClientSize = new System.Drawing.Size(489, 173);
            this.Controls.Add(this._CloseButton);
            this.Controls.Add(this._SaveButton);
            this.Controls.Add(this._DetailsTextBox);
            this.Controls.Add(this._StackTraceListBox);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "ErrorDialog";
            this.ShowInTaskbar = false;
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "Error";
            this.TopMost = true;
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.ListBox _StackTraceListBox;
        private System.Windows.Forms.TextBox _DetailsTextBox;
        private System.Windows.Forms.Button _SaveButton;
        private System.Windows.Forms.Button _CloseButton;
    }
}