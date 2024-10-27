using System;

namespace TimeTracker3.Db.API
{
    /// <summary>
    ///     Account capabilities governing access rights.
    /// </summary>
    [Flags]
    public enum Capabilities
    {
        Administrator = 0x0001,
        ManageUsers = 0x0002,
        ManageBeneficiaries = 0x0004,
        ManageWorkloads = 0x0008,
        ManagePublicActivities = 0x0010,
        ManagePublicTasks = 0x0020,
        ManagePrivateActivities = 0x0040,
        ManagePrivateTasks = 0x0080,
        LogWork = 0x0100,
        LogEvents = 0x0200,
        GenerateReports = 0x0400,
        BackupAndRestore = 0x0800
    }
}
