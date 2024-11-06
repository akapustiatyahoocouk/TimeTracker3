using System;
using System.Diagnostics;
using TimeTracker3.Db.API.Exceptions;

namespace TimeTracker3.Workspace.Exceptions
{
    /// <summary>
    ///     Thrown when an attempt is made to use a dead
    ///     workspace object.
    /// </summary>
    public class ObjectDeadWorkspaceException : WorkspaceException
    {
        //////////
        //  Construction

        /// <summary>
        ///     Constructs the exception.
        /// </summary>
        /// <param name="objectType">
        ///     The type of the dead business object.
        /// </param>
        /// <param name="objectId">
        ///     The OID of the dead business object.
        /// </param>
        public ObjectDeadWorkspaceException(BusinessObjectType objectType, BusinessObjectId objectId)
        : base(Convert.ToString(objectType.Name) + " " +
               Convert.ToString(objectId.ToString()) + " is dead")
        {
            Debug.Assert(objectType != null);
            Debug.Assert(objectId != null);

            ObjectType = objectType;
            ObjectId = objectId;
        }

        //////////
        //  Properties

        /// <summary>
        ///     The type of the dead business object.
        /// </summary>
        public readonly BusinessObjectType ObjectType;

        /// <summary>
        ///     The OID of the dead business object.
        /// </summary>
        public readonly BusinessObjectId ObjectId;
    }
}
