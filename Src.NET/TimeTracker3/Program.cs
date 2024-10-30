using System;
using System.Windows.Forms;
using TimeTracker3.GUI;
using TimeTracker3.Util;

namespace TimeTracker3
{
    /// <summary>
    ///     The TimeTracker3 entry point.
    /// </summary>
    internal static class Program
    {
        /// <summary>
        ///     The main entry point for the application.
        /// </summary>
        [STAThread]
        private static void Main()
        {
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);

            PluginManager.LoadPlugins();

            using (var dlg = new LoginDialog("123"))
            {
                dlg.ShowDialog();
            }

            Application.Run(new Form1());
        }
    }
}
