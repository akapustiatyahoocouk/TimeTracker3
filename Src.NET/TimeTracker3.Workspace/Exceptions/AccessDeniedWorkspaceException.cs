namespace TimeTracker3.Workspace.Exceptions
{
    /// <summary>
    ///     Thrown when an operation is denied due
    ///     to insufficient access rights.
    /// </summary>
    public class AccessDeniedWorkspaceException : WorkspaceException
    {
        //////////
        //  Construction

        /// <summary>
        ///     Constructs the exception.
        /// </summary>
        public AccessDeniedWorkspaceException()
        : base("Access denied")
        {
        }
    }
}
