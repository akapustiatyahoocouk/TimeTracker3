using System;
using System.Windows.Forms;
using TimeTracker3.GUI;
using TimeTracker3.Skin.Admin;
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
            //  Initialize .NET/WinForms
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);

            //  Initialize TT3
            PluginManager.LoadPlugins(null);    //  TODO use splash screen
            SettingsManager.LoadSettings();

            //  Perform initial login
            string initialLogin = GuiSettings.Instance.LastLogin.Value ?? "";
            using (LoginDialog loginDialog = new LoginDialog(initialLogin))
            {
                if (loginDialog.ShowDialog() != DialogResult.OK)
                {   //  Don't even start!
                    Environment.Exit(0);
                }
            }

            //  Select initial skin
            ISkin initialSkin = SkinManager.FindSkin(GuiSettings.Instance.CurrentSkin.Value) ?? AdminSkin.Instance;

            //  Go!
            SkinManager.CurrentSkin = initialSkin;
            Application.Run();

            //  Cleanup & exit
            SettingsManager.SaveSettings();
        }
    }
}
