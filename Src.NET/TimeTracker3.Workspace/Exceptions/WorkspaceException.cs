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
                case DatabaseClosedDatabaseException exx:
                    return new WorkspaceClosedWorkspaceException(new WorkspaceAddress(exx.DatabaseAddress));
                case DatabaseInUseDatabaseException exx:
                    return new WorkspaceInUseWorkspaceException(new WorkspaceAddress(exx.DatabaseAddress));
                case DoesNotExistDatabaseException exx:
                    return new DoesNotExistWorkspaceException(exx.ObjectTypeName, exx.PropertyName, exx.PropertyValue);
                case IncompatibleObjectsDatabaseException exx:
                    return new IncompatibleObjectsWorkspaceException(exx.Object1, exx.Object2);
                case InvalidPropertyValueDatabaseException exx:
                    return new InvalidPropertyValueWorkspaceException(exx.ObjectTypeName, exx.PropertyName, exx.PropertyValue);
                case ObjectDeadDatabaseException exx:
                    return new ObjectDeadWorkspaceException(BusinessObjectType._Translate(exx.ObjectType), BusinessObjectId._Translate( exx.ObjectId));
                //  TODO other database exceptions
                default:
                    return (ex is WorkspaceException wex) ? wex : new WorkspaceException(ex);
            }
        }
    }
}
