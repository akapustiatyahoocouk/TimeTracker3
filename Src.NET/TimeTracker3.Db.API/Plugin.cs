using System.Reflection;
using System;
using System.Diagnostics;
using System.Windows.Forms.ComponentModel.Com2Interop;
using TimeTracker3.Util;
using System.Threading;

namespace TimeTracker3.Db.API
{
    /// <summary>
    ///     The DB API plugin.
    /// </summary>
    public sealed class Plugin : IPlugin
    {   //  Public default constructor is generated automatically

        //////////
        //  IPlugin
        public string DisplayName => "Database API";
        public Version Version => Assembly.GetCallingAssembly().GetName().Version;
        public string Copyright => "Copyright (C) 2025, Andrey Kapustin";
        public string Summary => "Defines abstract API for database access";

        public void Initialize()
        {
            Formatting.RegisterDefaultFormatter(
                (Formatter<Capabilities>)Delegate.CreateDelegate(typeof(Formatter<Capabilities>), typeof(Plugin), "_FormatCapabilities", false));

            Parsing.RegisterDefaultFragmentParser(
                (FragmentParser<Capabilities>)Delegate.CreateDelegate(typeof(FragmentParser<Capabilities>), typeof(Plugin), "_ParseCapabilitiesFragment", false));
        }

        //////////
        //  Implementation helpers
        private static string _FormatCapabilities(Capabilities c)
        {
            string result = "";
            for (int mask = 0x0001; (mask & (int)Capabilities.All) != 0; mask <<= 1)
            {
                if (((int)c & mask) != 0)
                {
                    string capabilityName = Enum.GetName(typeof(Capabilities), (Capabilities)mask);
                    if (result.Length != 0)
                    {
                        result += '+';
                    }
                    result += capabilityName;
                }
            }
            return result;
        }

        private static bool _ParseCapabilitiesFragment(string valueString, ref int scan, out Capabilities value)
        {
            if (valueString == null || scan > valueString.Length)
            {
                value = Capabilities.None;
                return false;
            }

            //  <c1>+<c2>+...+<cN>
            int prescan = scan;
            Capabilities capabilities = Capabilities.None;
            for (bool keepTrying = true; keepTrying; )
            {
                keepTrying = false;
                //  Does a capability start at [prescan] ?
                for (int mask = 0x0001; (mask & (int)Capabilities.All) != 0; mask <<= 1)
                {
                    string capabilityName = Enum.GetName(typeof(Capabilities), (Capabilities)mask);
                    Debug.Assert(capabilityName != null);
                    if (valueString.Substring(prescan).StartsWith(capabilityName))
                    {   //  Yes - record & advance
                        capabilities |= (Capabilities)mask;
                        prescan += capabilityName.Length;
                        //  More to follow ?
                        if (prescan < valueString.Length && valueString[prescan] == '+')
                        {   //  Yes
                            prescan++;
                            keepTrying = true;
                        }
                        break;
                    }
                }

                if (!keepTrying)
                {   //  No more capabilities
                    break;
                }
            }

            value = capabilities;
            scan = prescan;
            return true;
        }
    }
}
