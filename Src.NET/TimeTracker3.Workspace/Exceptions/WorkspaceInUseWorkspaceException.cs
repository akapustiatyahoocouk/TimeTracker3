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
        public WorkspaceInUseWorkspaceException(WorkspaceAddress address)
        : base("Workspace " + address.ExternalForm + " is already in use")
        {
            Debug.Assert(address != null);

            Address = address;
        }

        //////////
        //  Properties
        public readonly WorkspaceAddress Address;
    }
}
