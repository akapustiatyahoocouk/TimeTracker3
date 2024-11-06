using System.Diagnostics;

namespace TimeTracker3.Workspace.Exceptions
{
    /// <summary>
    ///     Thrown when an attempt is made to use a closed workspace.
    /// </summary>
    public class WorkspaceClosedWorkspaceException : WorkspaceException
    {
        //////////
        //  Construction

        /// <summary>
        ///     Constructs the exception.
        /// </summary>
        /// <param name="workspaceAddress">
        ///     The address of the "closed" workspace.
        /// </param>
        public WorkspaceClosedWorkspaceException(WorkspaceAddress workspaceAddress)
        : base("The database " + workspaceAddress.ExternalForm + " is closed")
        {
            Debug.Assert(workspaceAddress != null);
            WorkspaceAddress = workspaceAddress;
        }

        //////////
        //  Properties

        /// <summary>
        ///     The address of the "closed" workspace.
        /// </summary>
        public readonly WorkspaceAddress WorkspaceAddress;
    }
}
