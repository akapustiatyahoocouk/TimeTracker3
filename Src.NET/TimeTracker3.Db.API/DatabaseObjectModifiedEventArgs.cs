using System.Diagnostics;

namespace TimeTracker3.Db.API
{
    /// <summary>
    ///     The event raised by a Database when an existing
    ///     object is modified there.
    /// </summary>
    public sealed class DatabaseObjectModifiedEventArgs : DatabaseEventArgs
    {
        //////////
        //  Construction

        /// <summary>
        ///     Constructs the event arguments.
        /// </summary>
        /// <param name="database">
        ///     The database where an existing object has
        ///     been modified.
        /// </param>
        /// <param name="objectType">
        ///     The object's type.
        /// </param>
        /// <param name="oid">
        ///     The OID of the modified object.
        /// </param>
        public DatabaseObjectModifiedEventArgs(
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
        ///     The OID of the modified object.
        /// </summary>
        public readonly IDatabaseObjectId Oid;
    }
}
