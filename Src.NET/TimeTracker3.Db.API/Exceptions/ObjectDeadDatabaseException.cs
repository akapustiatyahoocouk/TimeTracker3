using System;
using System.Diagnostics;

namespace TimeTracker3.Db.API.Exceptions
{
    /// <summary>
    ///     Thrown when an attempt is made to use a dead
    ///     database object.
    /// </summary>
    public class ObjectDeadDatabaseException : DatabaseException
    {
        //////////
        //  Construction

        /// <summary>
        ///     Constructs the exception.
        /// </summary>
        /// <param name="objectType">
        ///     The type of the dead database object.
        /// </param>
        /// <param name="objectId">
        ///     The OID of the dead database object.
        /// </param>
        public ObjectDeadDatabaseException(DatabaseObjectType objectType, IDatabaseObjectId objectId)
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
        ///     The type of the dead database object.
        /// </summary>
        public readonly DatabaseObjectType ObjectType;

        /// <summary>
        ///     The OID of the dead database object.
        /// </summary>
        public readonly IDatabaseObjectId ObjectId;
    }
}
