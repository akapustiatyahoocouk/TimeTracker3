using System.Diagnostics;

namespace TimeTracker3.Workspace.Exceptions
{
    /// <summary>
    ///     Thrown when an attempt is made to open a
    ///     single-user workspace or destroy a workspace
    ///     that are currently open elsewhere.
    /// </summary>
    public class WorkspaceInUseWorkspaceException : WorkspaceException
    {
        //////////
        //  Construction
        public WorkspaceInUseWorkspaceException(WorkspaceAddress workspaceAddress)
        : base("Workspace " + workspaceAddress.ExternalForm + " is already in use")
        {
            Debug.Assert(workspaceAddress != null);

            WorkspaceWorkspaceAddress = workspaceAddress;
        }

        //////////
        //  Properties
        public readonly WorkspaceAddress WorkspaceWorkspaceAddress;
    }
}
