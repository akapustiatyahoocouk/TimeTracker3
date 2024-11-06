using System.Diagnostics;

namespace TimeTracker3.Db.API
{
    /// <summary>
    ///     The event raised by a Database when an existing
    ///     object is destroyed there.
    ///     IMPORTANT: This event is raised AFTER the object
    ///     has been destroyed.
    /// </summary>
    public sealed class DatabaseObjectDestroyedEventArgs : DatabaseEventArgs
    {
        //////////
        //  Construction

        /// <summary>
        ///     Constructs the event arguments.
        /// </summary>
        /// <param name="database">
        ///     The database where an existing object has
        ///     been destroyed.
        /// </param>
        /// <param name="objectType">
        ///     The object's type.
        /// </param>
        /// <param name="oid">
        ///     The OID of the destroyed object.
        /// </param>
        public DatabaseObjectDestroyedEventArgs(
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
        ///     The OID of the destroyed object.
        /// </summary>
        public readonly IDatabaseObjectId Oid;
    }
}
