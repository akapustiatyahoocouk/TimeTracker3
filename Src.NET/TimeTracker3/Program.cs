using System;
using System.Threading;
using System.Windows.Forms;
using TimeTracker3.GUI;
using TimeTracker3.Skin.Admin;
using TimeTracker3.Util;
using TimeTracker3.Workspace;

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
        private static void Main(string[] args)
        {
            //  Initialize .NET/WinForms
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);

            CommandLine.Instance.Parse(args);

            //  Do we need a splash screen ?
            SplashScreen splashScreen = null;
            DateTime splashScreenStartedAt = DateTime.UtcNow;
            if (CommandLine.Instance.ShowSplashScreen)
            {
                splashScreen = new SplashScreen();
                splashScreen.Visible = true;
                Application.DoEvents();
            }

            //  Initialize TT3
            PluginManager.LoadPlugins(splashScreen);
            SettingsManager.LoadSettings();

            //  Away with the splash screen
            if (splashScreen != null)
            {
                while (DateTime.UtcNow < splashScreenStartedAt + new TimeSpan(0, 0, 10))
                {
                    Thread.Sleep(100);
                }
                splashScreen.Dispose();
            }

            //  Perform initial login
            string initialLogin = GuiSettings.Instance.LastLogin.Value ?? "";
            using (LoginDialog loginDialog = new LoginDialog(initialLogin))
            {
                if (loginDialog.ShowDialog() != DialogResult.OK)
                {   //  Don't even start!
                    Environment.Exit(0);
                }
                CurrentCredentialsProvider.Instance.Value = loginDialog.Credentials;
            }

            //  Select initial skin
            ISkin initialSkin = SkinManager.FindSkin(GuiSettings.Instance.CurrentSkin.Value) ?? AdminSkin.Instance;

            //  Go!
            SkinManager.CurrentSkin = initialSkin;
            Application.Run();

            //  Cleanup & exit

            //  TODO stop "current" activity if there is one
            //  Close "current" workspace if there is one
            Workspace.Workspace currentWorkspace = CurrentWorkspaceProvider.Instance.Value;
            CurrentWorkspaceProvider.Instance.Value = null;
            try
            {
                currentWorkspace?.Close();
            }
            catch (Exception ex)
            {
                ErrorDialog.Show(null, ex);
            }

            //  We're done
            SettingsManager.SaveSettings();
        }
    }
}
