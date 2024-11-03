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

        /// <summary>
        ///     Translates an arbitrary exception to a
        ///     WorkspaceException.
        /// </summary>
        /// <param name="ex">
        ///     The exception to translate to WorkspaceException.
        /// </param>
        /// <returns>
        ///     The resulting WorkspaceException.
        /// </returns>
        public static WorkspaceException Translate(Exception ex)
        {
            switch (ex)
            {
                case AlreadyExistsDatabaseException exx:
                    return new AlreadyExistsWorkspaceException(exx.ObjectTypeName, exx.PropertyName, exx.PropertyValue);
                case DatabaseInUseDatabaseException exx:
                    return new WorkspaceInUseWorkspaceException(new WorkspaceAddress(exx.Address));
                case DoesNotExistDatabaseException exx:
                    return new DoesNotExistWorkspaceException(exx.ObjectTypeName, exx.PropertyName, exx.PropertyValue);
                case IncompatibleObjectsDatabaseException exx:
                    return new IncompatibleObjectsWorkspaceException(exx.Object1, exx.Object2);
                //  TODO other database exceptions
                default:
                    return (ex is WorkspaceException wex) ? wex : new WorkspaceException(ex);
            }
        }
    }
}
