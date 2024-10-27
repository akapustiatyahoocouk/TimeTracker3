using System;
using System.Windows.Forms;
using TimeTracker3.Util;

namespace TimeTracker3
{
    internal static class Program
    {
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main()
        {
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);

            PluginManager.LoadPlugins();

            Application.Run(new Form1());
        }
    }
}
