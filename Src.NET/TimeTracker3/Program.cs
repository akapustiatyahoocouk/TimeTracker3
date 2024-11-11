//  TimeTracker3 - working time tracking helper
//  Copyright (C) 2025, Andrey Kapustin
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <https://www.gnu.org/licenses/>.
//  TODO add the above notice to the beginning of every .cs file
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
