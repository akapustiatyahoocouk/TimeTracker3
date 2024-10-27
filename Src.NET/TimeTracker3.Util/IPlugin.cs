using System;

namespace TimeTracker3.Util
{
    /// <summary>
    ///     An abstract interface to a "plugin" - an agent 
    ///     discovered, instantiated and initialized at runtime.
    ///     Concrete classes implementing this interface must
    ///     be public and provide no-argument public constructor.
    /// </summary>
    public interface IPlugin
    {
        //////////
        //  Properties

        /// <summary>
        ///     The user-readable display name of this plugin.
        /// </summary>
        string DisplayName { get; }

        /// <summary>
        ///     The version of this plugin.
        /// </summary>
        Version Version { get; }

        /// <summary>
        ///     The 1-line copyright message of this plugin.
        /// </summary>
        string Copyright { get; }

        /// <summary>
        ///     The 1-line summary of this plugin's function.
        /// </summary>
        string Summary{ get; }

        //////////
        //  Operations

        /// <summary>
        ///     Called by plugin manager to initialize this plugin.
        /// </summary>
        /// <exception cref="Exception">
        ///     If plugin initialization fails.
        /// </exception>
        void Initialize();
    }
}
