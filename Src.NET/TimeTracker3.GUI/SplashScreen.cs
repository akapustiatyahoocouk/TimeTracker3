using System;
using System.Threading;
using System.Windows.Forms;
using TimeTracker3.Util;

namespace TimeTracker3.GUI
{
    /// <summary>
    ///     The splash screen popup form.
    /// </summary>
    public sealed partial class SplashScreen : Form, IProgressTracker
    {
        //////////
        //  Construction

        /// <summary>
        ///     Constructs the splash screen.
        /// </summary>
        public SplashScreen()
        {
            InitializeComponent();
            _OperationLabel.Text = "";
            _ContextLabel.Text = "";
            _ProgressBar.Visible = false;
        }

        //////////
        //  IProgressTracker
        public void OnProgressReached(string activity, string context, double? completed)
        {
            _OperationLabel.Text = activity ?? "";
            _ContextLabel.Text = context ?? "";
            if (completed.HasValue)
            {
                double completedRation = Math.Max(0.0, Math.Min(1.0, completed.Value));
                _ProgressBar.Value = (int)(completedRation * 100);
                _ProgressBar.Visible = true;
            }
            else
            {
                _ProgressBar.Visible = false;
            }
            for (int i = 0; i < 50; i++)
            {
                Application.DoEvents();
                Thread.Sleep(10);
            }
        }
    }
}
