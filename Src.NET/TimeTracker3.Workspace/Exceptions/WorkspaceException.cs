using System;
using TimeTracker3.Db.API.Exceptions;

namespace TimeTracker3.Workspace.Exceptions
{
    /// <summary>
    ///     The common base class for all exceptions throws 
    ///     by Workspace services.
    /// </summary>
    public class WorkspaceException : Exception
    {
        //////////
        //  Construction
        //  TODO document all

        public WorkspaceException(string message)
            : base(message)
        {
        }

        public WorkspaceException(string message, Exception cause)
            : base(message, cause)
        {
        }

        public WorkspaceException(Exception cause)
            : base(cause.Message, cause)
        {
        }

        //////////
        //  Operations

        public static WorkspaceException Translate(Exception ex)
        {
            if (ex is IncompatibleObjectsDatabaseException exx)
            {
                return new IncompatibleObjectsWorkspaceException(exx.Object1, exx.Object2);
            }
            //  TODO implement properly
            return (ex is WorkspaceException wex) ? wex : new WorkspaceException(ex);
        }
    }
}
