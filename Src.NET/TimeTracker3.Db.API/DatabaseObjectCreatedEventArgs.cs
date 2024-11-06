using System.Diagnostics;

namespace TimeTracker3.Db.API
{
    /// <summary>
    ///     The event raised by a Database when a new
    ///     object is created there.
    /// </summary>
    public sealed class DatabaseObjectCreatedEventArgs : DatabaseEventArgs
    {
        //////////
        //  Construction

        /// <summary>
        ///     Constructs the event arguments.
        /// </summary>
        /// <param name="database">
        ///     The database where a new object has
        ///     been created.
        /// </param>
        /// <param name="objectType">
        ///     The object's type.
        /// </param>
        /// <param name="oid">
        ///     The OID of the newly created object.
        /// </param>
        public DatabaseObjectCreatedEventArgs(
            IDatabase database, DatabaseObjectType objectType, IDatabaseObjectId oid)
        :   base(database)
        {
            Debug.Assert(objectType != null);
            Debug.Assert(oid != null);

            ObjectType = objectType;
            Oid = oid;
        }

        //////////
        //  Properties

        /// <summary>
        ///     The object's type.
        /// </summary>
        public readonly DatabaseObjectType ObjectType;

        /// <summary>
        ///     The OID of the newly created object.
        /// </summary>
        public readonly IDatabaseObjectId Oid;
    }
}
