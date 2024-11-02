namespace TimeTracker3.GUI
{
    partial class SplashScreen
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(SplashScreen));
            this._OperationLabel = new System.Windows.Forms.Label();
            this._ContextLabel = new System.Windows.Forms.Label();
            this._ProgressBar = new System.Windows.Forms.ProgressBar();
            this.SuspendLayout();
            // 
            // _OperationLabel
            // 
            this._OperationLabel.Anchor = System.Windows.Forms.AnchorStyles.Left;
            this._OperationLabel.Location = new System.Drawing.Point(8, 152);
            this._OperationLabel.Name = "_OperationLabel";
            this._OperationLabel.Size = new System.Drawing.Size(608, 23);
            this._OperationLabel.TabIndex = 0;
            this._OperationLabel.Text = "<<OPERATION>>";
            // 
            // _ContextLabel
            // 
            this._ContextLabel.Anchor = System.Windows.Forms.AnchorStyles.Left;
            this._ContextLabel.Location = new System.Drawing.Point(8, 176);
            this._ContextLabel.Name = "_ContextLabel";
            this._ContextLabel.Size = new System.Drawing.Size(608, 23);
            this._ContextLabel.TabIndex = 1;
            this._ContextLabel.Text = "<<CONTEXT>>";
            // 
            // _ProgressBar
            // 
            this._ProgressBar.Location = new System.Drawing.Point(16, 200);
            this._ProgressBar.Name = "_ProgressBar";
            this._ProgressBar.Size = new System.Drawing.Size(600, 23);
            this._ProgressBar.Step = 1;
            this._ProgressBar.TabIndex = 2;
            // 
            // SplashScreen
            // 
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.None;
            this.BackgroundImage = ((System.Drawing.Image)(resources.GetObject("$this.BackgroundImage")));
            this.ClientSize = new System.Drawing.Size(627, 231);
            this.ControlBox = false;
            this.Controls.Add(this._ProgressBar);
            this.Controls.Add(this._ContextLabel);
            this.Controls.Add(this._OperationLabel);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "SplashScreen";
            this.ShowIcon = false;
            this.ShowInTaskbar = false;
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.TopMost = true;
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Label _OperationLabel;
        private System.Windows.Forms.Label _ContextLabel;
        private System.Windows.Forms.ProgressBar _ProgressBar;
    }
}