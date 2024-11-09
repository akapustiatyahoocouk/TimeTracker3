using System;
using TimeTracker3.Util;

namespace TimeTracker3.Workspace
{
    /// <summary>
    ///     The Workspace settings.
    /// </summary>
    public sealed class WorkspaceSettings : Settings
    {
        //////////
        //  Constants

        /// <summary>
        ///     The maximum number of recent workspace
        ///     addresses to keep.
        /// </summary>
        public const int MaxRecentWorkspaces = 9;

        //////////
        //  Singleton
        private static WorkspaceSettings _Instance /*= null*/;

        private WorkspaceSettings()
        : base(typeof(WorkspaceSettings).FullName)
        {
            Add(RecentWorkspaces);
        }

        /// <summary>
        ///     The one and only instance of this class
        /// </summary>
        public static WorkspaceSettings Instance => _Instance ?? (_Instance = new WorkspaceSettings());

        //////////
        //  Properties

        /// <summary>
        ///     The ordered list of recent workspaces, most
        ///     recent at [0], least recent as the end.
        /// </summary>
        public readonly Setting<WorkspaceAddress[]> RecentWorkspaces =
            new Setting<WorkspaceAddress[]>("RecentWorkspaces", Array.Empty<WorkspaceAddress>());

        //////////
        //  Operations
    }
}
