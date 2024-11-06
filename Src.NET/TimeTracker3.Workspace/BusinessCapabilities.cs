using System;
using TimeTracker3.Db.API;

namespace TimeTracker3.Workspace
{
    /// <summary>
    ///     Account capabilities governing access rights.
    /// </summary>
    [Flags]
    public enum BusinessCapabilities
    {
        Administrator = Capabilities.Administrator,
        ManageUsers = Capabilities.ManageUsers,
        ManageBeneficiaries = Capabilities.ManageBeneficiaries,
        ManageWorkloads = Capabilities.ManageWorkloads,
        ManagePublicActivities = Capabilities.ManagePublicActivities,
        ManagePublicTasks = Capabilities.ManagePublicTasks,
        ManagePrivateActivities = Capabilities.ManagePrivateActivities,
        ManagePrivateTasks = Capabilities.ManagePrivateTasks,
        LogWork = Capabilities.LogWork,
        LogEvents = Capabilities.LogEvents,
        GenerateReports = Capabilities.GenerateReports,
        BackupAndRestore = Capabilities.BackupAndRestore,
        //  Flag sets
        None = Capabilities.None,
        All = Capabilities.All
    }
}
