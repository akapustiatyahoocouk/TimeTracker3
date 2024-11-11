using System.Reflection;
using System;
using System.Collections.Generic;
using System.Linq;
using TimeTracker3.Util;

namespace TimeTracker3.Workspace
{
    /// <summary>
    ///     The Workspace plugin.
    /// </summary>
    public sealed class Plugin : IPlugin
    {
        //  Public default constructor is generated automatically

        //////////
        //  IPlugin
        public string DisplayName => "Workspace";
        public Version Version => Assembly.GetCallingAssembly().GetName().Version;
        public string Copyright => "Copyright (C) 2025, Andrey Kapustin";
        public string Summary => "Defines business workspace storage services";

        public void Initialize()
        {
            Formatting.RegisterDefaultFormatter(
                (Formatter<WorkspaceAddress[]>)Delegate.CreateDelegate(typeof(Formatter<WorkspaceAddress[]>), typeof(Plugin), "_FormatWorkspaceAddresses",
                    false));

            Parsing.RegisterDefaultFragmentParser(
                (FragmentParser<WorkspaceAddress[]>)Delegate.CreateDelegate(typeof(FragmentParser<WorkspaceAddress[]>), typeof(Plugin),
                    "_ParseWorkspaceAddressesFragment", false));

            SettingsManager.RegisterSettings(WorkspaceSettings.Instance);
        }

        //////////
        //  Implementation
        private const char _AddressSeparator = '\n';
        private const char _TypeAddressSeparator = '\r';

        //  Helpers
        //  ReSharper disable once UnusedMember.Local
        private static string _FormatWorkspaceAddresses(WorkspaceAddress[] was)
        {
            if (was.Length == 0 || was.Contains(null))
            {
                return "";
            }

            string result = "";
            foreach (WorkspaceAddress wa in was.Distinct())
            {
                if (result.Length > 0)
                {
                    result += _AddressSeparator;
                }

                result += wa.WorkspaceType.Mnemonic;
                result += _TypeAddressSeparator;
                result += wa.ExternalForm;
            }

            return result;
        }

        //  ReSharper disable once UnusedMember.Local
        private static bool _ParseWorkspaceAddressesFragment(string valueString, ref int scan, out WorkspaceAddress[] value)
        {
            if (string.IsNullOrEmpty(valueString) || scan > valueString.Length)
            {
                value = Array.Empty<WorkspaceAddress>();
                return false;
            }
            for (int len = valueString.Length - scan; len > 0; len--)
            {
                WorkspaceAddress[] result = _ParseWorkspaceAddressesString(valueString.Substring(scan, len));
                if (result != null)
                {
                    scan += len;
                    value = result;
                    return true;
                }
            }
            value = null;
            return false;
        }

        private static WorkspaceAddress[] _ParseWorkspaceAddressesString(string s)
        {
            List<WorkspaceAddress> result = new List<WorkspaceAddress>();
            string[] fragments = s.Split(_AddressSeparator);
            foreach (string fragment in fragments)
            {
                string[] typeAndAddress = fragment.Split(_TypeAddressSeparator);
                if (typeAndAddress.Length != 2)
                {   //  OOPS! Can't be
                    return null;
                }

                WorkspaceType workspaceType =
                    WorkspaceType.All
                        .FirstOrDefault(wt => wt.Mnemonic == typeAndAddress[0]);
                if (workspaceType == null)
                {   //  OOPS! Not good!
                    return null;
                }
                try
                {
                    WorkspaceAddress address = workspaceType.ParseWorkspaceAddress(typeAndAddress[1]);
                    if (!result.Contains(address))
                    {
                        result.Add(address);
                    }
                }
                catch
                {   //  OOPS! Not good!
                    return null;
                }
            }
            return result.ToArray();
        }
    }
}
