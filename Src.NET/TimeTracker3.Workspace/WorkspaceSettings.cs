using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
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

        /// <summary>
        ///     Adds the specified workspace address to the
        ///     beginning of the recent workspace addresses
        ///     list. If the workspace address is already
        ///     somewhere in the list, it becomes first.
        /// </summary>
        /// <param name="workspaceAddress">
        ///     The workspace address to add.
        /// </param>
        public void AddRecentWorkspaceAddress(WorkspaceAddress workspaceAddress)
        {
            Debug.Assert(workspaceAddress != null);

            IList<WorkspaceAddress> workspaceAddresses =
                RecentWorkspaces.Value.ToList();
            workspaceAddresses.Remove(workspaceAddress);
            workspaceAddresses.Insert(0, workspaceAddress);
            RecentWorkspaces.Value =
                workspaceAddresses.Take(MaxRecentWorkspaces).ToArray();
        }

        /// <summary>
        ///     Removes the specified workspace address from
        ///     the list of recent workspace addresses. If the
        ///     workspace address is not in that list, the call
        ///     has no effect.
        /// </summary>
        /// <param name="workspaceAddress">
        ///     The workspace address to remove.
        /// </param>
        public void RemoveRecentWorkspaceAddress(WorkspaceAddress workspaceAddress)
        {
            Debug.Assert(workspaceAddress != null);

            IList<WorkspaceAddress> workspaceAddresses =
                RecentWorkspaces.Value.ToList();
            if (workspaceAddresses.Remove(workspaceAddress))
            {   //  Must save the changes
                RecentWorkspaces.Value =
                    workspaceAddresses.Take(MaxRecentWorkspaces).ToArray();
            }
        }
    }
}
