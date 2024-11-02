using System.Diagnostics;
using System.Linq;

namespace TimeTracker3
{
    /// <summary>
    ///     The parsed TT3 command line.
    /// </summary>
    public sealed class CommandLine
    {
        //////////
        //  Singleton
        private static CommandLine _Instance /*= null*/;

        private CommandLine()
        {
        }

        /// <summary>
        ///     The one and only instance of this class
        /// </summary>
        public static CommandLine Instance => _Instance ?? (_Instance = new CommandLine());

        //////////
        //  Properties
        public bool ShowSplashScreen => _ShowSplashScreen;

        //////////
        //  Operations

        /// <summary>
        ///     Parses the command line arguments (as supplied to
        ///     Main() upon entry), modifying the properties of
        ///     this CommandLine object accordingly.
        /// </summary>
        /// <param name="args">
        ///     The command line arguments.
        /// </param>
        public void Parse(string[] args)
        {
            Debug.Assert(args != null && !args.Contains(null));
            
            foreach (string arg in args)
            {
                if (arg == "--no-splash")
                {
                    _ShowSplashScreen = false;
                }
                //  TODO else log ?
            }
        }

        // ////////
        //  Implementation
        private bool _ShowSplashScreen = true;
    }
}
