using System;
using System.Diagnostics;
using System.Windows.Forms;

namespace TimeTracker3.GUI.Dialogs
{
    /// <summary>
    ///     The modal "error" dialog.
    /// </summary>
    public sealed partial class ErrorDialog : Form
    {
        //////////
        //  Construction
        public ErrorDialog(Exception ex)
        {
            InitializeComponent();
            //  Populate stack trace list box
            for (Exception exx = ex; exx != null; exx = exx.InnerException)
            {
                _StackTraceListBox.Items.Add(new _StackTraceListBoxItem(exx));
            }
            _StackTraceListBox.SelectedIndex = 0;
            _PupulateDetailsTextBox();
        }

        //////////
        //  Operations
        //  TODO document
        public static void Show(IWin32Window parent, Exception ex)
        {
            using (ErrorDialog dlg = new ErrorDialog(ex))
            {
                dlg.ShowDialog(parent);
            }
        }

        public static void Show(IWin32Window parent, string message)
        {
            Debug.Assert(message != null);
            MessageBox.Show(message, "Error", MessageBoxButtons.OK, MessageBoxIcon.Error, MessageBoxDefaultButton.Button1);
        }

        //////////
        //  Implementation
        private sealed class _StackTraceListBoxItem
        {
            //  Construction
            internal _StackTraceListBoxItem(Exception exception)
            {
                Debug.Assert(exception != null);
                _Exception = exception;
            }
            //  object
            public override string ToString()
            {
                return _Exception.GetType().FullName ?? "???";
            }

            //  Properties
            internal readonly Exception _Exception;
        }

        //  Helpers
        private void _PupulateDetailsTextBox()
        {
            _StackTraceListBoxItem item =
                (_StackTraceListBoxItem)_StackTraceListBox.SelectedItem;
            string details = "";
            if (item != null)
            {
                details += item._Exception.GetType().FullName + "\r\n\r\n";
                details += item._Exception.Message + "\r\n\r\n";
                details += item._Exception.StackTrace + "\r\n";
            }
            _DetailsTextBox.Text = details;
        }

        //////////
        //  Event handlers
        private void _StackTraceListBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            _PupulateDetailsTextBox();
        }

        private void _SaveButton_Click(object sender, EventArgs e)
        {
            //  TODO implement
        }
    }
}
